extrn _circle:byte
extrn _circle2:byte
code 	SEGMENT para public 'CODE'
	ASSUME cs:code
.386
LOCALS

ALIGN 16
_rows	dw	200 dup(0)
_blit16t dw	256 dup(0)
_vbufseg dw	0
clipleft dw	0

polyisides dw	0
polyixy	dw	16 dup(0,0)
polysides dw	0
polyxy	dw	16 dup(0,0)
include polyclip.asm

ALIGN 2
PUBLIC	_sin1024
include sin1024.inc

REPOUTSB MACRO
	local l1
l1:	mov	al,ds:[si]
	inc	si
	out	dx,al
	dec	cx
	jnz	l1
	ENDM

PUBLIC _asminit	
_asminit PROC FAR
	push 	bp
	mov	bp,sp
	push	si
	push	di
	push	ds
	mov	ax,[bp+8]
	mov	cs:_vbufseg,ax
	call	blitinit
	pop	ds
	pop	di
	pop	si
	pop	bp
	ret
_asminit ENDP

PUBLIC _asmdoit	
_asmdoit PROC FAR
	push 	bp
	mov	bp,sp
	push	si
	push	di
	push	ds
	lds	si,[bp+6]
	les	di,[bp+10]
	call	blit16
	pop	ds
	pop	di
	pop	si
	pop	bp
	ret
_asmdoit ENDP

PUBLIC _asmdoit2
_asmdoit2 PROC FAR
	push 	bp
	mov	bp,sp
	push	si
	push	di
	push	ds
	lds	si,[bp+6]
	les	di,[bp+10]
	call	blit16b
	pop	ds
	pop	di
	pop	si
	pop	bp
	ret
_asmdoit2 ENDP

blitinit PROC NEAR
	mov	bx,OFFSET _rows
	mov	cx,200
	mov	dx,40
	xor	ax,ax
@@l1:	mov	cs:[bx],ax
	add	ax,dx
	add	bx,2
	loop	@@l1
	xor	al,al
	mov	bx,OFFSET _blit16t
	mov	cx,256
@@1:	mov	dh,255
	mov	dl,al
	xor	ah,ah
	REPT	8
	local	l2
	rcl	dl,1
	jnc	l2
	xor	ah,dh
l2:	shr	dh,1
	ENDM
	mov	cs:[bx],ah
	ror	ah,1
	and	ah,80h
	mov	cs:[bx+1],ah
	add	bx,2
	inc	al
	loop	@@1
	ret
blitinit ENDP

blit16	PROC NEAR
	xor	ebx,ebx
	mov	cx,200	
	jmp	@@1
	ALIGN	16
@@1:	zzz=0
	xor	dh,dh ;line starts black
	REPT	40/2
	mov	bl,ds:[si+zzz]
	xor	bl,dh
	mov	ax,cs:_blit16t[ebx*2]
	mov	bl,ds:[si+1+zzz]
	xor	bl,ah
	mov	dx,cs:_blit16t[ebx*2]
	mov	ah,dl
	mov	es:[di+zzz],ax
	zzz=zzz+2
	ENDM
	add	si,40
	add	di,40
	dec	cx
	jz	@@2
	jmp	@@1
@@2:
	ret
blit16	ENDP

blit16b	PROC NEAR
	xor	ebx,ebx
	mov	cx,200	
	jmp	@@1
	ALIGN	16
@@1:	zzz=0
	xor	dh,dh ;line starts black
	REPT	40/2
	mov	bl,ds:[si+zzz]
	xor	bl,dh
	mov	ax,cs:_blit16t[ebx*2]
	mov	bl,ds:[si+1+zzz]
	xor	bl,ah
	mov	dx,cs:_blit16t[ebx*2]
	mov	ah,dl
	mov	es:[di+zzz],ax
	zzz=zzz+2
	ENDM
	add	si,40
	add	di,80
	dec	cx
	jz	@@2
	jmp	@@1
@@2:
	ret
blit16b	ENDP

drawline PROC NEAR
	push	si
	push	di
	push	bp
@@vis:	movzx	ebx,bx
	cmp	bx,cx
	je	@@0
	jle	@@1
	xchg	bx,cx
	xchg	ax,dx
@@1:	sub	cx,bx
	mov	di,cx
	mov	si,cs:_rows[ebx*2]
	mov	bp,cs:clipleft
	or	bp,bp
	jz	@@nl
	push	si
	;left overflow fill
	jge	@@ndn
@@nup:	add	si,40
	xor	byte ptr ds:[si],080h
	inc	bp
	jnz	@@nup
	jmp	@@nl2
@@ndn:	sub	si,40
	xor	byte ptr ds:[si],080h
	dec	bp
	jnz	@@ndn
@@nl2:	pop	si
@@nl:	;
	jcxz	@@0
	movzx	ebp,ax
	shr	bp,3
	add	si,bp
	mov	bp,ax
	and	bp,7
	;go on
	cmp	ax,dx
	jl	@@r
@@l:	;=============== left
	neg	dx
	add	dx,ax
	mov	bx,di
	shr	bx,1
	neg	bx
	jmp	cs:_loffs[ebp*2]
ALIGN 16
_loffs	LABEL WORD
	dw	OFFSET @@l7
	dw	OFFSET @@l6
	dw	OFFSET @@l5
	dw	OFFSET @@l4
	dw	OFFSET @@l3
	dw	OFFSET @@l2
	dw	OFFSET @@l1
	dw	OFFSET @@l0
llinemacro MACRO mask,lbl1,lbl2,lbl3,lbl4,lbl5,lbl6,lbl7,lbl0
	local	l1,l2
	;ds:si=startpoint
	;di=ycnt
	;dx=xcnt
	;bx=counter
l1:	xor	byte ptr ds:[si],mask
	add	si,40
	dec	cx
	jz	@@0
	add	bx,dx
	jl	l1
l2:	IF lbl1 EQ @@l0
	dec	si
	ENDIF
	sub	bx,di
	jl	lbl1
	IF lbl2 EQ @@l0
	dec	si
	ENDIF
	sub	bx,di
	jl	lbl2
	IF lbl3 EQ @@l0
	dec	si
	ENDIF
	sub	bx,di
	jl	lbl3
	IF lbl4 EQ @@l0
	dec	si
	ENDIF
	sub	bx,di
	jl	lbl4
	IF lbl5 EQ @@l0
	dec	si
	ENDIF
	sub	bx,di
	jl	lbl5
	IF lbl6 EQ @@l0
	dec	si
	ENDIF
	sub	bx,di
	jl	lbl6
	IF lbl7 EQ @@l0
	dec	si
	ENDIF
	sub	bx,di
	jl	lbl7
	IF lbl0 EQ @@l0
	dec	si
	ENDIF
	sub	bx,di
	jl	l1
	jmp	l2
	ENDM
@@l7:	llinemacro 10000000b,@@l0,@@l1,@@l2,@@l3,@@l4,@@l5,@@l6,@@l7
@@l6:	llinemacro 01000000b,@@l7,@@l0,@@l1,@@l2,@@l3,@@l4,@@l5,@@l6
@@l5:	llinemacro 00100000b,@@l6,@@l7,@@l0,@@l1,@@l2,@@l3,@@l4,@@l5
@@l4:	llinemacro 00010000b,@@l5,@@l6,@@l7,@@l0,@@l1,@@l2,@@l3,@@l4
@@l3:	llinemacro 00001000b,@@l4,@@l5,@@l6,@@l7,@@l0,@@l1,@@l2,@@l3
@@l2:	llinemacro 00000100b,@@l3,@@l4,@@l5,@@l6,@@l7,@@l0,@@l1,@@l2
@@l1:	llinemacro 00000010b,@@l2,@@l3,@@l4,@@l5,@@l6,@@l7,@@l0,@@l1
@@l0:	llinemacro 00000001b,@@l1,@@l2,@@l3,@@l4,@@l5,@@l6,@@l7,@@l0
@@r:	;=============== right
	sub	dx,ax
	mov	bx,di
	shr	bx,1
	neg	bx
	jmp	cs:_roffs[ebp*2]
ALIGN 16
_roffs	LABEL WORD
	dw	OFFSET @@r7
	dw	OFFSET @@r6
	dw	OFFSET @@r5
	dw	OFFSET @@r4
	dw	OFFSET @@r3
	dw	OFFSET @@r2
	dw	OFFSET @@r1
	dw	OFFSET @@r0
rlinemacro MACRO mask,lbl1,lbl2,lbl3,lbl4,lbl5,lbl6,lbl7,lbl0
	local	l1,l2
	;ds:si=startpoint
	;di=ycnt
	;dx=xcnt
	;bx=counter
l1:	xor	byte ptr ds:[si],mask
	add	si,40
	dec	cx
	jz	@@0
	add	bx,dx
	jl	l1
l2:	IF lbl1 EQ @@r7
	inc	si
	ENDIF
	sub	bx,di
	jl	lbl1
	IF lbl2 EQ @@r7
	inc	si
	ENDIF
	sub	bx,di
	jl	lbl2
	IF lbl3 EQ @@r7
	inc	si
	ENDIF
	sub	bx,di
	jl	lbl3
	IF lbl4 EQ @@r7
	inc	si
	ENDIF
	sub	bx,di
	jl	lbl4
	IF lbl5 EQ @@r7
	inc	si
	ENDIF
	sub	bx,di
	jl	lbl5
	IF lbl6 EQ @@r7
	inc	si
	ENDIF
	sub	bx,di
	jl	lbl6
	IF lbl7 EQ @@r7
	inc	si
	ENDIF
	sub	bx,di
	jl	lbl7
	IF lbl0 EQ @@r7
	inc	si
	ENDIF
	sub	bx,di
	jl	l1
	jmp	l2
	ENDM
@@r7:	rlinemacro 10000000b,@@r6,@@r5,@@r4,@@r3,@@r2,@@r1,@@r0,@@r7
@@r6:	rlinemacro 01000000b,@@r5,@@r4,@@r3,@@r2,@@r1,@@r0,@@r7,@@r6
@@r5:	rlinemacro 00100000b,@@r4,@@r3,@@r2,@@r1,@@r0,@@r7,@@r6,@@r5
@@r4:	rlinemacro 00010000b,@@r3,@@r2,@@r1,@@r0,@@r7,@@r6,@@r5,@@r4
@@r3:	rlinemacro 00001000b,@@r2,@@r1,@@r0,@@r7,@@r6,@@r5,@@r4,@@r3
@@r2:	rlinemacro 00000100b,@@r1,@@r0,@@r7,@@r6,@@r5,@@r4,@@r3,@@r2
@@r1:	rlinemacro 00000010b,@@r0,@@r7,@@r6,@@r5,@@r4,@@r3,@@r2,@@r1
@@r0:	rlinemacro 00000001b,@@r7,@@r6,@@r5,@@r4,@@r3,@@r2,@@r1,@@r0
@@0:	pop	bp
	pop	di
	pop	si
	ret
drawline ENDP

PUBLIC _asmbox	
_asmbox PROC FAR
	push 	bp
	mov	bp,sp
	push	si
	push	di
	push	ds
	
	mov	eax,[bp+6]
	mov	dword ptr cs:polyixy[0],eax
	mov	eax,[bp+10]
	mov	dword ptr cs:polyixy[4],eax
	mov	eax,[bp+14]
	mov	dword ptr cs:polyixy[8],eax
	mov	eax,[bp+18]
	mov	dword ptr cs:polyixy[12],eax
	mov	cs:polyisides,4
	call	clipanypoly

	mov	ds,cs:_vbufseg
	mov	si,OFFSET polyxy
	mov	di,cs:polysides
	or	di,di
	jz	@@0
	dec	di
	jz	@@2

@@1:	mov	ax,cs:[si+0]
	mov	bx,cs:[si+2]
	mov	dx,cs:[si+4]
	mov	cx,cs:[si+6]
	call	drawline
	add	si,4
	dec	di
	jnz	@@1

@@2:	mov	ax,cs:[si+0]
	mov	bx,cs:[si+2]
	mov	dx,cs:polyxy[0]
	mov	cx,cs:polyxy[2]
	call	drawline

@@0:	pop	ds
	pop	di
	pop	si
	pop	bp
	ret
_asmbox ENDP

;################################################################

ALIGN 16
	
flip8 LABEL BYTE							
db 0,128,64,192,32,160,96,224,16,144,80,208,48,176,112,240,8,136,72,200
db 40,168,104,232,24,152,88,216,56,184,120,248,4,132,68,196,36,164,100
db 228,20,148,84,212,52,180,116,244,12,140,76,204,44,172,108,236,28,156
db 92,220,60,188,124,252,2,130,66,194,34,162,98,226,18,146,82,210,50,178
db 114,242,10,138,74,202,42,170,106,234,26,154,90,218,58,186,122,250,6
db 134,70,198,38,166,102,230,22,150,86,214,54,182,118,246,14,142,78,206
db 46,174,110,238,30,158,94,222,62,190,126,254,1,129,65,193,33,161,97,225
db 17,145,81,209,49,177,113,241,9,137,73,201,41,169,105,233,25,153,89,217
db 57,185,121,249,5,133,69,197,37,165,101,229,21,149,85,213,53,181,117,245
db 13,141,77,205,45,173,109,237,29,157,93,221,61,189,125,253,3,131,67,195
db 35,163,99,227,19,147,83,211,51,179,115,243,11,139,75,203,43,171,107,235
db 27,155,91,219,59,187,123,251,7,135,71,199,39,167,103,231,23,151,87,215
db 55,183,119,247,15,143,79,207,47,175,111,239,31,159,95,223,63,191,127,255

circles dw	8 dup(0)

pal2 LABEL WORD	
	db	 0, 0*7/9, 0
	db	10,10*7/9,10
	db	20,20*7/9,20
	db	30,30*7/9,30
	db	40,40*7/9,40
	db	50,50*7/9,50
	db	60,60*7/9,60
	db	30,30*7/9,30
	db	 0, 0*7/9, 0
	db	10,10*7/9,10
	db	20,20*7/9,20
	db	30,30*7/9,30
	db	40,40*7/9,40
	db	50,50*7/9,50
	db	60,60*7/9,60
	db	30,30*7/9,30

pal1 LABEL WORD
	db	30,30*8/9,30
	db	60,60*8/9,60
	db	50,50*8/9,50
	db	40,40*8/9,40
	db	30,30*8/9,30
	db	20,20*8/9,20
	db	10,10*8/9,10
	db	 0, 0*8/9, 0
	db	30,30*8/9,30
	db	60,60*8/9,60
	db	50,50*8/9,50
	db	40,40*8/9,40
	db	30,30*8/9,30
	db	20,20*8/9,20
	db	10,10*8/9,10
	db	 0, 0*8/9, 0
	
sinuspower db	0
powercnt db	0
PUBLIC _power0
_power0	LABEL WORD
power0	db	256 dup(0)
	db	256 dup(0)
	db	256 dup(0)	
	db	256 dup(0)
	db	256 dup(0)
	db	256 dup(0)
	db	256 dup(0)
	db	256 dup(0)
	db	256 dup(0)
	db	256 dup(0)
	db	256 dup(0)
	db	256 dup(0)
	db	256 dup(0)
	db	256 dup(0)
	db	256 dup(0)
	db	256 dup(0)
	
PLANE	MACRO pl
	mov	dx,3c4h
	mov	ax,0002h+pl*100h
	out	dx,ax
	ENDM

bltline PROC NEAR
	push	si
	mov	dx,3c4h
	mov	al,2
	out	dx,al
	inc	dx
@@1:	mov	al,ch
	out	dx,al
	zzz=0
	REPT	10
	mov	eax,ds:[si+zzz]
	mov	es:[di+zzz],eax
	zzz=zzz+4
	ENDM
	add	si,40
	shl	ch,1
	dec	cl
	jnz	@@1
	pop	si
	ret
bltline ENDP

bltlinerev PROC NEAR
	push	si
	mov	dx,3c4h
	mov	al,2
	out	dx,al
	inc	dx
	xor	bx,bx
@@1:	mov	al,ch
	out	dx,al
	zzz=0
	REPT	10
	mov	bl,ds:[si+36-zzz]
	mov	al,cs:flip8[bx]
	rol	eax,8
	mov	bl,ds:[si+37-zzz]
	mov	al,cs:flip8[bx]
	rol	eax,8
	mov	bl,ds:[si+38-zzz]
	mov	al,cs:flip8[bx]
	rol	eax,8
	mov	bl,ds:[si+39-zzz]
	mov	al,cs:flip8[bx]
	mov	es:[di+zzz],eax
	zzz=zzz+4
	ENDM
	add	si,40
	shl	ch,1
	dec	cl
	jnz	@@1
	pop	si
	ret
bltlinerev ENDP

resetmode13 PROC NEAR
	mov	ax,13
	int	10h
	mov	dx,3dah
	in	al,dx
	mov	dx,3c0h
	xor	al,al
	REPT 16
	out	dx,al
	out	dx,al
	inc	al
	ENDM
	mov	al,11h
	out	dx,al
	mov	al,255
	out	dx,al
	mov	al,32
	out	dx,al
	;clear pal
	mov	dx,3c8h
	xor	al,al
	out	dx,al
	inc	dx
	mov	cx,768
@@clp:	out	dx,al
	loop	@@clp
	ret
resetmode13 ENDP

outpal	PROC NEAR
	mov	dx,3c8h
	out	dx,al
	mov	ax,cs
	mov	ds,ax
	inc	dx
	REPOUTSB
	ret
outpal	ENDP

waitb	PROC NEAR
	mov	bx,1
	int	0fch
	ret
waitb	ENDP

rotate1	PROC NEAR
	xor	si,si
	mov	cx,32000/32-2
	cld
	jmp	@@2
	;edx.eax
@@1:	popf
	zzz=0
	REPT 16
	mov	ax,ds:[si+zzz]
	rcr	al,1
	rcr	ah,1
	mov	es:[si+zzz],ax
	zzz=zzz+2
	ENDM
@@2:	pushf
	add	si,zzz
	dec	cx
	jz	@@0
	jmp	@@1
@@0:	popf
	ret
rotate1	ENDP

ALIGN 2
framecount dw	0
palanimc dw	0
palanimc2 dw	0
scrnpos dw	0
scrnposl dw	0
scrnx	dw	0
scrny	dw	0
scrnrot dw	0
sinurot dw	0
overrot dw	211
overx	dw	0
overya	dw	0
patdir	dw	0

memseg	dw	0

init_interference PROC NEAR
	mov	dx,3d4h
	mov	ax,2813h
	out	dx,ax
	
	mov	bx,20+100*80
	
	;get mem for circles
	mov	ah,48h
	mov	bx,16384
	int	21h
	mov	cs:memseg,ax
	zzz=0
	REPT 8
	mov	cs:circles[zzz],ax
	add	ax,2048
	zzz=zzz+2
	ENDM
	
	mov	ax,SEG _circle2
	mov	ds,ax
	xor	si,si
	mov	ax,0a000h
	mov	es,ax
	mov	cx,200
	xor	di,di
	mov	bp,80*399
@@1:	push	cx
	push	di
	mov	cx,0401h
	call	bltline
	add	di,40
	mov	cx,0401h
	call	bltlinerev
	add	di,40
	mov	di,bp
	mov	cx,0401h
	call	bltline
	add	di,40
	mov	cx,0401h
	call	bltlinerev
	add	di,40
	pop	di
	add	di,80
	sub	bp,80
	add	si,40
	pop	cx
	loop	@@1
	
	mov	dx,3ceh
	mov	ax,0204h
	out	dx,ax
	mov	cx,400
	mov	es,cs:circles[0]
	mov	ax,0a000h
	mov	ds,ax
	mov	cx,32000/4
	xor	si,si
	xor	di,di
	rep	movsd
	zzz=0
	REPT	7	
	mov	ds,cs:circles[zzz]
	mov	es,cs:circles[zzz+2]
	call	rotate1
	zzz=zzz+2
	ENDM

	mov	ax,SEG _circle
	mov	ds,ax
	xor	si,si
	mov	ax,0a000h
	mov	es,ax
	mov	cx,200
	xor	di,di
	mov	bp,80*399
@@10:	push	cx
	push	di
	mov	cx,0103h ;start at plane 1, copy 3 planes
	call	bltline
	add	di,40
	mov	cx,0103h ;start at plane 1, copy 3 planes
	call	bltlinerev
	add	di,40
	mov	di,bp
	mov	cx,0103h ;start at plane 1, copy 3 planes
	call	bltline
	add	di,40
	mov	cx,0103h ;start at plane 1, copy 3 planes
	call	bltlinerev
	add	di,40
	pop	di
	add	di,80
	sub	bp,80
	add	si,40*3
	pop	cx
	loop	@@10
	mov	cs:framecount,0
	ret
init_interference ENDP

do_interference PROC NEAR
@@aga:	call	waitb
	mov	dx,3c0h
	mov	al,13h
	out	dx,al
	mov	al,byte ptr cs:scrnposl
	out	dx,al
	mov	al,32
	out	dx,al
	
	mov	si,cs:palanimc
	add	si,cs:patdir
	cmp	si,0
	jge	@@a11
	mov	si,8*3-3
@@a11:	cmp	si,8*3
	jb	@@a1
	xor	si,si
@@a1:	mov	cs:palanimc,si
	mov	cs:palanimc2,si

	mov	si,cs:palanimc
	add	si,OFFSET pal1
	xor	al,al
	mov	cx,8*3
	call	outpal
	mov	si,cs:palanimc
	add	si,OFFSET pal2
	mov	al,8
	mov	cx,8*3
	call	outpal

	PLANE 8
	mov	ax,0a000h
	mov	es,ax
	xor	si,si
	mov	di,cs:scrnpos
	mov	bp,cs:sinurot
	add	bp,7*2
	and	bp,2047
	mov	cs:sinurot,bp
	mov	cx,200
@@cp1:	zzz=0
	push	si
	add	bp,9*2
	and	bp,2047
	mov	bx,cs:_sin1024[bp]
	sar	bx,3
	mov	bh,cs:sinuspower
	movsx	ax,byte ptr cs:power0[bx]
	sub	ax,cs:scrnposl
	add	ax,cs:overx
	mov	bx,ax
	and	bx,7
	shl	bx,1
	neg	bx
	mov	ds,cs:circles[bx+7*2]
	sar	ax,3
	add	si,ax
	add	si,cs:overya
	REPT	40/4+1
	mov	eax,ds:[si+zzz]
	mov	es:[di+zzz],eax
	zzz=zzz+4
	ENDM
	pop	si
	add	di,80
	add	si,80
	dec	cx
	jz	@@cp0
	jmp	@@cp1
@@cp0:
	;MOVE

	mov	bx,6
	int	0fch
	;bx=row
	and	bx,7
	cmp	bx,0
	jne	@@m1
	mov	cs:patdir,-3
@@m1:	cmp	bx,4
	jne	@@m2
	mov	cs:patdir,-3 ;-3
@@m2:
	mov	bx,cs:scrnrot
	add	bx,5
	and	bx,1023
	mov	cs:scrnrot,bx

	shl	bx,1
	mov	ax,cs:_sin1024[bx]
	sar	ax,2
	add	ax,160
	mov	cs:scrnx,ax
	add	bx,256*2
	and	bx,1024*2-1
	mov	ax,cs:_sin1024[bx]
	sar	ax,2
	add	ax,100
	mov	cs:scrny,ax

	mov	bx,cs:overrot
	add	bx,7
	and	bx,1023
	mov	cs:overrot,bx

	shl	bx,1
	mov	ax,cs:_sin1024[bx]
	sar	ax,2
	add	ax,160
	mov	cs:overx,ax
	add	bx,256*2
	and	bx,1024*2-1
	mov	ax,cs:_sin1024[bx]
	sar	ax,2
	add	ax,100
	mov	bx,80
	mul	bx
	mov	cs:overya,ax

	mov	ax,cs:scrnx
	mov	bx,ax
	and	ax,7
	mov	cs:scrnposl,ax
	mov	ax,80
	mul	cs:scrny
	sar	bx,3
	add	ax,bx
	mov	cs:scrnpos,ax

	mov	bx,cs:scrnpos
	mov	dx,3d4h
	mov	al,0ch
	mov	ah,bh
	out	dx,ax
	inc	al
	mov	ah,bl
	out	dx,ax

	cmp	cs:framecount,70*5
	jb	@@p1	
	inc	cs:powercnt
	cmp	cs:powercnt,16
	jb	@@p1
	mov	cs:powercnt,0
	cmp	cs:sinuspower,15
	jae	@@p1
	inc	cs:sinuspower
@@p1:
	inc	cs:framecount
	;cmp	cs:framecount,70*13
	;je	@@xx
 	mov	ax,0
	mov	bx,9
	int	0fch
	cmp	ax,925
	jae	@@xx
	mov	bx,2
	int	0fch
	or	ax,ax
	jz	@@aga
@@xx:	ret
do_interference ENDP

PUBLIC _initinterference
_initinterference PROC FAR
	push	bp
	mov	bp,sp
	push	si
	push	di
	push	ds
	
;	call	resetmode13

;@@wm1:	mov	bx,2
;	int	0fch
;	or	ax,ax
;	jnz	@@xit
;	mov	bx,6
;	mov	ax,0f1h
;	int	0fch
;	cmp	ax,0f1h
;	jne	@@wm1
	
	call	init_interference
	
	pop	ds
	pop	di
	pop	si
	pop	bp
	ret
_initinterference ENDP

PUBLIC _dointerference
_dointerference PROC FAR
	push	bp
	mov	bp,sp
	push	si
	push	di
	push	ds
	
	call	do_interference	
	
	mov	es,cs:memseg
	mov	ah,49h
	int	21h

	pop	ds
	pop	di
	pop	si
	pop	bp
	ret
_dointerference ENDP
	
PUBLIC _inittwk
_inittwk PROC FAR
	push	bp
	mov	bp,sp
	push	si
	push	di
	push	ds
	;clear palette
	mov	dx,3c8h
	xor	al,al
	out	dx,al
	inc	dx
	mov	cx,768
@@1:	out	dx,al
	loop	@@1
	;400 rows
	mov	dx,3d4h
	mov	ax,00009h
	out	dx,ax
	;tweak
	mov	dx,3d4h
	mov	ax,00014h
	out	dx,ax
	mov	ax,0e317h
	out	dx,ax
	mov	dx,3c4h
	mov	ax,0604h
	out	dx,ax
	;
	mov	dx,3c4h
	mov	ax,0f02h
	out	dx,ax
	mov	ax,0a000h
	mov	es,ax
	xor	di,di
	mov	cx,32768
	xor	ax,ax
	rep	stosw
	;
	pop	ds
	pop	di
	pop	si
	pop	bp
	ret
_inittwk ENDP

PUBLIC _lineblit
_lineblit PROC FAR
	push	bp
	mov	bp,sp
	push	si
	push	di
	push	ds
	mov	di,[bp+6]
	mov	es,[bp+8]
	mov	si,[bp+10]
	mov	ds,[bp+12]
	zpl=0
	REPT	4
	mov	dx,3c4h
	mov	ax,02h+(100h shl zpl)
	out	dx,ax
	zzz=0
	REPT	80/2
	mov	al,ds:[si+(zzz+0)*4+zpl]
	mov	ah,ds:[si+(zzz+1)*4+zpl]
	mov	es:[di+zzz],ax
	zzz=zzz+2
	ENDM
	zpl=zpl+1
	ENDM
	pop	ds
	pop	di
	pop	si
	pop	bp
	ret
_lineblit ENDP

PUBLIC _setpalarea
_setpalarea PROC FAR
	push	bp
	mov	bp,sp
	push	si
	push	di
	push	ds
	mov	si,[bp+6]
	mov	ds,[bp+8]
	mov	ax,[bp+10]
	mov	dx,3c8h
	out	dx,al
	inc	dx
	mov	cx,[bp+12]
	shl	cx,1
	add	cx,ax
	REPOUTSB
	pop	ds
	pop	di
	pop	si
	pop	bp
	ret
_setpalarea ENDP

code	ENDS
	END
	
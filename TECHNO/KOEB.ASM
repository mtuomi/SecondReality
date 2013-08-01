extrn _circle:byte
extrn _circle2:byte
code2 	SEGMENT para public 'CODE'
	ASSUME cs:code2
.386
LOCALS

include sin1024.inc

;################################################################

sizefade dw	0
rotspeed dw	0
palfader dw	0
palfader2 db	255
zumplane db	11h

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

pal	db	32*3 dup(0)

pal0 LABEL WORD
	db	0,30,40
	db	0,0 ,0 
	db	0,0 ,0 
	db	0,0 ,0 
	db	0,0 ,0 
	db	0,0 ,0 
	db	0,0 ,0 
	db	0,0 ,0 
	db	0,30,40
	db	0,0 ,0 
	db	0,0 ,0 
	db	0,0 ,0 
	db	0,0 ,0 
	db	0,0 ,0 
	db	0,0 ,0 
	db	0,0 ,0 

pal1 LABEL WORD
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

pal2 LABEL WORD	
	db	50,50*6/9,50
	db	60,60*6/9,60
	db	30,30*6/9,30
	db	 0, 0*6/9, 0
	db	10,10*6/9,10
	db	20,20*6/9,20
	db	30,30*6/9,30
	db	40,40*6/9,40
	db	50,50*6/9,50
	db	60,60*6/9,60
	db	30,30*6/9,30
	db	 0, 0*6/9, 0
	db	10,10*6/9,10
	db	20,20*6/9,20
	db	30,30*6/9,30
	db	40,40*6/9,40
	
sinuspower db	0
powercnt db	0
PUBLIC _power1
_power1	LABEL WORD
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
	mov	bx,1
	int	0fch
	mov	dx,3c8h
	mov	al,0
	out	dx,al
	inc	dx
	mov	cx,768
@@ccc:	out	dx,al
	loop	@@ccc
	mov	bx,1
	int	0fch
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

mixpal	PROC NEAR
	;cs:si=>cs:di, for cx
	push	dx
	cmp	dx,256
	jg	@@2
	mov	bx,dx
@@1:	mov	al,cs:[si]
	xor	ah,ah
	inc	si
	mul	bx
	shr	ax,8
	mov	cs:[di],al
	inc	di
	loop	@@1
	pop	dx
	ret
@@2:	mov	bx,dx
	sub	bx,256	
@@4:	mov	al,cs:[si]
	xor	ah,ah
	inc	si
	add	ax,bx
	cmp	ax,64
	jb	@@3
	mov	al,63
@@3:	mov	cs:[di],al
	inc	di
	loop	@@4
	pop	dx
	ret
mixpal ENDP

outpal	PROC NEAR
	mov	dx,3c8h
	out	dx,al
	mov	ax,cs
	mov	ds,ax
	inc	dx
	rep	outsb
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
overrot dw	0
overx	dw	0
overya	dw	0
patdir	dw	-3

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
	
	xor	al,al
	mov	si,OFFSET pal
	mov	cx,16*3
	call	outpal

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

	mov	dx,cs:palfader
	add	dx,2
	cmp	dx,512
	jb	@@pf1
	mov	dx,512
@@pf1:	mov	cs:palfader,dx
;
	mov	si,cs:palanimc
	add	si,OFFSET pal0
	mov	di,OFFSET pal
	mov	cx,8*3
	call	mixpal
	mov	si,cs:palanimc
	add	si,OFFSET pal0
	mov	di,OFFSET pal+8*3
	mov	cx,8*3
	call	mixpal

	mov	si,OFFSET pal
	mov	al,0
	mov	cx,16*3
	call	outpal

	jmp	@@OVER3
	mov	dx,3c4h
	mov	al,2
	mov	ah,cs:zumplane
	rol	ah,1
	mov	cs:zumplane,ah
	out	dx,ax

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
@@OVER3:
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

	cmp	cs:framecount,64
	jb	@@szf1
	inc	cs:rotspeed
	mov	ax,cs:sizefade
	cmp	ax,16834
	jge	@@1
	;add	ax,256
@@1:	mov	cs:sizefade,ax
@@szf1:
	shl	bx,1
	mov	ax,cs:_sin1024[bx]
	imul	cs:sizefade
	mov	ax,dx
	add	ax,160
	mov	cs:scrnx,ax
	add	bx,256*2
	and	bx,1024*2-1
	mov	ax,cs:_sin1024[bx]
	imul	cs:sizefade
	mov	ax,dx
	add	ax,100
	mov	cs:scrny,ax

	mov	bx,cs:overrot
	add	bx,cs:rotspeed
	and	bx,1023
	mov	cs:overrot,bx

	shl	bx,1
	mov	ax,cs:_sin1024[bx]
	sar	ax,2
	imul	cs:sizefade
	mov	ax,dx
	add	ax,160
	mov	cs:overx,ax
	add	bx,256*2
	and	bx,1024*2-1
	mov	ax,cs:_sin1024[bx]
	sar	ax,2
	imul	cs:sizefade
	mov	ax,dx
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

	cmp	cs:framecount,192
	jb	@@asd2
	test	cs:framecount,3
	jnz	@@asd2
@@asd2:	
;	cmp	cs:framecount,256
;	jb	@@p1
;	inc	cs:powercnt
;	cmp	cs:powercnt,16
;	jb	@@p1
;	mov	cs:powercnt,0
;	cmp	cs:sinuspower,15
;	jae	@@p1
;	inc	cs:sinuspower
@@p1:
	inc	cs:framecount
	cmp	cs:framecount,256
	je	@@xx
	mov	bx,2
	int	0fch
	or	ax,ax
	jz	@@aga
@@xx:	ret
do_interference ENDP

PUBLIC _dointerference2
_dointerference2 PROC FAR
	push	bp
	mov	bp,sp
	push	si
	push	di
	push	ds
	
	call	resetmode13
	call	init_interference
	call	do_interference	
	mov	es,cs:memseg
	mov	ah,49h
	int	21h
	
	pop	ds
	pop	di
	pop	si
	pop	bp
	ret
_dointerference2 ENDP
	
code2	ENDS
	END

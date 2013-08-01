;included to koe.asm

.386

include polyega.asm

STARS	equ	512
STARS2	equ	1024

starvram  dw	0a000h
emmhandle dw	0
emmseg	dw	0
emmpage4 dw	0
starlimit dw	0
startxtopen dw	0
startxtclose dw	0
startxtp0 dw	0
starframe dw	0
starpalfade db	0,0
_nostar1 dw	200
_nostar2 dw	199

rows1	dw	200 dup(0)
rows	dw	400 dup(0)

star	dw	STARS2 dup(0,0,0,0)

muldivx	dw	256 dup(0)
muldivy	dw	256 dup(0)

ALIGN 2
seed	dd	0 ;12345678h
random	PROC	NEAR
	push	edx
	mov	eax,0343fdh
	mul	cs:seed
	add	eax,269ec3h
	mov	cs:seed,eax
	mov	ax,dx
	pop	edx
	ret
random	ENDP

setborder PROC NEAR
	push	ax
	mov	dx,3dah
	in	al,dx
	mov	dx,3c0h
	mov	al,11h+20h
	out	dx,al
	pop	ax
	out	dx,al
	ret	
setborder ENDP

init_stars PROC NEAR
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
	mov	al,20h
	out	dx,al
	
	mov	dx,3d4h
	mov	al,9
	out	dx,al
	inc	dx
	in	al,dx
	and	al,127
	out	dx,al
	
	mov	ax,0a000h
	mov	es,ax
	xor	di,di
	mov	cx,32768
	xor	ax,ax
	rep	stosw
	
	mov	ah,43h
	mov	bx,32*2
	int	67h
	or	ah,ah
	jz	@@1
	mov	ax,4c00h
	int	21h
@@1:	mov	cs:emmhandle,dx
	
	mov	ah,41h
	int	67h
	mov	cs:emmseg,bx
	
	call	clearsbu

	mov	cx,200
	mov	ax,0
	mov	dx,320/8
	mov	bx,OFFSET rows1
@@4:	mov	cs:[bx],ax
	add	ax,dx
	add	bx,2
	loop	@@4
	
	mov	cx,400
	mov	ax,0
	mov	dx,320/8
	mov	bx,OFFSET rows
@@4b:	mov	cs:[bx],ax
	add	ax,dx
	add	bx,2
	loop	@@4b
	
	mov	ax,cs
	mov	es,ax
	
	mov	di,OFFSET star
	mov	cx,STARS2
@@3:	mov	al,cl
	dec	al
	xor	ah,ah
	stosw
	call	random
	and	ax,1023
	sub	ax,512
	stosw
	call	random
	and	ax,1023
	sub	ax,512
	stosw
	xor	ax,ax
	stosw
	loop	@@3
	
	mov	di,OFFSET muldivy
	mov	cx,256
	mov	bp,150
@@2b:	mov	dx,108
	xor	ax,ax
	div	bp
	shr	ax,1
	stosw
	add	bp,4
	loop	@@2b

	mov	di,OFFSET muldivx
	mov	cx,256
	mov	bp,150
@@2c:	mov	dx,144
	xor	ax,ax
	div	bp
	shr	ax,1
	stosw
	add	bp,4
	loop	@@2c

	mov	cs:starlimit,STARS
	mov	cs:starpalfade,0
	
	mov	cs:startxtopen,-9999
	mov	cs:startxtclose,10000

	mov	cx,100
@@sa:	push	cx
	call	starfetch0
	call	staradd
	pop	cx
	loop	@@sa
	ret
init_stars ENDP

deinit_stars PROC NEAR
	mov	ah,45h
	mov	dx,cs:emmhandle
	int	67h
	ret
deinit_stars ENDP

fetch4ax PROC NEAR
	shl	ax,1
	mov	bx,ax
	push	ax
	mov	ax,4400h
	shr	bx,2
	mov	dx,cs:emmhandle
	int	67h
	pop	ax
	and	ax,3
	shl	ax,12-4
	add	ax,cs:emmseg
	ret
fetch4ax ENDP

fetch4ax2 PROC NEAR
	shl	ax,1
	mov	bx,ax
	push	ax
	mov	ax,4401h
	shr	bx,2
	mov	dx,cs:emmhandle
	int	67h
	pop	ax
	and	ax,3
	shl	ax,12-4
	add	ax,cs:emmseg
	add	ax,400h
	ret
fetch4ax2 ENDP

fetch4ax3 PROC NEAR
	shl	ax,1
	mov	bx,ax
	push	ax
	mov	ax,4402h
	shr	bx,2
	mov	dx,cs:emmhandle
	int	67h
	pop	ax
	and	ax,3
	shl	ax,12-4
	add	ax,cs:emmseg
	add	ax,800h
	ret
fetch4ax3 ENDP

clearsbu PROC NEAR
	mov	cx,128
	mov	ax,0
@@1:	push	cx
	push	ax
	call	fetch4ax
	mov	es,ax
	mov	cx,4096/4*2
	xor	eax,eax
	xor	di,di
	rep	stosd
	pop	ax
	pop	cx
	inc	ax
	loop	@@1
	ret
clearsbu ENDP

starfetch0 PROC NEAR
	mov	ax,cs:emmpage4
	inc	ax
	and	ax,63
	mov	cs:emmpage4,ax
	call	fetch4ax
	mov	es,ax
	ret
starfetch0 ENDP

starfetch1 PROC NEAR
	mov	ax,cs:emmpage4
	add	ax,17
	and	ax,63
	call	fetch4ax2
	mov	fs,ax
	ret
starfetch1 ENDP

starfetch2 PROC NEAR
	mov	ax,cs:emmpage4
	add	ax,43
	and	ax,63
	call	fetch4ax3
	mov	gs,ax
	ret
starfetch2 ENDP

staradd PROC NEAR
	mov	ax,cs
	mov	ds,ax
	mov	ax,ds:starlimit
	or	ax,ax
	jz	@@11
	dec	ax
	mov	ds:starlimit,ax
@@11:	mov	bp,STARS
	mov	si,OFFSET star
@@1:	movzx	bx,ds:[si]
	sub	bl,2
	mov	ds:[si],bl
	jc	@@4
	cmp	bp,cs:starlimit
	jb	@@2
	shl	bx,1
	mov	ax,ds:[si+4]
	imul	ds:muldivy[bx]
	shld	dx,ax,2
	add	dx,100
	cmp	dx,99
	ja	@@2
	mov	cx,dx
	mov	ax,ds:[si+2]
	imul	ds:muldivx[bx]
	mov	bx,cx
	shld	dx,ax,2
	add	dx,160
	cmp	dx,319
	ja	@@2
	mov	cl,dl
	and	cl,7
	shr	dx,3
	shl	bx,1
	mov	bx,ds:rows1[bx]
	add	bx,dx
	mov	ch,80h
	shr	ch,cl
	mov	cl,ds:[si]
	cmp	cl,180
	jb	@@sc1
	or	byte ptr es:[bx],ch
	jmp	@@2
@@sc1:	cmp	cl,110
	jb	@@sc2
	or	byte ptr es:[bx+4096],ch
	jmp	@@2
@@sc2:	or	byte ptr es:[bx],ch
	or	byte ptr es:[bx+4096],ch
@@2:	add	si,8
	dec	bp
	jnz	@@1
	ret
@@4:	call	random
	and	ax,1023
	sub	ax,512
	mov	cs:[si+2],ax
	call	random
	and	ax,1023
	sub	ax,512
	mov	cs:[si+4],ax
	jmp	@@2
staradd ENDP

staradd2 PROC NEAR
	call	starfetch1
	mov	ax,cs
	mov	ds,ax
	mov	ax,ds:starlimit
	or	ax,ax
	jz	@@11
	sub	ax,4
	mov	ds:starlimit,ax
@@11:	mov	bp,STARS2
	mov	si,OFFSET star
@@1:	movzx	bx,ds:[si]
	sub	bl,2
	mov	ds:[si],bl
	jc	@@4
	cmp	bp,cs:starlimit
	jb	@@2
	shl	bx,1
	mov	ax,ds:[si+4]
	imul	ds:muldivy[bx]
	shld	dx,ax,2
	add	dx,100
	cmp	dx,99
	ja	@@2
	mov	cx,dx
	mov	ax,ds:[si+2]
	imul	ds:muldivx[bx]
	mov	bx,cx
	shld	dx,ax,2
	add	dx,160
	cmp	dx,319
	ja	@@2
	mov	cl,dl
	and	cl,7
	shr	dx,3
	shl	bx,1
	mov	bx,ds:rows1[bx]
	add	bx,dx
	mov	ch,80h
	shr	ch,cl
	mov	cl,ds:[si]
	cmp	cl,180
	jb	@@sc1
	or	byte ptr es:[bx],ch
	or	byte ptr fs:[bx],ch
	jmp	@@2
@@sc1:	cmp	cl,110
	jb	@@sc2
	or	byte ptr es:[bx+4096],ch
	or	byte ptr fs:[bx+4096],ch
	jmp	@@2
@@sc2:	or	byte ptr es:[bx],ch
	or	byte ptr es:[bx+4096],ch
	or	byte ptr fs:[bx],ch
	or	byte ptr fs:[bx+4096],ch
@@2:	add	si,8
	dec	bp
	jnz	@@1
	ret
@@4:	call	random
	and	ax,1023
	sub	ax,512
	mov	cs:[si+2],ax
	call	random
	and	ax,1023
	sub	ax,512
	mov	cs:[si+4],ax
	jmp	@@2
staradd2 ENDP

risetext PROC NEAR
	mov	ax,cs:startxtopen
	cmp	ax,99
	jge	@@12
	inc	ax
	mov	cs:startxtopen,ax
@@12:	mov	dx,cs:startxtclose
	cmp	dx,0
	jle	@@21
	dec	dx
	mov	cs:startxtclose,dx
@@21:	cmp	dx,ax
	jge	@@22
	mov	ax,dx
@@22:	;ax=startxtuse

	cmp	ax,0
	jg	@@tcc
	jmp	@@tc0
@@tcc:	cmp	ax,1
	jg	@@tnz
	mov	ax,2
@@tnz:	push	ax
	mov	di,150
	sub	di,ax
	mov	cs:_nostar2,ax
	mov	ax,80
	mul	di
	mov	di,ax
	mov	cs:_nostar1,di
	mov	ax,SEG _textpic
	mov	ds,ax
	mov	si,040h
	add	si,cs:startxtp0
	pop	cx
	dec	cx

	sub	di,40	
	mov	dx,3c4h
	mov	ax,0f02h
	out	dx,ax
	xor	eax,eax
	zzz=0
	REPT	40/4
	mov	es:[di+zzz],eax
	zzz=zzz+4
	ENDM
	add	di,80
	dec	cx
	jz	@@tc0c
	
	mov	dx,3c4h
	mov	ax,0f02h
	out	dx,ax
	mov	eax,0
	zzz=0
	REPT	40/4
	mov	es:[di+zzz],eax
	zzz=zzz+4
	ENDM
	add	di,80
	dec	cx
	jz	@@tc0
	dec	cx
	jz	@@tc0b

	mov	dx,3ceh
	mov	ax,0400h
	out	dx,ax
	mov	ax,01h+400h+800h
	out	dx,ax

@@tc1:	mov	dx,3c4h
	mov	ax,0102h+400h+800h
	out	dx,ax
	zzz=0
	REPT	40/4
	mov	eax,ds:[si+zzz]
	mov	es:[di+zzz],eax
	zzz=zzz+4
	ENDM
	add	si,40
	mov	dx,3c4h
	mov	ax,0202h+400h+800h
	out	dx,ax
	zzz=0
	REPT	40/4
	mov	eax,ds:[si+zzz]
	mov	es:[di+zzz],eax
	zzz=zzz+4
	ENDM
	add	si,40
	add	di,80
	dec	cx
	jz	@@tc0b
	jmp	@@tc1
	
@@tc0b:	mov	dx,3ceh
	mov	ax,0001h
	out	dx,ax

	mov	dx,3c4h
	mov	ax,0f02h
	out	dx,ax
	mov	eax,0
	zzz=0
	REPT	40/4
	mov	es:[di+zzz],eax
	zzz=zzz+4
	ENDM
	add	di,80

	dec	di
	mov	cs:_nostar2,di
@@tc0:	ret
@@tc0c:	mov	dx,3c4h
	mov	ax,0f02h
	out	dx,ax
	xor	eax,eax
	zzz=0
	REPT	80/4
	mov	es:[di+zzz],eax
	zzz=zzz+4
	ENDM
	ret
risetext ENDP

do_stars PROC NEAR
@@aga:	mov	al,0
	;call	setborder

	mov	dx,3d4h
	mov	ax,cs:starvram
	cmp	ax,0a000h
	je	@@sw1
	mov	cs:starvram,0a000h
	mov	ax,400ch
	jmp	@@sw0
@@sw1:	mov	cs:starvram,0a400h
	mov	ax,000ch
@@sw0:	out	dx,ax
	call	waitb
	
	mov	al,1
	;call	setborder
	
	cmp	cs:starpalfade,32
	ja	@@p1
	
	mov	bl,cs:starpalfade
	inc	bl
	mov	cs:starpalfade,bl
	shl	bl,3
	jnc	@@p2
	mov	bl,255
@@p2:
	mov	dx,3c8h
	xor	al,al
	out	dx,al
	inc	dx
	
	mov	al,0
	out	dx,al
	mov	al,0
	out	dx,al
	mov	al,0
	out	dx,al
	
	mov	al,25*70/100
	mul	bl
	mov	al,ah
	out	dx,al
	mov	al,31*70/100
	mul	bl
	mov	al,ah
	out	dx,al
	mov	al,38*70/100
	mul	bl
	mov	al,ah
	out	dx,al
	
	mov	al,45*56/100
	mul	bl
	mov	al,ah
	out	dx,al
	mov	al,58*56/100
	mul	bl
	mov	al,ah
	out	dx,al
	mov	al,69*56/100
	mul	bl
	mov	al,ah
	out	dx,al

	mov	al,67*64/100
	mul	bl
	mov	al,ah
	out	dx,al
	mov	al,84*64/100
	mul	bl
	mov	al,ah
	out	dx,al
	mov	al,99*64/100
	mul	bl
	mov	al,ah
	out	dx,al

	;-----

	mov	al,0
	out	dx,al
	mov	al,0
	out	dx,al
	mov	al,0
	out	dx,al

	mov	al,10
	out	dx,al
	mov	al,20
	out	dx,al
	mov	al,35
	out	dx,al

	mov	al,20
	out	dx,al
	mov	al,30
	out	dx,al
	mov	al,45
	out	dx,al

	mov	al,30
	out	dx,al
	mov	al,40
	out	dx,al
	mov	al,60
	out	dx,al
@@p1:	
	mov	ax,cs:starframe
	inc	ax
	mov	cs:starframe,ax
	cmp	ax,1200
	jne	@@st1
	mov	cs:startxtp0,80
	mov	cs:startxtopen,-256
	mov	cs:startxtclose,1500
@@st1:	cmp	ax,3200
	jne	@@st2
	mov	cs:startxtp0,101*80
	mov	cs:startxtopen,-256
	mov	cs:startxtclose,1500
@@st2:	cmp	ax,1500
	jne	@@st3
	mov	cs:starlimit,STARS2
@@st3:
	call	risetext

	call	starfetch0
	cmp	cs:starframe,1200
	ja	@@stz
	cmp	cs:starframe,900
	ja	@@stz0
	call	staradd
	jmp	@@stz0
@@stz:	call	staradd2
	;mov	al,1
	;call	setborder
@@stz0:
	mov	ax,es
	mov	ds,ax
	
	mov	ax,cs:starvram
	mov	es,ax
	xor	di,di
	mov	dx,3c4h
	mov	ax,0102h
	out	dx,ax
	mov	si,0
	mov	cx,100
@@3:	zzz=0
;	cmp	di,cs:_nostar1
;	jl	@@3g
;	cmp	di,cs:_nostar2
;	jg	@@3g
;	jmp	@@3s
@@3g:	REPT	40/4
	mov	eax,ds:[si+zzz]
	mov	es:[di+zzz],eax
	zzz=zzz+4
	ENDM
@@3s:	add	si,40
	add	di,80
	loop	@@3
	xor	di,di
	mov	si,4096
	mov	dx,3c4h
	mov	ax,0202h
	out	dx,ax
	mov	cx,100
@@7:	zzz=0
;	cmp	di,cs:_nostar1
;	jl	@@7g
;	cmp	di,cs:_nostar2
;	jg	@@7g
;	jmp	@@7s
@@7g:	REPT	40/4
	mov	eax,ds:[si+zzz]
	mov	es:[di+zzz],eax
	zzz=zzz+4
	ENDM
@@7s:	add	si,40
	add	di,80
	loop	@@7

	mov	ax,cs:emmpage4
	add	ax,32
	and	ax,63
	call	fetch4ax
	mov	ds,ax
	mov	di,200*40
	mov	si,99*40
	mov	dx,3c4h
	mov	ax,0102h
	out	dx,ax
	mov	cx,100
@@5:	zzz=0
;	cmp	di,cs:_nostar1
;	jl	@@5g
;	cmp	di,cs:_nostar2
;	jg	@@5g
;	jmp	@@5s
@@5g:	REPT	40/4
	mov	eax,ds:[si+zzz]
	mov	es:[di+zzz],eax
	zzz=zzz+4
	ENDM
@@5s:	sub	si,40
	add	di,80
	loop	@@5
	mov	di,200*40
	mov	si,99*40+4096
	mov	dx,3c4h
	mov	ax,0202h
	out	dx,ax
	mov	cx,100
@@8:	zzz=0
;	cmp	di,cs:_nostar1
;	jl	@@8g
;	cmp	di,cs:_nostar2
;	jg	@@8g
;	jmp	@@8s
@@8g:	REPT	40/4
	mov	eax,ds:[si+zzz]
	mov	es:[di+zzz],eax
	zzz=zzz+4
	ENDM
@@8s:	sub	si,40
	add	di,80
	loop	@@8
	
	mov	bx,2
	int	0fch
	or	ax,ax
	jz	@@aga
	ret
	
	mov	ax,0a000h
	mov	ds,ax
	xor	si,si
	mov	cx,40*400
	mov	dx,0
@@1:	lodsb
	shl	al,1
	adc	dx,0
	shl	al,1
	adc	dx,0
	shl	al,1
	adc	dx,0
	shl	al,1
	adc	dx,0
	shl	al,1
	adc	dx,0
	shl	al,1
	adc	dx,0
	shl	al,1
	adc	dx,0
	shl	al,1
	adc	dx,0
	loop	@@1
	int	3
	ret
do_stars ENDP

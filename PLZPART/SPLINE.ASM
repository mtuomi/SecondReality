	IDEAL
	MODEL large
	P386

EXTRN	C kx:word, C ky:word, C kz:word
EXTRN	C dis:word, C tx:word, C ty:word
EXTRN	C ls_kx:word, C ls_ky:word, C ls_kz:word

CODESEG

PUBLIC C getspl

LABEL	buu	WORD
INCLUDE 'rata.inc'

LABEL splinecoef WORD
INCLUDE "spline.inc"

PROC C getspl

	ARG	position:word

	push	ds bp si di

	mov	si, OFFSET buu
	push	cs
	pop	ds
	mov	di, [position]

	MASM

	;ds:si=pointer to spline
	;di=position in spline, add 256 for next point

	mov	ax,di
	shr	ax,8
	shl	ax, 4d
	add	si,ax
	and	di,255
	shl	di,1

	ccc=0
	REPT 8
	mov	ax,ds:[si+3*2*8+ccc]
	imul	cs:splinecoef[di]
	mov	bx,ax
	mov	cx,dx
	mov	ax,ds:[si+2*2*8+ccc]
	imul	cs:splinecoef[di+64*8]
	add	bx,ax
	adc	cx,dx
	mov	ax,ds:[si+1*2*8+ccc]
	imul	cs:splinecoef[di+128*8]
	add	bx,ax
	adc	cx,dx
	mov	ax,ds:[si+0*2*8+ccc]
	imul	cs:splinecoef[di+192*8]
	add	bx,ax
	adc	cx,dx
	shld	cx,bx,1
	push	cx
	ccc=ccc+2
	ENDM

	mov	ax,  SEG kx
	mov	ds, ax
	pop	cx
	mov	[ls_ky], cx
	pop	cx
	mov	[ls_kx], cx
	pop	cx
	mov	[kz], cx
	pop	cx
	mov	[ky], cx
	pop	cx
	mov	[kx], cx
	pop	cx
	mov	[dis], cx
	pop	cx
	mov	[ty], cx
	pop	cx
	mov	[tx], cx

	pop	di si bp ds
	ret
	IDEAL
ENDP

END
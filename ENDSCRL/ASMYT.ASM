	IDEAL
	MODEL large
	P386

CODESEG

PUBLIC C waitvr

PROC    C waitvr

	mov     dx, 03dah
@@loop1:
	in	al, dx
	test	al, 08h
	jnz	@@loop1
@@loop2:
	in	al, dx
	test	al, 08h
	jz	@@loop2

	ret
ENDP

PUBLIC C setstart

PROC	setstart

	ARG	start:word

	push	bp
	mov	bp,sp

	mov	bx, [start]
	mov	dx, 03d4h
	mov	al, 0dh
	mov	ah, bl
	out	dx, ax
	mov	al, 0ch
	mov	ah, bh
	out	dx, ax

	pop	bp
	ret
ENDP

PUBLIC C setrgbpalette

PROC	C setrgbpalette

	ARG	pal:word, r:word, g:word, b:word

	mov	dx, 3c8h
	mov	ax, [pal]
	out	dx, al

	inc	dx
	mov	ax, [r]
	out	dx, al
	mov	ax, [g]
	out	dx, al
	mov	ax, [b]
	out	dx, al

	ret
ENDP


ENDS

SEGMENT asdf byte use16 private 'FAR_DATA'
PUBLIC C font
LABEL font
INCLUDE 'fona.inc'
ENDS
END

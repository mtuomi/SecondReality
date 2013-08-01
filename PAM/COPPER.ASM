	IDEAL
	MODEL large
	P386

CODESEG

PUBLIC	C frame_count, C init_copper, C close_copper, C cop_pal, C do_pal

twait		dw	0
frame_count	dw	0
cop_pal		dd	0
do_pal		dw	0

PROC	close_copper

	mov	bx, 7
	mov	ax, 0
	mov	cx, 0
	mov	dx, 0
	int	0fch

	mov	bx, 7
	mov	ax, 1
	mov	cx, 0
	mov	dx, 0
	int	0fch

	mov	bx, 7
	mov	ax, 2
	mov	cx, 0
	mov	dx, 0
	int	0fch

	ret
ENDP

PROC	init_copper

	mov	bx, 7
	mov	ax, 1
	mov	cx, OFFSET copper3
	mov	dx, SEG copper3
	int	0fch

	mov	bx, 7
	mov	ax, 2
	mov	cx, OFFSET copper2
	mov	dx, SEG copper2
	int	0fch

	mov	bx, 7
	mov	ax, 0
	mov	cx, OFFSET copper3
	mov	dx, SEG copper3
	int	0fch

	ret
ENDP

PROC	copper2

	inc	[cs:frame_count]

	cmp	[cs:do_pal], 0d
	je	@@no_pal

	pusha
	push	ds
	lds	si, [cs:cop_pal]
	mov	cx, 768d
	mov	dx, 3c8h
	mov	al, 0d
	out	dx, al
	inc	dx
	rep	outsb
	mov	[cs:do_pal], 0d
	pop	ds
	popa
@@no_pal:
	retf
ENDP

PROC 	copper3
	retf
ENDP

END


	IDEAL
	MODEL large
	P386

CODESEG

PUBLIC	C frame_count, C init_copper, C close_copper, C cop_pal, C do_pal, C cop_start, C cop_scrl

twait		dw	0
frame_count	dw	0
old_int8	dw	0
		dw	0
cop_pal		dd	0
do_pal		dw	0
cop_start	dw	0
cop_scrl	dw	0


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
	mov	cx, OFFSET copper1
	mov	dx, SEG copper1
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

PROC	copper1

	mov	dx, 03d4h
	mov	al, 0dh
	mov	ah, [Byte cs:cop_start]
	out	dx, ax
	mov	al, 0ch
	mov	ah, [Byte cs:cop_start+1]
	out	dx, ax

	mov	dx, 3c0h
	mov	al, 33h
	out	dx, al
	mov	ax, [cs:cop_scrl]
	out	dx, al

	retf
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

PUBLIC C cop_dofade
cop_dofade	dw	0

PUBLIC C fadepal
fadepal		db	768*2 dup(?)

PUBLIC C cop_fadepal
cop_fadepal	dd	0

PROC 	copper3
	cmp	[cs:cop_dofade], 0d
	je	@@l1
	dec	[cs:cop_dofade]

	push	ds si di cx

	mov	[Word cs:cop_pal], OFFSET fadepal
	mov	[Word cs:cop_pal+2], SEG fadepal
	mov	[do_pal], 1d

	lds	si, [cs:cop_fadepal]
	mov	di, OFFSET fadepal
	mov	cx, 768/16d
@@l4:
	ccc=0
	REPT 	16
	mov	ax, [ds:si+ccc*2]
	add	[cs:di+ccc+768], al
	adc	[cs:di+ccc], ah
	ccc=ccc+1
	ENDM
	add	di, 16d
	add	si, 32d
	dec	cx
	jnz	@@l4

	pop	cx di si ds
@@l1:
	retf
ENDP


END


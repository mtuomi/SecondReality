	IDEAL
	MODEL large
	P386

CODESEG

mmask	dw	0102h,0202h,0402h,0802h,0102h,0202h,0402h,0802h
rmask	dw	0004h,0104h,0204h,0304h,0004h,0104h,0204h,0304h

PUBLIC C ascrolltext

PROC C ascrolltext

	ARG	scrl:word, text:dword

	push	ds es si di
	push	0a000h
	pop	es
	lds	si, [text]
	mov	cx, 0d
@@l1:

	mov	bx, cx
	add	bx, [scrl]
	and	bx, 3d
	shl	bx, 1d
	mov	ax, [cs:mmask+bx]
	mov	dx, 3c4h
	out	dx, ax
	mov	ax, [cs:rmask+bx]
	mov	dx, 3ceh
	out	dx, ax

	mov	di, cx
	add	di, [scrl]
	shr	di, 2d
@@l3:
	REPT	20
	add	si, 4d
	mov	bx, [si-4]
	cmp	bx, -1
	je	@@l2
	mov	ax, [si-2]
	xor	[es:bx+di], al
	ENDM
	jmp	@@l3
@@l2:
	inc	cx
	cmp	cx, 4d
	jne	@@l1

	pop	di si es ds
	ret
ENDP

PUBLIC C outline

mrol	dw	0

PROC	C outline

	ARG	src:dword, dest:dword

	push	ds es si di

	mov	[cs:mrol], 0802h
	mov	cx, 4d
@@l1:
	mov	dx, 3c4h
	mov	ax, [cs:mrol]
	out	dx, ax

	lds	si, [src]
	add	si, cx
	dec	si
	les	di, [dest]
	xor	ax, ax
	mov	[es:di-352], al
	mov	[es:di-352+176], al

	ccc=0
	REPT	75
	mov	al, [ds:si+ccc*640]
	mov	[es:di+ccc*352], al
	mov	[es:di+ccc*352+176], al
	ccc=ccc+1
	ENDM

	mov	ax, ds
	add	ax, 75*40
	mov	ds, ax

	ccc=0
	REPT	75
	mov	al, [ds:si+ccc*640]
	mov	[es:di+ccc*352+75*352], al
	mov	[es:di+ccc*352+75*352+176], al
	ccc=ccc+1
	ENDM

	shr	[cs:(Byte mrol+1)], 1d
	dec	cx
	jnz	@@l1


	pop	di si es ds
	ret
ENDP

END
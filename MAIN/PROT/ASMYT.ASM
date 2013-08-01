	IDEAL
	MODEL large
	P386

CODESEG

PUBLIC C randtau
LABEL randtau WORD

	db	1024 dup(0)

rptr  	dw	0
eino	dd	22a53341h

PUBLIC C arand
PROC C arand
	ARG	buu:word

	mov	bx, [cs:rptr]

	mov	eax, [cs:eino]
	add	eax, 0a753cd3dh
	add	bx, 2*4
	and	bx, 3ffh
	add	eax, [dword cs:bx+randtau]
	add	bx, 23*4
	and	bx, 3ffh
	add	eax, [dword cs:bx+randtau]
	sub	bx, 24*4
	and	bx, 3ffh
	mov	[dword cs:bx+randtau], eax
	mov	[cs:eino], eax
	mov	[cs:rptr],bx
	ret
ENDP

PUBLIC C arsetkey

PROC C arsetkey
	ARG    	ckey:dword

	push	ds si di

	mov	ax, cs
	mov	ds, ax
	mov	es, ax
	mov	si, OFFSET randtau
	mov	di, si
	cld
	lfs	bx, [ckey]
	mov	cx, 100h
	mov	dl, [fs:bx]
@@l1:
	lodsb
	xor	al, dl
	stosb
	inc	bx
	cmp	[BYTE PTR fs:bx], 0d
	jnz	@@l2
	lfs	bx, [ckey]
@@l2:
	rol	dl, 1d
	add	dl, [fs:bx]
	loop	@@l1

	pop	di si ds
	ret
ENDP
END
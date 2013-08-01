	IDEAL
	MODEL large
	P386

CODESEG

PUBLIC C copyline

PROC	C copyline

	ARG	from:dword, to:dword, count:word

	push	ds si di

	lds	si, [from]
	les	di, [to]
	mov	cx, [count]

	cld
	rep	movsw

	pop	di si ds
	ret
ENDP

END
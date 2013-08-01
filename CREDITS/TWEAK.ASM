		IDEAL
		MODEL large
		P386

CODESEG

PUBLIC	C tw_opengraph, C tw_putpixel, C tw_setpalette, C tw_setstart
PUBLIC	C tw_pictovmem, C tw_closegraph, C tw_waitvr, C tw_setrgbpalette
PUBLIC 	C tw_clrscr, C tw_setpalarea, C tw_getpixel, C tw_setsplit

PUBLIC		C scr_seg		; videomuistin alku segmentti

planetau	db	1,2,4,8
scr_seg		dw	0a000h

;	avaa 320x200 tweak tilan, 4 sivua, 4 planea

PROC	tw_opengraph

	mov	ax,13h
	int	10h

	mov	dx, 03c4h
	mov	ax, 0604h
	out	dx, ax			; chain4 off

	mov	ax, 0f02h
	out	dx, ax
	mov	dx, 0a000h
	mov	es, dx
	xor	di, di
	xor	ax, ax
	mov	cx, 8000h
	rep	stosw			; clear vmem

	mov	dx, 03d4h
	mov	ax, 0014h
	out	dx, ax   		; crtc long off

	mov	ax, 0e317h
	out	dx, ax			; crtc byte on

	mov	ax, 5013h
	out	dx, ax		   	; 640 wide

	mov	ax,0109h
	out	dx, ax

	mov	ax, 06418h		; linecompare
	out	dx, ax
	mov	ax, 0f07h
	out	dx, ax			; 8th bit

	mov	dx, 3c0h
	mov	al, 30h
	out	dx, al
	mov	al, 61h
	out	dx, al
	ret
ENDP

PROC    tw_clrscr

	mov	dx, 3c4h
	mov	ax, 0f02h
	out	dx, ax
	mov	dx, [cs:scr_seg]
	mov	es, dx
	xor	di, di
	xor	eax, eax
	mov	cx, 1000h
	rep	stosd			; clear vmem

	ret
ENDP

PROC	tw_closegraph

	mov	ax, 03h
	int	10h

	ret
ENDP

;	piirt„„ pisteen ruudulle

PROC	tw_putpixel

	ARG	x:word, y:word, color:word

	push	bp
	mov	bp, sp

	mov	ax, [cs:scr_seg]
	mov	es, ax

	mov	dx, 03c4h
	mov 	bx, [x]
	and 	bx, 03h
	mov	ah, [cs:planetau+bx]
	mov	al, 02h
	out 	dx, ax              ; select plane

	mov 	bx, [x]
	shr 	bx, 2
	mov	ax, [y]
	shl	ax, 4
	add 	bx, ax
	shl	ax, 2
	add	bx, ax

	mov cx, [color]
	mov	[es:bx], cl

	pop	bp
	ret
ENDP

PROC	tw_getpixel

	ARG	x:word, y:word

	push	bp
	mov	bp, sp

	mov	ax, [scr_seg]
	mov	es, ax

	mov	dx, 03ceh
	mov	ax, [x]
	and	ax, 03h
	mov	ah, al
	mov	al, 04h
	out	dx, ax				; select plane

	mov	ax, [y]
	shl	ax, 4
	mov 	bx, ax
	shl	ax, 1
	add 	bx, ax
	shl	ax, 2
	add	bx, ax
	mov	ax, [x]
	shr	ax, 2
	add	bx, ax

	xor	ax, ax
	mov	al,[es:bx]

	pop	bp
	ret
ENDP
;	vaihtaa koko paletin

PROC	tw_setpalette

	ARG	pal:dword

	push	bp
	mov	bp, sp
	push	si ds

	lds	si,[pal]
	cld
	mov	cx, 300h
	mov	dx, 03c8h
	mov	al, 0
	out	dx, al
	inc	dx

	rep	outsb

	pop	ds si
	pop	bp
	ret
ENDP

PROC	C tw_setpalarea

	ARG	pal:dword, start:word, cnt:word

	push	si ds

	lds	si,[pal]
	cld
	mov	cx, [cnt]
	mov	ax, cx
	add	cx, ax
	add	cx, ax
	mov	dx, 03c8h
	mov	ax, [start]
	out	dx, al
	inc	dx

	rep	outsb

	pop	ds si
	ret
ENDP

;	asettaa videomuistin alun

PROC	tw_setstart

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

; kopioi muistista unpacked 1bpl kuvan 4planen kuvaksi videomuistiin
; void tw_pictovmem(char far *pic, unsigned int vstart, unsigned int bytes);

PROC	tw_pictovmem

	ARG	pic:dword,to:word,len:word

	push	bp
	mov	bp,sp
	push	ds es si di

	mov	es, [scr_seg]
	mov	di, [to]
	lds	si, [pic]
	mov	cx, [len]

	mov    dx,03ceh
	mov    ax,4005h			; w-mode 0
	out    dx,ax

	mov	dx, 03c4h
	mov	al, 02h
	mov	ah, 1h
	out	dx, ax
	shr	cx, 2
@@l1:
	movsb
	add	si, 3
	loop	@@l1


	lds	si, [pic]
	add	si, 1d
	mov	di, [to]
	mov	cx, [len]

	mov	ax, 0202h
	out	dx, ax
	shr	cx, 2
@@l2:
	movsb
	add	si, 3
	loop	@@l2


	lds	si, [pic]
	add	si, 2d
	mov	di, [to]
	mov	cx, [len]

	mov	ax, 0402h
	out	dx, ax
	shr	cx, 2
@@l3:
	movsb
	add	si, 3
	loop	@@l3


	lds	si, [pic]
	add	si, 3d
	mov	di, [to]
	mov	cx, [len]

	mov	ax, 0802h
	out	dx, ax
	shr	cx, 2
@@l4:
	movsb
	add	si, 3
	loop	@@l4

	pop	di si es ds
	pop	bp
	ret
ENDP

PROC    tw_waitvr

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


PROC	C tw_setrgbpalette

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

PROC	C tw_setsplit

	ARG	start:word

	mov	dx, 3d4h
	mov	al, 18h			; linecompare
	mov	ah, [Byte start]
	out	dx, ax
	mov	al, 07h
	mov	ah, [Byte start+1]
	shl	ah, 4d
	and	ah, 10h
	or	ah, 0fh
	out	dx, ax			; 8th bit
	ret

ENDP



END

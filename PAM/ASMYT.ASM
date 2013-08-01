	IDEAL
	MODEL large
	P386

CODESEG

aseg	dw	0

;ntaulu	db	1000b,1100b,1110b,1111b
;taulu	db	1111b,0111b,0011b,0001b

ntaulu	db	0001b,0011b,0111b,1111b
taulu	db	1111b,1110b,1100b,1000b

ptaulu	db	1,2,4,8

PUBLIC	C init_uframe, C ulosta_frame

PROC	C init_uframe

	ARG	fseg:word

	mov	ax, [fseg]
	mov	[cs:aseg], ax
	ret
ENDP

PROC	C ulosta_frame

	ARG	scseg:word

	push	ds di si

	xor	di, di
	xor	si, si
	mov	ax, [scseg]
	mov	es, ax
	mov	ds, [cs:aseg]
	mov	dx, 03c4h		; map mask... 03c4h, 02..

@@looppi:
	lodsb
	cmp	al, 0d
	jg	@@outtaa
	je	@@over
@@skip:
	cbw
	sub	di, ax

	lodsb
	cmp	al, 0d
	jl	@@skip
	je	@@over

@@outtaa:
	dec	al
	jz	@@single

	cbw
	mov	bx, di
	and	bx, 3d
	add	bx, ax
	shr	bx, 2d
	or	bx, bx
	jz	@@samebyte
	cmp	bx, 1d
	je	@@twobytes

	mov	cx, ax
	mov	bx, di
	and	bx, 3d
	mov	ah, [cs:taulu+bx]
	mov	al, 2d
	out	dx, ax
	mov	al, [ds:si]
	mov	bx, di
	shr	bx, 2d
	mov	[es:bx], al

	push	di cx

	mov	ax, di
	add	cx, ax
	shr	cx, 2d
	sub	cx, bx
	dec	cx
	shr	di, 2d
	inc	di

	mov	ax, 0f02h
	out	dx, ax
	mov	al, [ds:si]
	mov	ah, al
	shr	cx, 1d
	jnc	@@l1
	stosb
@@l1:   jcxz	@@l2
	rep	stosw

@@l2:	pop	cx di
	add	di, cx
	mov	bx, di
	and	bx, 3d
	mov	ah, [cs:ntaulu+bx]
	mov	al, 2d
	out	dx, ax
	lodsb
	mov	bx, di
	shr	bx, 2d
	mov	[es:bx], al

	inc	di

	lodsb
	cmp	al, 0d
	jl	@@skip
	jg	@@outtaa
	je	@@over

@@twobytes:
	mov	bx, di
	and	bx, 3d
	mov	cl, [cs:taulu+bx]
	add	di, ax
	mov	bx, di
	and	bx, 3d
	mov	ch, [cs:ntaulu+bx]
	mov	ah, cl
	mov	al, 2d
	out	dx, ax
	mov	cl, [ds:si]
	mov	bx, di
	shr	bx, 2d
	mov	[es:bx-1], cl
	mov	ah, ch
	out	dx, ax
	mov	[es:bx], cl
	inc	di
	inc	si

	lodsb
	cmp	al, 0d
	jl	@@skip
	jg	@@outtaa
	je	@@over


@@samebyte:
	mov	bx, di
	and    	bx, 3d
	mov	cl, [cs:taulu+bx]
	add	di, ax
	mov	bx, di
	and	bx, 3d
	and	cl, [cs:ntaulu+bx]
	mov	ah, cl
	mov	al, 02h
	out	dx, ax
	lodsb
	mov	bx, di
	shr	bx, 2d
	mov	[es:bx], al
	inc	di

	lodsb
	cmp	al, 0d
	jl	@@skip
	jg	@@outtaa
	je	@@over


@@single:
	mov	bx, di
	and	bx, 3d
	mov	ah, [cs:ptaulu+bx]
	mov	al, 2d
	out	dx, ax
	lodsb
	mov	bx, di
	shr	bx, 2d
	mov	[es:bx], al
	inc	di

;-------------
	lodsb
	cmp	al, 0d
	jl	@@skip
	jg	@@outtaa
	je	@@over

@@over:
	mov	di, si
	shr	si, 4d
	mov	ax, ds
	add	ax, si
	and	di, 15d
	jz	@@exit
	inc	ax
@@exit:
	mov	[cs:aseg], ax

	pop     si di ds

	ret
ENDP

PROC	ruudulle

	dec	al
	jz	@@single

	cbw
	mov	bx, di
	and	bx, 3d
	add	bx, ax
	shr	bx, 2d
	or	bx, bx
	jz	@@samebyte
	cmp	bx, 1d
	je	@@twobytes

	mov	cx, ax
	mov	bx, di
	and	bx, 3d
	mov	ah, [cs:taulu+bx]
	mov	al, 2d
	out	dx, ax
	mov	al, [ds:si]
	mov	bx, di
	shr	bx, 2d
	mov	[es:bx], al

	push	di cx

	mov	ax, di
	add	cx, ax
	shr	cx, 2d
	sub	cx, bx
	dec	cx
	shr	di, 2d
	inc	di

	mov	ax, 0f02h
	out	dx, ax
	mov	al, [ds:si]
	mov	ah, al
	shr	cx, 1d
	jnc	@@l1
	stosb
@@l1:   jcxz	@@l2
	rep	stosw

@@l2:	pop	cx di
	add	di, cx
	mov	bx, di
	and	bx, 3d
	mov	ah, [cs:ntaulu+bx]
	mov	al, 2d
	out	dx, ax
	lodsb
	mov	bx, di
	shr	bx, 2d
	mov	[es:bx], al

	inc	di
	ret

@@twobytes:
	mov	bx, di
	and	bx, 3d
	mov	cl, [cs:taulu+bx]
	add	di, ax
	mov	bx, di
	and	bx, 3d
	mov	ch, [cs:ntaulu+bx]
	mov	ah, cl
	mov	al, 2d
	out	dx, ax
	mov	cl, [ds:si]
	mov	bx, di
	shr	bx, 2d
	mov	[es:bx-1], cl
	mov	ah, ch
	out	dx, ax
	mov	[es:bx], cl
	inc	di
	inc	si
	ret

@@samebyte:
	mov	bx, di
	and    	bx, 3d
	mov	cl, [cs:taulu+bx]
	add	di, ax
	mov	bx, di
	and	bx, 3d
	and	cl, [cs:ntaulu+bx]
	mov	ah, cl
	mov	al, 02h
	out	dx, ax
	lodsb
	mov	bx, di
	shr	bx, 2d
	mov	[es:bx], al
	inc	di
	ret

@@single:
	mov	bx, di
	and	bx, 3d
	mov	ah, [cs:ptaulu+bx]
	mov	al, 2d
	out	dx, ax
	lodsb
	mov	bx, di
	shr	bx, 2d
	mov	[es:bx], al
	inc	di
	ret
ENDP

END
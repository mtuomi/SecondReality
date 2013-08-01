ALIGN 2
public _prtt,_txtx,_txty,_prttcol
_txtx	dw	0
_txty	dw	0
_prttcol dw	0
public	_font3x5
_font3x5 dw	0,0
truerowsadd dw	0

prtmacro MACRO
	local	prt41,prt42,prt43,prt44,prt45
	push	di
	out	dx,al
	mov	al,byte ptr cs:_prttcol
	rcr	bl,1
	jnc	prt41
	mov	es:[di],al
prt41:	add	di,cs:truerowsadd
	rcr	bh,1
	jnc	prt42
	mov	es:[di],al
prt42:	add	di,cs:truerowsadd
	rcr	cl,1
	jnc	prt43
	mov	es:[di],al
prt43:	add	di,cs:truerowsadd
	rcr	ch,1
	jnc	prt44
	mov	es:[di],al
prt44:	add	di,cs:truerowsadd
	rcr	ah,1
	jnc	prt45
	mov	es:[di],al
prt45:	pop	di
	ENDM

_prtt	PROC FAR
	CBEG
	call	vidstart
	mov	ax,ds:_rowlen
	mov	cs:truerowsadd,ax
	mov	fs,cs:_font3x5[2]
	movpar	ds,1
	movpar	si,0
	mov	di,cs:_txty
	mov	ax,cs:truerowsadd
	mul	di
	mov	di,ax
	mov	dx,cs:_txtx
	shr	dx,2
	add	di,dx
	mov	dx,3c4h
	mov	al,02h
	out	dx,al
	inc	dx
	xor	bp,bp ;cnt

	mov	cx,256
prt3:	lodsb
	cmp	al,9 ;tab
	je	prt21
	cmp	al,0
	je	prt1x
	cmp	al,31
	ja	prt2
prt22:	jmp	prt10
prt1x:	jmp	prt1
prt21:	inc	bp
	inc	di
	test	bp,7
	jz	prt22
	jmp	prt21
prt2:	inc	bp
	push	cx
	mov	bl,al
	xor	bh,bh
	shl	bx,3
	mov	ah,fs:[bx+4]
	mov	cx,fs:[bx+2]
	mov	bx,fs:[bx+0]
	mov	al,01h
	prtmacro
	mov	al,02h
	prtmacro
	mov	al,04h
	prtmacro
	inc	di
	inc	cs:_txtx
	pop	cx
prt10:	dec	cx
	jz	prt1
	jmp	prt3
prt1:	CEND
_prtt	ENDP


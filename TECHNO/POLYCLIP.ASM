ALIGN 2

WMINY	dw	0
WMAXY	dw	199
WMINX	dw	0
WMAXX	dw	319

clip_x1	dw	0
clip_y1	dw	0
clip_x2	dw	0
clip_y2	dw	0
clipxy2 dw	32 dup(0,0) ;tmp storage for polyclip

clipanypoly PROC NEAR
	;polyisides/polyixy =>polysides/polyxy
	mov	ax,cs
	mov	ds,ax
	mov	cx,ds:polyisides
	cmp	cx,2
	jg	cap3
	cmp	cx,1
	je	cap4
	jcxz	cap0
	;line
	mov	eax,dword ptr ds:polyixy[0]
	mov	dword ptr ds:clip_x1,eax
	mov	eax,dword ptr ds:polyixy[4]
	mov	dword ptr ds:clip_x2,eax
cap2r:	call	clipliney
	cmp	ax,0
	jnz	cap0
	call	cliplinex
	cmp	ax,0
	jnz	cap0
	mov	eax,dword ptr ds:clip_x1
	mov	dword ptr ds:polyxy[0],eax
	mov	edx,dword ptr ds:clip_x2
	mov	dword ptr ds:polyxy[4],edx
	cmp	eax,edx
	je	cap2
	mov	word ptr ds:polysides,2
	ret
cap2:	mov	word ptr ds:polysides,1
	ret
cap4:	;dot
	mov	eax,dword ptr ds:polyixy[0]
	cmp	ax,cs:WMINX
	jl	cap0
	cmp	ax,cs:WMAXX
	jg	cap0
	ror	eax,16
	cmp	ax,cs:WMINY
	jl	cap0
	cmp	ax,cs:WMAXY
	jg	cap0
	ror	eax,16
	mov	dword ptr ds:polyxy,eax
	mov	word ptr cs:polysides,1
	ret
cap0:	;all clipped away
	mov	word ptr ds:polysides,0
	ret
cap3:	;polygon, first clip y, then x
	mov	si,cx
	shl	si,2
	sub	si,4
	mov	di,0
	mov	eax,dword ptr ds:polyixy[si]
	mov	dword ptr ds:clip_x1,eax
	mov	eax,dword ptr ds:polyixy[di]
	mov	dword ptr ds:clip_x2,eax
	call	clipliney
	;
	mov	cx,ds:polyisides
	xor	di,di
	xor	bx,bx
	mov	edx,80008000h
	jmp	cap35
cap32:	push	di
	push	bx
	push	cx
	push	edx
	mov	si,di
	sub	si,4
	mov	eax,dword ptr ds:polyixy[si]
	mov	dword ptr ds:clip_x1,eax
	mov	eax,dword ptr ds:polyixy[di]
	mov	dword ptr ds:clip_x2,eax
	call	clipliney
	pop	edx
	pop	cx
	pop	bx
	pop	di
cap35:	cmp	ax,0
	jnz	cap34
	mov	eax,dword ptr ds:clip_x1
	cmp	eax,edx
	je	cap33
	mov	dword ptr ds:clipxy2[bx],eax
	mov	edx,eax
	add	bx,4
cap33:	mov	eax,dword ptr ds:clip_x2
	cmp	eax,edx
	je	cap34
	mov	dword ptr ds:clipxy2[bx],eax
	mov	edx,eax
	add	bx,4
cap34:	add	di,4
	loop	cap32
	;
	mov	cx,bx
	shr	cx,2
	cmp	dword ptr ds:clipxy2[0],edx
	jne	cap31
	dec	cx
cap31:	mov	ds:polysides,cx
	
	cmp	cx,2
	jg	cap39
	cmp	cx,0
	je	cap38
	mov	eax,dword ptr ds:clipxy2[0]
	mov	dword ptr ds:clip_x1,eax
	mov	eax,dword ptr ds:clipxy2[4]
	mov	dword ptr ds:clip_x2,eax
	jmp	cap2r ;reclip the remaining line
cap38:	ret
cap39:
	mov	si,cx
	shl	si,2
	sub	si,4
	mov	di,0
	mov	eax,dword ptr ds:clipxy2[si]
	mov	dword ptr ds:clip_x1,eax
	mov	eax,dword ptr ds:clipxy2[di]
	mov	dword ptr ds:clip_x2,eax
	call	cliplinex
	;
	mov	cx,ds:polysides
	xor	di,di
	xor	bx,bx
	mov	edx,80008000h
	jmp	cbp35
cbp32:	push	di
	push	bx
	push	cx
	push	edx
	mov	si,di
	sub	si,4
	mov	eax,dword ptr ds:clipxy2[si]
	mov	dword ptr ds:clip_x1,eax
	mov	eax,dword ptr ds:clipxy2[di]
	mov	dword ptr ds:clip_x2,eax
	call	cliplinex
	pop	edx
	pop	cx
	pop	bx
	pop	di
cbp35:	cmp	ax,0
	jnz	cbp34
	mov	eax,dword ptr ds:clip_x1
	cmp	eax,edx
	je	cbp33
	mov	dword ptr ds:polyxy[bx],eax
	mov	edx,eax
	add	bx,4
cbp33:	mov	eax,dword ptr ds:clip_x2
	cmp	eax,edx
	je	cbp34
	mov	dword ptr ds:polyxy[bx],eax
	mov	edx,eax
	add	bx,4
cbp34:	add	di,4
	loop	cbp32
	;
	mov	cx,bx
	shr	cx,2
	cmp	dword ptr ds:polyxy[0],edx
	jne	cbp31
	dec	cx
cbp31:	mov	ds:polysides,cx

	ret
clipanypoly ENDP

clipcheck MACRO reg,min,max,flagreg,flagmin,flagmax
	local	l1,l2
	cmp	reg,min
	jge	l1
	or	flagreg,flagmin
l1:	cmp	reg,max
	jle	l2
	or	flagreg,flagmax
l2:	ENDM
clipmacro MACRO v1,v2,w1,w2,wl
	local	l1,l2
	push	bx
	mov	bx,wl
	mov	cx,w2
	sub	cx,w1
	jcxz	l1
	mov	bp,bx
	sub	bp,w1
	mov	ax,v2
	sub	ax,v1
	imul	bp 
	idiv	cx
	add	ax,v1
	mov	v1,ax
	mov	word ptr w1,bx
	jmp	l2
l1:	mov	ax,v1
	mov	word ptr w1,bx
l2:	pop	bx
	ENDM
cliplinex PROC NEAR
	;input line polyxy[SI]=>polyxy[DI]
	xor	bx,bx
	mov	ax,ds:clip_x1
	clipcheck ax,cs:WMINX,cs:WMAXX,bl,1,2
	mov	ax,ds:clip_x2
	clipcheck ax,cs:WMINX,cs:WMAXX,bh,1,2
	mov	al,bl
	and	al,bh
	jz	clpx1
	ret
clpx1:
	test	bl,1
	jz	clpx13
	clipmacro ds:clip_y1,ds:clip_y2,ds:clip_x1,ds:clip_x2,cs:WMINX
clpx13:	test	bl,2
	jz	clpx14
	clipmacro ds:clip_y1,ds:clip_y2,ds:clip_x1,ds:clip_x2,cs:WMAXX
clpx14:
	test	bh,1
	jz	clpx23
	clipmacro ds:clip_y2,ds:clip_y1,ds:clip_x2,ds:clip_x1,cs:WMINX
clpx23:	test	bh,2
	jz	clpx24
	clipmacro ds:clip_y2,ds:clip_y1,ds:clip_x2,ds:clip_x1,cs:WMAXX
clpx24:
	xor	ax,ax
	ret
cliplinex ENDP
clipliney PROC NEAR
	xor	bx,bx
	mov	ax,ds:clip_y1
	clipcheck ax,cs:WMINY,cs:WMAXY,bl,4,8
	mov	ax,ds:clip_y2
	clipcheck ax,cs:WMINY,cs:WMAXY,bh,4,8
	mov	al,bl
	and	al,bh
	jz	clpy1
	ret
clpy1:
	test	bl,4
	jz	clpy11
	clipmacro ds:clip_x1,ds:clip_x2,ds:clip_y1,ds:clip_y2,cs:WMINY
clpy11:	test	bl,8
	jz	clpy12
	clipmacro ds:clip_x1,ds:clip_x2,ds:clip_y1,ds:clip_y2,cs:WMAXY
clpy12:
	test	bh,4
	jz	clpy21
	clipmacro ds:clip_x2,ds:clip_x1,ds:clip_y2,ds:clip_y1,cs:WMINY
clpy21:	test	bh,8
	jz	clpy22
	clipmacro ds:clip_x2,ds:clip_x1,ds:clip_y2,ds:clip_y1,cs:WMAXY
clpy22:	
	xor	ax,ax
	ret
clipliney ENDP


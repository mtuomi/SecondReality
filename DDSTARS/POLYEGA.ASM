;borders - can be found in asm.asm at the tmp work area (size 8K = 1024 rows)

ALIGN 4
polysides dw	0
polyxy	dw	16 dup(0,0);

poly	PROC NEAR
	mov	ax,cs
	mov	ds,ax
	mov	ax,cs:starvram
	mov	es,ax
	jmp	polyf
poly	ENDP

;*** POLYF / POLYFT

ALIGN 4
borders dw	4096 dup(0)

feax	dd	0
fedx	dd	0

fleftaddl dd	0 ;+0
fleftaddh dw	0 ;+4
fleftrown dw	0 ;+6
fleftzb dd	0 ;+8
fleftze dd	0 ;+12
flefttx0 dw	0 ;+16
fleftty0 dw	0 ;+18
flefttxa dw	0 ;+20
flefttya dw	0 ;+22
fleftcnt dw	0 ;+24
fleftcnta dw	0 ;+26
fleftd3a dw	0 ;+28

frightaddl dd	0 ;+0
frightaddh dw	0 ;+4
frightrown dw	0 ;+6
frightzb dd	0 ;+8
frightze dd	0 ;+12
frighttx0 dw	0 ;+16
frightty0 dw	0 ;+18
frighttxa dw	0 ;+20
frighttya dw	0 ;+22
frightcnt dw	0 ;+24
frightcnta dw	0 ;+26
frightd3a dw	0 ;+28

finfolen dw	0
finfo0 dw	0
finfo1 dw	0
fwmaxy1 dw 	0

wminx	dw	0
wmaxx	dw	319
wminy	dw	0+100h
wmaxy	dw	399+100h

finfo	dw	32 dup(0,0,0,0,0,0,0,0)
		;x,y,zlo,zhi,tx,ty,0,0

polyf	PROC NEAR ;ONLY CONVEX POLYGONS - FAST?
	;input: polysides/polyxy
	;requirements:
	;es=vram
	;cpolysides>=4 (not checked)
	;color=set
	;**COPY/SEEK UPPERMOST**
	mov	ax,cs
	mov	gs,ax
	mov	ds,ax
	mov	cx,ds:polysides
	mov	ax,cx
	shl	ax,4 ;*16
	mov	ds:finfolen,ax
	add	ax,OFFSET finfo
	mov	ds:finfo1,ax
	mov	ax,gs:wmaxy
	inc	ax
	mov	ds:fwmaxy1,ax
	mov	edx,077770000h
	xor	bx,bx
	mov	si,OFFSET polyxy
	mov	di,OFFSET finfo
	mov	ds:finfo0,di
pfn1:	mov	eax,dword ptr ds:[si]
	cmp	eax,edx
	jg	pfn2
	mov	edx,eax
	mov	bx,di
pfn2:	mov	dword ptr ds:[di],eax
	add	si,4
	add	di,16
	loop	pfn1
	;[bx]=uppermost
	;**SETUP REGS**
	mov	ds:fleftrown,-32767
	mov	ds:frightrown,-32767
	mov	si,bx
	mov	di,bx
	mov	bp,ds:[si+2]
	mov	bx,OFFSET borders
	mov	ax,bp
	cmp	ax,gs:wminy
	jge	pfn35
	mov	ax,gs:wminy
pfn35:	mov	gs:[bx],ax
	add	bx,2
	mov	cx,16 ;max tmp count to avoid hanging on illegal polygons
	;eax=left
	;bx=pointer to borders[]
	;cx=count
	;edx=right
	;si=left
	;di=right
	;bp=y
pfn63:	push	cx
	push	bx
	
	cmp	bp,ds:fleftrown
	jl	pfn42
	push	edx
	push	di
	mov	di,si
	sub	di,16
	cmp	di,ds:finfo0
	jae	pfn41
	add	di,ds:finfolen
pfn41:	mov	bx,OFFSET fleftaddl
	call	polyfcalc
	add	cx,bp
	mov	ds:fleftrown,cx
	movzx	eax,word ptr ds:[si+0]
	mov	ebx,ds:fleftaddl
	mov	dx,ds:fleftaddh
	sar	dx,1
	rcr	ebx,1
	xor	bx,bx
	sub	eax,ebx
	sbb	ax,dx
	mov	si,di
	pop	di
	pop	edx
pfn42:
	cmp	bp,ds:frightrown
	jl	pfn52
	push	eax
	push	si
	mov	si,di
	add	di,16
	cmp	di,ds:finfo1
	jb	pfn51
	sub	di,ds:finfolen
pfn51:	mov	bx,OFFSET frightaddl
	call	polyfcalc
	add	cx,bp
	mov	ds:frightrown,cx
	movzx	edx,word ptr ds:[si+0]
	mov	ebx,ds:frightaddl
	mov	ax,ds:frightaddh
	sar	ax,1
	rcr	ebx,1
	xor	bx,bx
	sub	edx,ebx
	sbb	dx,ax
	pop	si
	pop	eax
	
pfn52:	mov	bx,ds:fleftrown
	mov	cx,ds:frightrown
	cmp	cx,bx
	jl	pfn61
	mov	cx,bx
pfn61:	sub	cx,bp
	pop	bx
	cmp	cx,0
	jg	pfn71
pfn6:	pop	cx
	cmp	bp,ds:fwmaxy1
	jg	pfn64
	cmp	si,di
	je	pfn64
	dec	cx
	jz	pfn64
	jmp	pfn63
pfn64:	mov	word ptr gs:[bx],-32767
	mov	si,OFFSET borders
	call	polyn_disp
	ret
pfn65:	;above screen
	;entering screen, cut.
	add	bp,cx
	push	bp
	push	cx
	cmp	bp,gs:wminy
	jl	pfn66
	sub	bp,cx
	mov	cx,gs:wminy
	sub	cx,bp
pfn66:	;
	movsx	ecx,cx
	ror	eax,16
	mov	ds:feax,eax
	ror	edx,16
	mov	ds:fedx,edx
	;
	mov	ax,ds:fleftaddh
	shl	eax,16
	mov	ax,word ptr ds:fleftaddl[2]
	imul	ecx
	add	ds:feax,eax
	;
	mov	ax,ds:frightaddh
	shl	eax,16
	mov	ax,word ptr ds:frightaddl[2]
	imul	ecx
	add	ds:fedx,eax
	;
	mov	eax,ds:feax
	ror	eax,16
	mov	edx,ds:fedx
	ror	edx,16
	mov	bp,cx
	pop	cx
	sub	cx,bp
	pop	bp
	cmp	cx,0
	jne	pfn6b
	jmp	pfn6
pfn6b:	mov	bp,gs:wminy
pfn71:	;process segment
	cmp	bp,gs:wminy
	jl	pfn65 ;above screen still
	add	bp,cx
	;clip max to maxy
	cmp	bp,ds:fwmaxy1
	jle	pfn72
	sub	bp,cx
	mov	cx,ds:fwmaxy1
	sub	cx,bp
	mov	bp,ds:fwmaxy1
pfn72:	cmp	cx,0
	jle	pfn6
	push	si
	push	di
	push	bp
	ror	ebx,16
	neg	cx
	mov	bx,cx
	ror	ebx,16
	mov	esi,ds:fleftaddl
	mov	edi,ds:frightaddl
	mov	bp,ds:fleftaddh
	mov	cx,ds:frightaddh
pfn7:	add	eax,esi
	adc	ax,bp
	add	edx,edi
	adc	dx,cx
	mov	gs:[bx],ax
	mov	gs:[bx+2],dx
	add	ebx,10004h
	jnc	pfn7
	pop	bp
	pop	di
	pop	si
	jmp	pfn6
polyfcalc: ;**** subroutine ****
	;calc slope for line [SI]->[DI] to [BX], returns CX=len
	mov	cx,ds:[di+2]
	sub	cx,bp ;ds:[si+2]
	jle	pfc1
	mov	ax,ds:[di+0]
	sub	ax,ds:[si+0]
	jl	pfc2
	xor	dx,dx
	div	cx
	mov	ds:[bx+4],ax
	xor	ax,ax
	div	cx
	mov	ds:[bx+2],ax
	;dec	cx
	ret
pfc1:	xor	cx,cx
	ret
pfc2:	neg	ax
	xor	dx,dx
	div	cx
	push	ax
	xor	ax,ax
	div	cx
	pop	dx
	neg	ax
	adc	dx,0
	neg	dx
	mov	ds:[bx+4],dx
	mov	ds:[bx+2],ax
	;dec	cx
	ret
polyf	ENDP

polyn_disp PROC NEAR
	;calc/load regs
	mov	si,OFFSET borders
	mov	bx,ds:[si]
	add	si,2
plnd3:	;draw hlines
	push	cx
	push	bx
	mov	ax,ds:[si]
	cmp	ax,-32767
	je	polyn_dispx
	mov	dx,ds:[si+2]
	call	hline1 ;must not change DS:SI!
	pop	bx
	pop	cx
	inc	bx
	add	si,8
	jmp	plnd3
polyn_dispx:
	pop	bx
	pop	cx
	mov	dx,3ceh
	mov	ax,0ff08h
	out	dx,ax
	ret
polyn_disp ENDP

ALIGN	4
leftside db	11111111b,01111111b,00111111b,00011111b,00001111b,00000111b,00000011b,00000001b
rightside db	10000000b,11000000b,11100000b,11110000b,11111000b,11111100b,11111110b,11111111b

ALIGN	4
hline1	PROC	NEAR
	;DS:SI must be preserved!
	;(ax,bx)-(dx,bx)
	sub	bx,256
	cmp	bx,199
	ja	hlixx
	
	cmp	ax,dx
	jl	hli1
	xchg	ax,dx
hli1:	inc	dx

	dec	dx
	cmp	dx,ax
	jg	hli21
hlixx:	ret
	
hli21:	cmp	ax,0
	jnl	hli2
	cmp	dx,0
	jl	hli0
	xor	ax,ax
hli2:	cmp	dx,cs:wmaxx
	jng	hliok
	cmp	ax,cs:wmaxx
	jg	hli0
	mov	dx,cs:wmaxx

hliok:	mov	di,ax
	sar	di,3
	mov	cx,dx
	sar	cx,3
	sub	cx,di
	shl	bx,1
	add	di,ds:rows[bx]

	mov	bp,7
	and	bp,ax
	mov	bl,ds:leftside[bp]
	mov	bp,7
	and	bp,dx
	mov	bh,ds:rightside[bp]

	cmp	cx,0
	je	hli30

	mov	dx,3ceh
	mov	al,8
	mov	ah,bl
	out	dx,ax
	
	mov	ah,es:[di]
	mov	byte ptr es:[di],255
	inc	di

	dec	cx
	jcxz	hli33
	
	mov	ah,0ffh
	out	dx,ax
	mov	ax,0ffffh
	test	di,1
	jz	hli32
	mov	es:[di],al
	inc	di
	dec	cx
hli32:	shr	cx,1
	rep	stosw
	adc	cx,cx
	rep	stosb
	
hli33:	mov	dx,3ceh
	mov	al,8
	mov	ah,bh
	out	dx,ax
	mov	ah,es:[di]
	mov	byte ptr es:[di],255
	
hli0:	ret
hli30:	;end and beg in same byte
	mov	dx,3ceh
	mov	al,8
	mov	ah,bh
	and	ah,bl
	out	dx,ax
	mov	al,es:[di]
	mov	byte ptr es:[di],255
	ret
hline1	ENDP

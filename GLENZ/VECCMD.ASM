
cmdo1max equ 15 ;##??
cmdo2	LABEL WORD ;commads 77##
	dw	OFFSET cmd700
	dw	OFFSET cmd701
	dw	OFFSET cmd702
	dw	OFFSET nrserr;cmd703
	dw	OFFSET nrserr;cmd704
	dw	OFFSET cmd705
	dw	OFFSET cmd706
	dw	OFFSET cmd707
	dw	OFFSET cmd708
	dw	OFFSET cmd709
	dw	OFFSET cmd70a
	dw	OFFSET cmd70b
	dw	OFFSET nrserr;cmd70c
	dw	OFFSET nrserr
	dw	OFFSET nrserr
	dw	OFFSET nrserr
	dw	OFFSET cmd710
	dw	OFFSET cmd711
	dw	OFFSET cmd712
	dw	OFFSET cmd713
	dw	OFFSET cmd714
	dw	OFFSET cmd715
	dw	OFFSET cmd716
	dw	OFFSET nrserr
	dw	OFFSET nrserr
	dw	OFFSET cmd719
	dw	OFFSET nrserr
	dw	OFFSET nrserr
	dw	OFFSET nrserr
	dw	OFFSET nrserr
	dw	OFFSET nrserr
	dw	OFFSET cmd71F
	dw	OFFSET cmd720
	dw	OFFSET cmd721
	dw	OFFSET cmd722
cmdo2max equ 22h

ALIGN 2
nrscount dw	0
nrsbase	 dw	0
docmd_dssi PROC NEAR
	push	cs:[nrscount]
	inc	ax
	mov	cs:nrscount,ax
nrsl:	;nodeloop, entered also by goto
	dec	cs:nrscount
	jz	nrsret
	lodsw
	cmp	ah,77h
	je	nrs77
	cmp	ah,cmdo1max
	ja	nrserr
	cmp	ah,0
	je	cmd00
	call	nrs_poly
	jmp	nrsl
nrs77:	cmp	al,cmdo2max
	ja	nrserr
	mov	bl,al
	xor	bh,bh
	shl	bx,1
	jmp	cs:cmdo2[bx]
nrsret:	pop	cs:[nrscount]
	ret

nrserr: ;error, unknown block etc!
	sub	si,2
	mov	ax,ds:[si]
	int	3
	mov	cx,16
nrserr1: push	cx
	forcesetborder 1
	forcesetborder 15
	pop	cx
	loop	nrserr1
	
cmd00:	mov	ax,1 ;add this object to OBJD list
	jmp	nrsret

cmd700: ;goto
	mov	si,ds:[si]
	jmp	nrsl

cmd701: ;call
	push	si
	push	ds
	mov	si,ds:[si]
	call	donrsnode
	pop	ds
	pop	si
	add	si,2
	jmp	nrsl

cmd702: ;call multiple
	lodsw
	cmp	ax,-1
	je	nr7c1
	push	si
	push	ds
	mov	si,ax
	call	donrsnode
	pop	ds
	pop	si
	jmp	cmd702
nr7c1:	jmp	nrsl

cmd705:	;call just one command (no node push/pop/etc)
	lodsw
	push	si
	push	ds
	add	ax,cs:nrsbase
	mov	si,ax
	mov	ax,1 ;do just one command
	call	docmd_dssi
	pop	ds
	pop	si
	jmp	nrsl

cmd70a:	;call multiple one command (no node push/pop/etc)
	mov	cx,ds:[si]
	add	si,2
cm70a2:	push	cx
	lodsw
	push	si
	push	ds
	add	ax,cs:nrsbase
	mov	si,ax
	mov	ax,1 ;do just one command
	call	docmd_dssi
	pop	ds
	pop	si
	pop	cx
	loop	cm70a2
	jmp	nrsl

cmd706:	;set base offset
	mov	ax,si
	sub	ax,2
	mov	cs:nrsbase,ax
	jmp	nrsl

cmd707: ;goto/relative
	mov	si,ds:[si]
	add	si,cs:nrsbase
	jmp	nrsl

cmd708: lodsw
	or	cs:noflags,ax
	jmp	nrsl
	
cmd709: ;call NRS header equipped node
	push	si
	push	ds
	mov	si,ds:[si]
	call	donrswheader
	pop	ds
	pop	si
	add	si,2
	jmp	nrsl

cmd712: ;translate and move
cmd710: ;translate
	push	ax
	mov	di,OFFSET nmatrix
	call	rotateposition ;si=>rotatedx,y,z
	mov	eax,cs:rotatedx
	add	cs:nxpos,eax
	mov	eax,cs:rotatedy
	add	cs:nypos,eax
	mov	eax,cs:rotatedz
	add	cs:nzpos,eax

	mov	ebp,ds:[si+4]
	mov	dword ptr ds:[si+4],0
	mov	di,OFFSET nmatrix
	call	rotateposition ;si=>rotatedx,y,z
	mov	eax,cs:rotatedx
	add	cs:nxspos,eax
	mov	eax,cs:rotatedy
	add	cs:nyspos,eax
	mov	eax,cs:rotatedz
	add	cs:nzspos,eax
	mov	ds:[si+4],ebp

	add	si,12
	pop	ax
	cmp	al,12h
	jne	nr7d2
	mov	cx,cs:_framestaken
cm710b:	mov	eax,ds:[si]
	add	ds:[si-3*4],eax	
	mov	eax,ds:[si+4]
	add	ds:[si-2*4],eax	
	mov	eax,ds:[si+8]
	add	ds:[si-1*4],eax	
	loop	cm710b
	add	si,12
nr7d2:	jmp	nrsl

cmd711: ;rotate ZR*YR*XR
	push	si
	push	ds
	mov	ax,ds:[si+0]
	mov	bx,ds:[si+2]
	mov	cx,ds:[si+4]
	cmp	ax,0
	jne	nr7e1
	cmp	bx,0
	jne	nr7e1
	cmp	cx,0
	jne	nr7e1
	jmp	nr7e2
nr7e1:	cmp	ax,ds:[si+6]
	jne	nr7e3
	cmp	bx,ds:[si+8]
	jne	nr7e3
	cmp	cx,ds:[si+10]
	jne	nr7e3
	mov	ax,ds
	mov	es,ax
	mov	di,si
	add	di,12
	jmp	nr7e4
nr7e3:	mov	ds:[si+6],ax
	mov	ds:[si+8],bx
	mov	ds:[si+10],cx
	mov	di,si
	add	di,12
	call	calcmatrix ;ds:si=src(rx,ry,rz) => ds:di=dest
	mov	ax,ds
	mov	es,ax
nr7e4:	mov	si,OFFSET nmatrix
	mov	ax,cs
	mov	ds,ax
	call	mulmatrices
nr7e2:	pop	ds
	pop	si
	add	si,6+6+9*2
	jmp	nrsl

cmd719: ;simplex precalced zsort jmp
	mov	cx,ds:[si]
	add	si,2
	mov	edx,32767
cm719b:	mov	ebx,ds:[si]
	shl	bx,pointshl
	push	edx
	mov	eax,dword ptr cs:point[PX+bx] ;X
	imul	eax
	mov	edi,eax
	mov	eax,dword ptr cs:point[PY+bx] ;Y
	imul	eax
	add	edi,eax
	mov	eax,dword ptr cs:point[PZ+bx] ;Z
	imul	eax
	add	edi,eax
	sar	edi,16
	mov	bx,di
	pop	edx
	cmp	bx,dx
	jg	cm719c
	mov	edx,ebx
cm719c:	add	si,4
	loop	cm719b
	shr	edx,16
	add	dx,cs:nrsbase
	mov	si,dx
	jmp	nrsl

cmd720: ;bounding box
	add	si,6
	call	initnrsobj
	jmp	nrsl

cmd721: ;points
	push	si
	push	ds
	mov	ax,cs
	mov	ds,ax
	mov	si,OFFSET nmatrix
	call	nwritematrix
	pop	ds
	pop	si
	mov	cx,ds:[si]
	add	si,2
	;calc
	push	cx
	push	ds
	push	si
	call	ncalcpoints ;cx,ds:si
	pop	si
	pop	ds
	pop	cx
	;add si
	shl	cx,1
	mov	dx,cx
	shl	cx,1
	add	cx,dx
	add	si,cx
	jmp	nrsl

cmd722: ;normals
	push	si
	push	ds
	mov	ax,cs
	mov	ds,ax
	mov	si,OFFSET nmatrix
	call	nwritematrix2
	pop	ds
	pop	si
	mov	cx,ds:[si]
	add	si,2
	;calc
	push	cx
	push	ds
	push	si
	call	ncalcpoints2 ;cx,ds:si
	pop	si
	pop	ds
	pop	cx
	;add si
	shl	cx,1
	mov	dx,cx
	shl	cx,1
	add	cx,dx
	add	si,cx
	jmp	nrsl

cmd70b: ;breakpoint
	int	3
	jmp	nrsl

cmd713: ;killer counter
	cmp	word ptr ds:[si],0
	jg	nr7k1
	mov	ax,0 ;do NOT add this object to OBJD list
	jmp	nrsret
nr7k1:	dec	word ptr ds:[si]
	add	si,2
	jmp	nrsl

cmd714: ;rotate ZR*YR*XR
	push	si
	push	ds
	mov	ax,ds
	mov	es,ax
	mov	di,si
	mov	si,OFFSET nmatrix
	mov	ax,cs
	mov	ds,ax
	call	mulmatrices
	pop	ds
	pop	si
	add	si,9*2
	jmp	nrsl

cmd715: ;far goto
	mov	bx,ds:[si]
	mov	ax,ds:[si+2]
	cmp	bx,0
	jne	cm715a
	cmp	ax,0
	jne	cm715a
	jmp	cmd00
cm715a:	mov	ds,ax
	mov	si,bx
	jmp	nrsl
	
cmd716: ;lightsource position
	push	ds
	push	si
	mov	di,OFFSET nmatrix
	call	rotateposition
	mov	eax,cs:rotatedx
	mov	ebx,cs:rotatedy
	mov	ecx,cs:rotatedz
	call	unify32
	shl	ax,6
	shl	bx,6
	shl	cx,6
	mov	cs:newlight[0],ax
	mov	cs:newlight[2],bx
	mov	cs:newlight[4],cx
	pop	si
	pop	ds
	add	si,12
	jmp	nrsl
	
cmd71F: ;set shadow level
	jmp	nrsl
docmd_dssi ENDP

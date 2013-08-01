include vasm.inc

;±±±±±±±±±±±±±±±±±±±±±±± DATA ±±±±±±±±±±±±±±±±±±±±±±±

include vdata.asm

;±±±±±±±±±±±±±±±±±±±±±±± CODE ±±±±±±±±±±±±±±±±±±±±±±±

text__vec SEGMENT para public 'CODE'
	ASSUME cs:text__vec

.386

include vnew.asm
include vmath.asm ;includes vmathsin.asm

PUBLIC _lightshift
_lightshift db	0

PUBLIC _csetmatrix
_csetmatrix PROC FAR
	CBEG
	LOADDS
	mov	eax,[bp+10]
	mov	ds:xadd,eax		
	mov	eax,[bp+14]
	mov	ds:yadd,eax		
	mov	eax,[bp+18]
	mov	ds:zadd,eax		
	mov	si,[bp+6]
	mov	ds,word ptr [bp+8]
	call	setmatrix
	CEND
_csetmatrix ENDP

PUBLIC _crotlist
_crotlist PROC FAR
	CBEG
	mov	si,[bp+10]
	mov	ds,word ptr [bp+12]
	mov	di,[bp+6]
	mov	es,word ptr [bp+8]
	call	rotlist
	CEND
_crotlist ENDP

PUBLIC _crotprojlist
_crotprojlist PROC FAR
	CBEG
	mov	si,[bp+10]
	mov	ds,word ptr [bp+12]
	mov	di,[bp+6]
	mov	es,word ptr [bp+8]
	call	rotprojlist
	CEND
_crotprojlist ENDP

PUBLIC _cprojlist
_cprojlist PROC FAR
	CBEG
	mov	si,[bp+10]
	mov	ds,word ptr [bp+12]
	mov	di,[bp+6]
	mov	es,word ptr [bp+8]
	call	projlist
	CEND
_cprojlist ENDP

PUBLIC setmatrix
setmatrix PROC FAR
	;ds:si=matrix
	movsx	eax,word ptr ds:[si+0]
	mov	dword ptr cs:[2+mtrm00],eax
	mov	dword ptr cs:[2+mtr600],eax
	movsx	eax,word ptr ds:[si+2]
	mov	dword ptr cs:[2+mtrm02],eax
	mov	dword ptr cs:[2+mtr602],eax
	movsx	eax,word ptr ds:[si+4]
	mov	dword ptr cs:[2+mtrm04],eax
	mov	dword ptr cs:[2+mtr604],eax
	movsx	eax,word ptr ds:[si+6]
	mov	dword ptr cs:[2+mtrm06],eax
	mov	dword ptr cs:[2+mtr606],eax
	movsx	eax,word ptr ds:[si+8]
	mov	dword ptr cs:[2+mtrm08],eax
	mov	dword ptr cs:[2+mtr608],eax
	movsx	eax,word ptr ds:[si+10]
	mov	dword ptr cs:[2+mtrm10],eax
	mov	dword ptr cs:[2+mtr610],eax
	movsx	eax,word ptr ds:[si+12]
	mov	dword ptr cs:[2+mtrm12],eax
	mov	dword ptr cs:[2+mtr612],eax
	movsx	eax,word ptr ds:[si+14]
	mov	dword ptr cs:[2+mtrm14],eax
	mov	dword ptr cs:[2+mtr614],eax
	movsx	eax,word ptr ds:[si+16]
	mov	dword ptr cs:[2+mtrm16],eax
	mov	dword ptr cs:[2+mtr616],eax
	ret
setmatrix ENDP

ALIGN 2
count	dw	0

rotlist PROC FAR
	;pointlist@DS:SI=>pointlist@ES:DI
	push	di
	LOADGS
	mov	cx,ds:[si]
	add	si,4
	mov	cs:count,cx
	mov	ax,es:[di]
	add	es:[di],cx
	mov	bx,ax
	shl	ax,4
	add	di,ax
	add	di,4

nrup1:	;rotate with matrix
	push	si
	push	di
	mov	ebp,ds:[si+8]
	mov	edi,ds:[si+4]
	mov	esi,ds:[si+0]
mtrm00:	mov	eax,12345678h
	imul	esi
	mov	ebx,eax
	mov	ecx,edx
mtrm02:	mov	eax,12345678h
	imul	edi
	add	ebx,eax
	adc	ecx,edx
mtrm04:	mov	eax,12345678h
	imul	ebp
	add	ebx,eax
	adc	ecx,edx
	shld	ecx,ebx,17
	push	ecx
mtrm06:	mov	eax,12345678h
	imul	esi
	mov	ebx,eax
	mov	ecx,edx
mtrm08:	mov	eax,12345678h
	imul	edi
	add	ebx,eax
	adc	ecx,edx
mtrm10:	mov	eax,12345678h
	imul	ebp
	add	ebx,eax
	adc	ecx,edx
	shld	ecx,ebx,17
	push	ecx
mtrm12:	mov	eax,12345678h
	imul	esi
	mov	ebx,eax
	mov	ecx,edx
mtrm14:	mov	eax,12345678h
	imul	edi
	add	ebx,eax
	adc	ecx,edx
mtrm16:	mov	eax,12345678h
	imul	ebp
	add	ebx,eax
	adc	ecx,edx
	shld	ecx,ebx,17
	pop	edx
	pop	eax
	pop	di
	pop	si
	;
	add	ecx,gs:xadd
	mov	es:[di+0],ecx
	add	edx,gs:yadd
	mov	es:[di+4],edx
	add	eax,gs:zadd
	mov	es:[di+8],eax
	;next point
	add	si,12
	add	di,12
	dec	cs:count
	jnz	nrup1
	pop	di
	ret
rotlist ENDP

projlist PROC FAR
	;pointlist@DS:SI=>projectedpointlist@ES:DI
	push	di
	LOADGS
	mov	cx,ds:[si]
	add	si,4
	mov	cs:count,cx
	mov	ax,es:[di]
	add	es:[di],cx
	mov	bx,ax
	shl	ax,4
	add	di,ax
	add	di,4

@@1:	;rotate with matrix
	mov	ebx,ds:[si+8]
	mov	eax,ds:[si+4]
	mov	ecx,ds:[si+0]
	
	;project ebx=z, eax=y, ecx=x

	xor	bp,bp
	mov	dword ptr es:[di+8],ebx
	cmp	ebx,gs:projminz
	jge	@@2
	mov	ebx,gs:projminz
	or	bp,16
@@2:	;
	mov	es:[di+14],ax
	imul	gs:projymul
	idiv	ebx
	add	ax,gs:projyadd
	cmp	ax,gs:wmaxy
	jng	@@41
	or	bp,8
@@41:	cmp	ax,gs:wminy
	jnl	@@42
	or	bp,4
@@42:	mov	es:[di+2],ax ;store Y
	;
	mov	es:[di+12],cx
	mov	eax,gs:projxmul
	imul	ecx
	idiv	ebx
	add	ax,gs:projxadd
	cmp	ax,gs:wmaxx
	jng	@@43
	or	bp,2
@@43:	cmp	ax,gs:wminx
	jnl	@@44
	or	bp,1
@@44:	mov	es:[di+0],ax ;store X

@@5:	mov	es:[di+4],bp ;store visiblity flags
	
	;next point
	add	si,12
	add	di,16
	dec	cs:count
	jnz	@@1
	pop	di
	ret
projlist ENDP

rotprojlist PROC FAR
	;pointlist@DS:SI=>pointlist@ES:DI
	push	di
	LOADGS
	mov	cx,ds:[si]
	add	si,4
	mov	cs:count,cx
	mov	ax,es:[di]
	add	es:[di],cx
	mov	bx,ax
	shl	ax,4
	add	di,ax
	add	di,4

@@1:	;rotate with matrix
	push	si
	push	di
	mov	ebp,ds:[si+8]
	mov	edi,ds:[si+4]
	mov	esi,ds:[si+0]
mtr600:	mov	eax,12345678h
	imul	esi
	mov	ebx,eax
	mov	ecx,edx
mtr602:	mov	eax,12345678h
	imul	edi
	add	ebx,eax
	adc	ecx,edx
mtr604:	mov	eax,12345678h
	imul	ebp
	add	ebx,eax
	adc	ecx,edx
	shld	ecx,ebx,17
	push	ecx
mtr606:	mov	eax,12345678h
	imul	esi
	mov	ebx,eax
	mov	ecx,edx
mtr608:	mov	eax,12345678h
	imul	edi
	add	ebx,eax
	adc	ecx,edx
mtr610:	mov	eax,12345678h
	imul	ebp
	add	ebx,eax
	adc	ecx,edx
	shld	ecx,ebx,17
	push	ecx
mtr612:	mov	eax,12345678h
	imul	esi
	mov	ebx,eax
	mov	ecx,edx
mtr614:	mov	eax,12345678h
	imul	edi
	add	ebx,eax
	adc	ecx,edx
mtr616:	mov	eax,12345678h
	imul	ebp
	add	ebx,eax
	adc	ecx,edx
	shld	ecx,ebx,17
	pop	edx
	pop	eax
	pop	di
	pop	si
	;
	add	ecx,gs:xadd
	add	edx,gs:yadd
	add	eax,gs:zadd

	mov	ebx,eax
	mov	eax,edx
	mov	dword ptr es:[di+8],ebx
	;ax=y,cx=x,bx=y
	;
	;mov	es:[di+14],ax
	imul	word ptr gs:projymul
	idiv	bx
	add	ax,gs:projyadd
	mov	es:[di+2],ax ;store Y
	;
	;mov	es:[di+12],cx
	mov	ax,word ptr gs:projxmul
	imul	cx
	idiv	bx
	add	ax,gs:projxadd
	mov	es:[di+0],ax ;store X

	;next point
	add	si,12
	add	di,16
	dec	cs:count
	jnz	@@1
	pop	di
	ret
rotprojlist ENDP

ALIGN 2
edgesoff dw	0
pointsoff dw	0
cntoff	dw	0

adddot PROC NEAR
	cmp	ax,bp
	je	@@3
	mov	bp,ax
	;add dot
	push	bx
	mov	bx,ax
	shl	bx,4
	add	bx,cs:pointsoff
	mov	eax,gs:[bx]
	stosd
	pop	bx
@@3:	ret
adddot ENDP

znorm	dw	0,0

checkhiddenbx PROC NEAR
	mov	cx,es:[bx-4]
	dec	cx
	push	bx
	xor	ax,ax
	mov	cs:znorm[0],ax
	mov	cs:znorm[2],ax
@@2:	mov	ax,es:[bx+0+0]
	mov	dx,es:[bx+0+2]
	sub	ax,es:[bx+4+0]
	add	dx,es:[bx+4+2]
	imul	dx
	add	cs:znorm[0],ax
	adc	cs:znorm[2],dx
	add	bx,4
	loop	@@2
	mov	ax,es:[bx+0]
	mov	dx,es:[bx+2]
	pop	bx
	sub	ax,es:[bx+0]
	add	dx,es:[bx+2]
	imul	dx
	add	ax,cs:znorm[0]
	adc	dx,cs:znorm[2]
	
	cmp	dx,0
	jl	@@1
	clc
	ret
@@1:	stc
	ret
checkhiddenbx ENDP

PUBLIC _cpolylist
_cpolylist PROC FAR ;polylist,polys,edges,points3
	CBEG
	mov	di,[bp+6]
	mov	es,word ptr [bp+8]
	mov	si,[bp+10]
	mov	ds,word ptr [bp+12]
	mov	ax,[bp+14]
	add	ax,4
	mov	cs:edgesoff,ax
	mov	fs,word ptr [bp+16]
	mov	ax,[bp+18]
	add	ax,4
	mov	cs:pointsoff,ax
	mov	gs,word ptr [bp+20]
	mov	bp,-1
@@2:	lodsw
	cmp	ax,0
	je	@@1
	add	di,2
	mov	cx,ax
	movsw
	mov	cs:cntoff,di
@@3:	push	cx
	mov	bx,ds:[si]
	add	si,2
	shl	bx,3
	add	bx,cs:edgesoff
	test	word ptr fs:[bx+4],8000h
	jnz	@@7
	mov	ax,fs:[bx+2]
	cmp	ax,bp
	je	@@5
	mov	ax,fs:[bx]
	call	adddot
	mov	ax,fs:[bx+2]
	call	adddot
@@7:	pop	cx
	loop	@@3
	jmp	@@6
@@5:	mov	ax,fs:[bx+2]
	call	adddot
	mov	ax,fs:[bx]
	call	adddot
	pop	cx
	loop	@@3
@@6:	mov	bx,cs:cntoff
	mov	eax,es:[bx]
	cmp	eax,es:[di-4]
	jne	@@4
	sub	di,4
@@4:	mov	ax,di
	sub	ax,cs:cntoff
	shr	ax,2
	mov	es:[bx-4],ax
	call	checkhiddenbx
	jnc	@@2
	xor	word ptr es:[bx-2],8000h
	jmp	@@2
@@1:	mov	word ptr es:[di],0
	CEND
_cpolylist ENDP

setpalxxx MACRO
	local	l1,l2,l3
	mov	al,cl
	cmp	al,64
	jb	l1
	mov	al,63
l1:	out	dx,al
	mov	al,bl
	cmp	al,64
	jb	l2
	mov	al,63
l2:	out	dx,al
	mov	al,bh
	cmp	al,64
	jb	l3
	mov	al,63
l3:	out	dx,al
	ENDM
	
demo_do	dw	OFFSET demo_hide

demo_norm PROC NEAR
	push	ax
	jc	@@7 ;visible
	mov	ax,es:[bx-2]
	;mov	ax,40h
	mov	es:[bx-2],ax
	pop	ax
	ret
@@7:	pop	ax
	ret
demo_norm ENDP

demo_hide PROC NEAR
	push	ax
	jc	@@7 ;visible
	xor	ax,ax
	mov	es:[bx-2],ax
	pop	ax
	ret
@@7:	pop	ax
	ret
demo_hide ENDP

demo_glz PROC NEAR
	jnc	@@7 ;visible
	push	ax
	mov	ax,es:[bx-2]
	shr	al,3
	and	al,1
	mov	es:[bx-2],ax
	pop	ax
	ret
@@7:	mov	cl,cs:_lightshift
	sub	cl,2
	shrd	ax,dx,cl
	cmp	ax,0
	jge	@@s1
	mov	ax,0
@@s1:	cmp	ax,63
	jle	@@s2
	mov	ax,63
@@s2:	mov	ah,al
	;
	mov	dx,3c8h
	mov	al,es:[bx-2]
	test	al,7
	jnz	@@xx
	out	dx,al
	inc	dx
	mov	cl,ah ;R
	mov	bl,ah ;G
	mov	bh,ah ;B
				;Inner1 Inner2 Back	
	setpalxxx		;-      -      -
	
	add	cl,16
	setpalxxx		;-      -      +
	sub	cl,16
	
	add	cl,16
	setpalxxx		;-      +      -
	sub	cl,16
	
	add	cl,16
	add	cl,16
	setpalxxx		;-      +      +
	sub	cl,16
	sub	cl,16

	add	bh,16
	setpalxxx		;+      -      -
	sub	bh,16
	
	add	cl,16
	add	bh,16
	setpalxxx		;+      -      +
	sub	bh,16
	sub	cl,16

	add	cl,16	
	add	bh,16
	setpalxxx		;+      +      -
	sub	bh,16
	sub	cl,16

	add	cl,16
	add	bl,16
	add	bh,16
	setpalxxx		;+      +      +
	add	bh,16
	add	bl,16
	sub	cl,16

@@xx:	ret
demo_glz ENDP

PUBLIC _ceasypolylist
_ceasypolylist PROC FAR ;polylist,polys,points3
	CBEG
	mov	di,[bp+6]
	mov	es,word ptr [bp+8]
	mov	si,[bp+10]
	mov	ds,word ptr [bp+12]
	mov	ax,[bp+14]
	add	ax,4
	mov	cs:pointsoff,ax
	mov	gs,word ptr [bp+16]
	mov	bp,-1
@@2:	lodsw
	cmp	ax,0
	je	@@1
	add	di,2
	mov	cx,ax
	movsw
	mov	cs:cntoff,di
@@3:	push	cx
	mov	ax,ds:[si]
	add	si,2
	call	adddot
	pop	cx
	loop	@@3
@@6:	mov	bx,cs:cntoff
	mov	eax,es:[bx]
	cmp	eax,es:[di-4]
	jne	@@4
	sub	di,4
@@4:	mov	ax,di
	sub	ax,cs:cntoff
	shr	ax,2
	mov	es:[bx-4],ax
	call	checkhiddenbx
;;;	call	demo_glz ;sets colors etc / hidden faces flipped
	call	demo_norm ;sets colors etc / hidden faces flipped
	jmp	@@2
@@1:	mov	word ptr es:[di],0
	CEND
_ceasypolylist ENDP

PUBLIC _cglenzinit
_cglenzinit PROC FAR
	CBEG
	LOADDS
	mov	ax,0
	call	__newgroup
	CEND
_cglenzinit ENDP

PUBLIC _cglenzdone
_cglenzdone PROC FAR
	CBEG
	LOADDS
	mov	ax,2
	call	__newgroup
	CEND
_cglenzdone ENDP

PUBLIC _cglenzpolylist
_cglenzpolylist PROC FAR
	CBEG
	LOADDS
	movpar	di,0
	movpar	es,1
	mov	ax,1
	call	__newgroup
	CEND
_cglenzpolylist ENDP

PUBLIC _cdrawpolylist
_cdrawpolylist PROC FAR
	CBEG
	LOADDS
	movpar	di,0
	mov	es,word ptr [bp+8]
	call	VIDPOLYGROUP
	CEND
_cdrawpolylist ENDP

adddotxy PROC NEAR
	cmp	eax,ebp
	je	@@3
	mov	ebp,eax
	stosd	;adddot
@@3:	ret
adddotxy ENDP

ALIGN 2
cllastdi dw	0
clipz	dd	4000

zclipsidi PROC NEAR ;CLASSIC, clip <cs:clipz
	;si=point1(end to clip), di=point2
	;returns new point xy (in eax)
	mov	ax,fs
	push	bx
	mov	eax,fs:[si+8]
	cmp	eax,cs:clipz
	jl	@@1
	;si is visible, just return it.
	mov	eax,fs:[si+0]
	pop	bx
	ret
@@2:	mov	eax,080008000h ;both hidden
	pop	bx
	ret
@@1:	mov	ecx,fs:[di+8]
	cmp	ecx,cs:clipz
	jl	@@2
	mov	edx,ecx
	sub	ecx,eax
	;cx=length of entire edge
	sub	edx,cs:clipz
	;dx=length of clipped edge
	xor	eax,eax
	div	ecx
	shr	eax,16
	mov	ecx,eax
	;ecx=multiplier (0..65535)
	mov	ax,fs:[si+12]
	sub	ax,fs:[di+12]
	movsx	eax,ax
	imul	ecx
	shld	edx,eax,16
	movsx	eax,word ptr fs:[di+12]
	add	edx,eax
	push	edx ;X
	mov	ax,fs:[si+14]
	sub	ax,fs:[di+14]
	movsx	eax,ax
	imul	ecx
	shld	edx,eax,16
	movsx	eax,word ptr fs:[di+14]
	add	edx,eax
	mov	eax,edx ;eax=Y

	;project ebx=z, eax=y, ecx=x

	imul	gs:projymul
	idiv	cs:clipz
	add	ax,gs:projyadd
	mov	bx,ax
	;
	pop	ecx ;ecx=X
	mov	eax,gs:projxmul
	imul	ecx
	idiv	cs:clipz
	add	ax,gs:projxadd
	
	shl	ebx,16
	mov	bx,ax
	mov	eax,ebx
	
	;eax=xy

	pop	bx
	ret
zclipsidi ENDP

xclipsidi PROC NEAR ;REVERSE, clip >cs:zclip
	;si=point1(end to clip), di=point2
	;returns new point xy (in eax)
	mov	ax,fs
	push	bx
	mov	eax,fs:[si+8]
	cmp	eax,cs:clipz
	jge	@@1
	;si is visible, just return it.
	mov	eax,fs:[si+0]
	pop	bx
	ret
@@2:	mov	eax,080008000h ;both hidden
	pop	bx
	ret
@@1:	mov	ecx,fs:[di+8]
	cmp	ecx,cs:clipz
	jge	@@2
	xchg	ecx,eax
	mov	edx,ecx
	sub	ecx,eax
	;cx=length of entire edge
	sub	edx,cs:clipz
	;dx=length of clipped edge
	xor	eax,eax
	div	ecx
	shr	eax,16
	mov	ecx,eax
	;ecx=multiplier (0..65535)
	mov	ax,fs:[di+12]
	sub	ax,fs:[si+12]
	movsx	eax,ax
	imul	ecx
	shld	edx,eax,16
	movsx	eax,word ptr fs:[si+12]
	add	edx,eax
	push	edx ;X
	mov	ax,fs:[di+14]
	sub	ax,fs:[si+14]
	movsx	eax,ax
	imul	ecx
	shld	edx,eax,16
	movsx	eax,word ptr fs:[si+14]
	add	edx,eax
	mov	eax,edx ;eax=Y

	;project ebx=z, eax=y, ecx=x

	imul	gs:projymul
	idiv	cs:clipz
	add	ax,gs:projyadd
	mov	bx,ax
	;
	pop	ecx ;ecx=X
	mov	eax,gs:projxmul
	imul	ecx
	idiv	cs:clipz
	add	ax,gs:projxadd
	
	shl	ebx,16
	mov	bx,ax
	mov	eax,ebx
	
	;eax=xy

	pop	bx
	ret
xclipsidi ENDP

eclipsidi PROC NEAR
	shl	si,4
	shl	di,4
	mov	ax,cs:pointsoff
	add	si,ax
	add	di,ax
	;
	push	si
	push	di
	call	xclipsidi
	pop	si
	pop	di
	push	eax
	call	xclipsidi
	mov	edx,eax
	pop	eax
	;returns: eax=si-end-of-line-xy
	;returns: edx=di-end-of-line-xy
	ret
eclipsidi ENDP

PUBLIC _cclipeasypolylist
_cclipeasypolylist PROC FAR ;polylist,polys,points3
	CBEG
	mov	di,[bp+6]
	mov	es,word ptr [bp+8]
	mov	si,[bp+10]
	mov	ds,word ptr [bp+12]
	mov	ax,[bp+14]
	add	ax,4
	mov	cs:pointsoff,ax
	mov	fs,word ptr [bp+16]
@@2:	lodsw
	cmp	ax,0
	je	@@1
	add	di,2
	mov	cx,ax
	movsw
	mov	cs:cntoff,di
	push	si
	add	si,cx
	add	si,cx
	mov	ax,ds:[si-2]
	mov	cs:cllastdi,ax
	pop	si
	mov	ebp,-1
@@3:	push	cx
	mov	ax,ds:[si]
	add	si,2
	push	si
	push	di
	push	ebp
	mov	di,ax
	mov	si,cs:cllastdi
	mov	cs:cllastdi,ax
	call	eclipsidi ;eax=first point, edx=second (0x80008000=no point)
	pop	ebp
	pop	di
	pop	si
	cmp	eax,080008000h
	je	@@8
	call	adddotxy ;doesn't destroy edx
@@8:	cmp	edx,080008000h
	je	@@7
	mov	eax,edx
	call	adddotxy
@@7:	pop	cx
	loop	@@3
@@6:	mov	bx,cs:cntoff
;	mov	eax,es:[bx]
;	cmp	eax,es:[di-4]
;	jne	@@4
;	sub	di,4
@@4:	mov	ax,di
	sub	ax,cs:cntoff
	shr	ax,2
	cmp	ax,0
	jne	@@10
	mov	di,cs:cntoff
	sub	di,4
	jmp	@@2
@@10:	mov	es:[bx-4],ax
	call	checkhiddenbx
	cmc
;;;	call	demo_glz ;sets colors etc / hidden faces flipped
;;;	call	demo_norm ;sets colors etc / hidden faces flipped
	call	cs:demo_do
	jmp	@@2
@@1:	mov	word ptr es:[di],0
	CEND
_cclipeasypolylist ENDP

PUBLIC _ceasymode
_ceasymode PROC FAR
	push	bp
	mov	bp,sp
	mov	ax,[bp+6]
	mov	dx,OFFSET demo_hide
	cmp	ax,1
	jne	@@1
	mov	dx,OFFSET demo_norm
@@1:	mov	cs:demo_do,dx
	pop	bp
	ret
_ceasymode ENDP

text__vec ENDS
	END
	
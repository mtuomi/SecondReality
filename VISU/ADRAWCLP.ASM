;/****************************************************************************
;** MODULE:	adrawclp.asm
;** AUTHOR:	Sami Tammilehto / Fennosoftec OY
;** DOCUMENT:	?
;** VERSION:	1.0
;** REFERENCE:	-
;** REVISED BY:	-
;*****************************************************************************
;**
;** Polygon clipping (included to adraw.asm)
;** All routines take DS:SI as an entry list and write a clipped list to
;** ES:DI. The clipnear also returns in AX the orred visvf for the clipped
;** polygon since it might change during Z clipping.
;**
;****************************************************************************/

ALIGN 16	
newclipflip db 0,0
newclipprog dw 0
clipcount dw	0
clipzero dw	0
clipvisfl dw	0

newclip_grd PROC NEAR
	;DS:BX->DS:SI is clipped at CX (BX end=0..16384=SI end)
	;resulted clip stored to ES:DI
	mov	ax,ds:[bx+POLYGR]
	sub	ax,ds:[si+POLYGR]
	imul	cx
	shld	dx,ax,2
	add	dx,ds:[si+POLYGR]
	mov	ds:[di+POLYGR],dx
	ret
	ENDP
	
NEWCLIPCALC MACRO clipped,other
	local	l1,l2,l3,l4,l5,l6
	;DS:BX->DS:SI is clipped at (-,BP) with result stored to DS:DI
	push	si
	push	bx
	push	bp
	mov	dx,ds:[si+other]
	mov	cx,ds:[si+clipped]
	mov	ax,ds:[bx+other]
	mov	bx,ds:[bx+clipped]
	mov	cs:newclipflip,0
	cmp	cx,bx
	jl	l3
	mov	cs:newclipflip,1
	xchg	cx,bx
	xchg	ax,dx
l3:	push	dx
	sub	bp,cx
	sub	ax,dx
	push	ax
	mov	dx,bp
	xor	ax,ax
	shrd	ax,dx,2
	sar	dx,2
	;dx:ax=short length*16384
	sub	bx,cx
	idiv	bx
	mov	bx,ax
	;bx=0..16384 multiplier 
	pop	ax
	imul	bx
	shld	dx,ax,2
	pop	ax
	add	dx,ax ;dx=clipped other
	pop	bp
	mov	cx,bp
	cmp	ds:[di+other-4],dx
	jne	l1
	cmp	ds:[di+clipped-4],cx
	jne	l1
	jmp	l2
l1:	mov	ds:[di+other],dx
	mov	ds:[di+clipped],cx
	mov	cx,bx
	pop	bx
	mov	ax,cs:newclipprog
	or	ax,ax
	jz	l4
	mov	dl,cs:newclipflip
	or	dl,dl
	jz	l6
	xchg	si,bx
l6:	call	ax ;also stores gouraud&texture clipped
l4:	add	di,4
	jmp	l5
l2:	pop	bx
l5:	pop	si
	ENDM

NEWCLIPNEXT MACRO ;sets bx(last)=si(current) and loads si with next vertex
	local	l1,l2
	mov	bx,si
	mov	dx,cs:clipcount
	dec	dx
	cmp	dx,1
	jl	@@0 ;list ended, jmp to end of clip procedure
	mov	cs:clipcount,dx
	jg	l1
	;1 vertex left, means we rotated the list
	mov	si,cs:clipzero
	jmp	l2
l1:	add	si,4
l2:	ENDM

NEWCLIPCOPY MACRO ;should copy also texture
	local	l1
	mov	eax,ds:[si+POLYX] ;&POLYY
	cmp	ds:[di+POLYX-4],eax
	je	l1
	mov	ds:[di+POLYX],eax
	mov	ax,ds:[si+POLYGR]
	mov	ds:[di+POLYGR],ax
	add	di,4
l1:	ENDM

;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	
ZCLIPCLIP MACRO
	local	l1,l41,l42,l43,l44
	;(fs:bp)-(fs:bx) is clipped at (-,-,ECX) with result stored to (DX,CX) & AX=visfl
	push	bx
	push	bp
	push	esi
	push	edi
	push	ecx
	;
	mov	eax,fs:[bx+vlist_z]
	mov	edx,fs:[bp+vlist_z]
	cmp	eax,edx
	jg	l1
	xchg	eax,edx
	xchg	bx,bp
l1:	;EAX=farther vertex (>EDX), BX=offset of farther vertex
	mov	esi,eax
	mov	edi,eax
	sub	esi,edx ;>=0 divver
	sub	edi,ecx ;>=0 muller, esi>edi
	;
	mov	eax,fs:[bp+vlist_x]
	sub	eax,fs:[bx+vlist_x]
	imul	edi
	idiv	esi
	add	eax,fs:[bx+vlist_x]
	push	eax
	;
	mov	eax,fs:[bp+vlist_y]
	sub	eax,fs:[bx+vlist_y]
	imul	edi
	idiv	esi
	add	eax,fs:[bx+vlist_y]
	;
	pop	esi ;X
	pop	ecx ;cliplimit
	xor	di,di
	;
	imul	gs:_projmuly
	idiv	ecx
	add	eax,gs:_projaddy
	cmp	eax,gs:_projclipy[CLIPMAX]
	jng	l41
	or	di,VF_DOWN
l41:	cmp	eax,gs:_projclipy[CLIPMIN]
	jnl	l42
	or	di,VF_UP
l42:	push	ax
	;
	mov	eax,esi
	imul	gs:_projmulx
	idiv	ecx
	add	eax,gs:_projaddx
	cmp	eax,gs:_projclipx[CLIPMAX]
	jng	l43
	or	di,VF_RIGHT
l43:	cmp	eax,gs:_projclipx[CLIPMIN]
	jnl	l44
	or	di,VF_LEFT
l44:	;
	mov	dx,ax
	mov	ax,di
	pop	cx
	;dx=x,cx=y,ax=visfl
	pop	edi
	pop	esi
	pop	bp
	pop	bx
	;NOTE: ECX was destroyed while clipping. It is used to return data.
	;It must be pushed by the 'calling' procedure after the macro and
	;the usage of the data the macro returned!
	ENDM

ZCLIPGETVX MACRO ;reads to bx the offset of next vertex from DS:SI (copies last vx to bp) 
	local	l1,l2
	mov	bp,bx
	mov	dx,cs:clipcount
	dec	dx
	cmp	dx,1
	jl	@@0 ;list ended, jmp to end of clip procedure
	mov	cs:clipcount,dx
	jg	l1
	;1 vertex left, means we rotated the list
	mov	si,cs:clipzero
	jmp	l2
l1:	add	si,4
l2:	mov	bx,ds:[si+POLYVX]
	ENDM

ZCLIPADDVISVX MACRO ;adds ds:si[bx+POLYX/Y] to list (the vx must be visible for this to work) at DS:DI
	local	l1,l2
	mov	dx,ds:[si+POLYX]
	mov	ax,ds:[si+POLYY]
	cmp	ds:[di+POLYX-4],dx
	jne	l1
	cmp	ds:[di+POLYY-4],ax
	jne	l1
	jmp	l2
l1:	mov	ds:[di+POLYX],dx
	mov	ds:[di+POLYY],ax
	mov	ax,ds:[si+POLYGR]
	mov	ds:[di+POLYGR],ax
	add	di,4
l2:	;;;
	ENDM

ZCLIPADDVX MACRO ;adds (DX,CX) to list at DS:DI
	local	l1,l2
	cmp	ds:[di+POLYX-4],dx
	jne	l1
	cmp	ds:[di+POLYY-4],cx
	jne	l1
	jmp	l2
l1:	mov	ds:[di+POLYX],dx
	mov	ds:[di+POLYY],cx
	mov	ax,ds:[di-4+POLYGR]
	mov	ds:[di+POLYGR],ax
	add	di,4
l2:	;;;
	ENDM

clipnear PROC NEAR
	;clips list DS:SI -> DS:DI (source list must NOT be empty)
	push	bp
	push	si
	push	di
	mov	cs:clipzero,si
	mov	ecx,gs:_projclipz[CLIPMIN] ;ECX=cliplimit
	mov	eax,ds:[si+POLYSIDES] ;&POLYCOLOR
	mov	ds:[di+POLYSIDES],eax
	inc	ax
	mov	cs:clipcount,ax
	mov	eax,ds:[si+POLYFLAGS] ;&POLYVXSEG
	mov	ds:[di+POLYFLAGS],eax
	mov	fs,ds:[si+POLYVXSEG]
	
	mov	word ptr ds:[di+POLYX-4],07fffh ;for CLIPADDVX to surely do it's work
		
	mov	bx,ds:[si+POLYVX]
	cmp	ecx,fs:[bx+vlist_z]
	jle	@@2
	
@@1:	;LAST POINT HIDDEN
	ZCLIPGETVX
	cmp	ecx,fs:[bx+vlist_z]
	jge	@@1
	;this point visible, clip
	ZCLIPCLIP ;returns data in cx
	or	cs:clipvisfl,ax
	ZCLIPADDVX
	mov	ecx,gs:_projclipz[CLIPMIN] ;ECX was destroyed by ZCLIPCLIP
	
@@2:	;LAST POINT VISIBLE
	ZCLIPADDVISVX
	ZCLIPGETVX
	cmp	ecx,fs:[bx+vlist_z]
	jle	@@2
	;this point hidden, clip
	ZCLIPCLIP ;returns data in cx
	or	cs:clipvisfl,ax
	ZCLIPADDVX
	mov	ecx,gs:_projclipz[CLIPMIN] ;ECX was destroyed by ZCLIPCLIP
	jmp	@@1 ;to hidden

@@0:	pop	bx
	sub	di,bx
	shr	di,2
	mov	ds:[bx+POLYSIDES],di ;Sides in clipped polygon
	mov	di,bx
	pop	si
	pop	bp
	mov	ax,cs:clipvisfl
	ret
clipnear ENDP

;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

newclipup PROC NEAR
	;clips list DS:SI -> DS:DI (source list must NOT be empty)
	push	bp
	push	si
	push	di
	mov	cs:clipzero,si
	mov	bp,word ptr gs:_projclipy[CLIPMIN]
	mov	eax,ds:[si+POLYSIDES] ;&POLYCOLOR
	mov	ds:[di+POLYSIDES],eax
	inc	ax
	mov	cs:clipcount,ax
	mov	eax,ds:[si+POLYFLAGS] ;&POLYVXSEG
	mov	ds:[di+POLYFLAGS],eax
	
	mov	word ptr ds:[di+POLYX-4],07fffh ;for CLIPADDVX to surely do it's work

	;bx=last, si=current
	cmp	ds:[si+POLYY],bp
	jge	@@2
	
@@1:	;LAST POINT HIDDEN
	NEWCLIPNEXT
	cmp	ds:[si+POLYY],bp
	jle	@@1
	;this point visible, clip
	NEWCLIPCALC POLYY,POLYX
	
@@2:	;LAST POINT VISIBLE
	NEWCLIPCOPY ;copies always also texture & gouraud
	NEWCLIPNEXT
	cmp	ds:[si+POLYY],bp
	jge	@@2
	;this point hidden, clip
	NEWCLIPCALC POLYY,POLYX
	jmp	@@1 ;to hidden

@@0:	pop	bx
	sub	di,bx
	shr	di,2
	mov	ds:[bx+POLYSIDES],di ;Sides in clipped polygon
	mov	di,bx
	pop	si
	pop	bp
	ret
newclipup ENDP

newclipdown PROC NEAR
	;clips list DS:SI -> DS:DI (source list must NOT be empty)
	push	bp
	push	si
	push	di
	mov	cs:clipzero,si
	mov	bp,word ptr gs:_projclipy[CLIPMAX]
	mov	eax,ds:[si+POLYSIDES] ;&POLYCOLOR
	mov	ds:[di+POLYSIDES],eax
	inc	ax
	mov	cs:clipcount,ax
	mov	eax,ds:[si+POLYFLAGS] ;&POLYVXSEG
	mov	ds:[di+POLYFLAGS],eax
	
	mov	word ptr ds:[di+POLYX-4],07fffh ;for CLIPADDVX to surely do it's work

	;bx=last, si=current
	cmp	ds:[si+POLYY],bp
	jle	@@2
	
@@1:	;LAST POINT HIDDEN
	NEWCLIPNEXT
	cmp	ds:[si+POLYY],bp
	jge	@@1
	;this point visible, clip
	NEWCLIPCALC POLYY,POLYX
	
@@2:	;LAST POINT VISIBLE
	NEWCLIPCOPY ;copies always also texture & gouraud
	NEWCLIPNEXT
	cmp	ds:[si+POLYY],bp
	jle	@@2
	;this point hidden, clip
	NEWCLIPCALC POLYY,POLYX
	jmp	@@1 ;to hidden

@@0:	pop	bx
	sub	di,bx
	shr	di,2
	mov	ds:[bx+POLYSIDES],di ;Sides in clipped polygon
	mov	di,bx
	pop	si
	pop	bp
	ret
newclipdown ENDP
	
newclipleft PROC NEAR
	;clips list DS:SI -> DS:DI (source list must NOT be empty)
	push	bp
	push	si
	push	di
	mov	cs:clipzero,si
	mov	bp,word ptr gs:_projclipx[CLIPMIN]
	mov	eax,ds:[si+POLYSIDES] ;&POLYCOLOR
	mov	ds:[di+POLYSIDES],eax
	inc	ax
	mov	cs:clipcount,ax
	mov	eax,ds:[si+POLYFLAGS] ;&POLYVXSEG
	mov	ds:[di+POLYFLAGS],eax
	
	mov	word ptr ds:[di+POLYX-4],07fffh ;for CLIPADDVX to surely do it's work

	;bx=last, si=current
	cmp	ds:[si+POLYX],bp
	jge	@@2
	
@@1:	;LAST POINT HIDDEN
	NEWCLIPNEXT
	cmp	ds:[si+POLYX],bp
	jle	@@1
	;this point visible, clip
	NEWCLIPCALC POLYX,POLYY
	
@@2:	;LAST POINT VISIBLE
	NEWCLIPCOPY ;copies always also texture & gouraud
	NEWCLIPNEXT
	cmp	ds:[si+POLYX],bp
	jge	@@2
	;this point hidden, clip
	NEWCLIPCALC POLYX,POLYY
	jmp	@@1 ;to hidden

@@0:	pop	bx
	sub	di,bx
	shr	di,2
	mov	ds:[bx+POLYSIDES],di ;Sides in clipped polygon
	mov	di,bx
	pop	si
	pop	bp
	ret
newclipleft ENDP

newclipright PROC NEAR
	;clips list DS:SI -> DS:DI (source list must NOT be empty)
	push	bp
	push	si
	push	di
	mov	cs:clipzero,si
	mov	bp,word ptr gs:_projclipx[CLIPMAX]
	mov	eax,ds:[si+POLYSIDES] ;&POLYCOLOR
	mov	ds:[di+POLYSIDES],eax
	inc	ax
	mov	cs:clipcount,ax
	mov	eax,ds:[si+POLYFLAGS] ;&POLYVXSEG
	mov	ds:[di+POLYFLAGS],eax
	
	mov	word ptr ds:[di+POLYX-4],07fffh ;for CLIPADDVX to surely do it's work

	;bx=last, si=current
	cmp	ds:[si+POLYX],bp
	jle	@@2
	
@@1:	;LAST POINT HIDDEN
	NEWCLIPNEXT
	cmp	ds:[si+POLYX],bp
	jge	@@1
	;this point visible, clip
	NEWCLIPCALC POLYX,POLYY
	
@@2:	;LAST POINT VISIBLE
	NEWCLIPCOPY ;copies always also texture & gouraud
	NEWCLIPNEXT
	cmp	ds:[si+POLYX],bp
	jle	@@2
	;this point hidden, clip
	NEWCLIPCALC POLYX,POLYY
	jmp	@@1 ;to hidden

@@0:	pop	bx
	sub	di,bx
	shr	di,2
	mov	ds:[bx+POLYSIDES],di ;Sides in clipped polygon
	mov	di,bx
	pop	si
	pop	bp
	ret
newclipright ENDP

newclip PROC NEAR
	;ENTRY:
	;ds:si=pointer to list to be clipped
	;ds:di=pointer to temporary list
	;dx=visibility flag for polygon
	;ax=flags for polygon (determines if texture and/or gouraud is clipped)
	;EXIT:
	;ds:si=clipped list (could be either of the entry lists)
	xor	bx,bx
	test	ax,F_GOURAUD
	jz	@@f1
	mov	bx,OFFSET newclip_grd
@@f1:	mov	cs:newclipprog,bx
	;
	LOADGS
	test	dh,VF_FAR	;If *ANY* vertex is 'far', entire polygon is
	jnz	@@cl7		;skipped (no clipping visible far away)
	test	dl,VF_NEAR 	;ClipNear must be done first, for in NEAR case 
	jz	@@cl6		;not all vertices are even calculated!
	cmp	word ptr ds:[si+POLYSIDES],0
	je	@@cl6
	push	dx
	call	clipnear
	xchg	si,di
	pop	dx	;  clipnear return in ax the visfl for new vertices. 
	or	dl,al 	;<-this is done so that if the new vertices are outside the screen they'll be clipped
@@cl6:	;
	test	dl,VF_UP
	jz	@@cl1
	cmp	word ptr ds:[si+POLYSIDES],0
	je	@@cl1
	push	dx
	call	newclipup
	xchg	si,di
	pop	dx
@@cl1:	;
	test	dl,VF_DOWN
	jz	@@cl2
	cmp	word ptr ds:[si+POLYSIDES],0
	je	@@cl2
	push	dx
	call	newclipdown
	xchg	si,di
	pop	dx
@@cl2:	;
	test	dl,VF_LEFT
	jz	@@cl3
	cmp	word ptr ds:[si+POLYSIDES],0
	je	@@cl3
	push	dx
	call	newclipleft
	xchg	si,di
	pop	dx
@@cl3:	;
	test	dl,VF_RIGHT
	jz	@@cl4
	cmp	word ptr ds:[si+POLYSIDES],0
	je	@@cl4
	push	dx
	call	newclipright
	xchg	si,di
	pop	dx
	;
@@cl4:	ret
@@cl7:	;polygon marked invisible:
	mov	word ptr ds:[si+POLYSIDES],0
	ret
newclip ENDP


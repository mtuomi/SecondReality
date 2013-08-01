;/****************************************************************************
;** MODULE:	adraw.asm
;** AUTHOR:	Sami Tammilehto / Fennosoftec OY
;** DOCUMENT:	?
;** VERSION:	1.0
;** REFERENCE:	-
;** REVISED BY:	-
;*****************************************************************************
;**
;** Object drawing (and polygon calculations & clipping (included))
;**
;****************************************************************************/

include a.inc

asm_code SEGMENT para public use16 'CODE'
	ASSUME cs:asm_code

ALIGN 2
newlight dw	12118,10603,3030

;entry: AX=polyflags,ES:DI=normal for which to calculate light
; exit:	AX=colorshade (based 0)
calclight PROC NEAR
	and	ax,F_SHADE32 ;=F_SHADE* orred together)
	jz	@@nc
	;lightsource
	push	ax
	call	normallight
	pop	dx
	shr	dx,10
	mov	cx,6
	sub	cx,dx
	shr	ax,cl ;0400=5, 0800=4, 0C00=3
@@mm:	cmp	ax,1
	jg	@@m1
	mov	ax,1
@@m1:	cmp	ax,30
	jl	@@m2
	mov	ax,30
@@m2:	ret
@@nc:	xor	ax,ax
	jmp	@@mm
calclight ENDP

normallight PROC NEAR
	;return: ax=relative brightness 0..255
	push	bp
	mov	ax,es:[di+nlist_x]
	imul	cs:newlight[0]
	mov	bp,ax
	mov	cx,dx
	mov	ax,es:[di+nlist_y]
	imul	cs:newlight[2]
	add	bp,ax
	adc	cx,dx
	mov	ax,es:[di+nlist_z]
	imul	cs:newlight[4]
	add	ax,bp
	adc	dx,cx
	mov	ax,dx
	sar	ax,2*unitshr-7-16
	add	ax,128
	cmp	ax,255
	jle	@@1
	mov	ax,255
@@1:	cmp	ax,0
	jge	@@2
	mov	ax,0
@@2:	pop	bp
	ret ;ax=0..255
normallight ENDP

checkculling PROC NEAR
	;es:di=normal
	;fs:si=vertex
	;ret: carry=1=hidden
	push	bp
	movsx	eax,word ptr es:[di+nlist_x]
	imul	dword ptr fs:[si+vlist_x]
	mov	ebp,eax
	mov	ecx,edx
	movsx	eax,word ptr es:[di+nlist_y]
	imul	dword ptr fs:[si+vlist_y]
	add	ebp,eax
	adc	ecx,edx
	movsx	eax,word ptr es:[di+nlist_z]
	imul	dword ptr fs:[si+vlist_z]
	add	ebp,eax
	adc	ecx,edx
	rcl	ecx,1 ;if cx<0, carry=1=visible
	cmc	;now carry=1 when invisible
	pop	bp
	ret
checkculling ENDP

ALIGN 16
poly1	LABEL WORD
	db	POLYSIZE dup(0)
ALIGN 16
poly2	LABEL WORD
	db	POLYSIZE dup(0)

include adrawclp.asm

ALIGN 16

drawfill_routine dd	_vid_drawfill
drawfill_routine0 dd	_vid_drawfill ;nrm ;original (=NULL)

polyomin dw	0
polyomax dw	0
polycury dw	0
polylhig dw	0
polyrhig dw	0
polyoversample16 db 0
polyoversamplea db 0
polyoversamples db 0
polyoversample db 0

POLYSIDECALC MACRO fr,to
	local	l1,l2
	;when entering, AX should be heigth
	push	ax
	mov	word ptr es:[bp-2],07777h
	movsx	edx,word ptr ds:[fr+POLYX]
	movzx	ecx,byte ptr cs:polyoversample16 ;sets ECX hi to zero
	shl	edx,cl
	mov	dx,32768 ;NOOVERSAMPLECENTER
	mov	es:[bp+0],edx ;Xstart
	movsx	eax,word ptr ds:[to+POLYX]
	shl	eax,cl
	mov	ax,32768 ;NOOVERSAMPLECENTER
	sub	eax,edx
	cdq
	pop	cx
	idiv	ecx
	mov	cl,byte ptr cs:polycury
	and	cl,cs:polyoversamplea
	jz	l1
l2:	sub	es:[bp+0],eax
	dec	cl
	jnz	l2
l1:	mov	cl,cs:polyoversample
	shl	eax,cl
	mov	es:[bp+4],eax ;Xadd
	add	bp,8
	ENDM

poly_nrm PROC NEAR
	;handles a normal polygon (preclipped one)
	;polydata at DS:SI. SIDES must be >=3
	;drawing orders written to ES:DI
	;-------Calculate bounds for SI
	mov	bx,ds:[si+POLYSIDES]
	shl	bx,2
	lea	ax,[bx-4+si]
	mov	cs:polyomax,ax
	mov	cs:polyomin,si
	;the SI should always be in range POLYOMIN..POLYOMAX
	;-------Find uppermost vertex in polygon
	push	si
	mov	bx,si
	mov	dx,ds:[si+POLYY]
	mov	bp,dx
	mov	cx,ds:[si+POLYSIDES]
@@11:	add	si,4
	dec	cx
	jz	@@2
@@1:	mov	ax,ds:[si+POLYY]
	cmp	ax,bp
	jle	@@13
	mov	bp,ax
@@13:	cmp	ax,dx
	jge	@@11
	mov	dx,ax
	mov	bx,si
	jmp	@@11
@@2:	mov	cx,bp
	;DS:BX=uppermost vertex, DX=uppermost Y, CX=lowermost Y
	mov	cs:polycury,dx
	pop	si
	;-------Write the startup info to drawinfo
	push	cx
	mov	bp,di ;es:di was to drawinfo, now ES:BP is
	mov	ax,ds:[si+POLYCOLOR]
	mov	es:[bp],ax
	mov	cl,cs:polyoversample
	mov	ax,dx
	sar	ax,cl
	mov	es:[bp+2],ax
	add	bp,4+2
	pop	cx
	cmp	dx,cx
	je	@@d0 ;heigth zero, nothing to do
	sub	bp,2
	;-------Set up SI for left edge (--), DI for right edge (++)
	mov	si,bx
	mov	di,bx
	;-------Loop
@@l1:	mov	word ptr es:[bp],0
	add	bp,2
	mov	ax,ds:[si+POLYY]
	cmp	ax,cs:polycury
	jne	@@a2
@@a4:	;Left side reload
	lea	bx,[si-4] ;BX=SI--
	cmp	bx,cs:polyomin
	jge	@@a1
	mov	bx,cs:polyomax
@@a1:	mov	ax,ds:[bx+POLYY]
	sub	ax,ds:[si+POLYY]
	jc	@@d0 ;turned upwards, end of polygon
	jnz	@@a3
	mov	si,bx
	jmp	@@a4
@@a3:	mov	cs:polylhig,ax
	POLYSIDECALC si,bx
	mov	si,bx
@@a2:	;
	mov	word ptr es:[bp],0
	add	bp,2
	mov	ax,ds:[di+POLYY]
	cmp	ax,cs:polycury
	jne	@@b2
@@b4:	;Right side reload
	lea	bx,[di+4] ;BP=DI++
	cmp	bx,cs:polyomax
	jle	@@b1
	mov	bx,cs:polyomin
@@b1:	mov	ax,ds:[bx+POLYY]
	sub	ax,ds:[di+POLYY]
	jc	@@d0 ;turned upwards, end of polygon
	jnz	@@b3
	mov	di,bx
	jmp	@@b4
@@b3:	mov	cs:polyrhig,ax
	POLYSIDECALC di,bx
	mov	di,bx
@@b2:	;	
	mov	ax,cs:polylhig
	cmp	ax,cs:polyrhig
	jl	@@c1
	;right shorter
	mov	ax,cs:polyrhig
@@c1:	;AX=shorter
	mov	cl,cs:polyoversample
	mov	dx,ax
	sar	dx,cl
	mov	es:[bp],dx ;Ycount
	sub	cs:polylhig,ax
	sub	cs:polyrhig,ax
	add	bp,2
	add	cs:polycury,ax
	jmp	@@l1
	
@@d0:	mov	word ptr es:[bp-2],0ffffh
	ret
poly_nrm ENDP

POLYSIDECALC_GRD MACRO fr,to
	local	l1,l2
	;when entering, AX should be heigth
	push	ax
	mov	word ptr es:[bp-2],07777h
	;
	mov	cx,ax
	mov	ax,word ptr ds:[to+POLYGR]
	mov	dx,word ptr ds:[fr+POLYGR]
	mov	es:[bp+0],dx ;COLORstart
	sub	ax,dx
	cwd
	idiv	cx
	mov	es:[bp+2],ax ;COLORadd
	;
	movsx	edx,word ptr ds:[fr+POLYX]
	xor	ecx,ecx
	mov	cl,byte ptr cs:polyoversample16
	shl	edx,cl
	mov	dx,32768 ;NOOVERSAMPLECENTER
	mov	es:[bp+4],edx ;Xstart
	movsx	eax,word ptr ds:[to+POLYX]
	shl	eax,cl
	mov	ax,32768 ;NOOVERSAMPLECENTER
	sub	eax,edx
	cdq
	pop	cx
	idiv	ecx
	mov	cl,byte ptr cs:polycury
	and	cl,cs:polyoversamplea
	jz	l1
l2:	sub	es:[bp+0],eax
	dec	cl
	jnz	l2
l1:	mov	cl,cs:polyoversample
	shl	eax,cl
	mov	es:[bp+8],eax ;Xadd
	add	bp,12
	ENDM

poly_grd PROC NEAR
	;handles a gouraud polygon (preclipped one)
	;polydata at DS:SI. SIDES must be >=3
	;drawing orders written to ES:DI
	;-------Calculate bounds for SI
	mov	bx,ds:[si+POLYSIDES]
	shl	bx,2
	lea	ax,[bx-4+si]
	mov	cs:polyomax,ax
	mov	cs:polyomin,si
	;the SI should always be in range POLYOMIN..POLYOMAX
	;-------Find uppermost vertex in polygon
	push	si
	mov	bx,si
	mov	dx,ds:[si+POLYY]
	mov	bp,dx
	mov	cx,ds:[si+POLYSIDES]
@@11:	add	si,4
	dec	cx
	jz	@@2
@@1:	mov	ax,ds:[si+POLYY]
	cmp	ax,bp
	jle	@@13
	mov	bp,ax
@@13:	cmp	ax,dx
	jge	@@11
	mov	dx,ax
	mov	bx,si
	jmp	@@11
@@2:	mov	cx,bp
	;DS:BX=uppermost vertex, DX=uppermost Y, CX=lowermost Y
	mov	cs:polycury,dx
	pop	si
	;-------Write the startup info to drawinfo
	push	cx
	mov	bp,di ;es:di was to drawinfo, now ES:BP is
	mov	ax,ds:[si+POLYCOLOR]
	mov	es:[bp],ax
	mov	cl,cs:polyoversample
	mov	ax,dx
	sar	ax,cl
	mov	es:[bp+2],ax
	add	bp,4+2
	pop	cx
	cmp	dx,cx
	je	@@d0 ;heigth zero, nothing to do
	sub	bp,2
	;-------Set up SI for left edge (--), DI for right edge (++)
	mov	si,bx
	mov	di,bx
	;-------Loop
@@l1:	mov	word ptr es:[bp],0
	add	bp,2
	mov	ax,ds:[si+POLYY]
	cmp	ax,cs:polycury
	jne	@@a2
@@a4:	;Left side reload
	lea	bx,[si-4] ;BX=SI--
	cmp	bx,cs:polyomin
	jge	@@a1
	mov	bx,cs:polyomax
@@a1:	mov	ax,ds:[bx+POLYY]
	sub	ax,ds:[si+POLYY]
	jc	@@d0 ;turned upwards, end of polygon
	jnz	@@a3
	mov	si,bx
	jmp	@@a4
@@a3:	mov	cs:polylhig,ax
	POLYSIDECALC_GRD si,bx
	mov	si,bx
@@a2:	;
	mov	word ptr es:[bp],0
	add	bp,2
	mov	ax,ds:[di+POLYY]
	cmp	ax,cs:polycury
	jne	@@b2
@@b4:	;Right side reload
	lea	bx,[di+4] ;BP=DI++
	cmp	bx,cs:polyomax
	jle	@@b1
	mov	bx,cs:polyomin
@@b1:	mov	ax,ds:[bx+POLYY]
	sub	ax,ds:[di+POLYY]
	jc	@@d0 ;turned upwards, end of polygon
	jnz	@@b3
	mov	di,bx
	jmp	@@b4
@@b3:	mov	cs:polyrhig,ax
	POLYSIDECALC_GRD di,bx
	mov	di,bx
@@b2:	;	
	mov	ax,cs:polylhig
	cmp	ax,cs:polyrhig
	jl	@@c1
	;right shorter
	mov	ax,cs:polyrhig
@@c1:	;AX=shorter
	mov	cl,cs:polyoversample
	mov	dx,ax
	sar	dx,cl
	mov	es:[bp],dx ;Ycount
	sub	cs:polylhig,ax
	sub	cs:polyrhig,ax
	add	bp,2
	add	cs:polycury,ax
	jmp	@@l1
	
@@d0:	mov	word ptr es:[bp-2],0ffffh
	ret
poly_grd ENDP

;北北北北 _draw_setfillroutine(void (*fillroutine)(int *)) 北北北北
;entry:	Pointer to a routine to handle polygon filling
; exit: - 
;descr: The specified function will be called for each polygon drawed
;	with a pointer to {NORMAL-FILL-DATA} of the polygon.
_draw_setfillroutine PROC FAR
	CBEG
	movpar	eax,0
	or	ax,ax
	jnz	@@1
	mov	eax,cs:drawfill_routine0
@@1:	mov	dword ptr cs:drawfill_routine,eax
	CEND
_draw_setfillroutine ENDP

;北北北北 _draw_polylist(polylist *l,polydata *d,vlist *v,pvlist *pv,
;                                                      nlist *n,int f) 北北北北
;entry:	0 l=pointer to a polygon list (in polylist format)
;	2 d=pointer to polygon data (to which polylist points using indices)
;	4 v=pointer to rotated 3D vertices (for 3D clipping)
;	6 pv=pointer to projected vertices (for drawing)
;	8 n=pointer to normals (for culling)
;	10 f=object flags
; exit: -
;descr: draw the contents of the polylist.
_draw_polylist PROC FAR
	CBEG
	movpar	ax,10
	test	ax,1
	jz	@@invi ;F_VISIBLE not set
	;set poly oversampling
	mov	ax,16
	mov	cx,ds:_projoversampleshr
	mov	cs:polyoversample,cl
	sub	ax,cx
	mov	cs:polyoversample16,al
	mov	al,1
	shl	al,cl
	mov	cs:polyoversamples,al
	dec	al
	mov	cs:polyoversamplea,al
	;start with the actual work
	movpar	si,0+0
	add	si,4 ;skip count - sort vertex
@@1:	movpar	ds,0+1
	push	si
	mov	si,ds:[si]
	cmp	si,0
	je	@@0 ;end of list
	movpar	ax,2+0
	add	si,ax
	movpar	ds,2+1
	;ds:si points to polydata/polygon we are now drawing
	mov	cx,ds:[si+0]
	movpar	ax,10
	or	ax,0f00h
	and	ax,cx
	mov	cs:poly1[POLYFLAGS],ax
	and	cx,0ffh
	mov	cs:poly1[POLYSIDES],cx
	push	si
	mov	ax,ds:[si+4] ;normal
	mov	bx,ds:[si+6] ;first point
	cmp	word ptr ds:[si+2],-1 ;color
	je 	@@cull2
	shl	bx,vlist_sizeshl
	lfspar	si,4 ;rotated vertices
	add	si,bx
	mov	bx,ax ;normal
	shl	bx,nlist_sizeshl
	lespar	di,8 ;rotated normals
	add	di,bx
	test	cs:poly1[POLYFLAGS],F_2SIDE
	jnz	@@2side
	call	checkculling
	pop	si
	jc	@@cull
	jmp	@@nocl
@@cull2: pop	si
	jmp	@@cull
	;es:di=still normal
@@2side: pop	si	
@@nocl:	;lightsource
	mov	ax,cs:poly1[POLYFLAGS]
	test	ax,F_GOURAUD
	jnz	@@nosh
	call	calclight
	add	al,ds:[si+2]
	mov	byte ptr cs:poly1[POLYCOLOR],al
	jmp	@@yosh
@@nosh:	mov	al,ds:[si+2]
	mov	byte ptr cs:poly1[POLYCOLOR],al
@@yosh:	;
	mov	cx,cs:poly1[POLYSIDES]
	lespar	di,6 ;projected vertices
	mov	dx,0ff00h ;for vf calc
	push	bp
	movpar	ax,4+1
	movpar	bp,4+0
	xchg	bp,di
	mov	cs:poly1[POLYVXSEG],ax
	;ds:bp=projected vertices
	;??:di=rotated vertices
	zzs=0
	zzd=0
	REPT	MAXPOLYSIDES ;max sides in polygon
	mov	bx,ds:[si+6+zzs]
	shl	bx,pvlist_sizeshl ;==vlist_sizeshl (required) [!!]
	lea	ax,[bx+di]
	mov	word ptr cs:poly1[POLYVX+zzd],ax
	add	bx,bp
	mov	al,es:[bx+pvlist_vf]
	and	dh,al ;if anded!=0, out of screen
	or	dl,al ;if orred!=0, must clip
	mov	eax,es:[bx+pvlist_x]
	mov	dword ptr cs:poly1[POLYX+zzd],eax
	dec	cx
	jz	@@2
	zzs=zzs+2
	zzd=zzd+4
	ENDM
@@2:	pop	bp
	push	bp
	push	dx
	;gouraud color calcs
	mov	ax,cs:poly1[POLYFLAGS]
	test	ax,F_GOURAUD
	jz	@@nogr
	;
	mov	cx,cs:poly1[POLYSIDES]
	lespar	di,8 ;rotated normals
	mov	fs,cs:poly1[POLYVXSEG]
	mov	bp,OFFSET poly1
@@gr1:	push	cx
	push	di
	mov	bx,cs:[POLYVX+bp]
	mov	bx,fs:[bx+vlist_normal]
	shl	bx,nlist_sizeshl
	add	di,bx
	;es:di=now normal for which to calculate
	mov	ax,cs:poly1[POLYFLAGS]
	call	calclight
	add	al,byte ptr cs:poly1[POLYCOLOR]
	shl	ax,8
	mov	cs:[POLYGR+bp],ax
	pop	di
	pop	cx
	add	bp,4
	loop	@@gr1
@@nogr:	;
	pop	dx
	pop	bp
	LOADDS
	push	bp
	push	ds
	mov	ax,cs
	mov	ds,ax
	mov	si,OFFSET poly1
	or	dh,dh ;dh=visfl anded
	jnz	@@cl0 ;entire polygon invisible
	or	dl,dl ;dl=visfl orred
	jz	@@cl4 ;no clipping
	mov	di,OFFSET poly2
	mov	ax,ds:[si+POLYFLAGS]
	call	newclip
	cmp	word ptr ds:[si+POLYSIDES],0
	je	@@cl0 ;entire polygon clipped away, nothing to draw
	;
@@cl4:	LOADES
	mov	di,OFFSET _polydrw
	test	word ptr ds:[si+POLYFLAGS],F_GOURAUD
	jz	@@ngrd
	mov	word ptr es:[di],1 ;gouraud fill
	add	di,2
	call	poly_grd
	jmp	@@pgdn
@@ngrd:	mov	word ptr es:[di],0 ;normal fill
	add	di,2
	call	poly_nrm
@@pgdn:	
	push	es
	push	OFFSET _polydrw
	call	cs:drawfill_routine
	add	sp,4
	
@@cl0:	pop	ds
	pop	bp
@@cull:	pop	si
	add	si,2
	jmp	@@1
@@0:	pop	si
@@invi:	CEND
_draw_polylist ENDP

asm_code ENDS
	END
	
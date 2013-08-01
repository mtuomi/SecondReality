 ;/****************************************************************************
;** MODULE:	avid.asm
;** AUTHOR:	Sami Tammilehto / Fennosoftec OY
;** DOCUMENT:	?
;** VERSION:	1.0
;** REFERENCE:	-
;** REVISED BY:	-
;*****************************************************************************
;**
;** Assembler / Video (drawing, init...)
;**
;****************************************************************************/

include a.inc

asm_code SEGMENT para public use16 'CODE'
	ASSUME cs:asm_code
	
framecounter dw	0

include avidm1.asm ;320x200x256 (tweak)
include avidm2.asm ;640x400x256 (tweak)

;entry: -
; exit: -
;descr: does nothing
emptyroutine PROC NEAR
	ret
emptyroutine ENDP

;entry: -
; exit: -
;descr: executed before using graphics routines at entries.
;	loads es => vram, and sets some VGA registers the
;	way the gfx routines expect them to be.
vidstart PROC NEAR
	mov	es,ds:_vramseg
	mov	dx,3c4h
	mov	ax,0f02h
	out	dx,ax ;set the plane register ready (&enable all planes)
	ret
vidstart ENDP

;entry: cx=rows to calc, dx=row length
setrows PROC NEAR
	mov	ds:_rowlen,dx
	mov	bx,OFFSET _rows
	xor	ax,ax
@@1:	mov	ds:[bx],ax
	add	ax,dx
	add	bx,2
	loop	@@1
	ret
setrows ENDP

;北北北北 _vid_cameraangle(angle a) 北北北北
;entry:	(see above)
; exit: -
;descr: sets the vision angle (a=0..65535)
_vid_cameraangle PROC FAR
	CBEG
	mov	eax,ds:_projclipx[CLIPMAX] ;right edge
	sub	eax,ds:_projaddx ;center X
	;ax=width of half of screen
	movpar	bx,0
	shr	bx,1 ;divide by 2 for half of angle
	cmp	bx,8*64
	jae	@@2 ;about 3 degrees minimum
	mov	bx,8*64
@@2:	cmp	bx,16384
	jb	@@1
	mov	bx,16383 ;90 degrees maximum
@@1:	shr	bx,6-1
	and	bx,not 1
	;bx=word index (0..255)*2
	movzx	ecx,word ptr ds:_avistan[bx]
	mul	ecx	
	shrd	eax,edx,8
	mov	ds:_projmulx,eax
	movzx	ecx,word ptr ds:_projaspect
	mul	ecx	
	shrd	eax,edx,8
	mov	ds:_projmuly,eax
	CEND
_vid_cameraangle ENDP

;北北北北 _vid_window(long x1,y1,x2,y2,z1,z2) 北北北北
;entry:	(see above)
; exit: -
;descr: sets the video window (for clipping) and sets xadd/yadd to the
;	center of the window
_vid_window PROC FAR
	CBEG
	movpar	eax,0
	mov	edx,eax
	mov	ds:_projclipx[CLIPMIN],eax
	movpar	eax,2
	add	edx,eax
	mov	ds:_projclipx[CLIPMAX],eax
	sar	edx,1
	mov	ds:_projaddx,edx
	movpar	eax,4
	mov	edx,eax
	mov	ds:_projclipy[CLIPMIN],eax
	movpar	eax,6
	add	edx,eax
	mov	ds:_projclipy[CLIPMAX],eax
	sar	edx,1
	mov	ds:_projaddy,edx
	movpar	eax,8
	mov	ds:_projclipz[CLIPMIN],eax
	movpar	eax,10
	mov	ds:_projclipz[CLIPMAX],eax
	CEND
_vid_window ENDP

;北北北北 _vid_init(int mode) 北北北北
;entry:	mode (see below)
; exit: -
;descr: initializes screen to graphics and sets up required variables
;	NOTE: the screen / palette is left black.
;modes:	1=320x200x256 (tweak)
_vid_init PROC FAR
	;CBEG (follows)
	push	bp
	mov	bp,sp
	push	si
	push	di
	push	ds
	mov	ax,ds
	LOADDS
	mov	ds:_cdataseg,ax
	movpar	ax,0
	cmp	ax,1
	jne	@@1
	call	m1_init
@@1:	cmp	ax,11
	jne	@@11
	call	m11_init
@@11:	cmp	ax,2
	jne	@@2
	call	m2_init
@@2:	cmp	ax,3
	jne	@@3
	call	m1o_init
@@3:	CEND
_vid_init ENDP
	
;北北北北 _vid_deinit() 北北北北
;entry:	-
; exit: -
;descr: resets screen to text mode
_vid_deinit PROC FAR
	CBEG
	mov	ax,3
	int	10h
	CEND
_vid_deinit ENDP

;北北北北 _vid_setpal(char far *pal) 北北北北
;entry:	pal=palette (768 bytes VGA RGB)
; exit: -
;descr: sets screen palette 
_vid_setpal PROC FAR
	CBEG
	ldspar	si,0
	mov	dx,3dah
@@1:	in	al,dx
	test	al,8
	jnz	@@1
@@2:	in	al,dx
	test	al,8
	jz	@@2
	mov	dx,3c8h
	xor	al,al
	out	dx,al
	inc	dx
	mov	cx,256
@@3:	mov	al,ds:[si+0]
	out	dx,al
	mov	al,ds:[si+1]
	out	dx,al
	mov	al,ds:[si+2]
	out	dx,al
	add	si,3
	loop	@@3
	CEND
_vid_setpal ENDP

;北北北北 _vid_drawdots(int count,pvlist *pv) 北北北北
;entry:	count=number of points to draw
;	pv=pointer to projected vertex list
; exit: -
;descr: plots vertices in list
_vid_drawdots PROC FAR
	CBEG
	call	vidstart
	movpar	cx,0
	lfspar	si,1
@@1:	mov	ax,fs:pvlist_vf[si]
	or	ax,ax
	jnz	@@2
	mov	dx,fs:pvlist_x[si]
	mov	bx,fs:pvlist_y[si]
	mov	ah,31
	push	cx
	call	ds:vr[PSET]
	pop	cx
@@2:	add	si,pvlist_size
	loop	@@1
	CEND
_vid_drawdots ENDP

;北北北北 _vid_dotdisplay_pvlist(int count,pvlist *list) 北北北北
;entry:	count=number of elements in pvlist
;       list=pointer to pvlist
; exit: -
;descr: TEST ROUTINE
;	displays the projected vertices in the list as dots
_vid_dotdisplay_pvlist PROC FAR
	CBEG
	call	vidstart
	lfspar	si,1
	movpar	cx,0
@@1:	push	cx
	mov	ax,fs:[si+pvlist_vf]
	or	ax,ax
	jnz	@@2
	mov	dx,fs:[si+pvlist_x]
	mov	bx,fs:[si+pvlist_y]
	mov	ah,15 ;color
	call	ds:vr[PSET]
@@2:	add	si,pvlist_size
	pop	cx
	loop	@@1
	CEND
_vid_dotdisplay_pvlist ENDP

;北北北北 _vid_pset(int x,int y,int color) 北北北北
_vid_pset PROC FAR
	CBEG
	call	vidstart
	movpar	dx,0
	movpar	bx,1
	movpar	ah,2
	call	ds:vr[PSET]
	CEND
_vid_pset ENDP

;北北北北 _vid_dotdisplay_zcolor(int count,pvlist *list1,vlist *list2) 北北北北
;entry:	count=number of elements in pvlist
;       list1=pointer to pvlist
;       list2=pointer to vlist (both lists should have same vertices!)
; exit: -
;descr: TEST ROUTINE
;	displays the projected vertices in the list as dots colored
;	according to depth
_vid_dotdisplay_zcolor PROC FAR
	CBEG
	call	vidstart
	lfspar	si,1
	lgspar	di,3
	movpar	cx,0
@@1:	push	cx
	mov	ax,fs:[si+pvlist_vf]
	or	ax,ax
	jnz	@@2
	mov	dx,fs:[si+pvlist_x]
	mov	bx,fs:[si+pvlist_y]
	mov	eax,gs:[di+vlist_z]
	sar	eax,11
	cmp	ax,1
	jge	@@3
	mov	ax,1
@@3:	cmp	ax,31
	jle	@@4
	mov	ax,31
@@4:	mov	ah,al
	call	ds:vr[PSET]
@@2:	add	si,pvlist_size
	add	di,vlist_size
	pop	cx
	loop	@@1
	CEND
_vid_dotdisplay_zcolor ENDP

;北北北北 _vid_clear(void) 北北北北
;entry: -
; exit: -
;descr: Clears the current screen to black (color 0)
_vid_clear PROC FAR
	CBEG
	call	vidstart
	mov	ax,0 ;blackclear
	call	ds:vr[CLEAR]
	CEND
_vid_clear ENDP

;北北北北 _vid_clear255(void) 北北北北
;entry: -
; exit: -
;descr: Clears the current screen to black (color 0)
_vid_clear255 PROC FAR
	CBEG
	call	vidstart
	mov	ax,9 ;255clear
	call	ds:vr[CLEAR]
	CEND
_vid_clear255 ENDP

;北北北北 _vid_clearbg(char *bg) 北北北北
;entry: -
; exit: -
;descr: Copies the bg to screen
_vid_clearbg PROC FAR
	CBEG
	call	vidstart
	mov	ax,2 ;bgcopy
	mov	si,[bp+6]
	mov	fs,[bp+8]
	call	ds:vr[CLEAR]
	CEND
_vid_clearbg ENDP

;北北北北 _vid_switch(void) 北北北北
;entry: -
; exit: -
;descr: Switch to the next screen page (_vid_waitb should be called after
;       this)
_vid_switch PROC FAR
	CBEG
	call	vidstart
	call	ds:vr[SWITCH]
	CEND
_vid_switch ENDP

;北北北北 _vid_setswitch(int,int) 北北北北
;entry: -
; exit: -
;descr: Forces a separate write/display switch
_vid_setswitch PROC FAR
	CBEG
	;call	vidstart
	movpar	ax,0
	cmp	ax,-1
	je	@@1
	shl	ax,14-4
	add	ax,0a000h
	mov	ds:_vramseg,ax
@@1:	movpar	ax,1
	cmp	ax,-1
	je	@@2
	shl	ax,14
	mov	dx,3d4h
	mov	al,0ch
	out	dx,ax
@@2:	CEND
_vid_setswitch ENDP

;北北北北 _vid_waitb(void) 北北北北
;entry: -
; exit: -
;descr: Waits for border (=retrace)
_vid_waitb PROC FAR
	CBEG
	call	vidstart
	call	ds:vr[WAITB]
	CEND
_vid_waitb ENDP

;北北北北 _vid_clear(char *sky) 北北北北
;entry: [sky]=pointer to sky data structure
; exit: -
;descr: Clears the current screen with a sky pattern according to [sky]
_vid_skyclear PROC FAR
	CBEG
	call	vidstart
	lfspar	si,0
	mov	ax,1 ;skyclear
	call	ds:vr[CLEAR]
	CEND
_vid_skyclear ENDP

;##########################################################################
;         'Temporary' text routines
;##########################################################################

include atext.asm

;##########################################################################
;         Common routines that work in all current screen modes
;##########################################################################

;entry: dx=x, bx=y, ax=width, cx=heigth
; exit:
;descr: calculates the 'good' copying sequence for the bitmap
ALIGN 2
bb_hidden dw	0
bb_yskip dw	0
bb_ycount dw	0
bb_xskip dw	0
bb_xcopy dw	0
;
bb_doskip dw	0,0,0,0 ;bytes 
bb_doskip2 dw	0,0,0,0 ;pages 0..3
bb_doplane dw	0,0,0,0
bb_dodest dw	0,0,0,0
bb_docount dw	0,0,0,0
bitbltanalyze PROC NEAR
	push	si
	push	di
	;clip up/down
	mov	cs:bb_xskip,0
	mov	cs:bb_yskip,0
	mov	si,word ptr ds:_projclipy[CLIPMAX]
	cmp	bx,si
	jg	@@out
	mov	si,word ptr ds:_projclipy[CLIPMIN]
	cmp	bx,si
	jge	@@1
	sub	si,bx
	mov	cs:bb_yskip,si
	add	bx,si
	sub	cx,si
@@1:	mov	si,word ptr ds:_projclipy[CLIPMAX]
	sub	si,bx
	cmp	cx,si
	jle	@@2
	mov	cx,si
	inc	cx
@@2:	mov	cs:bb_ycount,cx
	mov	si,bx
	add	si,cx
	cmp	si,word ptr ds:_projclipy[CLIPMIN]
	jl	@@out
	;clip left/right
	mov	si,word ptr ds:_projclipx[CLIPMAX]
	cmp	dx,si
	jg	@@out
	mov	si,word ptr ds:_projclipx[CLIPMIN]
	cmp	dx,si
	jge	@@3
	sub	si,dx
	mov	cs:bb_xskip,si
	add	dx,si
	sub	ax,si
@@3:	mov	si,word ptr ds:_projclipx[CLIPMAX]
	sub	si,dx
	cmp	ax,si
	jle	@@4
	mov	ax,si
	inc	ax
@@4:	mov	cs:bb_xcopy,ax
	mov	si,dx
	add	si,ax
	cmp	si,word ptr ds:_projclipx[CLIPMIN]
	jl	@@out
	;calc start address
	shl	bx,1
	mov	bx,ds:_rows[bx]
	mov	cx,dx
	shr	dx,2
	add	bx,dx
	and	cl,3
	mov	ax,1102h
	rol	ah,cl
	mov	dx,bx
	mov	si,cs:bb_xskip
	mov	di,cs:bb_xcopy
	add	di,3
	xor	bx,bx
	REPT 4
	local	l1
	push	ax
	mov	cs:bb_doplane[bx],ax
	mov	cs:bb_dodest[bx],dx
	mov	ax,si
	shr	ax,2
	mov	cs:bb_doskip[bx],ax
	mov	ax,si
	and	ax,3
	mov	cs:bb_doskip2[bx],ax
	mov	ax,di
	shr	ax,2
	mov	cs:bb_docount[bx],ax
	pop	ax
	rol	ah,1
	jnc	l1
	inc	dx
l1:	inc	si
	dec	di
	add	bx,2
	ENDM
	mov	cs:bb_hidden,0
	pop	di	
	pop	si
	ret
@@out:	mov	cs:bb_hidden,1
	pop	di	
	pop	si
	ret
bitbltanalyze ENDP

;北北北北 _vid_pic320200(char *p,int x,int y) 北北北北
;entry:	p=pointer to pic, x/y=screen position to write to
; exit: -
;descr: copies pic to screen. The pic format is {PIC64TW}
ALIGN 2
yrow	dw	0
yrowlen dw	0
xm0	dw	0
xmstart dw	0
_vid_pic320200 PROC FAR
	CBEG
	call	vidstart
	movpar	dx,2
	movpar	bx,3
	mov	cx,200
	mov	ax,320
	call	bitbltanalyze
	cmp	cs:bb_hidden,0
	jne	@@0
	mov	ax,ds:_rowlen
	mov	cs:yrowlen,ax
	ldspar	si,0
	xor	bx,bx
	mov	cx,4
@@1:	push	cx
	push	bx
	push	si
	push	di
	mov	ax,320 ;line length in source buf
	mul	cs:bb_yskip
	add	si,ax
	mov	ax,80 ;lenght of single 'bit'plane
	mul	cs:bb_doskip2[bx]
	add	si,ax
	add	si,cs:bb_doskip[bx]
	mov	ax,cs:bb_doplane[bx]
	mov	dx,3c4h
	out	dx,ax
	mov	di,cs:bb_dodest[bx]
	mov	dx,cs:bb_docount[bx]
	or	dx,dx
	jz	@@20
	mov	cx,cs:bb_ycount
	jcxz	@@20
	cmp	cx,7
	ja	@@2
	
@@5:	;simplex nonaligned copy
	push	cx
	push	si
	push	di
	mov	cx,dx
	shr	cx,2
	rep	movsd ;copy dwords
	mov	cx,dx
	and	cx,3
	rep	movsb ;copy rest
	pop	di
	pop	si
	pop	cx
	add	di,cs:yrowlen
	add	si,320 ;line length in source buf
	loop	@@5
	jmp	@@20

@@2:	;aligned copy
	push	cx
	push	dx
	push	si
	push	di
	mov	cx,di
	neg	cx
	and	cx,3
	sub	dx,cx
	rep	movsb ;align
	mov	cx,dx
	shr	cx,2
	rep	movsd ;copy dwords
	mov	cx,dx
	and	cx,3
	rep	movsb ;copy rest
	pop	di
	pop	si
	pop	dx
	pop	cx
	add	di,cs:yrowlen
	add	si,320 ;line length in source buf
	loop	@@2
	
@@20:	pop	di
	pop	si
	pop	bx
	pop	cx
	add	bx,2
	loop	@@1
@@0:	CEND
_vid_pic320200 ENDP

;北北北北 _vid_drawsight(char *sight) 北北北北
;entry:	[sight]=pointer to sight data
; exit: -
;descr: overlays a sight picture to the screen. The picture must be in
;       sight format {SIGHT-DATA}
_vid_drawsight PROC FAR
	CBEG
	call	vidstart
	ldspar	si,0
	zzz=1
	REPT	4
	local	l1
	mov	dx,3c5h
	mov	al,zzz
	out	dx,al
	mov	cx,ds:[si]
	add	si,2
	mov	al,127
l1:	mov	di,ds:[si]
	add	si,2
	mov	es:[di],al
	loop	l1	
	zzz=zzz*2
	ENDM
	CEND
_vid_drawsight ENDP

include avidfill.asm

asm_code ENDS
	END

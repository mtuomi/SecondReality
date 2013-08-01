;/****************************************************************************
;** MODULE:	avidm1.asm
;** AUTHOR:	Sami Tammilehto / Fennosoftec OY
;** DOCUMENT:	?
;** VERSION:	1.0
;** REFERENCE:	-
;** REVISED BY:	-
;*****************************************************************************
;**
;** Video driver for 320x200x256 (tweak)
;** included to avid.asm
;**
;****************************************************************************/

;======== public routines ========

m1_init	PROC NEAR
	mov	ds:_vramseg,0a000h
	call	tweak320x200
	mov	dx,320/4
	mov	cx,200
	call	setrows
	mov	ds:vr[PSET],OFFSET m1_pset
	mov	ds:vr[CLEAR],OFFSET m1_clear
	mov	ds:vr[SWITCH],OFFSET m1_switch
	mov	ds:vr[WAITB],OFFSET m1_waitb
	mov	ds:_projclipz[CLIPMIN],256
	mov	ds:_projclipz[CLIPMAX],1000000000
	;these affected by oversampling
	mov	ds:_projclipx[CLIPMIN],0
	mov	ds:_projclipx[CLIPMAX],319
	mov	ds:_projclipy[CLIPMIN],0
	mov	ds:_projclipy[CLIPMAX],199
	mov	ds:_projmulx,250
	mov	ds:_projmuly,220
	mov	ds:_projaddx,160
	mov	ds:_projaddy,100
	mov	ds:_projoversampleshr,0
	;
	mov	ds:_projaspect,225
	ret
m1_init	ENDP

m11_init PROC NEAR
	mov	ds:_vramseg,0a000h
;	call	tweak320x200
	mov	dx,320/4
	mov	cx,200
	call	setrows
	mov	ds:vr[PSET],OFFSET m1_pset
	mov	ds:vr[CLEAR],OFFSET m1_clear
	mov	ds:vr[SWITCH],OFFSET m1_switch
	mov	ds:vr[WAITB],OFFSET m1_waitb
	mov	ds:_projclipz[CLIPMIN],256
	mov	ds:_projclipz[CLIPMAX],1000000000
	;these affected by oversampling
	mov	ds:_projclipx[CLIPMIN],0
	mov	ds:_projclipx[CLIPMAX],319
	mov	ds:_projclipy[CLIPMIN],0
	mov	ds:_projclipy[CLIPMAX],199
	mov	ds:_projmulx,250
	mov	ds:_projmuly,220
	mov	ds:_projaddx,160
	mov	ds:_projaddy,100
	mov	ds:_projoversampleshr,0
	;
	mov	ds:_projaspect,225
	ret
m11_init ENDP

m1o_init PROC NEAR ;oversampling version
	mov	ds:_vramseg,0a000h
	;call	tweak320x200
	mov	dx,320/4
	mov	cx,200
	call	setrows
	mov	ds:vr[PSET],OFFSET m1_pset
	mov	ds:vr[CLEAR],OFFSET m1_clear
	mov	ds:vr[SWITCH],OFFSET m1_switch
	mov	ds:vr[WAITB],OFFSET m1_waitb
	mov	ds:_projclipz[CLIPMIN],256
	mov	ds:_projclipz[CLIPMAX],1000000000
	;these affected by oversampling
	mov	ds:_projclipx[CLIPMIN],0
	mov	ds:_projclipx[CLIPMAX],319
	mov	ds:_projclipy[CLIPMIN],0
	mov	ds:_projclipy[CLIPMAX],199
	mov	ds:_projmulx,250
	mov	ds:_projmuly,220
	mov	ds:_projaddx,160
	mov	ds:_projaddy,100
	mov	ds:_projoversampleshr,0
	;
	mov	ds:_projaspect,225
	ret
m1o_init	ENDP

;======== public routines called through the vr[] pointers ========

;dx=X, bx=Y, ah=color
m1_pset	PROC NEAR
	;requires ES=vram, OUT 3C4,2  [set by vidstart]
	shl	bx,1
	mov	bx,ds:_rows[bx]
	mov	cx,3
	and	cx,dx
	sar	dx,2
	add	bx,dx
	mov	al,1
	shl	al,cl
	mov	dx,3c5h
	out	dx,al
	mov	es:[bx],ah
	ret
m1_pset ENDP

m1_waitb PROC NEAR
	mov	dx,3dah
@@1:	in	al,dx
	test	al,8
	jnz	@@1
@@2:	in	al,dx
	test	al,8
	jz	@@2
	ret
m1_waitb ENDP

m1_clear PROC NEAR ;clear current page
	push	di
	cmp	ax,9
	je	@@bgd
	cmp	ax,2
	je	@@bgc
	mov	dx,3c4h
	mov	ax,0f02h
	out	dx,ax
	xor	di,di
	xor	eax,eax
	mov	cx,200
@@1:	REPT	320/4/4
	mov	es:[di],eax
	add	di,4
	ENDM
	dec	cx
	jnz	@@1
	pop	di
	ret
@@bgd:	mov	dx,3c4h
	mov	ax,0f02h
	out	dx,ax
	xor	di,di
	mov	eax,0ffffffffh
	mov	cx,200
@@9:	REPT	320/4/4
	mov	es:[di],eax
	add	di,4
	ENDM
	dec	cx
	jnz	@@9
	pop	di
	ret
@@bgc:	zzpl=1
	push	ds
	mov	ax,fs
	mov	ds,ax
	REPT	4
	local	l1
	push	di
	mov	dx,3c4h
	mov	ax,02h+100h*zzpl
	out	dx,ax
	xor	di,di
	xor	eax,eax
	mov	cx,200
l1:	REPT	320/4/4
	mov	eax,ds:[si]
	add	si,4
	mov	es:[di],eax
	add	di,4
	ENDM
	loop	l1
	pop	di
	zzpl=zzpl*2
	ENDM
	pop	ds
	pop	di
	ret
m1_clear ENDP

m1_switch PROC NEAR
	mov	ax,ds:_vramseg
	cmp	ax,0a140h
	jne	@@1
	;---[
	mov	ds:_vramseg,0a5f0h
	mov	ah,014h
	;---]
	jmp	@@0
@@1:	cmp	ax,0a5f0h
	jne	@@2
	;---[
	mov	ds:_vramseg,0aaa0h
	mov	ah,05fh
	;---]
	jmp	@@0
@@2:	;---[
	mov	ds:_vramseg,0a140h
	mov	ah,0aah
	;---]
@@0:	mov	dx,3d4h
	mov	al,0ch
	out	dx,ax
	ret
m1_switch ENDP

;======== internal routines ========

;sets up 320x200x256 tweak
tweak320x200 PROC NEAR
	mov     ax,0013h
	int     10h   
	mov	dx,3c8h
	mov	al,0
	out	dx,al
	inc	dx
	mov	cx,768
@@1:	out	dx,al
	loop	@@1
	mov     dx,3c4h
	mov     al,4
	out     dx,al
	inc     dx
	in      al,dx
	and     al,not 08h      
	or      al,04h          
	out     dx,al
	mov     dx,3ceh
	mov     al,5
	out     dx,al
	inc     dx
	in      al,dx
	and     al,not 10h      
	out     dx,al
	dec     dx
	mov     al,6
	out     dx,al
	inc     dx
	in      al,dx
	and     al,not 02h      
	out     dx,al
	mov     dx,3d4h
	mov     al,9
	inc     dx
	and     al,not 5fh 
	or      al,1
	dec     dx
	mov     al,14h
	out     dx,al
	inc     dx
	in      al,dx
	and     al,not 40h
	out     dx,al
	dec     dx
	mov     al,17h
	out     dx,al
	inc     dx
	in      al,dx
	or      al,40h 
	out     dx,al
	;clear vram
	mov	dx,3c4h
	mov	ax,0f02h
	out	dx,ax
	mov	cx,32768
	mov	ax,ds:_vramseg
	mov	es,ax
	xor	di,di
	xor	ax,ax
	rep	stosw
	ret
tweak320x200 ENDP


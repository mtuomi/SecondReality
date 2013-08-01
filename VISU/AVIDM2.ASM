;/****************************************************************************
;** MODULE:	avidm2.asm
;** AUTHOR:	Sami Tammilehto / Fennosoftec OY
;** DOCUMENT:	?
;** VERSION:	1.0
;** REFERENCE:	-
;** REVISED BY:	-
;*****************************************************************************
;**
;** Video driver for 640x400x256 (tweak)
;** included to avid.asm
;**
;****************************************************************************/

;======== public routines ========

m2_init	PROC NEAR
	mov	ds:_vramseg,0a000h
	call	tweak640x400
	mov	dx,640/4
	mov	cx,400
	call	setrows
	mov	ds:vr[PSET],OFFSET m2_pset
	mov	ds:vr[CLEAR],OFFSET m2_clear
	mov	ds:vr[SWITCH],OFFSET m2_switch
	mov	ds:vr[WAITB],OFFSET m2_waitb
	mov	ds:_projclipx[CLIPMIN],0
	mov	ds:_projclipx[CLIPMAX],639
	mov	ds:_projclipy[CLIPMIN],0
	mov	ds:_projclipy[CLIPMAX],399
	mov	ds:_projclipz[CLIPMIN],512*1
	mov	ds:_projclipz[CLIPMAX],1000000000
	mov	ds:_projmulx,480*1
	mov	ds:_projmuly,400*1
	mov	ds:_projaddx,320
	mov	ds:_projaddy,200
	mov	ds:_projaspect,256
	ret
m2_init	ENDP

;======== public routines called through the vr[] pointers ========

;dx=X, bx=Y, ah=color
m2_pset	PROC NEAR
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
m2_pset ENDP

m2_waitb PROC NEAR
	mov	dx,3dah
@@1:	in	al,dx
	test	al,8
	jnz	@@1
@@2:	in	al,dx
	test	al,8
	jz	@@2
	ret
m2_waitb ENDP

ALIGN 2
tmptest	dw	0

m2_clear PROC NEAR ;clear current page
	inc	cs:tmptest
	push	di
	cmp	ax,1
	je	@@sky
	mov	dx,3c4h
	mov	ax,0f02h
	out	dx,ax
	mov	cx,640*400/4/4
	xor	di,di
	xor	eax,eax
	rep	stosd
	pop	di
	ret
@@sky:	mov	dx,3c4h
	mov	ax,0f02h
	out	dx,ax
	mov	cx,400
	xor	di,di
@@1:	mov	al,fs:[si]
	cmp	al,1
	je	@@colr
	cmp	al,2
	je	@@bitm
	cmp	al,4
	je	@@depth
	jmp	@@noth
	
@@depth:
	push	si
	push	ds
	mov	dx,3c4h
	mov	ax,0f02h
	out	dx,ax
	mov	ax,fs:[si+2]
	xor	dx,dx
	mov	bx,320
	div	bx
	lds	ax,fs:[si+4]
	mov	si,ax
	add	si,dx
	zzz=0;
	REPT	640/4/4
	mov	eax,ds:[si+zzz]
	mov	es:[di+zzz],eax
	zzz=zzz+4
	ENDM
	pop	ds
	pop	si
	jmp	@@noth

@@colr:	mov	al,fs:[si+2]
	mov	ah,al
	mov	bx,ax
	shl	eax,16
	mov	ax,bx
	zzz=0;
	REPT	640/4/4
	mov	es:[di+zzz],eax
	zzz=zzz+4
	ENDM
	jmp	@@noth

@@bitm:	push	cx
	push	ds
	push	si
@@bc:	lds	ax,fs:[si+4]
	mov	si,ax
	mov	ax,cs:tmptest
	mov	dx,ax
	shr	dx,2
	add	si,dx
	and	ax,3
	shl	ax,11
	add	si,ax
	mov	cx,4
	mov	ax,0102h
@@b1:	push	ax
	mov	dx,3c4h
	out	dx,ax
	zzz=0
	REPT	640/4/4
	mov	eax,ds:[si+zzz]
	mov	es:[di+zzz],eax
	zzz=zzz+4
	ENDM
	add	si,2048
	cmp	si,8192
	jb	@@b3
	sub	si,8192-1
@@b3:	pop	ax
	shl	ah,1
	loop	@@b1
	pop	si
	pop	ds
	pop	cx
	mov	dx,3c4h
	mov	ax,0f02h
	out	dx,ax
	jmp	@@noth

@@noth:	add	di,640/4
	add	si,8
	loop	@@1
	pop	di
	ret
m2_clear ENDP

ALIGN 2
pagep	dw	0
wpage	dw	1,2,3
spage	dw	3,1,2

m2_switch PROC NEAR
	mov	ax,0a000h
	mov	ds:_vramseg,ax
	mov	bx,cs:pagep
	inc	bx
	cmp	bx,3
	jb	sws1
	xor	bx,bx
sws1:	mov	cs:pagep,bx
	shl	bx,1
	mov	dx,3d4h
	mov	al,33h
	mov	ah,byte ptr cs:spage[bx]
	out	dx,ax ;spage
	;set wpage
	mov	al,byte ptr cs:wpage[bx]
	mov	ah,al
	rol	ah,4
	or	al,ah
	mov	dx,3cdh
	out	dx,al
	;page low offset
	mov	dx,3d4h
	mov	ax,000dh
	out	dx,ax
	mov	ax,000ch
	out	dx,ax
	ret
m2_switch ENDP

;======== internal routines ========

;sets up 640x400x256 tweak
tweak640x400 PROC NEAR
	mov     ax,002fh
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
	ret
tweak640x400 ENDP

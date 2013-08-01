
ALIGN 16
mode0d	db	256 dup(0)
mode0e	db	256 dup(0)
mode13	db	256 dup(0)
vid_oldint dd	0
vid_curmode dw	-1

vid_intti PROC NEAR
	cmp	ah,0fh
	je	@@f
	cmp	ah,0
	je	@@1
@@nn:	jmp	cs:vid_oldint
@@f:	cmp	cs:vid_curmode,-1
	je	@@nn
	mov	ax,cs:vid_curmode
	mov	ah,40
	mov	bh,0
	iret
@@1:	nop
;	cmp	al,13h
;	je	@@mode13
;	cmp	al,13h+80h
;	je	@@mode13noclr
;	cmp	al,0dh
;	je	@@mode0d
;	cmp	al,0dh+80h
;	je	@@mode0dnoclr
;	cmp	al,0eh
;	je	@@mode0e
	cmp	al,4
	ja	@@gn
	cmp	cs:notextmode,0
	je	@@gn
	iret
@@gn:	mov	cs:vid_curmode,-1
	jmp	cs:vid_oldint
@@mode13:
	mov	cs:vid_curmode,13h
	pusha
	push	ds
	push	es
	call	clearpalette
	mov	ax,cs
	mov	ds,ax
	mov	si,OFFSET mode13
	call	loadvmode
	call	clearvram
	pop	es
	pop	ds
	popa
	iret
@@mode13noclr:
	mov	cs:vid_curmode,13h
	pusha
	push	ds
	push	es
	;call	clearpalette
	mov	ax,cs
	mov	ds,ax
	mov	si,OFFSET mode13
	call	loadvmode
	pop	es
	pop	ds
	popa
	iret
@@mode0d:
	mov	cs:vid_curmode,0dh
	pusha
	push	ds
	push	es
	call	clearpalette
	mov	ax,cs
	mov	ds,ax
	mov	si,OFFSET mode0d
	call	loadvmode
	call	clearvram
	pop	es
	pop	ds
	popa
	iret
@@mode0dnoclr:
	mov	cs:vid_curmode,13h
	pusha
	push	ds
	push	es
	call	clearpalette
	mov	ax,cs
	mov	ds,ax
	mov	si,OFFSET mode0d
	call	loadvmode
	pop	es
	pop	ds
	popa
	iret
@@mode0e:
	mov	cs:vid_curmode,0eh
	pusha
	push	ds
	push	es
	call	clearpalette
	mov	ax,cs
	mov	ds,ax
	mov	si,OFFSET mode0e
	call	loadvmode
	call	clearvram
	pop	es
	pop	ds
	popa
	iret
vid_intti ENDP

vmode_deinit PROC NEAR
	xor	ax,ax
	mov	ds,ax
	mov	ax,word ptr cs:vid_oldint[0]
	mov	bx,word ptr cs:vid_oldint[2]
	mov	word ptr ds:[010h*4+0],ax
	mov	word ptr ds:[010h*4+2],bx
	ret
vmode_deinit ENDP

vmode_init PROC NEAR
	pusha
	push	ds
	push	es
	push	fs
	push	gs

	IF 0	
	mov	ax,0dh ;320x200x16
	int	10h
	mov	ax,cs
	mov	ds,ax
	mov	si,OFFSET mode0d
	call	savevmode
	mov	ax,13h ;320x200x256
	int	10h
	mov	ax,cs
	mov	ds,ax
	mov	si,OFFSET mode13
	call	savevmode
	mov	ax,0eh ;640x350x16
	int	10h
	mov	ax,cs
	mov	ds,ax
	mov	si,OFFSET mode0e
	call	savevmode
	mov	ax,3
	int	10h
	ENDIF

	xor	ax,ax
	mov	ds,ax
	mov	ax,word ptr ds:[010h*4+0]
	mov	bx,word ptr ds:[010h*4+2]
	mov	word ptr cs:vid_oldint[0],ax
	mov	word ptr cs:vid_oldint[2],bx
	mov	word ptr ds:[010h*4+0],OFFSET vid_intti
	mov	word ptr ds:[010h*4+2],cs

	pop	gs
	pop	fs
	pop	es
	pop	ds
	popa	
	ret
vmode_init ENDP

VSAL	MACRO
	mov	ds:[si],al
	inc	si
	ENDM

;vmode data to DS:SI
savevmode PROC NEAR
	mov	dx,3cch
	in	al,dx
	VSAL
	;Read Sequencer (3C4)
	mov	dx,3c4h
	xor	ah,ah
	mov	cx,04h+1
@@r1:	mov	al,ah
	out	dx,al
	inc	dx
	in	al,dx
	VSAL
	dec	dx
	inc	ah
	loop	@@r1
	;Read CRTC (3D4)
	mov	dx,3d4h
	xor	ah,ah
	mov	cx,18h+1
@@r2:	mov	al,ah
	out	dx,al
	inc	dx
	in	al,dx
	VSAL
	dec	dx
	inc	ah
	LOOP	@@r2
	;Read GFX controller (3CE)
	mov	dx,3ceh
	xor	ah,ah
	mov	cx,08h+1
@@r3:	mov	al,ah
	out	dx,al
	inc	dx
	in	al,dx
	VSAL
	dec	dx
	inc	ah
	loop	@@r3
	;Read Attribute controller (3C0)
	xor	ah,ah
	mov	cx,14h+1
@@r4:	mov	dx,3dah
	in	al,dx
	mov	dx,3c0h
	mov	al,ah
	out	dx,al
	inc	dx
	in	al,dx
	VSAL
	dec	dx
	out	dx,al
	mov	al,20h
	out	dx,al
	inc	ah
	loop	@@r4
	ret
savevmode ENDP

;vmode data from DS:SI
loadvmode PROC NEAR
	mov	dx,3dah
@@1:	in	al,dx
	test	al,8
	jnz	@@1
@@2:	in	al,dx
	test	al,8
	jz	@@2
	;Syncronous reset
	mov	dx,3c4h
	mov	ax,0200h
	out	dx,ax
	;Clear CRTC protection flag
	mov	dx,3d4h
	mov	al,011h
	out	dx,al
	inc	dx
	in	al,dx
	and	al,not 128
	out	dx,al
	;Set misc register
	mov	al,ds:[si]
	inc	si
	mov	dx,3c2h
	out	dx,al
	;Set Sequencer (3C4)
	mov	dx,3c4h
	xor	al,al
	mov	cx,04h+1
@@w1:	mov	ah,ds:[si]
	inc	si
	out	dx,ax
	inc	al
	loop	@@w1
	;Set CRTC (3D4)
	mov	dx,3d4h
	xor	al,al
	mov	cx,18h+1
@@w2:	mov	ah,ds:[si]
	inc	si
	out	dx,ax
	inc	al
	loop	@@w2
	;Set GFX controller (3CE)
	mov	dx,3ceh
	xor	al,al
	mov	cx,08h+1
@@w3:	mov	ah,ds:[si]
	inc	si
	out	dx,ax
	inc	al
	loop	@@w3
	;Set Attribute controller (3C0)
	mov	dx,3dah
	in	al,dx
	mov	dx,3c0h
	xor	ah,ah
	mov	cx,14h+1
@@w4:	mov	al,ah
	out	dx,al
	mov	al,ds:[si]
	out	dx,al
	inc	si
	inc	ah
	loop	@@w4
	;Enable display, enable PEL mask
	mov	dx,3c0h
	mov	al,20h
	out	dx,al
	mov	dx,3c6h
	mov	al,0ffh
	out	dx,al
	ret
loadvmode ENDP

clearvram PROC NEAR
	push	es
	push	di
	mov	dx,3c4h
	mov	ax,0f02h
	out	dx,ax
	mov	ax,0a000h
	mov	es,ax
	xor	di,di
	xor	eax,eax
	mov	cx,16384
	rep	stosd
	pop	di
	pop	es
	ret
clearvram ENDP

clearpalette PROC NEAR
	mov	dx,3c8h
	xor	al,al
	out	dx,al
	inc	dx
	mov	cx,768
@@1:	out	dx,al
	loop	@@1
	ret
clearpalette ENDP

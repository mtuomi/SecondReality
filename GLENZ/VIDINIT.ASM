SC_INDEX        equ     3c4h    ;Sequence Controller Index register
GC_INDEX        equ     3ceh    ;Graphics Controller Index register
CRTC_INDEX      equ     3d4h    ;CRT Controller Index register
MEMORY_MODE     equ     4       ;Memory Mode register index in SC
UNDERLINE       equ     14h     ;Underline Location reg index in CRTC
MODE_CONTROL    equ     17h     ;Mode Control register index in CRTC
GRAPHICS_MODE   equ     5       ;Graphics Mode register index in GC
MISCELLANEOUS   equ     6       ;Miscellaneous register index in GC

hseq1 LABEL WORD ;0=start
	dw	479	;display enable end
	dw	485	;start vertical blank
	dw	490	;start vertical retrace
	dw	500	;end vertical retrace
	dw	505	;end vertical blank
	dw	510	;vertical total
	dw	480+1	;200/350/400/480=lines/monitor, +1=double scanning
	
hseq2 LABEL WORD ;0=start
	dw	349	;display enable end
	dw	354	;start vertical blank
	dw	360+50	;start vertical retrace
	dw	375+100	;end vertical retrace
	dw	395+100	;end vertical blank
	dw	399+100	;vertical total
	
tweak320x200 PROC NEAR
;
; First, go to normal 320x200 256-color mode, which is really a
; 320x400 256-color mode with each line scanned twice.
;
	mov     ax,0013h  ;AH = 0 means mode set, AL = 13h selects
			; 256-color graphics mode
	int     10h     ;BIOS video interrupt
	mov	dx,3c8h
	mov	al,0
	out	dx,al
	inc	dx
	mov	cx,768
reclr:	out	dx,al
	loop	reclr
;
; Change CPU addressing of video memory to linear (not odd/even,
; chain, or chain 4), to allow us to access all 256K of display
; memory. When this is done, VGA memory will look just like memory
; in modes 10h and 12h, except that each byte of display memory will
; control one 256-color pixel, with 4 adjacent pixels at any given
; address, one pixel per plane.
;
	mov     dx,SC_INDEX
	mov     al,MEMORY_MODE
	out     dx,al
	inc     dx
	in      al,dx
	and     al,not 08h      ;turn off chain 4
	or      al,04h          ;turn off odd/even
	out     dx,al
	mov     dx,GC_INDEX
	mov     al,GRAPHICS_MODE
	out     dx,al
	inc     dx
	in      al,dx
	and     al,not 10h      ;turn off odd/even
	out     dx,al
	dec     dx
	mov     al,MISCELLANEOUS
	out     dx,al
	inc     dx
	in      al,dx
	and     al,not 02h      ;turn off chain
	out     dx,al

;
; Tweak the mode to 320x200 256-color mode by scanning each
; line twice.
;
	mov     dx,CRTC_INDEX
	mov     al,9
;	out     dx,al
	inc     dx
;	in      al,dx
	and     al,not 5fh      ;set maximum scan line = 0
	OR      AL,1
;	out     dx,al
	dec     dx
;
; Change CRTC scanning from doubleword mode to byte mode, allowing
; the CRTC to scan more than 64K of video data.
;
	mov     al,UNDERLINE
	out     dx,al
	inc     dx
	in      al,dx
	and     al,not 40h      ;turn off doubleword
	out     dx,al
	dec     dx
	mov     al,MODE_CONTROL
	out     dx,al
	inc     dx
	in      al,dx
	or      al,40h  ;turn on the byte mode bit, so memory is
			; scanned for video data in a purely
			; linear way, just as in modes 10h and 12h
	out     dx,al
	
	ret
	
;
; First, go to normal 320x200 256-color mode, which is really a
; 320x400 256-color mode with each line scanned twice.
;
	mov     ax,0013h  ;AH = 0 means mode set, AL = 13h selects
			; 256-color graphics mode
	int     10h     ;BIOS video interrupt
;
; Change CPU addressing of video memory to linear (not odd/even,
; chain, or chain 4), to allow us to access all 256K of display
; memory. When this is done, VGA memory will look just like memory
; in modes 10h and 12h, except that each byte of display memory will
; control one 256-color pixel, with 4 adjacent pixels at any given
; address, one pixel per plane.
;
	mov     dx,SC_INDEX
	mov     al,MEMORY_MODE
	out     dx,al
	inc     dx
	in      al,dx
	and     al,not 08h      ;turn off chain 4
	or      al,04h          ;turn off odd/even
	out     dx,al
	mov     dx,GC_INDEX
	mov     al,GRAPHICS_MODE
	out     dx,al
	inc     dx
	in      al,dx
	and     al,not 10h      ;turn off odd/even
	out     dx,al
	dec     dx
	mov     al,MISCELLANEOUS
	out     dx,al
	inc     dx
	in      al,dx
	and     al,not 02h      ;turn off chain
	out     dx,al

	mov     dx,SC_INDEX
	mov     ax,00f02h       ;CLEAR ALL PLANES
	out     dx,ax
	;mov     ax,cs:vram
	;mov     es,ax
	;mov     cx,32768
	;xor     ax,ax
	;rep     stosw
;
; Tweak the mode to 320x200 256-color mode by scanning each
; line twice.
;
	mov     dx,CRTC_INDEX
	mov     al,9
	out     dx,al
	inc     dx
	in      al,dx
	and     al,not 5fh      ;set maximum scan line = 0
	OR      AL,1
	out     dx,al
	dec     dx
;
; Change CRTC scanning from doubleword mode to byte mode, allowing
; the CRTC to scan more than 64K of video data.
;
	mov     al,UNDERLINE
	out     dx,al
	inc     dx
	in      al,dx
	and     al,not 40h      ;turn off doubleword
	out     dx,al
	dec     dx
	mov     al,MODE_CONTROL
	out     dx,al
	inc     dx
	in      al,dx
	or      al,40h  ;turn on the byte mode bit, so memory is
			; scanned for video data in a purely
			; linear way, just as in modes 10h and 12h
	out     dx,al
	
	mov	dx,CRTC_INDEX
	mov	ax,2813h
	out	dx,ax ;width>=320
	
	ret
tweak320x200 ENDP

sethseq PROC NEAR ;ds:si=hseq
	mov	bl,1fh 	;overflow
	mov	bh,40h	;maximum scan line
	test	word ptr ds:[si+12],1
	jz	sss9
	or	bh,80h
sss9:
	mov	ax,ds:[si+0]
	test	ax,256
	jz	sss3
	or	bl,2
sss3:	test	ax,512
	jz	sss4
	or	bl,64
sss4:	mov	ah,al
	mov	dx,3d4h
	mov	al,12h
	out	dx,ax
	
	mov	ax,ds:[si+2]
	test	ax,256
	jz	sss1
	or	bl,8
sss1:	test	ax,512
	jz	sss2
	or	bh,32
sss2:	mov	ah,al
	mov	dx,3d4h
	mov	al,15h
	out	dx,ax
	
	mov	ax,ds:[si+4]
	test	ax,256
	jz	sss7
	or	bl,4
sss7:	test	ax,512
	jz	sss8
	or	bl,128
sss8:	mov	ah,al
	mov	dx,3d4h
	mov	al,10h
	out	dx,ax
	
	mov	ax,ds:[si+6]
	mov	ah,al
	and	ah,15
	or	ah,20h ;disable vertical int
	mov	dx,3d4h
	mov	al,11h
	out	dx,ax
	
	mov	ax,ds:[si+8]
	mov	ah,al
	mov	dx,3d4h
	mov	al,16h
	out	dx,ax
	
	mov	ax,ds:[si+10]
	test	ax,256
	jz	sss5
	or	bl,1
sss5:	test	ax,512
	jz	sss6
	or	bl,32
sss6:	mov	ah,al
	mov	dx,3d4h
	mov	al,6h
	out	dx,ax
	
	mov	dx,3d4h
	mov	al,7h
	mov	ah,bl
	out	dx,ax
	
	mov	dx,3d4h
	mov	al,9h
	mov	ah,bh
	out	dx,ax

	mov	ax,ds:[si+12]
	and	ax,not 1
	mov	bh,0
	cmp	ax,350
	jne	sss10
	mov	bh,1*64
sss10:	cmp	ax,400
	jne	sss11
	mov	bh,2*64
sss11:	cmp	ax,480
	jne	sss12
	mov	bh,3*64
sss12:	mov	dx,3cch
	in	al,dx
	and	al,3fh
	or	al,bh
	mov	dx,3c2h
	out	dx,al
	ret
sethseq ENDP

vptbl	dw	06a00h	; horz total 
	dw	05901h	; horz displayed
	dw	05a02h	; start horz blanking
	dw	08d03h	; end horz blanking
	dw	05e04h	; start h sync
	dw	08a05h	; end h sync
	dw	08b06h ;0d06h	; vertical total
	dw	01f07h ;03e07h	; overflow
	dw	04009h	; cell height
	dw	06810h	; v sync start
	dw	02a11h	; v sync end and protect cr0-cr7
	dw	05d12h	; vertical displayed
	dw	02e13h	; offset 92
	dw	00014h	; turn off dword mode
	dw	05d15h	; v blank start
	dw	00016h	; v blank end
	dw	0e317h	; turn on byte mode
vpend	label	word

mode13x	proc
	push	ds
	mov	ax,cs
	mov	ds,ax

	mov	ax,13h		; start with standard mode 13h
	int	10h		; let the bios set the mode

	mov	dx,3c4h		; alter sequencer registers
	mov	ax,0604h	; disable chain 4
	out	dx,ax

	mov	ax,0f02h	; set write plane mask to all bit planes
	out	dx,ax
	push	di
	xor	di,di
	mov	ax,0a000h	; screen starts at segment A000
	mov	es,ax
	mov	cx,21600	; ((XSIZE*YSIZE)/(4 planes))/(2 bytes per word)
	xor	ax,ax
	cld
	rep	stosw		; clear the whole of the screen
	pop	di

	mov	ax,0100h	; synchronous reset
	out	dx,ax		; asserted
	mov	dx,3c2h		; misc output
	mov	al,0a7h		; use 28 mHz dot clock ;a7
	out	dx,al		; select it
	mov	dx,3c4h		; sequencer again
	mov	ax,0300h	; restart sequencer
	out	dx,ax		; running again

	mov	dx,3d4h		; alter crtc registers

	mov	al,11h		; cr11
	out	dx,al		; current value
	inc	dx		; point to data
	in	al,dx		; get cr11 value
	and	al,7fh		; remove cr0 -> cr7
	out	dx,al		;    write protect
	dec	dx		; point to index
	cld
	mov	si,offset vptbl
	mov	cx,((offset vpend)-(offset vptbl)) shr 1
outlp:	lodsw
	out	dx,ax
	loop	outlp

	mov	ax,cs
	mov	ds,ax
	mov	si,OFFSET hseq2
	call	sethseq

;	mov	dx,3dah
;	in	al,dx
;	mov	dx,3c0h
;	mov	al,11h+32
;	out	dx,al
;	mov	al,8
;	out	dx,al

	pop	ds
	ret
mode13x	endp

tweak640x400 PROC NEAR
;
; First, go to normal 320x200 256-color mode, which is really a
; 320x400 256-color mode with each line scanned twice.
;
	mov     ax,002fh
	int     10h     ;BIOS video interrupt
;
; Change CPU addressing of video memory to linear (not odd/even,
; chain, or chain 4), to allow us to access all 256K of display
; memory. When this is done, VGA memory will look just like memory
; in modes 10h and 12h, except that each byte of display memory will
; control one 256-color pixel, with 4 adjacent pixels at any given
; address, one pixel per plane.
;
	mov     dx,SC_INDEX
	mov     al,MEMORY_MODE
	out     dx,al
	inc     dx
	in      al,dx
	and     al,not 08h      ;turn off chain 4
	or      al,04h          ;turn off odd/even
	out     dx,al
	mov     dx,GC_INDEX
	mov     al,GRAPHICS_MODE
	out     dx,al
	inc     dx
	in      al,dx
	and     al,not 10h      ;turn off odd/even
	out     dx,al
	dec     dx
	mov     al,MISCELLANEOUS
	out     dx,al
	inc     dx
	in      al,dx
	and     al,not 02h      ;turn off chain
	out     dx,al
	ret
;
; Change CRTC scanning from doubleword mode to byte mode, allowing
; the CRTC to scan more than 64K of video data.
;
	mov     al,UNDERLINE
	out     dx,al
	inc     dx
	in      al,dx
	and     al,not 40h      ;turn off doubleword
	out     dx,al
	dec     dx
	mov     al,MODE_CONTROL
	out     dx,al
	inc     dx
	in      al,dx
	or      al,40h  ;turn on the byte mode bit, so memory is
			; scanned for video data in a purely
			; linear way, just as in modes 10h and 12h
	out     dx,al
	
	ret
tweak640x400 ENDP

hseq3 LABEL WORD ;0=start
	dw	349	;display enable end
	dw	355	;start vertical blank
	dw	395	;start vertical retrace
	dw	405	;end vertical retrace
	dw	445	;end vertical blank
	dw	455	;vertical total
	dw	350	;200/350/400/480=lines/monitor, +1=double scanning

tweak360x350	proc
	push	ds
	mov	ax,cs
	mov	ds,ax

	mov	ax,13h		; start with standard mode 13h
	int	10h		; let the bios set the mode

	mov	dx,3c4h		; alter sequencer registers
	mov	ax,0604h	; disable chain 4
	out	dx,ax

	mov	ax,0f02h	; set write plane mask to all bit planes
	out	dx,ax
	push	di
	xor	di,di
	mov	ax,0a000h	; screen starts at segment A000
	mov	es,ax
	mov	cx,21600	; ((XSIZE*YSIZE)/(4 planes))/(2 bytes per word)
	xor	ax,ax
	cld
	rep	stosw		; clear the whole of the screen
	pop	di

	mov	ax,0100h	; synchronous reset
	out	dx,ax		; asserted
	mov	dx,3c2h		; misc output
	mov	al,0a7h		; use 28 mHz dot clock
	out	dx,al		; select it
	mov	dx,3c4h		; sequencer again
	mov	ax,0300h	; restart sequencer
	out	dx,ax		; running again

	mov	dx,3d4h		; alter crtc registers

	mov	al,11h		; cr11
	out	dx,al		; current value
	inc	dx		; point to data
	in	al,dx		; get cr11 value
	and	al,7fh		; remove cr0 -> cr7
	out	dx,al		;    write protect
	dec	dx		; point to index
	cld
	mov	si,offset vptbl
	mov	cx,((offset vpend)-(offset vptbl)) shr 1
outlpn:	lodsw
	out	dx,ax
	loop	outlpn

	mov	ax,cs
	mov	ds,ax
	mov	si,OFFSET hseq3
	call	sethseq

	pop	ds
	ret
tweak360x350 endp

hseq4 LABEL WORD ;0=start
	dw	349	;display enable end
	dw	355	;start vertical blank
	dw	395	;start vertical retrace
	dw	405	;end vertical retrace
	dw	450	;end vertical blank
	dw	455	;vertical total
	dw	350	;200/350/400/480=lines/monitor, +1=double scanning

tweak320x350	proc
	call	tweak360x350
	ret
tweak320x350 endp

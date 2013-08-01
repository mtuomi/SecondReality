include asm.inc

;publics:
;setvmode()

extrn _bgpic:far

text__vid SEGMENT para public 'CODE'
	ASSUME cs:text__vid
	
.386

modes	dw	OFFSET v320x200x256
	dw	OFFSET v320x350x256
	dw	OFFSET v320x350x256
	dw	OFFSET v320x240x256
	dw	OFFSET vtseng640x400x256

PUBLIC setvmode
setvmode PROC FAR
	;ax=mode => ds:si
	mov	bx,ax
	shl	bx,1
	mov	bx,cs:modes[bx]
	mov	cx,16
@@1:	mov	ax,cs:[bx]
	mov	ds:[si],ax
	mov	word ptr ds:[si+2],cs
	add	si,4
	add	bx,2
	loop	@@1
	ret
setvmode ENDP

;video mode structure
v320x200x256 LABEL WORD
	dw	OFFSET init320x200	;init routine
	dw	OFFSET switch320x200	;page switcher
	dw	OFFSET clear64k		;clear all screens
	dw	OFFSET twpset
	dw	OFFSET twlineto
	dw	OFFSET twhline
	dw	OFFSET twhlinegroup
	dw	OFFSET twthlinegroup
	dw	OFFSET polygroup
	dw	OFFSET clearpage
	dw	OFFSET fwaitborder
v320x350x256 LABEL WORD
	dw	OFFSET init320x350	;init routine
	dw	OFFSET switch320x350	;page switcher
	dw	OFFSET clear64k		;clear all screens
	dw	OFFSET twpset
	dw	OFFSET twlineto
	dw	OFFSET twhline
	dw	OFFSET twhlinegroup
	dw	OFFSET twthlinegroup
	dw	OFFSET polygroup
	dw	OFFSET clearpage
	dw	OFFSET fwaitborder
v320x350x256unr LABEL WORD
	dw	OFFSET init320x350unr   ;init routine
	dw	OFFSET switch320x350	;page switcher
	dw	OFFSET clear64k		;clear all screens
	dw	OFFSET twpset
	dw	OFFSET twlineto
	dw	OFFSET twhline
	dw	OFFSET twhlinegroup
	dw	OFFSET twthlinegroup
	dw	OFFSET polygroup
	dw	OFFSET clearpage
	dw	OFFSET fwaitborder
v320x240x256 LABEL WORD
	dw	OFFSET init320x240	;init routine
	dw	OFFSET switch320x240	;page switcher
	dw	OFFSET clear64k		;clear all screens
	dw	OFFSET twpset
	dw	OFFSET twlineto
	dw	OFFSET twhline
	dw	OFFSET twhlinegroup
	dw	OFFSET twthlinegroup
	dw	OFFSET polygroup
	dw	OFFSET clearpage
	dw	OFFSET fwaitborder
vtseng640x400x256 LABEL WORD
	dw	OFFSET init640x400	;init routine
	dw	OFFSET switch640x400	;page switcher
	dw	OFFSET clear640x400	;clear all screens
	dw	OFFSET twpset
	dw	OFFSET twlineto
	dw	OFFSET twhline
	dw	OFFSET twhlinegroup
	dw	OFFSET twthlinegroup
	dw	OFFSET polygroup
	dw	OFFSET clearpage
	dw	OFFSET fwaitborder

include vidmisc.asm
include vidinit.asm
include vidtwe.asm
include vidnrm.asm
include vidpoly.asm

;****************************************************************

setrows PROC NEAR
	mov	cs:rowsadd,dx
	mov	cs:truerowsadd,dx
	xor	ax,ax
	mov	bx,OFFSET rows
ain1:	mov	ds:[bx],ax
	add	ax,dx
	add	bx,2
	loop	ain1
	ret
setrows ENDP

ALIGN 2
pagep	dw	0
wpage	dw	0,1,2
wpage2	dw	0,4,8
spage	dw	2,0,1

init640x400 PROC FAR
	LOADDS
	mov	ds:projxmul,256*2
	mov	ds:projymul,213*2
	mov	ds:projxadd,320
	mov	ds:projyadd,200
	mov	ds:projminz,128
	mov	ds:projminzshr,7
	mov	ds:wminx,0
	mov	ds:wminy,0
	mov	ds:wmaxx,639
	mov	ds:wmaxy,399
	mov	ds:framerate10,700 ;70 frames/sec
	;
	mov	ax,11h ;640x480/mono
	int	10h
	call	tweak640x400
	mov	cx,400
	mov	dx,160
	call	setrows
	ret
init640x400 ENDP

switch640x400 PROC FAR
swipa2:	mov	ax,0a000h
	mov	cs:vram,ax
	mov	cs:truevram,ax
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
switch640x400 ENDP

clear640x400 PROC FAR
	call	switch640x400
	call	clear64k
	call	switch640x400
	call	clear64k
	call	switch640x400
	call	clear64k
	call	switch640x400
	call	clear64k
	ret
clear640x400 ENDP

;****************************************************************

ALIGN 2
t324v	dw	0a5f0h
t324v1	dw	0aaa0h
t324v2	dw	0a140h
t324vout dw	01400h
t324vout1 dw	05f00h
t324vout2 dw	0aa00h

init320x200 PROC FAR
	LOADDS
	mov	ds:projxmul,256
	mov	ds:projymul,213
	mov	ds:projxadd,160
	mov	ds:projyadd,130
	mov	ds:projminz,128
	mov	ds:projminzshr,7
	mov	ds:wminx,0
	mov	ds:wminy,0
	mov	ds:wmaxx,319
	mov	ds:wmaxy,199
	mov	ds:framerate10,700
	;
;	call	tweak320x200
	mov	cx,200
	mov	dx,80
	call	setrows
	ret
init320x200 ENDP

init320x240 PROC FAR
	LOADDS
	mov	ds:projxmul,256
	mov	ds:projymul,256
	mov	ds:projxadd,160
	mov	ds:projyadd,120
	mov	ds:projminz,128
	mov	ds:projminzshr,7
	mov	ds:wminx,0
	mov	ds:wminy,0
	mov	ds:wmaxx,319
	mov	ds:wmaxy,239
	mov	ds:framerate10,610 ;60 frames/sec
	;
	call	tweak320x200
	LOADDS
	mov	si,OFFSET hseq1
	call	sethseq ;to 240 mode
	mov	cx,240
	mov	dx,80
	call	setrows
	ret
init320x240 ENDP

switch320x240b PROC FAR
	LOADDS
 	mov	ax,cs:t324v
	mov	bx,cs:t324v1
	mov	cs:t324v,bx
	mov	cs:t324v1,ax
	mov	ds:vram,bx
	mov	ds:truevram,bx
	mov	bx,cs:t324vout1
	mov	cx,cs:t324vout2
	mov	cs:t324vout1,cx
	mov	cs:t324vout2,bx
	mov	dx,3d4h
	mov	al,0ch
	mov	ah,bh
	out	dx,ax
	ret
switch320x240b ENDP

switch320x240 PROC FAR
switch320x200:
	LOADDS
	mov	ax,cs:t324v
	mov	bx,cs:t324v1
	mov	cx,cs:t324v2
	mov	cs:t324v,bx
	mov	cs:t324v1,cx
	mov	cs:t324v2,ax
	mov	ds:vram,bx
	mov	ds:truevram,bx
	mov	ax,cs:t324vout
	mov	bx,cs:t324vout1
	mov	cx,cs:t324vout2
	mov	cs:t324vout,bx
	mov	cs:t324vout1,cx
	mov	cs:t324vout2,ax
	mov	dx,3d4h
	mov	al,0ch
	mov	ah,bh
	out	dx,ax
	ret
switch320x240 ENDP

clear64k PROC FAR
	cld
	mov	dx,3c4h
	mov	ax,0f02h
	out	dx,ax
	mov	ax,0a000h
	mov	es,ax
	mov	cx,32768
	xor	ax,ax
	xor	di,di
	rep	stosw
	ret
clear64k ENDP

;****************************************************************

init320x350unr PROC FAR
	LOADDS
	mov	ds:projxmul,256
	mov	ds:projymul,420
	mov	ds:projxadd,180
	mov	ds:projyadd,175
	mov	ds:projminz,128
	mov	ds:projminzshr,7
	mov	ds:wminx,0
	mov	ds:wminy,32
	mov	ds:wmaxx,359
	mov	ds:wmaxy,349-32
	mov	ds:framerate10,700 ;70 frames/sec
	;
	call	tweak320x350
	mov	cx,350
	mov	dx,92
	call	setrows
	ret
init320x350unr ENDP

init320x350 PROC FAR
	LOADDS
	mov	ds:projxmul,256
	mov	ds:projymul,420
	mov	ds:projxadd,180
	mov	ds:projyadd,175
	mov	ds:projminz,128
	mov	ds:projminzshr,7
	mov	ds:wminx,0
	mov	ds:wminy,0
	mov	ds:wmaxx,359
	mov	ds:wmaxy,349
	mov	ds:framerate10,700 ;70 frames/sec
	;
	call	tweak360x350
	mov	cx,350
	mov	dx,92
	call	setrows
	ret
init320x350 ENDP

ALIGN 2
t365v	dw	0a000h
t365v1	dw	0a800h
t365vo	dw	08000h
t365vo1 dw	00000h

switch320x350 PROC FAR
	LOADDS
	mov	ax,cs:t365v
	xchg	ax,cs:t365v1
	mov	cs:t365v,ax
	mov	ds:vram,ax
	mov	ds:truevram,ax
	mov	bx,cs:t365vo
	xchg	bx,cs:t365vo1
	mov	cs:t365vo,bx
	mov	dx,3d4h
	mov	al,0ch
	mov	ah,bh
	out	dx,ax
	call	waitborder
	ret
switch320x350 ENDP

justret PROC FAR
	ret
justret	ENDP

clearpage PROC FAR
	push	ax
	mov	dx,3c4h
	mov	ax,0f02h
	out	dx,ax
	mov	es,ds:vram
	mov	di,ds:wminy
	shl	di,1
	mov	di,ds:rows[di]
	mov	ax,ds:wminx
	mov	dx,ds:wmaxx
	sub	dx,ax
	shr	ax,2
	add	di,ax
	add	dx,7
	shr	dx,4
	mov	cx,ds:wmaxy
	sub	cx,ds:wminy
	inc	cx
	pop	ax
	push	ax
	shl	eax,16
	pop	ax
@@1:	push	cx
	push	di
	mov	cx,dx
	rep	stosd
	pop	di
	pop	cx
	add	di,ds:rowsadd
	loop	@@1
	ret
clearpage ENDP

text__vid ENDS
	END
	
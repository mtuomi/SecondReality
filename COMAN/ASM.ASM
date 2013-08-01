EXTRN theloop:far

code SEGMENT para public 'CODE'
ASSUME cs:code
.386
LOCALS

PUBLIC _wave1,_wave2,_vbuf,_cameralevel
_wave1	dw	0,0
_wave2	dw	0,0
_vbuf	dw	0,0
_cameralevel dw 0

PUBLIC _sin1024
include sin1024.inc

PUBLIC _combguse
_combguse db	90*160 dup(0)

PUBLIC _setborder
_setborder PROC FAR
	push	bp
	mov	bp,sp
	mov	dx,3dah
	in	al,dx
	mov	dx,3c0h
	mov	al,11h+32
	out	dx,al
	mov	al,[bp+6]
	out	dx,al
	pop	bp
	ret
_setborder ENDP

PUBLIC _inittwk
_inittwk PROC FAR
	push	bp
	mov	bp,sp
	push	si
	push	di
	push	ds
	;clear palette
	mov	dx,3c8h
	xor	al,al
	out	dx,al
	inc	dx
	mov	cx,768
@@1:	out	dx,al
	loop	@@1
	mov	dx,3d4h
	;400 rows
	;mov	ax,00009h
	;out	dx,ax
	;tweak
	mov	ax,00014h
	out	dx,ax
	mov	ax,0e317h
	out	dx,ax
	mov	dx,3c4h
	mov	ax,0604h
	out	dx,ax
	;
	mov	dx,3c4h
	mov	ax,0f02h
	out	dx,ax
	mov	ax,0a000h
	mov	es,ax
	xor	di,di
	mov	cx,32768
	xor	ax,ax
	rep	stosw
	;
	pop	ds
	pop	di
	pop	si
	pop	bp
	ret
_inittwk ENDP

PUBLIC _setpalarea
_setpalarea PROC FAR
	push	bp
	mov	bp,sp
	push	si
	push	di
	push	ds
	lds	si,[bp+6]
	mov	ax,[bp+10]
	mov	dx,3c8h
	out	dx,al
	mov	cx,[bp+12]
	mov	ax,cx
	shl	cx,1
	add	cx,ax
	inc	dx
	rep	outsb
	sti
	pop	ds
	pop	di
	pop	si
	pop	bp
	ret
_setpalarea ENDP

PUBLIC _docol
_docol	PROC FAR
	push	bp
	mov	bp,sp
	push	si
	push	di
	push	ds
	mov	es,cs:_vbuf[2]
	mov	fs,cs:_wave1[2]
	mov	gs,cs:_wave2[2]
	mov	ax,not 1
	mov	si,[bp+6]
	and	si,ax
	mov	di,[bp+8]
	and	di,ax
	mov	cx,[bp+10]
	and	cx,ax
	mov	dx,[bp+12]
	and	dx,ax
	mov	bp,[bp+14]
	mov	ax,cs:_cameralevel
	call	theloop
	pop	ds
	pop	di
	pop	si
	pop	bp
	ret
_docol	ENDP

PUBLIC _docopy
_docopy	PROC FAR
	push	bp
	mov	bp,sp
	push	si
	push	di
	push	ds
	mov	es,[bp+6]

IFDEF PXLSUX
	mov	ax,cs
	mov	ds,ax
	mov	si,OFFSET _combguse
	mov	di,0
	mov	cx,60
	;
@@7:	mov	dx,3c4h
	mov	ax,0302h
	out	dx,ax
	zzz=0
	REPT	20
	mov	eax,ds:[si+zzz]
	mov	es:[di+zzz],eax
	zzz=zzz+4
	ENDM
	mov	dx,3c4h
	mov	ax,0C02h
	out	dx,ax
	zzz=0
	REPT	20
	mov	eax,ds:[si+zzz+80]
	mov	es:[di+zzz],eax
	zzz=zzz+4
	ENDM
	add	si,160
	add	di,80
	dec	cx
	jnz	@@7
ENDIF
	mov	dx,3c4h
	mov	ax,0f02h
	out	dx,ax
	mov	es,[bp+6]
	mov	ds,cs:_vbuf[2]
	mov	si,60*160
	mov	di,52*80
	mov	cx,18*80/4
	xor	eax,eax
	rep	stosd
	mov	cx,140
	mov	bl,255
	mov	bh,bl
	mov	ax,bx
	shl	ebx,16
	mov	bx,ax
	;
@@1:	mov	dx,3c4h
	mov	ax,0302h
	out	dx,ax
	zzz=0
	REPT	20
	mov	eax,ds:[si+zzz]
	mov	es:[di+zzz],eax
	zzz=zzz+4
	ENDM
	mov	dx,3c4h
	mov	ax,0C02h
	out	dx,ax
	zzz=0
	REPT	20
	mov	eax,ds:[si+zzz+80]
	mov	es:[di+zzz],eax
	zzz=zzz+4
	ENDM
	add	si,160
	add	di,80
	dec	cx
	jnz	@@1

;	mov	ax,cs
;	mov	ds,ax
;	mov	si,OFFSET _combguse+60*160
	xor	eax,eax
	mov	es,cs:_vbuf[2]
	mov	di,68*160
	mov	cx,30*160/4/4
@@cl1:	mov	es:[di],eax
	add	di,4
	mov	es:[di],eax
	add	di,4
	mov	es:[di],eax
	add	di,4
	mov	es:[di],eax
	add	di,4
	dec	cx
	jnz	@@cl1
	
	pop	ds
	pop	di
	pop	si
	pop	bp
	ret
_docopy ENDP

code ENDS
END
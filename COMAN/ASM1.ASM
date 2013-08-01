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
	mov	si,[bp+6]
	mov	di,[bp+8]
	mov	cx,[bp+10]
	add	cx,cx
	mov	dx,[bp+12]
	add	dx,dx
	mov	bp,[bp+14]
	mov	ax,cs:_cameralevel
	int	3
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
	mov	ds,cs:_vbuf[2]
	mov	si,50*160
	mov	di,50*80
	mov	cx,150
	mov	bl,255
	mov	bh,bl
	mov	ax,bx
	shl	ebx,16
	mov	bx,ax
@@1:	cmp	cx,100
	jge	@@2
	mov	dx,3c4h
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
	pop	ds
	pop	di
	pop	si
	pop	bp
	ret
@@2:	;this loop also clears
	mov	dx,3c4h
	mov	ax,0302h
	out	dx,ax
	zzz=0
	REPT	20
	mov	eax,ds:[si+zzz]
	mov	ds:[si+zzz],ebx
	mov	es:[di+zzz],eax
	zzz=zzz+4
	ENDM
	mov	dx,3c4h
	mov	ax,0C02h
	out	dx,ax
	zzz=0
	REPT	20
	mov	eax,ds:[si+zzz+80]
	mov	ds:[si+zzz+80],ebx
	mov	es:[di+zzz],eax
	zzz=zzz+4
	ENDM
	sub	ebx,01010101h
	cmp	bl,192
	jge	@@3
	mov	ebx,0c0c0c0c0h
@@3:	add	si,160
	add	di,80
	dec	cx
	jmp	@@1
_docopy ENDP

code ENDS
END
code SEGMENT para public 'CODE'
ASSUME cs:code
.386
LOCALS

PUBLIC _vram
_vram 	dw	0,0a000h

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

PUBLIC _leftline
_leftline PROC FAR
	push	bp
	mov	bp,sp
	push	si
	push	di
	push	ds
	
	mov	ax,80
	mul	word ptr [bp+6]
	mov	di,ax
	mov	ds,cs:_vram[2]
	
	mov	cx,160
	mov	ax,[bp+8]
	or	ax,ax
	jz	@@1
	sub	cx,ax
	
	mov	bx,cx
	shr	bx,2
	add	di,bx
	and	cl,3
	mov	ax,0ff02h
	shl	ah,cl
	mov	dx,3c4h
	out	dx,ax
	
	mov	ah,[bp+10]
	mov	ds:[di],ah
	inc	di
	inc	dx
	mov	al,0fh
	out	dx,al
	mov	al,ah
	test	di,1
	jz	@@2
	mov	ds:[di],ah
	inc	di
	inc	bx
@@2:	mov	cx,39
	sub	bx,cx
	add	bx,bx
	and	bx,not 3
	;bx=-4*(number of words)
	add	bx,OFFSET @@fill
	jmp	bx
	zzz=80
	REPT	40
	zzz=zzz-2
	db 89h,85h,zzz,0	;mov ds:[di+zzz],ax
	ENDM
@@fill:	
@@1:	pop	ds
	pop	di
	pop	si
	pop	bp
	ret
_leftline ENDP

PUBLIC _rightline
_rightline PROC FAR
	push	bp
	mov	bp,sp
	push	si
	push	di
	push	ds
	
	mov	ax,80
	mul	word ptr [bp+6]
	mov	di,ax
	mov	ds,cs:_vram[2]
	
	mov	cx,160
	mov	ax,[bp+8]
	or	ax,ax
	jz	@@1
	add	cx,ax
	
	mov	bx,cx
	shr	bx,2
	add	di,bx
	and	cl,3
	mov	ax,0f002h
	rol	ah,cl
	mov	dx,3c4h
	out	dx,ax
	
	mov	ah,[bp+10]
	mov	ds:[di],ah
	dec	di
	inc	dx
	mov	al,0fh
	out	dx,al
	
	mov	al,ah
	mov	cx,bx
	test	di,1
	jnz	@@2
	mov	ds:[di],ah
	dec	cx
@@2:	sub	cx,40
	neg	cx
	add	cx,cx
	and	cx,not 3
	;bx=-4*(number of words)
	add	cx,OFFSET @@fill
	sub	di,bx
	add	di,41
	jmp	cx
	zzz=80
	REPT	40
	zzz=zzz-2
	db 89h,85h,zzz,0	;mov ds:[di+zzz],ax
	ENDM
@@fill:	
@@1:	pop	ds
	pop	di
	pop	si
	pop	bp
	ret
_rightline ENDP

testcopper PROC FAR
	mov	dx,3c8h
	xor	al,al
	out	dx,al
	inc	dx
	mov	al,63
	out	dx,al
	out	dx,al
	out	dx,al
	mov	cx,100
@@1:	loop	@@1
	mov	dx,3c8h
	xor	al,al
	out	dx,al
	inc	dx
	mov	al,0
	out	dx,al
	out	dx,al
	out	dx,al
	ret
testcopper ENDP

PUBLIC _initcoppers
_initcoppers PROC FAR
	push	bp
	mov	bp,sp
	push	si
	push	di
	push	ds
	mov	bx,7
	mov	ax,0
	mov	cx,OFFSET testcopper
	mov	dx,cs
	int	0fch
	mov	bx,7
	mov	ax,1
	mov	cx,OFFSET testcopper
	mov	dx,cs
	int	0fch
	mov	bx,7
	mov	ax,2
	mov	cx,OFFSET testcopper
	mov	dx,cs
;	int	0fch
	pop	ds
	pop	di
	pop	si
	pop	bp
	ret
_initcoppers ENDP

code ENDS
END
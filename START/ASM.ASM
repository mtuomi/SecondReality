include clink.inc
code 	SEGMENT para public 'CODE'
	ASSUME cs:code
LOCALS
.386
	
waitb	PROC NEAR
	mov	dx,3dah
@@1:	in	al,dx
	test	al,8
	jnz	@@1
@@2:	in	al,dx
	test	al,8
	jz	@@2
	ret
waitb	ENDP

PUBLIC _waitb
_waitb PROC FAR
	CBEG
	call	waitb
	CEND
_waitb ENDP

PUBLIC _setborder
_setborder PROC FAR
	CBEG
	mov	dx,3dah
	in	al,dx
	mov	dx,3c0h
	mov	al,11h+32
	out	dx,al
	movpar	ax,0
	out	dx,al
	CEND
_setborder ENDP

PUBLIC _initvideo
_initvideo PROC FAR
	CBEG
	mov	ax,13h
	int	10h
	call	waitb
	;clear palette
	mov	dx,3c8h
	xor	al,al
	out	dx,al
	inc	dx
	mov	cx,768
@@1:	out	dx,al
	loop	@@1
	;400 rows
	mov	dx,3d4h
	mov	ax,00009h
	out	dx,ax
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
	;640 wide
	mov	dx,3d4h
	mov	ax,05013h
	out	dx,ax
	CEND
_initvideo ENDP
	
PUBLIC _deinitvideo
_deinitvideo PROC FAR
	CBEG
	mov	ax,3h
	int	10h
	CEND
_deinitvideo ENDP
	
PUBLIC _loadpal
_loadpal PROC FAR
	CBEG
	call	waitb
	;clear palette
	mov	dx,3c8h
	xor	al,al
	out	dx,al
	inc	dx
	mov	cx,768
	movpar	si,0
	movpar	ds,1
	rep	outsb
	CEND
_loadpal ENDP

PUBLIC _lineblit
_lineblit PROC FAR
	CBEG
	movpar	ax,0
	mov	cx,160
	mul	cx
	mov	di,ax
	mov	ax,0a000h
	mov	es,ax
	movpar	si,1
	movpar	ds,2
	zpl=0
	REPT	4
	mov	dx,3c4h
	mov	ax,02h+(100h shl zpl)
	out	dx,ax
	zzz=0
	REPT	160/2
	mov	al,ds:[si+(zzz+0)*4+zpl]
	mov	ah,ds:[si+(zzz+1)*4+zpl]
	mov	es:[di+zzz],ax
	zzz=zzz+2
	ENDM
	zpl=zpl+1
	ENDM
	rep	outsb
	CEND
_lineblit ENDP

PUBLIC _pget
_pget	PROC FAR
	CBEG
	movpar	bx,1
	mov	ax,160
	mul	bx
	mov	di,ax
	movpar	dx,0
	mov	ax,dx
	shr	dx,2
	add	di,dx
	mov	ah,al
	and	ah,3
	mov	al,4
	mov	dx,3ceh
	out	dx,ax
	mov	ax,0a000h
	mov	es,ax
	mov	al,es:[di]
	xor	ah,ah
	CEND
_pget	ENDP

PUBLIC _pset
_pset	PROC FAR
	CBEG
	movpar	bx,1
	mov	ax,160
	mul	bx
	mov	di,ax
	movpar	cx,0
	mov	ax,cx
	shr	ax,2
	add	di,ax
	mov	ax,0102h
	and	cl,3
	shl	ah,cl
	mov	dx,3c4h
	out	dx,ax
	mov	ax,0a000h
	mov	es,ax
	movpar	ax,2
	mov	es:[di],al
	CEND
_pset	ENDP

PUBLIC _setpalarea
_setpalarea PROC FAR
	CBEG
	movpar	si,0
	movpar	ds,1
	movpar	cx,3
	movpar	ax,2
	mov	dx,3c8h
	out	dx,al
	movpar	ax,3
	mov	cx,ax
	shl	cx,1
	add	cx,ax
	inc	dx
	rep	outsb
	sti
	CEND
_setpalarea ENDP

code	ENDS
	END
	
include asm.inc

text__asm SEGMENT para public 'CODE'
	ASSUME cs:text__asm
	
.386

tmp1	dw	0
tmp2	dw	0

PUBLIC _testpset 
_testpset PROC FAR
	CBEG
	LOADDS
	mov	dx,3c4h
	mov	al,2
	out	dx,al
	mov	es,ds:vram
	mov	ax,[bp+10]
	mov	ds:color,ax
	mov	bx,[bp+8]
	mov	dx,[bp+6]
	call	VIDPSET
	CEND
_testpset ENDP

PUBLIC _testline
_testline PROC FAR
	CBEG
	LOADDS
	mov	es,ds:vram
	mov	ax,[bp+14]
	mov	ds:color,ax
	mov	ax,[bp+12]
	mov	cx,[bp+10]
	mov	bx,[bp+8]
	mov	dx,[bp+6]
	call	VIDLINE
	CEND
_testline ENDP

PUBLIC _testasm
_testasm PROC FAR
	CBEG
	LOADDS
	mov	si,OFFSET video
	mov	ax,0
	call	setvmode
	call	VIDINIT
	CEND

	CBEG
	LOADDS
	setborder 0
	mov	si,OFFSET video
	mov	ax,0
	call	setvmode
	call	VIDINIT
	;call	VIDSWITCH
	;call	VIDCLEAR64
	mov	ax,13h
	int	10h
	;
	mov	dx,3c8h
	mov	al,0
	out	dx,al
	inc	dx
	xor	al,al
	out	dx,al
	out	dx,al
	out	dx,al
	mov	al,11
	mov	cx,255
@@1:	out	dx,al
	out	dx,al
	out	dx,al
	inc	al
	loop	@@1

	call	VIDSWITCH
	push	ds:vram
	call	VIDSWITCH
	pop	ds:vram

	mov	ax,13h
	int	10h
	CEND

	LOADDS
	mov	ds:color1,60
	
@@aga:	LOADDS
	call	VIDSWITCH
	
	mov	dx,3c4h
	mov	al,2
	out	dx,al
	
	add	ds:color,1
	add	cs:tmp1,3
	add	cs:tmp2,4
	mov	cx,0
	mov	ax,cs:tmp1
	shr	ax,2
	and	ax,255
	mov	dx,cs:tmp2
	shr	dx,2
	and	dx,255
	mov	bx,199
	mov	es,ds:vram
	call	VIDLINE
	mov	ah,1
	
	mov	ax,3
	int	10h
	CEND
_testasm ENDP	

PUBLIC _asmtestmode
_asmtestmode db	0

PUBLIC _asm
_asm	PROC FAR
	CBEG
	LOADDS
	push	word ptr [bp+6]
	push	word ptr [bp+8]
	cmp	cs:_asmtestmode,0
	je	@@1

@@2:	setborder 0
	call	VIDWAITB
	call	VIDWAITB
	call	VIDWAITB
	mov	ax,0a000h
	mov	es,ax
	mov	cx,32000
	xor	ax,ax
	rep	stosw
	pop	es
	pop	di
	CEND
	
@@1:	setborder 0
	call	VIDWAITB
	pop	es
	pop	di
	CEND
_asm	ENDP
		
text__asm ENDS
	END

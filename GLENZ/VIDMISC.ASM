
forcesetborder MACRO color
	local	l1
	push	ax
	push	dx
	mov	dx,3dah
	in	al,dx
	mov	dx,3c0h
	mov	al,11h+32
	out	dx,al
	mov	al,color
	out	dx,al
	pop	dx
	pop	ax
	ENDM
	
PUBLIC _setborder
_setborder PROC FAR
	CBEG
	push	cx
	mov	cx,[bp+6]
	forcesetborder cl
	pop	cx
	CEND
_setborder ENDP

fwaitborder PROC FAR
	call	waitborder
	ret
fwaitborder ENDP
	
waitborder PROC NEAR
	mov	dx,3dah
wbr1:	in	al,dx
	test	al,8
	jnz	wbr1
wbr2:	in	al,dx
	test	al,8
	jz	wbr2
	ret
waitborder ENDP

clearpal PROC NEAR
	cli
	call	waitborder
	mov	dx,3c8h
	mov	al,0
	out	dx,al
	mov	cx,768
	inc	dx
clp1:	out	dx,al
	loop	clp1
	call	waitborder
	sti
	ret
clearpal ENDP

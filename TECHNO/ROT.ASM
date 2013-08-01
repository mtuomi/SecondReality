code 	SEGMENT para public 'CODE'
	ASSUME cs:code
	
start	call	rol
	mov	ax,4c00h
	int	21h
	
waitb	PROC NEAR
	mov	dx,3dah
@@1:	in	al,dx
	test	al,8
	jnz	@@1
@@2:	in	al,dx
	test	al,8
	jnz	@@2
	ret
waitb	ENDP

rol	PROC NEAR
	call	waitb
	
	ret
rol	ENDP
	
code	ENDS
	END start
	
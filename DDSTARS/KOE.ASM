extrn _textpic:byte
extrn _dis_partstart:far

code 	SEGMENT para public 'CODE'
	ASSUME cs:code
LOCALS
.386

include sin1024.inc ;_sin1024

PLANE	MACRO pl
	mov	dx,3c4h
	mov	ax,0002h+pl*100h
	out	dx,ax
	ENDM

resetmode13 PROC NEAR
	mov	ax,13
	int	10h
	mov	dx,3dah
	in	al,dx
	mov	dx,3c0h
	xor	al,al
	REPT 16
	out	dx,al
	out	dx,al
	inc	al
	ENDM
	mov	al,11h
	out	dx,al
	mov	al,255
	out	dx,al
	mov	al,32
	out	dx,al
	;clear pal
	mov	dx,3c8h
	xor	al,al
	out	dx,al
	inc	dx
	mov	cx,768
@@clp:	out	dx,al
	loop	@@clp
	ret
resetmode13 ENDP

outpal	PROC NEAR
	mov	dx,3c8h
	out	dx,al
	mov	ax,cs
	mov	ds,ax
	inc	dx
	rep	outsb
	ret
outpal	ENDP

waitb	PROC NEAR
	mov	bx,1
	int	0fch
	ret
waitb	ENDP

ALIGN 16
include stars.asm

start:	mov	bx,SEG endcode
	mov	ax,es
	sub	bx,ax
	add	bx,64
        mov     ah,4ah
 	int	21h
	
	call	_dis_partstart
	
	call	resetmode13

	call	init_stars
	
	call	do_stars
	call	deinit_stars
	
@@xit:	mov	ax,3
	int	10h
	mov	ax,4c00h
	int	21h
	
code	ENDS

.8086
stack	SEGMENT word stack 'STACK'
	db	1024 dup(0)
stack	ENDS

lastseg segment para public 'DATA' ;temporary stack when code starts
	ALIGN 16
public endcode
endcode	db	16 dup(0)
lastseg ends

	END start
	
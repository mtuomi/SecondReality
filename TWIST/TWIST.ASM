tcode SEGMENT para public 'CODE'
ASSUME cs:tcode
.386
LOCALS

include twstloop.inc

PUBLIC _twistvram
_twistvram dw	0,0

ALIGN 16
PUBLIC _twist
_twist dw 200 dup(0,0,0,0)

PUBLIC _twister
_twister PROC FAR
	push	bp
	mov	bp,sp
	push	si
	push	di
	push	ds
	mov	ds,cs:_twistvram[2]
	mov	si,OFFSET _twist
	xor	di,di
	mov	cx,200
@@1:	mov	bx,cs:[si]
@@4:	cmp	bx,0
	jge	@@2
	add	bx,200
	jmp	@@4
@@2:	cmp	bx,200
	jl	@@3
	sub	bx,200
	jmp	@@2
@@3:	shl	bx,2
	call	cs:twistt[bx]
	call	cs:twistt[bx+2]
	add	di,80
	add	si,8
	loop	@@1
	pop	ds
	pop	di
	pop	si
	pop	bp
	ret
_twister ENDP

tcode ENDS
END

text__zoom SEGMENT para public 'CODE2'
	ASSUME cs:text__zoom
LOCALS
.386

PUBLIC _zoom
_zoom PROC FAR
	push	bp
	mov	bp,sp
	push	si
	push	di
	push	ds
	
	les	di,[bp+6]
	lds	si,[bp+10]
	mov	ax,[bp+14]
	call	zoom
	
	pop	ds
	pop	di
	pop	si
	pop	bp
	ret
_zoom ENDP

include zoomloop.inc

PUBLIC _sin1024
include sin1024.inc
	
text__zoom ENDS
END


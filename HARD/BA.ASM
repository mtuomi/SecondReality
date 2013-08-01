text_wa	SEGMENT para public 'CODE'
	ASSUME cs:text_wa
	
.386
LOCALS

NUM	equ 2048
public _rows,_dotxyz
_rows	dw	200 dup(0)
	dw	4 dup(64000)
_dotxyz dw	NUM dup(0,32767,0,0,100h,0)
	;dotmode=1: x,y,xa,ya,bytes:color/count,last
	
public _sin1024
include sin1024.inc

public _dodots	
_dodots PROC FAR
	push	bp
	mov	bp,sp
	push	si
	push	di
	push	ds
	
	mov	es,[bp+12]
	mov	ds,[bp+8]
	call	dot3d
	
	pop	ds
	pop	di
	pop	si
	pop	bp
	ret
_dodots ENDP

public _dodots2
_dodots2 PROC FAR
	push	bp
	mov	bp,sp
	push	si
	push	di
	push	ds
	
	mov	es,[bp+12]
	mov	ds,[bp+8]
	call	dot3d2
	
	pop	ds
	pop	di
	pop	si
	pop	bp
	ret
_dodots2 ENDP

public _do3dots
_do3dots PROC FAR
	push	bp
	mov	bp,sp
	push	si
	push	di
	push	ds
	
	mov	es,[bp+12]
	mov	si,[bp+6]
	mov	ds,[bp+8]
	call	do3dots
	
	pop	ds
	pop	di
	pop	si
	pop	bp
	ret
_do3dots ENDP

dot3d PROC NEAR
	mov	cx,NUM
	mov	si,OFFSET _dotxyz
	xor	bx,bx
	
a1:	mov	dx,cs:[si+8]
	cmp	dl,0
	jne	a4
	add	si,12
	loop	a1
	ret

a2:	mov	bx,cs:[si+10]
	mov	byte ptr ds:[bx],dl
	mov	byte ptr es:[bx],dl
	mov	word ptr cs:[si+8],0100h
	add	si,12
	loop	a1
	ret

a4:	;dec	dl 
	dec	dh
	jz	a2
	mov	cs:[si+8],dx
	mov	bx,cs:[si+10]
	mov	al,ds:[bx]
	mov	es:[bx],al
	mov	bx,cs:[si+2]
	add	bx,cs:[si+6]
	mov	cs:[si+2],bx
	sar	bx,6
	cmp	bx,127
	ja	a5
	shl	bx,1
	mov	bx,cs:_rows[bx]
	mov	ax,cs:[si+0]
	add	ax,cs:[si+4]
	mov	cs:[si+0],ax
	sar	ax,6
	cmp	ax,319
	ja	a5
	add	bx,ax
	mov	es:[bx],dl
	mov	cs:[si+10],bx
a5:	add	si,12
	loop	a1
	ret
dot3d	ENDP

dot3d2 PROC NEAR
	mov	cx,NUM
	mov	si,OFFSET _dotxyz
	xor	bx,bx
	
@@1:	mov	dx,cs:[si+8]
	cmp	dl,0
	jne	@@4
	add	si,12
	loop	@@1
	ret

@@2:	;mov	bx,cs:[si+10]
	;mov	byte ptr ds:[bx],dl
	;mov	byte ptr es:[bx],dl
	mov	word ptr cs:[si+8],0100h
	add	si,12
	loop	@@1
	ret

@@4:	dec	dl
	dec	dh
	jz	@@2
	mov	cs:[si+8],dx
;	mov	bx,cs:[si+10]
;	mov	al,ds:[bx]
;	mov	es:[bx],al
	mov	bx,cs:[si+2]
	add	bx,cs:[si+6]
	mov	cs:[si+2],bx
	sar	bx,6
	cmp	bx,127
	ja	@@5
	shl	bx,1
	mov	bx,cs:_rows[bx]
	mov	ax,cs:[si+0]
	add	ax,cs:[si+4]
	mov	cs:[si+0],ax
	sar	ax,6
	cmp	ax,319
	ja	@@5
	add	bx,ax
	mov	es:[bx],dl
;	mov	word ptr cs:[si+10],0
@@5:	add	si,12
	loop	@@1
	ret
dot3d2	ENDP

tmpblue db	0
PUBLIC _fadepal 
_fadepal PROC FAR
	push	bp
	mov	bp,sp
	push	si
	push	di
	push	ds
	mov	di,[bp+6]
	mov	es,[bp+8]
	mov	si,[bp+10]
	mov	ds,[bp+12]
	mov	dx,[bp+14]
	xor	bx,bx
	cmp	dx,256
	jb	@@1
	mov	dx,255
@@1:	mov	dh,255
	sub	dh,dl
	mov	al,20
	mul	dh
	mov	cs:tmpblue,ah
	mov	cx,256
@@3:	mov	al,ds:[si+bx]
	mul	dl
	mov	es:[di+bx],ah
	inc	bx
	mov	al,ds:[si+bx]
	mul	dl
	mov	es:[di+bx],ah
	inc	bx
	mov	al,ds:[si+bx]
	mul	dl
	add	ah,cs:tmpblue
	mov	es:[di+bx],ah
	inc	bx
	loop	@@3
	pop	ds
	pop	di
	pop	si
	pop	bp
	ret
_fadepal ENDP

ALIGN 2
shadowcx dw	250
shadowcy dw	150
mul10000 dw	3000
PUBLIC _depthcol
_depthcol db	8192 dup(0)

do3dots PROC NEAR
	mov	cx,ds:[si]
	add	si,4
@@1:	mov	bx,ds:[si+6]
	mov	byte ptr es:[bx],0
	mov	bx,ds:[si+12]
	mov	word ptr es:[bx],0
	mov	word ptr es:[bx+320],0
	mov	bp,ds:[si+8]
	cmp	bp,3000
	jg	@@2
	jmp	@@4
	mov	bx,ds:[si]
	mov	ax,bx
	sub	ax,cs:shadowcx
	imul	cs:mul10000
	idiv	bp
	add	ax,cs:shadowcx
	push	ax
	mov	bx,ds:[si+2]
	mov	ax,bx
	sub	ax,cs:shadowcy
	imul	cs:mul10000
	idiv	bp
	add	ax,cs:shadowcy
	mov	bx,ax
	pop	ax
	cmp	ax,318
	ja	@@3
	cmp	bx,126
	ja	@@3
	shl	bx,1
	mov	bx,cs:_rows[bx]
	add	bx,ax
	mov	ds:[si+12],bx
	mov	word ptr es:[bx],0ffffh
	mov	word ptr es:[bx+320],0ffffh
	
@@4:	mov	al,cs:_depthcol[bp]
	mov	bx,ds:[si+2]
	cmp	bx,127
	ja	@@2
	shl	bx,1
	mov	bx,cs:_rows[bx]
	add	bx,ds:[si]
	mov	ds:[si+6],bx
	mov	byte ptr es:[bx],al
	add	si,16
	loop	@@1
	ret
@@3:	mov	word ptr ds:[si+12],0
	jmp	@@4
@@2:	mov	word ptr ds:[si+6],0
	add	si,16
	loop	@@1
	ret
do3dots	ENDP

text_wa ENDS
	END
	
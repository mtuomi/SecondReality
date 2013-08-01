include clink.inc
code SEGMENT para public 'CODE'
ASSUME cs:code

.386

MAXDOTS equ 1024

BOTTOM equ 8000

public _gravitybottom
_gravitybottom dw BOTTOM

public _bpmin,_bpmax
_bpmin dw	30000
_bpmax dw	-30000

public _gravity
_gravity dw	0

public _dotnum
_dotnum dw	0

public _gravityd
_gravityd dw	16

dw -1280
dw -960
dw -640
dw -320
public _rows
_rows	dw  200 dup(0)

public _dot
_dot LABEL WORD
dot dw	MAXDOTS dup(0,0,0,0,0,0,0,0) ;x,y,z,oldposshadow,oldpos,-,-,-

public _rotsin,_rotcos
_rotsin dw 0
_rotcos dw 0

public _bgpic
_bgpic	dw 0,0

public _depthtable1,_depthtable2,_depthtable3,_depthtable4
_depthtable1 dd 128 dup(0)
_depthtable2 dd 128 dup(0)
_depthtable3 dd 128 dup(0)
_depthtable4 dd 128 dup(0)

public _drawdots
_drawdots PROC FAR
	CBEG
	mov	ax,0a000h
	mov	es,ax
	mov	ax,cs
	mov	ds,ax
	mov	fs,cs:_bgpic[2]
	mov	cx,cs:_dotnum
	mov	si,OFFSET dot
@@1:	push	cx

	mov	ax,ds:[si+0] ;X
	imul	ds:_rotsin
	mov	ax,ax
	mov	cx,dx
	mov	ax,ds:[si+4] ;Z
	imul	ds:_rotcos
	sub	ax,bx
	sub	dx,cx
	mov	bp,dx
	add	bp,9000
	
	mov	ax,ds:[si+0] ;X
	imul	ds:_rotcos
	mov	bx,ax
	mov	cx,dx
	mov	ax,ds:[si+4] ;Z
	imul	ds:_rotsin
	add	ax,bx
	adc	dx,cx
	shrd	ax,dx,8
	sar	dx,8
	
	mov	bx,ax
	mov	cx,dx
	shrd	ax,dx,3
	sar	dx,3
	add	ax,bx
	adc	dx,cx
	
	idiv	bp
	add	ax,160
	push	ax
	cmp	ax,319
	ja	@@2
	
	;shadow

	xor	ax,ax
	mov	dx,8
	idiv	bp
	add	ax,100
	cmp	ax,199
	ja	@@2
	mov	bx,ax
	shl	bx,1
	mov	bx,ds:_rows[bx]
	pop	ax
	add	bx,ax
	push	ax
	
	mov	di,ds:[si+6]
	mov	ax,fs:[di]
	mov	es:[di],ax
	mov	ax,87+87*256
	mov	word ptr es:[bx],ax
	mov	ds:[si+6],bx
	
	;ball
	
	mov	ax,ds:_gravity
	add	ds:[si+14],ax
	mov	ax,ds:[si+2] ;Y
	add	ax,ds:[si+14]
	cmp	ax,ds:_gravitybottom
	jl	@@4
	push	ax
	mov	ax,ds:[si+14]
	neg	ax
	imul	cs:_gravityd
	sar	ax,4
	mov	ds:[si+14],ax
	pop	ax
	add	ax,ds:[si+14]
@@4:	mov	ds:[si+2],ax
	cwd
	shld	dx,ax,6
	shl	ax,6
	idiv	bp
	add	ax,100
	cmp	ax,199
	ja	@@3
	mov	bx,ax
	shl	bx,1
	mov	bx,ds:_rows[bx]
	pop	ax
	add	bx,ax

	mov	di,ds:[si+8]
	mov	eax,fs:[di]
	mov	es:[di],eax
	add	di,320
	mov	eax,fs:[di]
	mov	es:[di],eax
	add	di,320
	mov	eax,fs:[di]
	mov	es:[di],eax
;	add	di,320
;	mov	eax,fs:[di]
;	mov	es:[di],eax
	shr	bp,6
	and	bp,not 3
	
	cmp	bp,cs:_bpmin
	jge	@@t1
	mov	cs:_bpmin,bp
@@t1:	cmp	bp,cs:_bpmax
	jle	@@t2
	mov	cs:_bpmax,bp
@@t2:
	mov	ax,word ptr ds:_depthtable1[bp]
	mov	word ptr es:[bx+1],ax
	mov	eax,ds:_depthtable2[bp]
	mov	dword ptr es:[bx+320],eax
	mov	ax,word ptr ds:_depthtable3[bp]
	mov	word ptr es:[bx+641],ax
	mov	ds:[si+8],bx

@@z:	pop	cx
	add	si,16
	loop	@@1
@@0:	CEND

@@2:	mov	di,ds:[si+8]
	mov	eax,fs:[di]
	mov	es:[di],eax
	add	di,320
	mov	eax,fs:[di]
	mov	es:[di],eax
	add	di,320
	mov	eax,fs:[di]
	mov	es:[di],eax
	mov	di,ds:[si+6]
	mov	ds:[si+6],ax
	mov	ax,fs:[di]
	mov	es:[di],ax
	pop	bx
	pop	cx
	add	si,16
	loop	@@1
	jmp	@@0
@@3:	mov	di,ds:[si+8]
	mov	eax,fs:[di]
	mov	es:[di],eax
	add	di,320
	mov	eax,fs:[di]
	mov	es:[di],eax
	add	di,320
	mov	eax,fs:[di]
	mov	es:[di],eax
	pop	bx
	pop	cx
	add	si,16
	loop	@@1
	jmp	@@0
_drawdots ENDP

PUBLIC _setpalette
_setpalette PROC FAR
	push	bp
	mov	bp,sp
	push	si
	push	di
	push	ds
	mov	si,[bp+6]
	mov	ds,[bp+8]
	mov	dx,3c8h
	mov	al,0
	out	dx,al
	inc	dx
	mov	cx,768
	rep	outsb
	pop	ds
	pop	di
	pop	si
	pop	bp
	ret
_setpalette ENDP

_face	LABEL WORD
public _face
include face.inc
dw	30000,30000,30000

PUBLIC _sin1024
include sin1024.inc

code ENDS
END

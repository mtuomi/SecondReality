include clink.inc

xbufseg	SEGMENT para public 'DATA'
PUBLIC _bufdata
_bufdata dw	32768 dup(0)
xbufseg	ENDS

xmulseg	SEGMENT para public 'DATA'
	;high=X, low=Y => result=X*Y/256
PUBLIC _muldata
_muldata dw	32768 dup(0)
xmulseg	ENDS

xlineseg SEGMENT para public 'DATA'
	;high=X, low=Y, shl=1 => result=offset to blit routine
PUBLIC _linedata
_linedata dw	32768 dup(0)
xlineseg ENDS

code 	SEGMENT para public 'CODE'
	ASSUME cs:code
LOCALS
.386
EXTRN	lblt_table:word

MINX	equ	-10
MAXX	equ	10
MINY	equ	-8
MAXY	equ	8

rows	dw	200 dup(0)
vram	dw	0a000h
mulseg	dw	SEG _muldata
lineseg	dw	SEG _linedata
bufseg	dw	SEG _bufdata

PUBLIC _xyzdata
		;x,y,-,-, e1a,e1b,e1c,e1d, e2a,e2b,e2c,e2d, e3a,e3b,e3c,e3d
_xyzdata dw	32*32 dup(4*4 dup(0))
RECLEN	equ	(4*4*2)
PUBLIC _plane
_plane	dw	0
drawplane dw	0101h,0202h,0404h
drawstore dw	8,16,24

PUBLIC _asminit
_asminit PROC FAR
	CBEG
	call	lineblit_init
	call	xyzdata_init
	call	rows_init
	CEND
_asminit ENDP

PUBLIC _setborder
_setborder PROC FAR
	CBEG
	mov	dx,3dah
	in	al,dx
	mov	dx,3c0h
	mov	al,11h+32
	out	dx,al
	mov	al,[bp+6]
	out	dx,al
	CEND
_setborder ENDP

xyzdata_init PROC NEAR
	mov	cx,32*32
	mov	bx,OFFSET _xyzdata
	mov	ax,OFFSET @@ret
@@1:	mov	cs:[bx+8+2],ax
	mov	cs:[bx+8+6],ax
	mov	cs:[bx+16+2],ax
	mov	cs:[bx+16+6],ax
	mov	cs:[bx+24+2],ax
	mov	cs:[bx+24+6],ax
	add	bx,RECLEN
	loop	@@1
@@ret:	ret
xyzdata_init ENDP

rows_init PROC NEAR
	mov	cx,200
	mov	bx,OFFSET rows
	mov	dx,320
	xor	ax,ax
@@1:	mov	cs:[bx],ax
	add	ax,dx
	add	bx,2
	loop	@@1
	ret
rows_init ENDP

PUBLIC _setpalette
_setpalette PROC FAR
	CBEG
	mov	dx,3c8h
	xor	al,al
	out	dx,al
	inc	dx
	movpar	si,0
	movpar	ds,1
	mov	cx,768
	rep	outsb
	CEND
_setpalette ENDP

ALIGN 2
tmpcolor dw	0
tmpcount dw	0
		
PUBLIC _asmloop2
_asmloop2 PROC FAR
	CBEG
	mov	ax,cs
	mov	ds,ax
	mov	si,OFFSET _xyzdata
	mov	cx,32*32
@@1:	mov	ax,ds:[si+4]
	mov	bx,ds:[si+6]
	add	bx,ax
	cmp	bx,16*199
	jb	@@2
	sub	bx,ax
	neg	ax
@@2:	mov	ds:[si+4],ax
	mov	ds:[si+6],bx
	shr	bx,4
	mov	ds:[si+2],bx
	add	si,RECLEN
	loop	@@1
	CEND
_asmloop2 ENDP

PUBLIC _asmloop1
_asmloop1 PROC FAR
	CBEG
	mov	ax,cs
	mov	ds,ax
	mov	bx,ds:_plane
	inc	bx
	cmp	bx,3
	jb	@@i1
	xor	bx,bx
@@i1:	mov	ds:_plane,bx
	shl	bx,1
	mov	ax,ds:drawplane[bx]
	mov	ds:tmpcolor,ax
	mov	bp,ds:drawstore[bx]
	mov	es,ds:vram
	mov	fs,ds:lineseg
	mov	gs,ds:bufseg
	xor	ebx,ebx
	;===erase===
	mov	ds:tmpcount,25
	mov	si,OFFSET _xyzdata
	mov	dx,ds:tmpcolor
@@3:	zzz=0
	REPT 40
	local	l1
	mov	di,ds:[si+zzz+bp+0]
	call	ds:[si+zzz+bp+2]
	zzz=zzz+RECLEN
l1:	ENDM
	add	si,40*RECLEN
	dec	ds:tmpcount
	jz	@@2
	jmp	@@3
@@2:	;===draw===
	mov	ds:tmpcount,25
	mov	si,OFFSET _xyzdata
	mov	dx,ds:tmpcolor
@@1:	zzz=0
	REPT 40
	local	l1
	mov	cx,ds:[si+zzz+2+RECLEN] ;Y2
	mov	bx,ds:[si+zzz+2] ;Y1
	sub	cl,bl
	add	bx,bx
	mov	di,ds:rows[bx]

	mov	bx,ds:[si+zzz+0+RECLEN] ;X2
	mov	ax,ds:[si+zzz+0] ;X1
	sub	bl,al
	add	di,ax
	
	mov	ds:[si+zzz+bp+0],di
	mov	bh,cl
	add	bx,bx
	mov	cx,fs:[bx]
	mov	ds:[si+zzz+bp+2],cx
	call	cx

	zzz=zzz+RECLEN
l1:	ENDM
	add	si,40*RECLEN
	dec	ds:tmpcount
	jz	@@0
	jmp	@@1
@@0:	CEND
_asmloop1 ENDP

;北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

;include lineblit.inc ;linked

lineblit_init PROC NEAR
	mov	es,cs:lineseg
	mov	ax,cs
	mov	ds,ax
	mov	si,OFFSET lblt_table
	;fill with offset/rets
	mov	ax,cs:[si]
	xor	di,di
	mov	cx,32768
	rep	stosw
	;add the actual line offsets
	mov	si,OFFSET lblt_table
	mov	cx,MAXY-MINY+1 ;Y
	xor	ebx,ebx
	mov	bh,MINY
@@2:	push	cx
	mov	cx,MAXX-MINX+1 ;X
	mov	bl,MINX
@@1:	mov	ax,ds:[si]
	add	si,2
	mov	dx,bx
	shl	bx,1
	mov	es:[bx],ax
	mov	bx,dx
	inc	bl
	loop	@@1
	pop	cx
	inc	bh
	loop	@@2
	ret
lineblit_init ENDP

lineblit PROC NEAR
	;di=x+y*320 (base)
	;bh=Y dist
	;bl=X dist
	ret
lineblit ENDP

PUBLIC _line
_line	PROC FAR
	CBEG
	mov	dx,[bp+8]
	mov	bx,dx
	shl	bx,1
	mov	di,cs:rows[bx]
	mov	ax,[bp+6]
	add	di,ax
	neg	ax
	add	ax,[bp+10]
	mov	bl,al
	neg	dx
	add	dx,[bp+12]
	mov	bh,dl
	shl	bx,1
	mov	al,[bp+14]
	mov	ah,al
	mov	es,cs:vram
	mov	ds,cs:lineseg
	call	word ptr ds:[bx]
	CEND
_line	ENDP

;北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
	
code	ENDS
	END
	
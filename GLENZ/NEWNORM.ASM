FREELINE equ 200

newgroup PROC NEAR
	;hline list in gs:si
	push	ds
	mov	ax,SEG newdata
	mov	ds,ax
	mov	bx,gs:[si]
	add	si,2
@@1:	mov	ax,gs:[si]
	add	si,2
	cmp	ax,-32767
	je	@@2
	mov	dx,gs:[si]
	add	si,2
	cmp	bx,7eh
	jne	@@z
@@z:	push	bx
	;hline ax..dx,bx with gs:color
	cmp	ax,dx
	jle	@@3
	xchg	ax,dx
@@3:	push	dx
	shl	bx,1
	mov	dx,ds:[bx]
	mov	bp,ds:[FREELINE*2]
	mov	ds:[bx],bp
	mov	ds:[FREELINE*2],dx
	mov	bx,dx
	;modify list bx=>bp (edx/hi=last color)
	;copy blocks < hline start
	xor	edx,edx
@@b:	mov	ecx,ds:[bx]
	cmp	cx,ax
	jae	@@a ;[bx] bigger..
	add	bx,4
	mov	ds:[bp],ecx
	add	bp,4
	jmp	@@b
@@a:	;add hline start
	jne	@@aa
	mov	edx,ecx
	add	bx,4
	mov	ecx,ds:[bx]
@@aa:	mov	ds:[bp],ax
	mov	ax,gs:color
	mov	ah,al
	mov	ds:[bp+2],ax
	add	bp,4
	;copy blocks < hline end
	pop	ax
@@d:	cmp	cx,ax
	jae	@@c
	add	bx,4
	mov	edx,ecx
	mov	ecx,ds:[bx]
	jmp	@@d
@@c:	;add hline end
	jne	@@ca
	mov	edx,ecx
	add	bx,4
	mov	ecx,ds:[bx]
@@ca:	mov	dx,ax
	mov	ds:[bp],edx
	add	bp,4
	;copy rest of the list
@@f:	cmp	cx,32767
	je	@@e
	add	bx,4
	mov	ds:[bp],ecx
	add	bp,4
	mov	ecx,ds:[bx]
	jmp	@@f
@@e:	;last 
	mov	ds:[bp],ecx
	pop	bx
	inc	bx
	jmp	@@1
@@2:	pop	ds
	ret
newgroup ENDP

grouppage dw	1024

PUBLIC _initnewgroup
_initnewgroup PROC FAR
	CBEG
	mov	ax,SEG newdata
	mov	ds,ax
	mov	dx,cs:grouppage
	xor	dx,32768
	mov	cs:grouppage,dx
	mov	cx,201
	xor	bx,bx
@@2:	mov	ax,ds:[bx]
	mov	ds:[bx],dx
	mov	ds:[bx+256*2],ax
	add	dx,128
	add	bx,2
	loop	@@2
	mov	cx,200
	mov	eax,32767
	mov	bx,cs:grouppage
@@1:	mov	ds:[bx],eax
	add	bx,128
	loop	@@1
	CEND
_initnewgroup ENDP

ALIGN 2
dng_bgcolor	dw	0
dng_rowbase	dw	0
dng_random	dw	0

PUBLIC _drawnewgroup
_drawnewgroup PROC FAR
	CBEG
	LOADDS
	mov	dx,3c4h
	mov	ax,0f02h
	out	dx,ax
	mov	es,ds:vram
	mov	ax,SEG newdata
	mov	gs,ax
	mov	cx,200
	xor	bx,bx
	xor	di,di
@@3:	push	cx
	push	di
	push	bx
	shl	bx,1
	mov	si,gs:[bx]
	mov	di,gs:[bx+512] ;last time row (for sorting out differences)
	mov	bp,32768
	;si=thistime
	;di=lasttime
	;dl=thiscolor
	;dh=lastcolor
	;cx=thisX
	;bx=lastX
	;ax=X
	;bp=tmpdest
	mov	dx,cs:dng_bgcolor
	mov	dh,dl
	xor	ax,ax
	mov	cx,gs:[si]
	mov	bx,gs:[di]
@@1:	cmp	bx,cx
	je	@@10
	jl	@@2
	;cx..bx
	cmp	dl,dh
	je	@@5
	;ax..cx color dl
	mov	gs:[bp],ax
	mov	gs:[bp+2],cx
	mov	gs:[bp+4],dx
	add	bp,6
@@5:	mov	ax,cx
	mov	dl,gs:[si+2]
	add	si,4
	mov	cx,gs:[si]
	jmp	@@1
@@10:	;bx=cx
	cmp	bx,32767
	je	@@7
	cmp	dl,dh
	je	@@11
	;ax..bx color dl
	mov	gs:[bp],ax
	mov	gs:[bp+2],bx
	mov	gs:[bp+4],dx
	add	bp,6
@@11:	mov	ax,bx
	mov	dh,gs:[di+2]
	add	di,4
	mov	bx,gs:[di]
	mov	dl,gs:[si+2]
	add	si,4
	mov	cx,gs:[si]
	jmp	@@1
@@2:	;bx..cx
	cmp	dl,dh
	je	@@4
	;ax..bx color dl
	mov	gs:[bp],ax
	mov	gs:[bp+2],bx
	mov	gs:[bp+4],dx
	add	bp,6
@@4:	mov	ax,bx
	mov	dh,gs:[di+2]
	add	di,4
	mov	bx,gs:[di]
	jmp	@@1
@@7:	pop	bx
	pop	di
	;this line data in 8000h..bp
	mov	si,32768
	cmp	si,bp
	je	@@8
@@6:	push	di
	push	bx
	push	bp
	mov	ch,gs:[si+4]
	mov	ds:color,ax
	mov	bx,gs:[si]
	mov	dx,gs:[si+2]
	;
	mov	cl,bl
	and	cl,3
	mov	al,00fh
	rol	al,cl
	mov	cl,dl
	and	cl,3
	mov	ah,0f0h
	rol	ah,cl
	
	shr	bx,2
	shr	dx,2
	cmp	bx,dx
	jne	@@h1
	
	and	al,ah
	mov	dx,3c5h
	out	dx,al
	mov	es:[di+bx],ch
	jmp	@@h0	
	
@@h1:	inc	bx
	cmp	bx,dx
	jne	@@h2
	mov	dx,3c5h
	out	dx,al
	mov	es:[di+bx-1],ch
	mov	al,ah
	out	dx,al
	mov	es:[di+bx],ch
	jmp	@@h0
	
@@h2:	mov	cl,dl
	sub	cl,bl
	add	di,bx
	mov	dx,3c5h
	out	dx,al
	mov	es:[di-1],ch
	mov	al,0fh
	out	dx,al
	mov	al,ch
	xor	ch,ch
	rep	stosb
	xchg	al,ah
	out	dx,al
	mov	es:[di],ah
@@h0:	;
	pop	bp
	pop	bx
	pop	di
	add	si,6
	cmp	si,bp
	jb	@@6
@@8:	pop	cx
	add	di,80
	inc	bx
	loop	@@3
	CEND
_drawnewgroup ENDP


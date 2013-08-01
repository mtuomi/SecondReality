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
dng_bgcolor	dw	101h
dng_rowbase	dw	0

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
	mov	bx,gs:[bx+512] ;last time row (for sorting out differences)
	mov	cs:dng_rowbase,di
	mov	bp,cs:dng_bgcolor
	mov	dx,3c5h
	mov	al,0fh
@@1:	mov	cx,gs:[si]
	cmp	cx,32767
	je	@@2
	mov	ah,cl
	shr	cx,2
	add	cx,cs:dng_rowbase
	sub	cx,di
	jcxz	@@4
	out	dx,al
	xchg	ax,bp
	dec	cx
	mov	es:[di],ah
	inc	di
	mov	al,0fh
	out	dx,al
	mov	al,ah
	shr	cx,1
	rep	stosw
	adc	cx,cx
	rep	stosb
	mov	ch,al
	mov	ax,bp
	mov	al,0f0h
	mov	cl,ah
	and	cl,3
	rol	al,cl
	out	dx,al
	mov	es:[di],ch
	xor	al,0fh
	mov	bp,gs:[si+2]
	add	si,4
	jmp	@@1
@@4:	mov	ch,0f0h
	mov	cl,ah
	and	cl,3
	rol	ch,cl
	mov	ah,ch
	and	al,ch
	out	dx,al
	mov	cx,bp
	mov	es:[di],cl
	mov	al,ah
	xor	al,0fh
	mov	bp,gs:[si+2]
	add	si,4
	jmp	@@1
@@2:	out	dx,al
	mov	ax,cs:dng_bgcolor
	mov	cx,cs:dng_rowbase
	add	cx,80
	sub	cx,di
	jcxz	@@5
	dec	cx
	mov	es:[di],ah
	inc	di
	mov	al,0fh
	out	dx,al
	mov	al,ah
	shr	cx,1
	rep	stosw
	adc	cx,cx
	rep	stosb
@@5:	;
	pop	bx
	pop	di
	pop	cx
	add	di,80
	inc	bx
	loop	@@3
	CEND
_drawnewgroup ENDP


code SEGMENT para public 'CODE'
ASSUME cs:code
.386
LOCALS

PUBLIC _back
_back	dw	0,0
PUBLIC _rotpic
_rotpic	dw	0,0
PUBLIC _rotpic90
_rotpic90 dw	0,0

DOWORD 	MACRO	diadd,siadd
	mov	bx,ds:[si+(siadd)]	;4
	mov	al,fs:[bx+di]		;3
	mov	bx,ds:[si+(siadd)+2]	;4
	mov	ah,fs:[bx+di]		;3
	or	ax,dx			;2
	mov	es:[bp+(diadd)],ax	;5
	ENDM				;=21

PUBLIC _dorow
_dorow	PROC FAR
	push 	bp
	mov 	bp,sp
	push 	si
	push	di
	push	ds
	
	mov	fs,cs:_back[2]
	mov	ax,0a000h
	mov	es,ax
	mov	ds,[bp+8]
	mov	dx,[bp+14]
	mov	dh,dl
	mov	di,[bp+10]
	mov	si,[bp+12]
	shl	si,2
	mov	cx,ds:[si+2]
	mov	si,ds:[si]
	cmp	cx,4
	jge	@@2
	jmp	@@0
@@2:	add	di,ds:[si]
	mov	bp,di
	add	si,2
	test	bp,1
	jz	@@1
	mov	bx,ds:[si]
	add	si,2
	mov	al,fs:[bx+di]
	or	ax,dx
	mov	es:[bp],al
	inc	bp
	dec	cx
@@1:	push	cx
	shr	cx,1
	sub	si,320
	sub	bp,320
	mov	ax,cx ;*1
	shl	cx,2
	add	ax,cx ;*4
	shl	cx,2
	add	ax,cx ;*16
	neg	ax
	;bx=-count*21
	add	ax,OFFSET @@l
	jmp	ax
	zzz=64
	REPT	64
	zzz=zzz-1
	DOWORD	320+zzz*2,320+zzz*4
	ENDM
@@l:	pop	cx
	test	cx,1
	jz	@@0
	and	cx,not 1
	add	bp,cx
	add	cx,cx
	add	si,cx
	mov	bx,ds:[si+320]
	mov	al,fs:[bx+di]
	or	ax,dx
	mov	es:[bp+320],al
@@0:	pop	ds
	pop	di
	pop	si
	pop 	bp
	ret
_dorow	ENDP

PUBLIC _dorow2
_dorow2	PROC FAR
	push 	bp
	mov 	bp,sp
	push 	si
	push	di
	push	ds
	
	mov	fs,[bp+8]
	mov	si,[bp+12]
	shl	si,2
	mov	cx,fs:[si+2]
	mov	si,fs:[si]
	or	cx,cx
	jcxz	@@0
	mov	ds,cs:_back[2]
	mov	ax,0a000h
	mov	es,ax
	mov	dx,[bp+14]
	mov	dh,dl
	mov	di,[bp+10]
	add	di,fs:[si]
	mov	ax,di
	lea	bp,[si+2]
	mov	si,ax
	
@@3:	mov	bx,fs:[bp+2]
	mov	al,ds:[bx+di]
	or	al,dl
	mov	bx,fs:[bp]
	add	bp,4
	mov	es:[bx+si],al
	dec	cx
	jnz	@@3
	
@@0:	pop	ds
	pop	di
	pop	si
	pop 	bp
	ret
_dorow2	ENDP

PUBLIC _dorow3
_dorow3	PROC FAR
	push 	bp
	mov 	bp,sp
	push 	si
	push	di
	push	ds
	
	mov	fs,[bp+8]
	mov	si,[bp+12]
	shl	si,2
	mov	cx,fs:[si+2]
	mov	si,fs:[si]
	or	cx,cx
	jcxz	@@0
	mov	ds,cs:_back[2]
	mov	ax,0a000h
	mov	es,ax
	mov	di,[bp+10]
	add	di,fs:[si]
	add	si,2
	
@@3:	mov	bx,fs:[si]
	add	si,2
	mov	al,ds:[bx+di]
	mov	es:[bx+di],al
	dec	cx
	jnz	@@3
	
@@0:	pop	ds
	pop	di
	pop	si
	pop 	bp
	ret
_dorow3	ENDP

PUBLIC _setborder
_setborder PROC FAR
	push	bp
	mov	bp,sp
	mov	dx,3dah
	in	al,dx
	mov	dx,3c0h
	mov	al,11h+32
	out	dx,al
	mov	al,[bp+6]
	out	dx,al
	pop	bp
	ret
_setborder ENDP

PUBLIC _inittwk
_inittwk PROC FAR
	push	bp
	mov	bp,sp
	push	si
	push	di
	push	ds
	;clear palette
	mov	dx,3c8h
	xor	al,al
	out	dx,al
	inc	dx
	mov	cx,768
@@1:	out	dx,al
	loop	@@1
	;400 rows
	mov	dx,3d4h
	mov	ax,00009h
	out	dx,ax
	;tweak
	mov	ax,00014h
	out	dx,ax
	mov	ax,0e317h
	out	dx,ax
	mov	dx,3c4h
	mov	ax,0604h
	out	dx,ax
	;
	mov	dx,3c4h
	mov	ax,0f02h
	out	dx,ax
	mov	ax,0a000h
	mov	es,ax
	xor	di,di
	mov	cx,32768
	xor	ax,ax
	rep	stosw
	;
	;set 4 x vertical
	mov	dx,3d4h
	mov	al,9
	out	dx,al
	inc	dx
	in	al,dx
	and	al,not 31
	or	al,3
	out	dx,al
	;
	pop	ds
	pop	di
	pop	si
	pop	bp
	ret
_inittwk ENDP

PUBLIC _setpalarea
_setpalarea PROC FAR
	push	bp
	mov	bp,sp
	push	si
	push	di
	push	ds
	lds	si,[bp+6]
	mov	ax,[bp+10]
	mov	dx,3c8h
	out	dx,al
	mov	cx,[bp+12]
	mov	ax,cx
	shl	cx,1
	add	cx,ax
	inc	dx
	rep	outsb
	sti
	pop	ds
	pop	di
	pop	si
	pop	bp
	ret
_setpalarea ENDP

ALIGN 16
xpos	dd	0
ypos	dd	0
xadd	dd	0
yadd	dd	0

ZOOMXW	equ	160
ZOOMYW	equ	100

PUBLIC _rotate
_rotate PROC FAR
	push	bp
	mov	bp,sp
	push	si
	push	di
	push	ds
	
	mov	ax,[bp+6]
	shl	eax,16
	mov	cs:xpos,eax
	mov	ax,[bp+8]
	shl	eax,16
	mov	cs:ypos,eax
	
	mov	ax,[bp+12]
	cwde
	shl	eax,6
	mov	ebx,eax
	mov	ax,[bp+10]
	cwde
	shl	eax,6
	
	mov	ecx,eax
	mov	edx,ebx
	
	mov	ds,cs:_rotpic[2]
	cmp	ecx,0
	jge	@@s1
	neg	ecx
@@s1:	cmp	edx,0
	jge	@@s2
	neg	edx
@@s2:	cmp	ecx,edx
	jle	@@s3
	
	mov	ds,cs:_rotpic90[2]
	xchg	eax,ebx
	neg	eax
	mov	ecx,cs:xpos
	mov	edx,cs:ypos
	xchg	ecx,edx
	neg	ecx
	mov	cs:xpos,ecx
	mov	cs:ypos,edx
	
@@s3:	mov	cs:xadd,eax
	mov	cs:yadd,ebx
	
	xor	ax,ax
	mov	cx,word ptr cs:yadd[0]
	mov	dx,word ptr cs:xadd[0]
	mov	bl,byte ptr cs:yadd[2]
	mov	bh,byte ptr cs:xadd[2]
	neg	bh
	neg	dx
	sbb	bh,0
	xor	si,si
	xor	di,di
	;si=lowx,di=lowy,ax=y/x
	;cx=addx,dx=addy,bx=yah/xah
	zzz=0
	REPT	ZOOMXW/4
	add	si,cx
	adc	al,bl
	add	di,dx
	adc	ah,bh
	mov	word ptr cs:[OFFSET @@moda+zzz+2],ax
	add	si,cx
	adc	al,bl
	add	di,dx
	adc	ah,bh
	mov	word ptr cs:[OFFSET @@modb+zzz+2],ax
	add	si,cx
	adc	al,bl
	add	di,dx
	adc	ah,bh
	mov	word ptr cs:[OFFSET @@moda+zzz+6],ax
	add	si,cx
	adc	al,bl
	add	di,dx
	adc	ah,bh
	mov	word ptr cs:[OFFSET @@modb+zzz+6],ax
	zzz=zzz+13
	ENDM
	
	;aspect ratio
	mov	eax,307
	mul	dword ptr cs:xadd
	sar	eax,8
	mov	cs:xadd,eax
	mov	eax,307
	mul	dword ptr cs:yadd
	sar	eax,8
	mov	cs:yadd,eax
	
	mov	ax,0a000h
	mov	es,ax
	mov	di,-1000h
	
	mov	cx,ZOOMYW
@@2:	mov	ebx,cs:ypos
	add	ebx,cs:yadd
	mov	cs:ypos,ebx
	shr	ebx,8
	mov	eax,cs:xpos
	add	eax,cs:xadd
	mov	cs:xpos,eax
	shr	eax,16
	mov	bl,al
	mov	si,bx
	;
	mov	dx,3c4h
	mov	ax,0302h
	out	dx,ax
@@moda:	zzz=1000h
	REPT	ZOOMXW/4
	mov	al,ds:[si+1234h]	;4
	mov	ah,ds:[si+1234h]	;4
	mov	es:[di+zzz],ax		;5
	zzz=zzz+2
	ENDM
	mov	dx,3c4h
	mov	ax,0c02h
	out	dx,ax
@@modb:	zzz=1000h
	REPT	ZOOMXW/4
	mov	al,ds:[si+1234h]	;4
	mov	ah,ds:[si+1234h]	;4
	mov	es:[di+zzz],ax		;5
	zzz=zzz+2
	ENDM
	;
	add	di,80
	dec	cx
	jz	@@1
	jmp	@@2
@@1:	
	pop	ds
	pop	di
	pop	si
	pop	bp
	ret
_rotate ENDP
	
PUBLIC _rotatez
_rotatez PROC FAR
	push	bp
	mov	bp,sp
	push	si
	push	di
	push	ds
	
	mov	ax,[bp+6]
	shl	eax,16
	mov	cs:xpos,eax
	mov	ax,[bp+8]
	shl	eax,16
	mov	cs:ypos,eax
	
	mov	ax,[bp+12]
	cwde
	shl	eax,6
	mov	ebx,eax
	mov	ax,[bp+10]
	cwde
	shl	eax,6
	
	mov	ecx,eax
	mov	edx,ebx
	
	mov	ds,cs:_rotpic[2]
	cmp	ecx,0
	jge	@@s1
	neg	ecx
@@s1:	cmp	edx,0
	jge	@@s2
	neg	edx
@@s2:	cmp	ecx,edx
	jle	@@s3
	
	mov	ds,cs:_rotpic90[2]
	xchg	eax,ebx
	neg	eax
	mov	ecx,cs:xpos
	mov	edx,cs:ypos
	xchg	ecx,edx
	neg	ecx
	mov	cs:xpos,ecx
	mov	cs:ypos,edx
	
@@s3:	mov	cs:xadd,eax
	mov	cs:yadd,ebx
	
	xor	ax,ax
	mov	cx,word ptr cs:yadd[0]
	mov	dx,word ptr cs:xadd[0]
	mov	bl,byte ptr cs:yadd[2]
	mov	bh,byte ptr cs:xadd[2]
	neg	bh
	neg	dx
	sbb	bh,0
	xor	si,si
	xor	di,di
	;si=lowx,di=lowy,ax=y/x
	;cx=addx,dx=addy,bx=yah/xah
	zzz=0
	REPT	ZOOMXW/4
	add	si,cx
	adc	al,bl
	add	di,dx
	adc	ah,bh
	mov	word ptr cs:[OFFSET @@moda+zzz+2],ax
	add	si,cx
	adc	al,bl
	add	di,dx
	adc	ah,bh
	mov	word ptr cs:[OFFSET @@modb+zzz+2],ax
	add	si,cx
	adc	al,bl
	add	di,dx
	adc	ah,bh
	mov	word ptr cs:[OFFSET @@moda+zzz+6],ax
	add	si,cx
	adc	al,bl
	add	di,dx
	adc	ah,bh
	mov	word ptr cs:[OFFSET @@modb+zzz+6],ax
	zzz=zzz+13
	ENDM
	
	;aspect ratio
	mov	eax,307
	mul	dword ptr cs:xadd
	sar	eax,8
	mov	cs:xadd,eax
	mov	eax,307
	mul	dword ptr cs:yadd
	sar	eax,8
	mov	cs:yadd,eax
	
	mov	ax,0a000h
	mov	es,ax
	mov	di,-1000h
	
	mov	cx,ZOOMYW
@@2:	mov	ebx,cs:ypos
	add	ebx,cs:yadd
	mov	cs:ypos,ebx
	shr	ebx,8
	mov	eax,cs:xpos
	add	eax,cs:xadd
	mov	cs:xpos,eax
	add	cs:xadd,256
	sub	cs:yadd,256
	shr	eax,16
	mov	bl,al
	mov	si,bx
	;
	mov	dx,3c4h
	mov	ax,0302h
	out	dx,ax
@@moda:	zzz=1000h
	REPT	ZOOMXW/4
	mov	al,ds:[si+1234h]	;4
	mov	ah,ds:[si+1234h]	;4
	mov	es:[di+zzz],ax		;5
	zzz=zzz+2
	ENDM
	mov	dx,3c4h
	mov	ax,0c02h
	out	dx,ax
@@modb:	zzz=1000h
	REPT	ZOOMXW/4
	mov	al,ds:[si+1234h]	;4
	mov	ah,ds:[si+1234h]	;4
	mov	es:[di+zzz],ax		;5
	zzz=zzz+2
	ENDM
	;
	add	di,80
	dec	cx
	jz	@@1
	jmp	@@2
@@1:	
	pop	ds
	pop	di
	pop	si
	pop	bp
	ret
_rotatez ENDP
	
code ENDS
END
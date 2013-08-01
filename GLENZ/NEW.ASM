
;!!!!!! TRANSPARENT NEW COPPER !!!!!!

public __ndebug1
__ndebug1 dw 0

NROWS equ 256
MAXLINES equ 512
ndp0	dw 0			;toggling pointer inside newdata1
ndp	dw 0			;pointer inside newdata1
nec	dw 0			;pointer to next free in ne
nlc	dw 0			;items in horizontal list
ALIGN 4
nep	dw NROWS dup(0)
ALIGN 4
ne	db MAXLINES dup(16 dup(0))
NE_X		equ 0  ;dd
NE_Y1 		equ 4  ;dw
NE_Y2		equ 6  ;dw
NE_COLOR	equ 8  ;dw
NE_NEXT		equ 10 ;dw
NE_DX		equ 12 ;dd
ALIGN 4
nl	db 256 dup(0)

ng_init PROC NEAR
	mov	cs:nec,OFFSET ne
	mov	ax,cs:ndp0
	xor	ax,8000h
	mov	cs:ndp0,ax
	mov	cs:ndp,ax
	mov	cx,NROWS/2
	mov	ax,cs
	mov	es,ax
	xor	eax,eax
	mov	di,OFFSET nep
	rep	stosd
	mov	ax,SEG newdata1
	mov	ds,ax
	cmp	word ptr ds:[0],0
	jne	@@1
	mov	word ptr ds:[0],-1
@@1:	cmp	word ptr ds:[8000h],0
	jne	@@2
	mov	word ptr ds:[8000h],-1
@@2:	ret
ng_init ENDP

ALIGN 2
yrow	dw	0
yrowad	dw	0
siend	dw	0

fillmacro MACRO
	local	l1,l2
	jcxz	l2
	push	ax
l1:	mov	ah,al
	or	ah,fs:[di]
	mov	es:[di],ah
	inc	di
	loop	l1
	pop	ax
l2:	ENDM

ng_pass3 PROC NEAR
	mov	ax,SEG newdata1
	mov	ds,ax
	mov	ax,0a000h
	mov	es,ax
	mov	ax,SEG _bgpic
	mov	fs,ax
	xor	di,di
	mov	si,cs:ndp0
	mov	bx,si
	xor	bx,8000h
	xor	ax,ax
	;si=new     bx=last
	;cx=newpos  dx=lastpos
	;al=newcol  ah=lastcol
	mov	edx,ds:[bx]
	add	bx,4
	mov	ecx,ds:[si]
	add	si,4
	;
@@21:	cmp	dx,cx
	jb	@@23
	je	@@22
	;cx<dx
	cmp	al,ah
	je	@@31
	push	cx
	sub	cx,di
	;cmp	cx,4
	;jae	@@r1
	;rep	stosb
	fillmacro
	pop	cx
@@31:	mov	di,cx
	shr	ecx,16
	xor	al,cl
	mov	ecx,ds:[si]
	add	si,4
	jmp	@@21
	
@@r1:	push	ax
	mov	ah,al
	test	di,1
	jz	@@r11
	dec	cx
	stosb
@@r11:	shr	cx,1
	rep	stosw
	adc	cx,cx
	rep	stosb
	pop	ax
	pop	cx
	jmp	@@31
	
@@22:	;cx=dx
	cmp	cx,-1
	je	@@20
@@23:	;dx<cx
	cmp	al,ah
	je	@@32
	push	cx
	mov	cx,dx
	sub	cx,di
	;cmp	cx,4
	;jae	@@r2
	;rep	stosb
	fillmacro
	pop	cx
@@32:	mov	di,dx
	shr	edx,16
	xor	ah,dl
	mov	edx,ds:[bx]
	add	bx,4
	jmp	@@21
	
@@r2:	push	ax
	mov	ah,al
	test	di,1
	jz	@@r21
	dec	cx
	stosb
@@r21:	shr	cx,1
	rep	stosw
	adc	cx,cx
	rep	stosb
	pop	ax
	pop	cx
	jmp	@@32
@@20:	ret

	mov	ax,0a000h
	mov	es,ax
	mov	ax,SEG newdata1
	mov	ds,ax
	xor	si,si
	xor	di,di
	xor	ax,ax
	mov	dx,cs:ndp
@@1:	mov	cx,ds:[si]
	sub	cx,di
	rep	stosb
	xor	al,ds:[si+2]
	add	si,4
	cmp	si,dx
	jb	@@1
	ret
ng_pass3 ENDP

ng_pass2 PROC NEAR
	mov	bp,OFFSET nep
	mov	di,OFFSET nl
	mov	cx,200 ;NROWS
	mov	cs:yrow,0
	mov	cs:yrowad,0
@@1:	push	cx
	;di=pointer to this row list end
	;bp=pointer to nep
	
	;add new items this row
	mov	bx,cs:[bp]
	jmp	@@3
@@2:	mov	cs:[di],bx
	add	di,2
	mov	bx,cs:[bx+NE_NEXT]
@@3:	cmp	bx,0
	jne	@@2

	;sort this row (insertion sort)
	push	bp
	push	di
	mov	bp,di
	mov	si,OFFSET nl+2
	jmp	@@4
@@5:	;for(k=1;k<listc;k++) {  // k=SI
	mov	bx,cs:[si] ;bx=list[k]
	mov	dx,bx ;dx=i
	mov	eax,cs:[bx+NE_X] ;eax=x
	mov	di,si
	sub	di,2 ;di=j
	jmp	@@6
	;;;for(j=k-1;j>=0 && x<e[list[j]].x;j--) {
@@9:	mov	cs:[di+2],bx	;bx=cs:di
	sub	di,2
@@6:	cmp	di,OFFSET nl
	jge	@@8
	jmp	@@7
@@8:	mov	bx,cs:[di]
	cmp	eax,cs:[bx+NE_X]
	jl	@@9
@@7:	;;;}
	mov	cs:[di+2],dx
	add	si,2
@@4:	cmp	si,bp
	jb	@@5
	;}
	pop	di
	;bp=nl end
	
	;process list & kill finished lines
	
	mov	cs:siend,bp
	mov	ax,SEG newdata1
	mov	fs,ax
	mov	bp,cs:ndp
	mov	dx,cs:yrow
	mov	cx,8000h
	mov	si,OFFSET nl
	mov	di,si
	jmp	@@10
@@11:	mov	bx,cs:[si]
	cmp	dx,cs:[bx+NE_Y2]
	jge	@@12
	mov	cs:[di],bx
	add	di,2
	mov	eax,cs:[bx+NE_X]
	push	eax
	add	eax,cs:[bx+NE_DX]
	;sub	dword ptr cs:[bx+NE_DX],500
	mov	cs:[bx+NE_X],eax
	pop	eax
	shr	eax,16
	;clip X
	cmp	ax,319
	jle	@@15
	mov	ax,319
@@15:	cmp	ax,1
	jge	@@16
	mov	ax,1
@@16:	;
	cmp	cx,ax
	jne	@@14
	;same x pos
	mov	ax,cs:[bx+NE_COLOR]
	xor	fs:[bp-2],ax
	jmp	@@12
@@14:	;new x pos
	add	ax,cs:yrowad
	mov	fs:[bp+0],ax
	mov	ax,cs:[bx+NE_COLOR]
	mov	fs:[bp+2],ax
	add	bp,4
	mov	cx,ax
@@12:	add	si,2
@@10:	cmp	si,cs:siend
	jb	@@11
	mov	cs:ndp,bp
	
	pop	bp
	pop	cx
	inc	cs:yrow
	add	cs:yrowad,320
	add	bp,2
	loop	@@1
	mov	bx,cs:ndp
	mov	word ptr fs:[bx+0],63999
	mov	word ptr fs:[bx+2],0
	add	bx,4
	mov	word ptr fs:[bx+0],-1
	mov	word ptr fs:[bx+2],0
	add	bx,4
	mov	cs:ndp,bx
	ret
ng_pass2 ENDP

tmp_firstvx dw	0
tmp_color dw	0
public __newgroup
__newgroup PROC FAR
	;es:di=polygroup
	;sides,color,x,y,x,y,x,y,...
	;sides=0=end
	cmp	ax,0
	jne	@@ng1
	call	ng_init
	ret
@@ng1:	cmp	ax,1
	je	@@ng2
	setborder 2
	call	ng_pass2
	setborder 3
	call	ng_pass3
	ret
@@ng2:	setborder 1

	;add polygons to list
	mov	bp,cs:nec
@@2:	mov	cx,es:[di] ;sides
	cmp	cx,0
	je	@@1
	mov	ax,es:[di+2] ;color
	mov	cs:tmp_color,ax
	mov	si,di
	add	di,4
	mov	cs:tmp_firstvx,di
@@3:	add	si,4
	add	di,4
	cmp	cx,1
	jne	@@4
	mov	di,cs:tmp_firstvx
@@4:	push	cx
	;dx=color, si->start, di->end
	mov	ax,cs:tmp_color
	mov	cs:[bp+NE_COLOR],ax
	mov	bx,es:[si+2] ;y1
	mov	cx,es:[di+2] ;y2
	cmp	bx,cx 
	jg	@@i1	;y1>y2
	mov	ax,es:[si+0] ;x1
	mov	dx,es:[di+0] ;x2
	jmp	@@i0
@@i1:	xchg	bx,cx
	mov	ax,es:[di+0] ;x1
	mov	dx,es:[si+0] ;x2	
@@i0:	;ax,bx=xy1  dx,cx=xy2
@@i9:	mov	cs:[bp+NE_Y1],bx
	mov	cs:[bp+NE_Y2],cx
	mov	word ptr cs:[bp+NE_X],0
	mov	cs:[bp+NE_X+2],ax
	neg	ax
	add	ax,dx
	shl	eax,16
	cdq
	sub	cx,bx
	;cx=y2-y1,edx:eax=(x2-x1)<<16
	cmp	cx,0
	je	@@n1 ;skip horizontal lines
	movzx	ecx,cx
	idiv	ecx
	mov	cs:[bp+NE_DX],eax
	;!!!if y1<0, clip
	cmp	bx,0
	jge	@@nc
	mov	dx,cs:[bp+NE_Y2]
	cmp	dx,0
	jle	@@n1
	neg	bx
	movzx	ebx,bx
	imul	ebx
	add	cs:[bp+NE_X],eax
	xor	bx,bx
	mov	cs:[bp+NE_Y1],bx
@@nc:	;!!!
	shl	bx,1
	mov	ax,cs:nep[bx]
	or	ax,ax
	jnz	@@j1
	;first on this row
	mov	cs:nep[bx],bp
	mov	word ptr cs:[bp+NE_NEXT],0
	jmp	@@j0	
@@j1:	;add to this row
	;scan if already exists
	push	ax
	push	si
	mov	si,ax
	jmp	@@h11
@@h1:	mov	si,cs:[si+NE_NEXT]
@@h11:	mov	ax,cs:[bp+NE_Y2]
	cmp	ax,cs:[si+NE_Y2]
	jne	@@h2
	mov	eax,cs:[bp+NE_X]
	cmp	eax,cs:[si+NE_X]
	jne	@@h2
	mov	eax,cs:[bp+NE_DX]
	cmp	eax,cs:[si+NE_DX]
	jne	@@h2
	;duplicate line SI
	mov	al,cs:[bp+NE_COLOR]
	xor	cs:[si+NE_COLOR],al
	pop	si
	pop	ax
	jmp	@@n1
@@h2:	cmp	si,0
	jne	@@h1 ;end of list
@@h3:	pop	si
	pop	ax
	mov	cs:nep[bx],bp
	mov	word ptr cs:[bp+NE_NEXT],ax
@@j0:	add	bp,16
@@n1:	;next
	pop	cx
	loop	@@3
	add	si,4
	mov	di,si
	jmp	@@2

@@1:	mov	cs:nec,bp
	sub	bp,OFFSET ne
	shr	bp,4
	mov	cs:__ndebug1,bp

	ret
__newgroup ENDP

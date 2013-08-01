;borders - can be found in asm.asm at the tmp work area (size 8K = 1024 rows)
ALIGN 4
PUBLIC _polyinfo
_polyinfo LABEL WORD
polysides dw	0		;polygon sides 
polyxy	dw	32 dup(0,0)	;x1,y1,x2,y2,...
polyz	dd	32 dup(0)	;z1,z2,...
polytxy	dw	32 dup(0,0)	;texturexy
POLYZ	equ	OFFSET polyz-OFFSET polyxy
POLYTXY	equ	OFFSET polytxy-OFFSET polyxy
;clipped from polyclip
cpolyxy	dw	32 dup(0,0)
cpolyxy2 dw	32 dup(0,0) ;tmp storage for polyclip
cpolysides dw	0
		
polygroup PROC FAR ;draw polygon group
	;es:di=pointer to polygon group:
	;	word: sides (0=end of list, &256=texture)
	;	word: color (|8000=hidden |4000=forceshow)
	;	word(s): x,y
@@1:	mov	cx,es:[di]
	add	di,2
	or	cx,cx
	jz	@@3
	mov	cs:polysides,cx
	mov	ax,es:[di]
	add	di,2
	test	ah,40h
	jnz	@@6
	test	ah,80h
	jnz	@@5
@@6:	mov	ds:color1,al
	mov	bx,OFFSET polyxy
	sub	bx,di
	sub	bx,4
@@2:	mov	eax,es:[di]
	add	di,4
	mov	cs:[di+bx],eax
	loop	@@2
	push	es
	push	di
	call	poly
	pop	di
	pop	es
	jmp	@@1
@@3:	ret
@@5:	shl	cx,2
	add	di,cx
	jmp	@@1
polygroup ENDP

polyroutines LABEL WORD
	dw	OFFSET polyret
	dw	OFFSET poly1
	dw	OFFSET poly2
	dw	OFFSET polyf ;poly3
	dw	OFFSET polyf ;poly4 ;poly4
	dw	OFFSET polyf ;polyn ;poly>4

IFDEF 0
tpolyroutines LABEL WORD
	dw	OFFSET polyret
	dw	OFFSET poly1
	dw	OFFSET poly2
	dw	OFFSET polyft
	dw	OFFSET polyft
	dw	OFFSET polyft
ENDIF

poly	PROC NEAR ;****
	mov	dx,3c4h
	mov	al,2
	out	dx,al
	mov	es,ds:vram
	mov	ax,cs:polysides
	mov	cs:cpolysides,ax
	cmp	ax,2
	ja	noclip
	call	clipanypoly ;only lines at the moment, others done by polyrout
noclip:	mov	si,OFFSET polyroutines
;	test	cs:groupflags,4
;	jz	notext
;	mov	si,OFFSET tpolyroutines
notext:	mov	bx,cs:cpolysides
	cmp	bx,5
	jb	ply1
	jmp	cs:[si+10]
ply1:	shl	bx,1
	jmp	cs:[si+bx]
polyret: ret
poly	ENDP

;***** POLYGON CLIP


clip_x1	dw	0
clip_y1	dw	0
clip_x2	dw	0
clip_y2	dw	0

clipanypoly PROC NEAR
	;polysides/polyxy => cpolysides/cpolyxy
	mov	cx,ds:polysides
	cmp	cx,2
	jg	cap3
	cmp	cx,1
	je	cap4
	jcxz	cap0
	;line
	mov	eax,dword ptr ds:polyxy[0]
	mov	dword ptr ds:clip_x1,eax
	mov	eax,dword ptr ds:polyxy[4]
	mov	dword ptr ds:clip_x2,eax
cap2r:	call	clipliney
	cmp	ax,0
	jnz	cap0
	call	cliplinex
	cmp	ax,0
	jnz	cap0
	mov	eax,dword ptr ds:clip_x1
	mov	dword ptr ds:cpolyxy[0],eax
	mov	edx,dword ptr ds:clip_x2
	mov	dword ptr ds:cpolyxy[4],edx
	cmp	eax,edx
	je	cap2
	mov	word ptr ds:cpolysides,2
	ret
cap2:	mov	word ptr ds:cpolysides,1
	ret
cap4:	;dot
	mov	eax,dword ptr ds:polyxy[0]
	cmp	ax,cs:wminx
	jl	cap0
	cmp	ax,cs:wmaxx
	jg	cap0
	ror	eax,16
	cmp	ax,cs:wminy
	jl	cap0
	cmp	ax,cs:wmaxy
	jg	cap0
	ror	eax,16
	mov	dword ptr ds:cpolyxy,eax
	mov	word ptr cs:cpolysides,1
	ret
cap0:	;all clipped away
	mov	word ptr ds:cpolysides,0
	ret
cap3:	;polygon, first clip y, then x
	mov	si,cx
	shl	si,2
	sub	si,4
	mov	di,0
	mov	eax,dword ptr ds:polyxy[si]
	mov	dword ptr ds:clip_x1,eax
	mov	eax,dword ptr ds:polyxy[di]
	mov	dword ptr ds:clip_x2,eax
	call	clipliney
	;
	mov	cx,ds:polysides
	xor	di,di
	xor	bx,bx
	mov	edx,80008000h
	jmp	cap35
cap32:	push	di
	push	bx
	push	cx
	push	edx
	mov	si,di
	sub	si,4
	mov	eax,dword ptr ds:polyxy[si]
	mov	dword ptr ds:clip_x1,eax
	mov	eax,dword ptr ds:polyxy[di]
	mov	dword ptr ds:clip_x2,eax
	call	clipliney
	pop	edx
	pop	cx
	pop	bx
	pop	di
cap35:	cmp	ax,0
	jnz	cap34
	mov	eax,dword ptr ds:clip_x1
	cmp	eax,edx
	je	cap33
	mov	dword ptr ds:cpolyxy2[bx],eax
	mov	edx,eax
	add	bx,4
cap33:	mov	eax,dword ptr ds:clip_x2
	cmp	eax,edx
	je	cap34
	mov	dword ptr ds:cpolyxy2[bx],eax
	mov	edx,eax
	add	bx,4
cap34:	add	di,4
	loop	cap32
	;
	mov	cx,bx
	shr	cx,2
	cmp	dword ptr ds:cpolyxy2[0],edx
	jne	cap31
	dec	cx
cap31:	mov	ds:cpolysides,cx
	
	cmp	cx,2
	jg	cap39
	cmp	cx,0
	je	cap38
	mov	eax,dword ptr ds:cpolyxy2[0]
	mov	dword ptr ds:clip_x1,eax
	mov	eax,dword ptr ds:cpolyxy2[4]
	mov	dword ptr ds:clip_x2,eax
	jmp	cap2r ;reclip the remaining line
cap38:	ret
cap39:
	mov	si,cx
	shl	si,2
	sub	si,4
	mov	di,0
	mov	eax,dword ptr ds:cpolyxy2[si]
	mov	dword ptr ds:clip_x1,eax
	mov	eax,dword ptr ds:cpolyxy2[di]
	mov	dword ptr ds:clip_x2,eax
	call	cliplinex
	;
	mov	cx,ds:cpolysides
	xor	di,di
	xor	bx,bx
	mov	edx,80008000h
	jmp	cbp35
cbp32:	push	di
	push	bx
	push	cx
	push	edx
	mov	si,di
	sub	si,4
	mov	eax,dword ptr ds:cpolyxy2[si]
	mov	dword ptr ds:clip_x1,eax
	mov	eax,dword ptr ds:cpolyxy2[di]
	mov	dword ptr ds:clip_x2,eax
	call	cliplinex
	pop	edx
	pop	cx
	pop	bx
	pop	di
cbp35:	cmp	ax,0
	jnz	cbp34
	mov	eax,dword ptr ds:clip_x1
	cmp	eax,edx
	je	cbp33
	mov	dword ptr ds:cpolyxy[bx],eax
	mov	edx,eax
	add	bx,4
cbp33:	mov	eax,dword ptr ds:clip_x2
	cmp	eax,edx
	je	cbp34
	mov	dword ptr ds:cpolyxy[bx],eax
	mov	edx,eax
	add	bx,4
cbp34:	add	di,4
	loop	cbp32
	;
	mov	cx,bx
	shr	cx,2
	cmp	dword ptr ds:cpolyxy[0],edx
	jne	cbp31
	dec	cx
cbp31:	mov	ds:cpolysides,cx

	ret
clipanypoly ENDP

clipcheck MACRO reg,min,max,flagreg,flagmin,flagmax
	local	l1,l2
	cmp	reg,min
	jge	l1
	or	flagreg,flagmin
l1:	cmp	reg,max
	jle	l2
	or	flagreg,flagmax
l2:	ENDM
clipmacro MACRO v1,v2,w1,w2,wl
	local	l1,l2
	push	bx
	mov	bx,wl
	mov	cx,w2
	sub	cx,w1
	jcxz	l1
	mov	bp,bx
	sub	bp,w1
	mov	ax,v2
	sub	ax,v1
	imul	bp 
	idiv	cx
	add	ax,v1
	mov	v1,ax
	mov	word ptr w1,bx
	jmp	l2
l1:	mov	ax,v1
	mov	word ptr w1,bx
l2:	pop	bx
	ENDM
cliplinex PROC NEAR
	;input line cpolyxy[SI]=>cpolyxy[DI]
	xor	bx,bx
	mov	ax,ds:clip_x1
	clipcheck ax,cs:wminx,cs:wmaxx,bl,1,2
	mov	ax,ds:clip_x2
	clipcheck ax,cs:wminx,cs:wmaxx,bh,1,2
	mov	al,bl
	and	al,bh
	jz	clpx1
	ret
clpx1:
	test	bl,1
	jz	clpx13
	clipmacro ds:clip_y1,ds:clip_y2,ds:clip_x1,ds:clip_x2,cs:wminx
clpx13:	test	bl,2
	jz	clpx14
	clipmacro ds:clip_y1,ds:clip_y2,ds:clip_x1,ds:clip_x2,cs:wmaxx
clpx14:
	test	bh,1
	jz	clpx23
	clipmacro ds:clip_y2,ds:clip_y1,ds:clip_x2,ds:clip_x1,cs:wminx
clpx23:	test	bh,2
	jz	clpx24
	clipmacro ds:clip_y2,ds:clip_y1,ds:clip_x2,ds:clip_x1,cs:wmaxx
clpx24:
	xor	ax,ax
	ret
cliplinex ENDP
clipliney PROC NEAR
	xor	bx,bx
	mov	ax,ds:clip_y1
	clipcheck ax,cs:wminy,cs:wmaxy,bl,4,8
	mov	ax,ds:clip_y2
	clipcheck ax,cs:wminy,cs:wmaxy,bh,4,8
	mov	al,bl
	and	al,bh
	jz	clpy1
	ret
clpy1:
	test	bl,4
	jz	clpy11
	clipmacro ds:clip_x1,ds:clip_x2,ds:clip_y1,ds:clip_y2,cs:wminy
clpy11:	test	bl,8
	jz	clpy12
	clipmacro ds:clip_x1,ds:clip_x2,ds:clip_y1,ds:clip_y2,cs:wmaxy
clpy12:
	test	bh,4
	jz	clpy21
	clipmacro ds:clip_x2,ds:clip_x1,ds:clip_y2,ds:clip_y1,cs:wminy
clpy21:	test	bh,8
	jz	clpy22
	clipmacro ds:clip_x2,ds:clip_x1,ds:clip_y2,ds:clip_y1,cs:wmaxy
clpy22:	
	xor	ax,ax
	ret
clipliney ENDP

;***** POLYGON DRAW

poly1	PROC NEAR
	mov	dx,cs:cpolyxy[0]
	mov	bx,cs:cpolyxy[2]
	call	VIDPSET
	ret
poly1	ENDP

poly2	PROC NEAR
	mov	cx,cs:cpolyxy[0]
	mov	ax,cs:cpolyxy[2]
	mov	dx,cs:cpolyxy[4]
	mov	bx,cs:cpolyxy[6]
	call	VIDLINE
	ret
poly2	ENDP

;*** POLYF / POLYFT

ALIGN 4
feax	dd	0
fedx	dd	0

fleftaddl dd	0 ;+0
fleftaddh dw	0 ;+4
fleftrown dw	0 ;+6
fleftzb dd	0 ;+8
fleftze dd	0 ;+12
flefttx0 dw	0 ;+16
fleftty0 dw	0 ;+18
flefttxa dw	0 ;+20
flefttya dw	0 ;+22
fleftcnt dw	0 ;+24
fleftcnta dw	0 ;+26
fleftd3a dw	0 ;+28

frightaddl dd	0 ;+0
frightaddh dw	0 ;+4
frightrown dw	0 ;+6
frightzb dd	0 ;+8
frightze dd	0 ;+12
frighttx0 dw	0 ;+16
frightty0 dw	0 ;+18
frighttxa dw	0 ;+20
frighttya dw	0 ;+22
frightcnt dw	0 ;+24
frightcnta dw	0 ;+26
frightd3a dw	0 ;+28

finfolen dw	0
finfo0 dw	0
finfo1 dw	0
fwmaxy1 dw 	0

finfo	dw	32 dup(0,0,0,0,0,0,0,0)
		;x,y,zlo,zhi,tx,ty,0,0

polyf	PROC NEAR ;ONLY CONVEX POLYGONS - FAST?
	;input: polysides/polyxy
	;requirements:
	;es=vram
	;cpolysides>=4 (not checked)
	;color=set
	;**COPY/SEEK UPPERMOST**
	LOADGS
	mov	ax,cs
	mov	ds,ax
	mov	cx,ds:polysides
	mov	ax,cx
	shl	ax,4 ;*16
	mov	ds:finfolen,ax
	add	ax,OFFSET finfo
	mov	ds:finfo1,ax
	mov	ax,gs:wmaxy
	inc	ax
	mov	ds:fwmaxy1,ax
	mov	edx,077770000h
	xor	bx,bx
	mov	si,OFFSET polyxy
	mov	di,OFFSET finfo
	mov	ds:finfo0,di
pfn1:	mov	eax,dword ptr ds:[si]
	cmp	eax,edx
	jg	pfn2
	mov	edx,eax
	mov	bx,di
pfn2:	mov	dword ptr ds:[di],eax
	add	si,4
	add	di,16
	loop	pfn1
	;[bx]=uppermost
	;**SETUP REGS**
	mov	ds:fleftrown,-32767
	mov	ds:frightrown,-32767
	mov	si,bx
	mov	di,bx
	mov	bp,ds:[si+2]
	mov	bx,OFFSET borders
	mov	ax,bp
	cmp	ax,gs:wminy
	jge	pfn35
	mov	ax,gs:wminy
pfn35:	mov	gs:[bx],ax
	add	bx,2
	mov	cx,16 ;max tmp count to avoid hanging on illegal polygons
	;eax=left
	;bx=pointer to borders[]
	;cx=count
	;edx=right
	;si=left
	;di=right
	;bp=y
pfn63:	push	cx
	push	bx
	
	cmp	bp,ds:fleftrown
	jl	pfn42
	push	edx
	push	di
	mov	di,si
	sub	di,16
	cmp	di,ds:finfo0
	jae	pfn41
	add	di,ds:finfolen
pfn41:	mov	bx,OFFSET fleftaddl
	call	polyfcalc
	add	cx,bp
	mov	ds:fleftrown,cx
	movzx	eax,word ptr ds:[si+0]
	mov	ebx,ds:fleftaddl
	mov	dx,ds:fleftaddh
	sar	dx,1
	rcr	ebx,1
	xor	bx,bx
	sub	eax,ebx
	sbb	ax,dx
	mov	si,di
	pop	di
	pop	edx
pfn42:
	cmp	bp,ds:frightrown
	jl	pfn52
	push	eax
	push	si
	mov	si,di
	add	di,16
	cmp	di,ds:finfo1
	jb	pfn51
	sub	di,ds:finfolen
pfn51:	mov	bx,OFFSET frightaddl
	call	polyfcalc
	add	cx,bp
	mov	ds:frightrown,cx
	movzx	edx,word ptr ds:[si+0]
	mov	ebx,ds:frightaddl
	mov	ax,ds:frightaddh
	sar	ax,1
	rcr	ebx,1
	xor	bx,bx
	sub	edx,ebx
	sbb	dx,ax
	pop	si
	pop	eax
	
pfn52:	mov	bx,ds:fleftrown
	mov	cx,ds:frightrown
	cmp	cx,bx
	jl	pfn61
	mov	cx,bx
pfn61:	sub	cx,bp
	pop	bx
	cmp	cx,0
	jg	pfn71
pfn6:	pop	cx
	cmp	bp,ds:fwmaxy1
	jg	pfn64
	cmp	si,di
	je	pfn64
	dec	cx
	jz	pfn64
	jmp	pfn63
pfn64:	mov	word ptr gs:[bx],-32767
	mov	si,OFFSET borders
	LOADDS
;	call	newgroup
	call	VIDHGROUP
	ret
pfn65:	;above screen
	;entering screen, cut.
	add	bp,cx
	push	bp
	push	cx
	cmp	bp,gs:wminy
	jl	pfn66
	sub	bp,cx
	mov	cx,gs:wminy
	sub	cx,bp
pfn66:	;
	movsx	ecx,cx
	ror	eax,16
	mov	ds:feax,eax
	ror	edx,16
	mov	ds:fedx,edx
	;
	mov	ax,ds:fleftaddh
	shl	eax,16
	mov	ax,word ptr ds:fleftaddl[2]
	imul	ecx
	add	ds:feax,eax
	;
	mov	ax,ds:frightaddh
	shl	eax,16
	mov	ax,word ptr ds:frightaddl[2]
	imul	ecx
	add	ds:fedx,eax
	;
	mov	eax,ds:feax
	ror	eax,16
	mov	edx,ds:fedx
	ror	edx,16
	mov	bp,cx
	pop	cx
	sub	cx,bp
	pop	bp
	cmp	cx,0
	jne	pfn6b
	jmp	pfn6
pfn6b:	mov	bp,gs:wminy
pfn71:	;process segment
	cmp	bp,gs:wminy
	jl	pfn65 ;above screen still
	add	bp,cx
	;clip max to maxy
	cmp	bp,ds:fwmaxy1
	jle	pfn72
	sub	bp,cx
	mov	cx,ds:fwmaxy1
	sub	cx,bp
	mov	bp,ds:fwmaxy1
pfn72:	cmp	cx,0
	jle	pfn6
	push	si
	push	di
	push	bp
	ror	ebx,16
	neg	cx
	mov	bx,cx
	ror	ebx,16
	mov	esi,ds:fleftaddl
	mov	edi,ds:frightaddl
	mov	bp,ds:fleftaddh
	mov	cx,ds:frightaddh
pfn7:	add	eax,esi
	adc	ax,bp
	add	edx,edi
	adc	dx,cx
	mov	gs:[bx],ax
	mov	gs:[bx+2],dx
	add	ebx,10004h
	jnc	pfn7
	pop	bp
	pop	di
	pop	si
	jmp	pfn6
polyfcalc: ;**** subroutine ****
	;calc slope for line [SI]->[DI] to [BX], returns CX=len
	mov	cx,ds:[di+2]
	sub	cx,bp ;ds:[si+2]
	jle	pfc1
	mov	ax,ds:[di+0]
	sub	ax,ds:[si+0]
	jl	pfc2
	xor	dx,dx
	div	cx
	mov	ds:[bx+4],ax
	xor	ax,ax
	div	cx
	mov	ds:[bx+2],ax
	;dec	cx
	ret
pfc1:	xor	cx,cx
	ret
pfc2:	neg	ax
	xor	dx,dx
	div	cx
	push	ax
	xor	ax,ax
	div	cx
	pop	dx
	neg	ax
	adc	dx,0
	neg	dx
	mov	ds:[bx+4],dx
	mov	ds:[bx+2],ax
	;dec	cx
	ret
polyf	ENDP

IFDEF 0
;fleftaddl dd	0 ;+0
;fleftaddh dw	0 ;+4
;fleftrown dw	0 ;+6
;fleftzb dd	0 ;+8
;fleftze dd	0 ;+12
;flefttx0 dw	0 ;+16
;fleftty0 dw	0 ;+18
;flefttxa dw	0 ;+20
;flefttya dw	0 ;+22
;fleftcnt dw	0 ;+24
;fleftcnta dw	0 ;+26

polyft	PROC NEAR ;ONLY CONVEX POLYGONS - FAST?
	;input: polysides/polyxy
	;requirements:
	;es=vram
	;ds=cs
	;cpolysides>=4 (not checked)
	;color=set
	;**COPY/SEEK UPPERMOST**
	LOADGS	;GS points to data seg!
	mov	ax,cs
	mov	ds,ax
	mov	fs,cs:depthseg
	mov	cx,cs:polysides
	mov	ax,cx
	shl	ax,4 ;*16
	mov	ds:finfolen,ax
	add	ax,OFFSET finfo
	mov	ds:finfo1,ax
	mov	ax,gs:wmaxy
	inc	ax
	mov	ds:fwmaxy1,ax
	mov	edx,077770000h
	xor	bx,bx
	mov	si,OFFSET polyxy
	mov	di,OFFSET finfo
	mov	ds:finfo0,di
pftn1:	mov	eax,dword ptr ds:[si]
	cmp	eax,edx
	jg	pftn2
	mov	edx,eax
	mov	bx,di
pftn2:	mov	dword ptr ds:[di],eax
	mov	eax,ds:[si+POLYZ]
	mov	ds:[di+4],eax
	mov	eax,ds:[si+POLYTXY]
	mov	ds:[di+8],eax
	add	si,4
	add	di,16
	loop	pftn1
	;[bx]=uppermost
	;**SETUP REGS**
	mov	ds:fleftrown,-32767
	mov	ds:frightrown,-32767
	mov	si,bx
	mov	di,bx
	mov	bp,ds:[si+2]
	mov	bx,OFFSET borders
	mov	ax,bp
	cmp	ax,gs:wminy
	jge	pftn35
	mov	ax,gs:wminy
pftn35:	mov	gs:[bx],ax
	add	bx,2
	mov	cx,16 ;max tmp count to avoid hanging on illegal polygons
	;eax=left
	;bx=pointer to borders[]
	;cx=count
	;edx=right
	;si=left
	;di=right
	;bp=y
pftn63:	push	cx
	push	bx
	
	cmp	bp,ds:fleftrown
	jl	pftn42
	push	edx
	push	di
	mov	di,si
	sub	di,16
	cmp	di,ds:finfo0
	jae	pftn41
	add	di,ds:finfolen
pftn41:	mov	bx,OFFSET fleftaddl
	call	polyftcalc
	add	cx,bp
	mov	ds:fleftrown,cx
	movzx	eax,word ptr ds:[si+0]
	mov	ebx,ds:fleftaddl
	mov	dx,ds:fleftaddh
	sar	dx,1
	rcr	ebx,1
	xor	bx,bx
	sub	eax,ebx
	sbb	ax,dx
	mov	si,di
	pop	di
	pop	edx
pftn42:
	cmp	bp,ds:frightrown
	jl	pftn52
	push	eax
	push	si
	mov	si,di
	add	di,16
	cmp	di,ds:finfo1
	jb	pftn51
	sub	di,ds:finfolen
pftn51:	mov	bx,OFFSET frightaddl
	call	polyftcalc
	add	cx,bp
	mov	ds:frightrown,cx
	movzx	edx,word ptr ds:[si+0]
	mov	ebx,ds:frightaddl
	mov	ax,ds:frightaddh
	sar	ax,1
	rcr	ebx,1
	xor	bx,bx
	sub	edx,ebx
	sbb	dx,ax
	pop	si
	pop	eax
	
pftn52:	mov	bx,ds:fleftrown
	mov	cx,ds:frightrown
	cmp	cx,bx
	jl	pftn61
	mov	cx,bx
pftn61:	sub	cx,bp
	pop	bx
	cmp	cx,0
	jg	pftn71
pftn6:	pop	cx
	cmp	bp,ds:fwmaxy1
	jg	pftn64
	cmp	si,di
	je	pftn64
	dec	cx
	jz	pftn64
	jmp	pftn63
pftn64:	mov	word ptr gs:[bx],-32767
	mov	si,OFFSET borders ;gs:si
	mov	fs,cs:_texture[2]
	LOADDS
;	call	newgroup
	call	VIDHGROUP
	ret
pftn65:	;above screen
	;entering screen, cut.
	add	bp,cx
	push	bp
	push	cx
	cmp	bp,gs:wminy
	jl	pftn66
	sub	bp,cx
	mov	cx,gs:wminy
	sub	cx,bp
pftn66:	;
	movsx	ecx,cx
	ror	eax,16
	mov	ds:feax,eax
	ror	edx,16
	mov	ds:fedx,edx
	;
	mov	ax,ds:fleftaddh
	shl	eax,16
	mov	ax,word ptr ds:fleftaddl[2]
	imul	ecx
	add	ds:feax,eax
	mov	ax,word ptr ds:fleftcnta
	imul	cx
	add	ds:fleftcnt,ax
	;
	mov	ax,ds:frightaddh
	shl	eax,16
	mov	ax,word ptr ds:frightaddl[2]
	imul	ecx
	add	ds:fedx,eax
	mov	ax,word ptr ds:frightcnta
	imul	cx
	add	ds:frightcnt,ax
	;
	mov	eax,ds:feax
	ror	eax,16
	mov	edx,ds:fedx
	ror	edx,16
	mov	bp,cx
	pop	cx
	sub	cx,bp
	pop	bp
	cmp	cx,0
	jne	pftn6b
	jmp	pftn6
pftn6b:	mov	bp,gs:wminy
pftn71:	;process segment
	cmp	bp,gs:wminy
	jl	pftn65 ;above screen still
	add	bp,cx
	;clip max to maxy
	cmp	bp,ds:fwmaxy1
	jle	pftn72
	sub	bp,cx
	mov	cx,ds:fwmaxy1
	sub	cx,bp
	mov	bp,ds:fwmaxy1
pftn72:	cmp	cx,0
	jle	pftn6
	push	si
	push	di
	push	bp
	ror	ebx,16
	neg	cx
	mov	bx,cx
	ror	ebx,16
	mov	esi,ds:fleftaddl
	mov	edi,ds:frightaddl
	;eax/edx=x position
	;esi/edi=x adder high
	;bp/cx=xadder low
	;ebx=borders pointer / end counter
	;mov	cx,gs
	
calccx MACRO bb ;bp=>cx
	;linear
	mov	bp,word ptr ds:bb[24]
	shr	bp,5
	shl	bp,1
	add	bp,word ptr ds:bb[28]
	mov	bp,fs:[bp]
	mov	eax,dword ptr ds:bb[20]
	imul	bp
	shld	dx,ax,7
	add	dx,word ptr ds:bb[16]
	mov	cl,dl
	shr	eax,16
	imul	bp
	shld	dx,ax,7
	add	dx,word ptr ds:bb[18]
	mov	ch,dl
	mov	bp,word ptr ds:bb[26]
	add	word ptr ds:bb[24],bp
	ENDM
	
pftn7:	add	eax,esi
	adc	ax,ds:fleftaddh
	add	edx,edi
	adc	dx,ds:frightaddh
	mov	gs:[bx],ax
	mov	gs:[bx+2],dx
	push	dx
	push	eax
	calccx	frightaddl
	shl	ecx,16
	calccx	fleftaddl
	mov	gs:[bx+4],ecx
	pop	eax
	pop	dx
	add	ebx,10008h
	jnc	pftn7
	pop	bp
	pop	di
	pop	si
	jmp	pftn6
ALIGN 4
zzzcxa	dw	0

;fleftaddl dd	0 ;+0
;fleftaddh dw	0 ;+4
;fleftrown dw	0 ;+6
;fleftzb dd	0 ;+8
;fleftze dd	0 ;+12
;flefttx0 dw	0 ;+16
;fleftty0 dw	0 ;+18
;flefttxa dw	0 ;+20
;flefttya dw	0 ;+22
;fleftcnt dw	0 ;+24
;fleftcnta dw	0 ;+26

polyftcalc: ;**** subroutine ****
	;calc slope for line [SI]->[DI] to [BX], returns CX=len
	;calc texture
	mov	eax,ds:[si+4]
	mov	ds:[bx+8],eax
	mov	eax,ds:[di+4]
	mov	ds:[bx+12],eax
	mov	eax,ds:[si+8]
	mov	ds:[bx+16],eax
	neg	ax
	add	ax,ds:[di+8]
	mov	ds:[bx+20],ax
	shr	eax,16
	neg	ax
	add	ax,ds:[di+10]
	mov	ds:[bx+22],ax
	;calc slope
	;bx+2=addl
	;bx+4=addh
	mov	cx,ds:[di+2]
	sub	cx,bp ;ds:[si+2]
	jle	pftc1
	;calc texture len
	mov	ax,16384
	xor	dx,dx
	mov	ds:[bx+24],dx
	div	cx
	mov	ds:[bx+26],ax
	;find suitable texture function
	push	bp
	mov	eax,256
	imul	dword ptr ds:[si+4]
	idiv	dword ptr ds:[di+4]
	mov	bp,-4
pftc3:	add	bp,4
	cmp	fs:[bp],ax
	ja	pftc3
	mov	ax,fs:[bp+2]
	mov	ds:[bx+28],ax
	pop	bp
	;
	mov	ax,ds:[di+0]
	sub	ax,ds:[si+0]
	jl	pftc2
	xor	dx,dx
	div	cx
	mov	ds:[bx+4],ax
	xor	ax,ax
	div	cx
	mov	ds:[bx+2],ax
	;dec	cx
	ret
pftc1:	xor	cx,cx
	ret
pftc2:	neg	ax
	xor	dx,dx
	div	cx
	push	ax
	xor	ax,ax
	div	cx
	pop	dx
	neg	ax
	adc	dx,0
	neg	dx
	mov	ds:[bx+4],dx
	mov	ds:[bx+2],ax
	ret
polyft	ENDP
ENDIF

include new.asm

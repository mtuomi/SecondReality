;================================================================
; Timer & Copper
;================================================================

;################################################################

; Copper simulator :-)
; Don't play with this one!

		ALIGN 16

coppercount	dw	0
public _longframecount
_longframecount dw	0,0
copperframecount dw	0

timermode	equ	36h ;30h
timermode2	equ	06h ;30h

frametime	dw	16000
frameheigth	dw	100 ;from hseq !!
copperlstpnt	dw	0
scanline	dw	0
reinitds	dw	0

enable		db	0
copperborder	db	0

callme		dw	0
writeme		dw	0

PUBLIC _runcloop
_runcloop	dw	0

listoffsets	dw	OFFSET copperlst0

copperlst	dw	64 dup(0,0,0,0);

;copper list... Well, sort of...
copperlst0 LABEL WORD
	dw	0
	dw	20,OFFSET copper_doit
	dw	95,0 
	
ALIGN 16
copper_int0	dd	copper_intretf
copper_int1	dd	copper_intretf
copper_int2	dd	copper_intretf
copper_int0flag	db	0
	
public	_asmcopperinit,_asmcopperdeinit

oldint8	dd	?

PUSHALL MACRO
	pusha
	push	ds
	push	es
	ENDM
	
POPALL MACRO
	pop	es
	pop	ds
	popa
	ENDM

copper_intretf PROC FAR
;;	call	testbar ;!!
	ret
copper_intretf ENDP

copperwaitborder PROC NEAR
	mov	dx,03dah
cbl1:	in	al,dx
	test	al,8
	jnz	cbl1
cbl2:	in	al,dx
	test	al,8
	jz	cbl2
	ret
copperwaitborder ENDP

copperwaitscreen PROC NEAR
	mov	dx,03dah
cbl3:	in	al,dx
	test	al,8
	jz	cbl3
cbl4:	in	al,dx
	test	al,8
	jnz	cbl4
	ret
copperwaitscreen ENDP

_asmcopperinit PROC FAR
	call	syncronize
	mov	cs:frametime,ax
	cli
	in	al,21h
	and	al,not 1
	out	21h,al
	xor	ax,ax
	mov	es,ax
	mov	bx,8h*4
	mov	di,OFFSET oldint8
	mov	si,OFFSET intti8
	mov	ax,es:[bx]
	mov	cs:[di],ax
	mov	ax,es:[bx+2]
	mov	cs:[di+2],ax
	mov	es:[bx],si
	mov	es:[bx+2],cs
	mov	si,OFFSET copperlst0
	call	loadcopperlist
	mov	al,timermode
	out	43h,al
	mov	ax,65535
	out	40h,al
	mov	al,ah
	out	40h,al
	;mov	cs:enable,0
	;mov	cs:writeme,65535
	call	copper_end
	sti
	ret
_asmcopperinit ENDP

_asmcopperdeinit PROC FAR
	cli
	xor	ax,ax
	mov	es,ax
	mov	bx,8h*4
	mov	di,OFFSET oldint8
	mov	ax,cs:[di]
	mov	es:[bx],ax
	mov	ax,cs:[di+2]
	mov	es:[bx+2],ax
	mov	al,36h
	out	43h,al
	mov	ax,65535
	out	40h,al
	mov	al,ah
	out	40h,al
	sti
	ret
_asmcopperdeinit ENDP

ALIGN 4

dolistentry PROC NEAR
	mov	bx,cs:copperlstpnt
	mov	ax,cs:copperlst[bx+2]
	mov	cs:callme,ax
	mov	ax,cs:copperlst[bx]
	mov	cs:scanline,ax
	mov	ax,cs:copperlst[bx+4]
	mov	cs:writeme,ax
	add	bx,8
	mov	cs:copperlstpnt,bx
	ret
dolistentry ENDP	

loadcopperlist PROC NEAR
	cmp	word ptr cs:[si],0
	je	icl4
	call	cs:[si]
icl4:	add	si,2
	mov	bx,0
	mov	ax,cs:[si]
	mov	cs:scanline,ax
	
lcl1:	mov	cx,cs:[si+2]
	mov	cs:copperlst[bx+2],cx
	mov	dx,cs:[si]
	mov	cs:copperlst[bx+0],dx
	cmp	cx,0
	jne	lcl2
	;last one
	mov	ax,cs:scanline
	jmp	lcl3
lcl2:	mov	ax,cs:[si+4]
	sub	ax,dx
lcl3:	mul	cs:frametime
	div	cs:frameheigth
	mov	cs:copperlst[bx+4],ax
	add	bx,8
	add	si,4
	cmp	cx,0
	jne	lcl1
	
	mov	cs:scanline,0
	mov	cs:copperlst[bx+4],cx
	
	mov	bx,OFFSET copperlst
	ret
loadcopperlist ENDP

inhere db 0
intti8 PROC FAR
	push	ax
	mov	al,20h
	out	20h,al
	cmp	cs:inhere,0
	jne	eoix
	inc	cs:inhere
	mov	al,timermode
	out	43h,al
	mov	ax,cs:writeme
	out	40h,al
	mov	al,ah
	out	40h,al
	
	sti
	
	;cmp	cs:enable,0
	;je	notactive

	push	bx
	push	cx
	push	dx
	
	cmp	cs:callme,0
	je	eos
		
	call	cs:callme
	
	call	dolistentry
	
	jmp	eoi

eos:	;call	testbar
	call	copper_end

eoi:	dec	cs:inhere
	pop	dx
	pop	cx
	pop	bx
eoix:	pop	ax
	iret
notactive:
	mov	al,20h
	out	20h,al
	pop	ax
	iret
intti8 ENDP

colorcounter db	0
setcolor PROC NEAR
	add	cs:colorcounter,20
	mov	cl,cs:colorcounter
	xor	ch,ch
	jmp	setblack0
setcolor ENDP

setcolor0 PROC NEAR
	xor	cx,cx
	jmp	setblack0
setcolor0 ENDP

setblack PROC NEAR
	ret
	cmp	cs:copperborder,0
	je	noblack
setblack0:
	mov	dx,3c8h
	mov	al,0
	out	dx,al
	inc	dx
	mov	al,cl
	out	dx,al
	mov	al,ch
	out	dx,al
	xor	al,al
	out	dx,al
noblack: ret
setblack ENDP

copper_nop PROC NEAR
	mov	cx,100
dingdong: loop	dingdong
	ret
copper_nop ENDP

copper_start PROC NEAR
	xor	cx,cx
	call	setblack
	ret
copper_start ENDP

copper_end PROC NEAR
	mov	al,timermode
	out	43h,al
	xor	ax,ax
	out	40h,al
	out	40h,al
	xor	ax,ax
	mov	cs:colorcounter,al
	mov	cs:copperlstpnt,ax
	mov	cs:scanline,ax
	push	cs:writeme
	call	dolistentry
	PUSHALL
	call	cs:copper_int1
	POPALL
	pop	bx
	mov	dx,03dah
cel3:	in	al,dx
	test	al,8
	jnz	cel3
cel4:	in	al,dx
	test	al,8
	jz	cel4
	mov	al,timermode
	out	43h,al
	mov	ax,bx
	out	40h,al
	mov	al,ah
	out	40h,al
	;mov	cs:enable,1
	inc	cs:coppercount
	inc	cs:copperframecount ;used by dis for waitb returns
	PUSHALL
	call	cs:copper_int2
	POPALL
	ret
copper_end ENDP

syncronize0 PROC NEAR
	;wait till start of screen
††	sti
	mov	al,timermode
	out	43h,al
	mov	ax,65535
	out	40h,al
	mov	al,ah
	out	40h,al
	mov	dx,3dah
scrz1:	in	al,dx
	test	al,8
	jnz	scrz1
scrz2:	in	al,dx
	test	al,8
	jz	scrz2
	mov	al,timermode
	out	43h,al
	mov	ax,65535
	out	40h,al
	mov	al,ah
	out	40h,al
	;wait till end of visible screen
	mov	dx,3dah
scrz6:	in	al,dx
	test	al,8
	jnz	scrz6
scrz7:	in	al,dx
	test	al,8
	jz	scrz7
	mov	al,timermode2
	out	43h,al
	in	al,40h
	mov	ah,al
	in	al,40h
	xchg	al,ah
	sti
	ret
syncronize0 ENDP

syncronize PROC NEAR
	mov	cx,32
retry:	loop	tryit
	mov	ax,0 
	ret	;failed completely: AX=0
tryit:	call	syncronize0
	push	ax
	call	syncronize0
	pop	bx
	mov	dx,ax
	sub	dx,bx
	cmp	dx,-20h
	jl	retry
	cmp	dx,20h
	jg	retry
	add	ax,bx
	rcr	ax,1
	neg	ax
	shr	ax,1
	int	3
	nop
	nop
	nop
	ret	;complete: AX=timercount for frame
syncronize ENDP

;################################################################

dostimercount dw	0

setblackx PROC NEAR
	ret
	mov	dx,3c8h
	mov	al,0
	out	dx,al
	inc	dx
	mov	al,cl
	out	dx,al
	mov	al,ch
	out	dx,al
	out	dx,al
	mov	dx,3c8h
	mov	al,16
	out	dx,al
	inc	dx
	mov	al,cl
	out	dx,al
	mov	al,ch
	out	dx,al
	out	dx,al
	mov	dx,3c8h
	mov	al,32
	out	dx,al
	inc	dx
	mov	al,cl
	out	dx,al
	mov	al,ch
	out	dx,al
	out	dx,al
	ret
setblackx ENDP

testbar	PROC NEAR
	mov	cx,1f1fh
	call	setblackx
	mov	cx,25
ppz2:	loop	ppz2
	xor	cx,cx
	call	setblackx
	ret
testbar ENDP

bigtestbar PROC NEAR
	mov	cx,1f1fh
	call	setblackx
	mov	cx,250
ppz3:	loop	ppz3
	xor	cx,cx
	call	setblackx
	ret
bigtestbar ENDP

copper_doit PROC NEAR
	push	si
	push	di
	push	bp
	push	ds
	push	es
	
	;;call	bigtestbar
	
	cmp	cs:copper_int0flag,0
	jne	@@1	
	mov	cs:copper_int0flag,1
	PUSHALL
	call	cs:copper_int0
	POPALL
	mov	cs:copper_int0flag,0
@@1:
	call	_zpollme
	
	call	u2copperint

;	mov	ax,cs:dostimercount
;	add	ax,19900
;	mov	cs:dostimercount,ax
;	jnc	@@skipi
;	pushf
;	call	cs:oldint8
;@@skipi:
	mov	ax,040h
	mov	ds,ax
	inc	word ptr ds:[6ch] ;advance dos timer counter (at a wrong speed)
	
	pop	es
	pop	ds
	pop	bp
	pop	di
	pop	si
	ret
copper_doit ENDP

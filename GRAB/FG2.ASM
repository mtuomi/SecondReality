debuginc MACRO xpos
	push	ax
	push	es
	mov	ax,0b800h
	mov	es,ax
	inc	byte ptr es:[xpos*2]
	mov	byte ptr es:[xpos*2+1],17h
	pop	es
	pop	ax
	ENDM

dosint	MACRO 	fn
	mov 	ah,fn
	int 	21h
	ENDM
	
int33	MACRO
	pushf
	call	cs:oldint8
	ENDM

code	SEGMENT word public 'code'
	ASSUME cs:code

start:	jmp	main

txt	db	13,10									       
	db	"ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»",13,10
	db	"º   Future-Multi-Grabber V1.0     Copyright (C) 1991 ÀÅÙ / The Future Crew    º",13,10
	db	"ÇÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¶",13,10
	db	"º Press the mouse RIGHT-button to grab a Deluxepaint LBM (PIC?.LBM).          º",13,10
	db	"ÇÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¶",13,10
	db	"º MOV AX,FCFCh ; MOV BX,1 ; INT 33h ; save LBM with FutureGrabber             º",13,10
	db	"ÇÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¶",13,10
	db	"º In error conditions the screen (border) color is flashed:                   º",13,10
	db	"º BLUE: Disk(full) error  GREEN: Invalid videomode  RED: Unknown problems :-) º",13,10
	db	"ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼",13,10
	db	'$'
txt2	db	13,10,'Future-Multi-Grabber already installed!',13,10,'$'
txt3	db	13,10,'MouseDriver (3 button mouse required) not found!',13,10,'$'
INTER	equ	33h
INTER2	equ	28h
INTER3	equ	8h
INTER4	equ	10h
INTER5	equ	9h
oldint6	dd	? ;int 10h
oldint7	dd	? ;timer
oldint8	dd	? ;mouse int
oldint9	dd	? ;int 28h
oldint5	dd	? ;keyb
oldint	dd	? ;mouse CALL
critoff	dw	?
critseg	dw	?
newint7 dd	? ;newtimer

whattodo db	0 ;1=savepic,2=savemem,3=menu
counter	dw	0
insideint db	0
insidemenu db	0

rowlength dw	0
screenstart dw	0
splitrow dw	0
tweaked	dw	0

pleasedoit db	0
lastbuttonstate dw	0

errorcols LABEL BYTE
	db	-1,63,0,0  ;red/unknown		0
	db	-1,0,28,0  ;green/videomode	1
	db	-1,0,0,32  ;blue/disk		2
	db	-1,8,8,16  ;INFO: saving mem	3
	db	-1,32,32,0 ;INFO: testtest  	4

apu	db	0

callmask dw	0
callpnt LABEL DWORD
calloff dw	0
callseg dw	0

mousex	dw	0
mousey	dw	0
mousegx	dw	0
mousegy	dw	0
mouseb	dw	0
omousex	dw	0
omousey	dw	0
omouseb	dw	0

mymin	dw	0
mymax	dw	0
mxmin	dw	0
mxmax	dw	0

defpal	LABEL BYTE
	db	00,00,00
	db	00,00,40
	db	00,40,00
	db	00,40,40
	db	40,00,00
	db	40,00,40
	db	40,20,00
	db	40,40,40
	db	20,20,20
	db	20,20,60
	db	20,60,20
	db	20,60,60
	db	60,20,20
	db	60,20,60
	db	60,60,20
	db	60,60,60
normalregs db	256 dup(0) ;text mode VGA regs
	
main:	;DO-IT!!
	mov	ax,cs
	mov	es,ax
	mov	di,OFFSET normalregs
	;call	fromvgaregs
	mov	ax,cs
	mov	ds,ax
	mov	si,OFFSET normalregs
	;call	tovgaregs
	mov	ax,0
	mov	dx,0
	int	33h
	cmp	dx,100h
	jne	notins
	mov	ax,cs
	mov	ds,ax
	mov	dx,OFFSET txt2
	mov	ah,9
	int	21h
	mov	ah,4ch
	int	21h
notins:	;chk mousedrv
	mov	ax,0
	int	33h
	cmp	ax,0
	jne	notmouseis
	mov	ax,cs
	mov	ds,ax
	mov	dx,OFFSET txt3
	mov	ah,9
	int	21h
	mov	ah,4ch
	int	21h
notmouseis:
	mov	ah,34h
	int	21h
	mov	cs:critoff,bx
	mov	cs:critseg,es
	mov	ax,cs
	mov	es,ax
	mov	ax,0ch
	mov	cx,0ffffh
	mov	dx,OFFSET intti
	int	33h
	cli
	mov	ax,0
	mov	es,ax
	mov	ax,es:[INTER*4]
	mov	WORD PTR cs:[oldint8+0],ax
	mov	ax,es:[INTER*4+2]
	mov	WORD PTR cs:[oldint8+0+2],ax
	mov	ax,OFFSET intti33
	mov	es:[INTER*4],ax
	mov	es:[INTER*4+2],cs
	
	mov	ax,es:[INTER2*4]
	mov	WORD PTR cs:[oldint9+0],ax
	mov	ax,es:[INTER2*4+2]
	mov	WORD PTR cs:[oldint9+0+2],ax
	mov	ax,OFFSET intti9
	mov	es:[INTER2*4],ax
	mov	es:[INTER2*4+2],cs
	
	mov	ax,es:[INTER5*4]
	mov	WORD PTR cs:[oldint5+0],ax
	mov	ax,es:[INTER5*4+2]
	mov	WORD PTR cs:[oldint5+0+2],ax
	mov	ax,OFFSET intti5
	mov	es:[INTER5*4],ax
	mov	es:[INTER5*4+2],cs
	
	mov	ax,es:[INTER3*4]
	mov	WORD PTR cs:[oldint7+0],ax
	mov	ax,es:[INTER3*4+2]
	mov	WORD PTR cs:[oldint7+0+2],ax
	mov	ax,OFFSET intti7
	mov	es:[INTER3*4],ax
	mov	es:[INTER3*4+2],cs

	mov	ax,es:[INTER4*4]
	mov	WORD PTR cs:[oldint6+0],ax
	mov	ax,es:[INTER4*4+2]
	mov	WORD PTR cs:[oldint6+0+2],ax
	mov	ax,OFFSET intti6
	mov	es:[INTER4*4],ax
	mov	es:[INTER4*4+2],cs
	sti
	mov	ax,cs
	mov	ds,ax
	mov	ah,9
	mov	dx,OFFSET txt
	int	21h
	mov	dx,16000/16
	mov	ah,31h
	int	21h
	
intti33	PROC	FAR 
	cmp	ax,0
	je	mousecheck
	cmp	ax,03h
	je	mouseand
	cmp	ax,0ch
	je	mousesubr
	cmp	ax,07h
	je	mousexrange
	cmp	ax,08h
	je	mouseyrange
	cmp	ax,0fcfch
	je	callfgc
	jmp	cs:oldint8
callfgc:
	call	fgfunctions
	iret
mousexrange:
	mov	cs:mxmin,cx
	mov	cs:mxmax,dx
	int33
	iret
mouseyrange:
	mov	cs:mymin,cx
	mov	cs:mymax,dx
	int33
	iret
mouseand: 
	int33
	test	bx,4
	jz	mad2
	xor	bx,bx ;if middlebutton pressed, lie all others are up
mad2:	and	bx,3
	iret
mousesubr:
	mov	cs:callmask,cx
	mov	cs:callseg,es
	mov	cs:calloff,dx
	iret
mousecheck:
	push	es
	push	cx
	push	dx
	mov	ax,cs
	mov	es,ax
	mov	ax,0ch
	mov	cx,-1
	mov	dx,OFFSET intti
	int33
	pop	dx
	pop	cx
	pop	es
	mov	ax,-1
	mov	bx,2 ;say it's a microsoft mouse
	mov	dx,0100h
	iret
intti33	ENDP

intti5	PROC FAR
	push	ax
	;revive mouse
	in	al,21h
	and	al,255-16
	out	21h,al
	pop	ax
	jmp	cs:oldint5
intti5	ENDP

fgfunctions PROC NEAR
	push	bx
	push	cx
	push	dx
	push	ds
	push	es
	push	si
	push	di
	push	bp
	cmp	bx,1
	jne	fgf1
	mov	cs:whattodo,1
	call	dosomething
	xor	ax,ax
fgf1:	pop	bp
	pop	di
	pop	si
	pop	es
	pop	ds
	pop	dx
	pop	cx
	pop	bx
	ret
fgfunctions ENDP

intti6	PROC	FAR
	cmp	ah,15
	jne	i6j1
	pushf
	call	cs:oldint6
	and	al,127
	iret
i6j1:	jmp	cs:oldint6
intti6	ENDP

intti7	PROC	FAR ;timer
	push	ds
	push	bx
	cmp	cs:insideint,0
	jne	gnope
	cmp	cs:pleasedoit,0
	je	gnope
	mov	bx,cs:critoff
	mov	ds,cs:critseg
	cmp	byte ptr ds:[bx],0
	jne	gnope
	;
	inc	cs:insideint
	push	ax
	push	cx
	push	dx
	push	si
	push	di
	push	bp
	push	es
	mov	cs:pleasedoit,0
	mov	al,20h
	out	20h,al
	call	dosomething
	pop	es
	pop	bp
	pop	di
	pop	si
	pop	dx
	pop	cx
	pop	ax
	dec	cs:insideint
	;
gnope:	pop	bx
	pop	ds
	jmp	cs:oldint7
	ENDP

intti9	PROC	FAR
	cmp	cs:insideint,0
	jne	nope9
	cmp	cs:pleasedoit,0
	je	nope9
	inc	cs:insideint
	push	ax
	push	bx
	push	cx
	push	dx
	push	si
	push	di	
	push	bp
	push	ds
	push	es
	mov	cs:pleasedoit,0
	call	dosomething
	pop	es
	pop	ds
	pop	bp
	pop	di
	pop	si
	pop	dx
	pop	cx
	pop	bx
	pop	ax
	dec	cs:insideint
nope9:	jmp	cs:oldint9
intti9	ENDP

intti	PROC	FAR ;FINDMEfindme
	cmp	cs:insidemenu,0
	je	notim1
	mov	cs:mouseb,bx
	mov	cs:mousegx,cx
	mov	cs:mousegy,dx
	shr	cx,1
	shr	cx,1
	shr	cx,1
	mov	cs:mousex,cx
	shr	dx,1
	shr	dx,1
	shr	dx,1
	mov	cs:mousey,dx
	ret
notim1:	cmp	cs:insideint,0
	je	nope10x
	ret
x2nope:	jmp	nope
nope10x: inc	cs:insideint
	push	ax
	push	bx
	push	cx
	push	dx
	push	si
	push	di
	push	bp
	push	ds
	push	es
	mov	al,20h
	out	20h,al
	test	bx,2
	jnz	x2nope ;MIDDLE must be released
	test	cs:lastbuttonstate,2
	jz	x2nope ;MIDDLE had to be pressed
	;middle now released, check for what was pressed
	mov	bx,cs:lastbuttonstate
	and	bx,1
	cmp	bx,0
	je	zsavelbm ;only MIDDLE pressed, <SAVELBM>
	cmp	bx,1+2
	je	zsavemem ;LEFT+RIGHT+MIDDLE pressed, <SAVEMEM>
	cmp	bx,2
	je	zrunmenu ;RIGHT+MIDDLE pressed, <RUNMENU>
	cmp	bx,1
	je	zscroll ;LEFT+MIDDLE pressed, <SCROLL>
	jmp	nope
zsavelbm: mov	cs:whattodo,1
	jmp	trytodoit
zsavemem: mov	cs:whattodo,2
	jmp	trytodoit
zrunmenu: mov	cs:whattodo,3
	jmp	trytodoit
zscroll: mov	cs:whattodo,4
	jmp	trytodoit
trytodoit:
	mov	cs:pleasedoit,1
nope:	pop	es
	pop	ds
	pop	bp
	pop	di
	pop	si
	pop	dx
	pop	cx
	pop	bx
	pop	ax
	or	cs:lastbuttonstate,bx
	test	bx,2
	jne	nope20 ;if MIDDLE button pressed, no call mask emulation
	mov	cs:lastbuttonstate,0
	and	ax,cs:callmask
	je	nope20
	call	cs:callpnt
nope20:	dec	cs:insideint
	ret
intti	ENDP

dosomething PROC NEAR
	cli
	mov	ax,0
	mov	es,ax
	mov	ax,es:[INTER3*4]
	mov	WORD PTR cs:[newint7+0],ax
	mov	ax,es:[INTER3*4+2]
	mov	WORD PTR cs:[newint7+2],ax
	mov	ax,OFFSET intti7
	mov	es:[INTER3*4],ax
	mov	es:[INTER3*4+2],cs
	sti	

	call	getpal
	sti
	mov	al,cs:whattodo
	mov	cs:whattodo,0
	mov	cs:lastbuttonstate,0
	cmp	al,1
	jne	dos1
	call	savelbm
	jmp	dos0
dos1:	cmp	al,2
	jne	dos2
	call	savemem
	jmp	dos0
dos2:	cmp	al,3
	jne	dos3
	call	runmenu
	jmp	dos0
dos3:	cmp	al,4
	jne	dos4
	call	scrollscrn
	jmp	dos0
dos4:	
dos0:	cli
	mov	ax,0
	mov	es,ax
	mov	ax,word ptr cs:[newint7+0]
	mov	dx,word ptr cs:[newint7+2]
	mov	es:[INTER3*4],ax 
	mov	es:[INTER3*4+2],dx
	sti
	ret
dosomething ENDP

memname db	'MEMORY.DAT',0
destpal	db	786 dup(0)
handle	dw	0
filename db	'PIC000.LBM',0

deflbm1	LABEL BYTE ;main header
	db	70,79,82,77, 0,0,0,0 ,73,76,66,77,66,77,72,68
			;    ^filelength-8
	db	0,0,0,20,1,64,0,200,0,0,0,0,8,0,1,0
	db	0,255,1,1,1,64,0,200,67,77,65,80,0,0,3,0
deflbm2	LABEL BYTE ;BODY header
	db	66,79,68,89, 0,0,0,0
			;    ^bodylength
	
savelbm PROC NEAR
	mov	al,1
	call	setpal
	
	;determine screen statistics
	;get screen width
	mov	dx,3d4h
	mov	al,13h
	out	dx,al
	inc	dx
	in	al,dx
	dec	dx
	mov	cl,3
	xor	ah,ah
	shl	ax,cl
	mov	cs:rowlength,ax
	;get split row
	mov	dx,3d4h
	mov	al,7
	out	dx,al
	inc	dx
	in	al,dx
	dec	dx
	mov	ah,al
	mov	al,18h
	out	dx,al
	inc	dx
	in	al,dx
	shr	ah,1
	shr	ah,1
	shr	ah,1
	shr	ah,1
	and	ah,1
	shr	ax,1
	mov	cs:splitrow,ax
	;get screen start
	mov	dx,3d4h
	mov	al,0ch
	out	dx,al
	inc	dx
	in	al,dx
	mov	ah,al
	dec	dx
	mov	al,0dh
	out	dx,al
	inc	dx
	in	al,dx
	dec	dx
	mov	cs:screenstart,ax
	;tweaked?
	mov	cs:tweaked,0
	mov	dx,3c4h
	mov	al,4
	out	dx,al
	inc	dx
	in	al,dx
	test	al,8
	jnz	notwe
	test	al,4
	jz	notwe
	shr	cs:rowlength,1
	shr	cs:rowlength,1
	mov	cs:tweaked,1
	jmp	istwe
notwe:	shl	cs:screenstart,1
	shl	cs:screenstart,1
	shl	cs:screenstart,1
	shl	cs:screenstart,1
istwe:	sti
	mov	cs:filename[3],'0'
	mov	cs:filename[4],'0'
	mov	cs:filename[5],'1'
	mov	ah,0fh
	int	10h
	cmp	al,19
	je	dispok
	jmp	sdispfail
dispok:	mov	ax,cs
	mov	ds,ax
sp2:	mov	ax,3d01h
	mov	dx,OFFSET filename
	int	21h
	jc	sp1 ;not exists
	mov	bx,ax
	mov	ah,3eh
	int	21h
	inc	cs:filename[5]
	cmp	cs:filename[5],'9'
	jna	spn1
	mov	cs:filename[5],'0'
	inc	cs:filename[4]
	cmp	cs:filename[4],'9'
	jna	spn1
	mov	cs:filename[4],'0'
	inc	cs:filename[3]
spn1:	jmp	sp2
sp1:	;create
	mov	ax,3c01h
	mov	cx,0
	mov	dx,OFFSET filename
	int	21h
	jc	spfail 
	;opened ok
	mov	cs:handle,ax

	call	savelbmdata	
	
	mov	bx,cs:handle
	mov	ah,3eh
	int	21h

	mov	al,0
	call	setpal
	ret
spfail:	mov	al,2
	call	errorpal
	ret	
sdispfail:
	mov	al,1
	call	errorpal
	ret
savelbm ENDP

getpal PROC	NEAR
	cli
	mov	ax,cs
	mov	ds,ax
	mov	dx,03dah
rws1:	in	al,dx
	test	al,8
	jz	rws1
rws2:	in	al,dx
	test	al,8
	jnz	rws2

	xor	ax,ax
	mov	dx,3c7h
	out	dx,al

	mov	si,OFFSET destpal
	mov	cx,768
	mov	dx,3c9h
rwag:	in	al,dx
	shl	al,1
	shl	al,1
	mov	ds:[si],al
	inc	si
	loop	rwag
	sti
	ret
getpal ENDP

waitborder PROC NEAR
	mov	dx,03dah
rrws1:	in	al,dx
	test	al,8
	jz	rrws1
rrws2:	in	al,dx
	test	al,8
	jnz	rrws2
	ret
waitborder ENDP

errorpal PROC NEAR
	cli
	mov	cs:apu,al
	mov	si,ax
	and	si,127
	shl	si,1
	shl	si,1
	add	si,OFFSET errorcols
	call	waitborder
	mov	ax,cs
	mov	ds,ax
	xor	al,al
	mov	dx,3c8h
	out	dx,al
	inc	dx
	mov	cx,256
epal1:	mov	al,ds:[si+1]
	out	dx,al
	mov	al,ds:[si+2]
	out	dx,al
	mov	al,ds:[si+3]
	out	dx,al
	loop	epal1
	test	cs:apu,128
	jz	epal3
	sti
	ret
epal3:	mov	cx,30
epal2:	call	waitborder
	loop	epal2
	mov	al,0
	call	setpal
	sti
	ret
errorpal ENDP

greypal PROC NEAR
	push	cx
	push	dx
	push	ax
	xor	al,al
	mov	dx,3c8h
	out	dx,al
	inc	dx
	pop	ax
	mov	cx,256*3
gpal1:	out	dx,al
	loop	gpal1
	pop	dx
	pop	cx
	ret
greypal ENDP

setpal PROC	NEAR
	cli
	mov	cs:apu,al
	mov	ax,cs
	mov	ds,ax
	call	waitborder

	xor	ax,ax
	mov	dx,3c8h
	out	dx,al

	mov	si,OFFSET destpal
	mov	cx,768
	mov	dx,3c9h
	mov	ah,cs:apu
rrwag:	mov	al,ds:[si]
	shr	al,1
	shr	al,1
	cmp	ah,0
	je	rrwag2
	xor	al,31 ;to create the flashing
rrwag2:	out	dx,al
	inc	si
	loop	rrwag
	sti
	ret
setpal ENDP

	ALIGN 2
prow	db	330 dup(0) ;packed row/screen row
row	db	8*40 dup(0) ;bitmapped row

datalen	dw	0,0
hilolen	db	0,0,0,0

getsirow PROC NEAR
	mov	bp,cs:screenstart
	cmp	si,cs:splitrow
	jb	gsr0
	sub	si,cs:splitrow
	xor	bp,bp
gsr0:	;row si => es:prow (†††)
	cmp	cs:tweaked,0
	jne	gsr1
	mov	ax,si
	mul	cs:rowlength
	mov	si,ax
	add	si,bp
	mov	di,OFFSET prow
	mov	cx,160
	rep	movsw
	ret
gsr1:	mov	di,OFFSET prow
	mov	ax,si
	mul	cs:rowlength
	mov	si,ax
	add	si,bp
	mov	dx,3ceh
	mov	al,4
	out	dx,al
	inc	dx
	in	al,dx
	push	ax
	mov	ax,0004h
gsr3:	mov	dx,3ceh
	out	dx,ax
	push	ax
	push	di
	push	si
	mov	cx,80
gsr2:	mov	al,ds:[si]
	mov	es:[di],al
	add	di,4
	inc	si
	loop	gsr2
	pop	si
	pop	di
	pop	ax
	inc	di
	inc	ah
	cmp	ah,4
	jb	gsr3
	mov	dx,3ceh
	mov	al,4
	out	dx,al
	inc	dx
	pop	ax
	out	dx,al
	ret
getsirow ENDP

savelbmdata PROC	NEAR
	mov	bx,cs:handle
	mov	ah,40h
	mov	dx,OFFSET deflbm1
	mov	cx,30h
	int	21h
	mov	bx,cs:handle
	mov	ah,40h
	mov	dx,OFFSET destpal
	mov	cx,768
	int	21h
	mov	bx,cs:handle
	mov	ah,40h
	mov	dx,OFFSET deflbm2
	mov	cx,8
	int	21h

	mov	ax,cs
	mov	es,ax
	mov	ax,0a000h
	mov	ds,ax
	mov	cx,200
	;mov	si,cs:screenstart
	xor	si,si
	mov	cs:datalen,si
	mov	cs:datalen[2],si
nextrow: push	cx
	push	si
	call	getsirow
	;bit prow to bits
	
	push	ds
	mov	ax,cs
	mov	ds,ax
	mov	di,OFFSET row+7*40
	mov	cx,8 ;8 bits
nr1:	push	cx
	push	di

	mov	si,OFFSET prow
	mov	cx,40	
rn2:	shl	byte ptr ds:[si],1
	rcl	ah,1
	shl	byte ptr ds:[si+1],1
	rcl	ah,1
	shl	byte ptr ds:[si+2],1
	rcl	ah,1
	shl	byte ptr ds:[si+3],1
	rcl	ah,1
	shl	byte ptr ds:[si+4],1
	rcl	ah,1
	shl	byte ptr ds:[si+5],1
	rcl	ah,1
	shl	byte ptr ds:[si+6],1
	rcl	ah,1
	shl	byte ptr ds:[si+7],1
	rcl	ah,1
	add	si,8
	mov	al,ah
	stosb
	loop	rn2
	
	pop	di
	pop	cx
	sub	di,40
	loop	nr1
	
	;pack row[] to prow[]
	mov	si,OFFSET row
	mov	di,OFFSET prow
	mov	cx,8
pr1:	push	si
	push	cx
	xor	bl,bl
	mov	dx,si
	xor	cl,cl ;40 bytes in a row
pr5:	mov	al,ds:[si]
	cmp	al,ds:[si+1]
	jne	pr2
	cmp	al,ds:[si+2]
	jne	pr2
	;three bytes same, more maybe?
	cmp	bl,0
	je	pr6
	;some nonsame stuff gathered, sort it out
	push	si
	push	cx
	mov	cl,bl
	xor	ch,ch
	mov	al,bl
	dec	al
	stosb
	mov	si,dx
	rep	movsb
	pop	cx
	pop	si
pr6:	xor	bl,bl
pr3:	dec	bl
	inc	si
	inc	cl
	mov	al,ds:[si]
	cmp	al,ds:[si+1]
	jne	pr4
	cmp	cl,39
	jl	pr3
pr4:	mov	al,bl
	stosb
	mov	al,ds:[si]
	stosb
pr11:	inc	si
	inc	cl
	xor	bl,bl
	mov	dx,si
	cmp	cl,38
	jl	pr5
	cmp	cl,38
	je	pr10
	cmp	cl,39
	je	pr9
	jmp	pr8
pr1x:	jmp	pr1
nextrowx: jmp	nextrow
	
pr2:	inc	cl
	inc	bl
	inc	si
	cmp	cl,38
	jl	pr5
pr10:	inc	bl
pr9:	inc	bl
pr8:	push	si
	push	cx
	mov	cl,bl
	xor	ch,ch
	jcxz	pr7
	mov	al,bl
	dec	al
	stosb
	mov	si,dx
	rep	movsb
pr7:	pop	cx
	pop	si
	
	pop	cx
	pop	si
	add	si,40
	loop	pr1x
	
	;save row to file
	sub	di,OFFSET prow
	mov	cx,di
	add	cs:datalen,cx
	adc	cs:datalen[2],0
	mov	bx,cs:handle
	mov	ah,40h
	mov	dx,OFFSET prow
	int	21h
	
	pop	ds
	pop	si
	pop	cx
	inc	si
	loop	nextrowx	;<<<<<<
	
	test	cs:datalen,1
	jz	sizeok
	mov	bx,cs:handle
	mov	ah,40h
	mov	dx,OFFSET prow 
	mov	cx,1
	add	cs:datalen,cx
	adc	cs:datalen[2],0
	int	21h ;add a byte
sizeok:	mov	ax,cs
	mov	ds,ax
	;write datalen
	mov	bx,ds:handle
	mov	ax,4200h
	mov	cx,0
	mov	dx,4+768+48
	int	21h
	mov	ax,ds:datalen[2]
	mov	ds:hilolen[0],ah
	mov	ds:hilolen[1],al
	mov	ax,ds:datalen[0]
	mov	ds:hilolen[2],ah
	mov	ds:hilolen[3],al
	mov	bx,ds:handle
	mov	ah,40h
	mov	dx,OFFSET hilolen
	mov	cx,4
	int	21h
	;write filelen
	mov	bx,ds:handle
	mov	ax,4200h
	mov	cx,0
	mov	dx,4
	int	21h
	add	ds:datalen,768+48
	adc	ds:datalen[2],0
	mov	ax,ds:datalen[2]
	mov	ds:hilolen[0],ah
	mov	ds:hilolen[1],al
	mov	ax,ds:datalen[0]
	mov	ds:hilolen[2],ah
	mov	ds:hilolen[3],al
	mov	bx,ds:handle
	mov	ah,40h
	mov	dx,OFFSET hilolen
	mov	cx,4
	int	21h
	ret
savelbmdata	ENDP

savemem PROC NEAR
	mov	al,3+128
	call	errorpal
	mov	dx,cs
	mov	ds,dx
	mov	dx,OFFSET memname
	mov	cx,32
	dosint	3ch
	jc	error
	mov	cs:[handle],ax

	mov	cx,20
	mov	bx,0

maga:	mov	ds,bx
	mov	dx,0
	push	cx
	push	bx
	mov	cx,32768
	mov	bx,cs:[handle]	
	dosint	40h
	cmp	ax,32768
	jne	error3
	jc	error3
	pop	bx
	pop	cx
	add	bx,2048
	mov	al,bh
	rol	al,1
	rol	al,1
	xor	al,bh
	call	greypal
	loop	maga
	
	mov	bx,cs:[handle]
	dosint	3eh
	jc	error

	jmp	iover

error3: pop	bx
	pop	cx
error:	mov	al,2
	call	errorpal
	ret

iover:	mov	al,0
	call	setpal
	ret
savemem ENDP

;************************************
;************************************
;		RUNMENU     
;************************************
;************************************

codseg	dw	0
exitmenu db	0
vram	dw	0b800h
rows	dw	25 dup(0)

keymode	dw	0
keymodes dw	OFFSET keyboard1,OFFSET keyboard2,OFFSET keyboard3,OFFSET keyboard3

strmaxlen equ	70 
strpnt dw	0
string	db	80 dup(0)

memseg	dw	0
mempos	dw	0 ;cursors position inside window
	
runmenu PROC NEAR
	sti
	mov	ax,cs
	mov	ds:codseg,ax
	mov	cs:insidemenu,1
	mov	cs:exitmenu,0
	call	initmenu
	call	redraw
	jmp	rm2
	;MAIN LOOP
rmaga:	mov	ax,cs:mousex
	mov	cs:omousex,ax
	mov	ax,cs:mousey
	mov	cs:omousey,ax
	mov	ax,cs:mouseb
	mov	cs:omouseb,ax
rm2:	call	drawmcur ;cursor on
	call	waitfor
	call	drawmcur ;cursor off
	call	checkkeyb
	call	checkmouse
	cmp	cs:exitmenu,1
	jne	rmaga
rm1:	call	waitrelease
	call	resetmenu
	mov	cs:insidemenu,0
	ret
runmenu ENDP

waitrelease PROC NEAR
wrm1:	cmp	cs:mouseb,0
	jne	wrm1 ;wait for button release
	ret
waitrelease ENDP

waitfor PROC NEAR
	mov	ax,cs:vram
	mov	es,ax
	mov	bx,cs:mouseb
	mov	cx,cs:mousex
	mov	dx,cs:mousey
wm1:	inc	byte ptr es:[156] ;we're alive
	test	keymode,2
	jz	wm3
	mov	ah,1
	int	16h
	jnz	wm2
wm3:	cmp	cs:mousex,cx
	jne	wm2
	cmp	cs:mousey,dx
	jne	wm2
	cmp	cs:mouseb,bx
	jne	wm2
	jmp	wm1
wm2:	inc	byte ptr es:[154] ;we're alive 2
	ret
waitfor ENDP

initmenu PROC NEAR
	call	storevideo
	call	initvideo
	mov	cx,25
	mov	bx,0
	mov	ax,0
im1:	mov	cs:rows[bx],ax
	add	bx,2
	add	ax,160
	loop	im1
	;set mouseparameters
	mov	ax,7
	mov	cx,0
	mov	dx,639
	int33
	mov	ax,8
	mov	cx,0
	mov	dx,199
	int33
	mov	ax,4
	mov	cx,324
	mov	dx,104
	int33
	mov	cx,320/8
	mov	dx,100/8
	mov	cs:mousex,cx
	mov	cs:mousey,dx
	mov	cs:omousex,cx
	mov	cs:omousey,dx
	mov	cs:mouseb,0
	mov	cs:omouseb,0
	ret
initmenu ENDP

resetmenu PROC NEAR
	call	restorevideo
	;restore mouseparameters
	mov	ax,7
	mov	cx,cs:mxmin
	mov	dx,cs:mxmax
	int33
	mov	ax,8
	mov	cx,cs:mymin
	mov	dx,cs:mymax
	int33
	ret
resetmenu ENDP

drawmcur PROC NEAR
	mov	ax,cs:vram
	mov	es,ax
	mov	di,cs:omousey
	shl	di,1
	mov	di,cs:rows[di]
	add	di,cs:omousex
	add	di,cs:omousex
	xor	byte ptr es:[di+1],255
	ret
drawmcur ENDP

oldvmode db	0
oldscrn	db	4000 dup(0)
oldcurpos dw	0
oldcurtype dw	0
oldregs	db	256 dup(0) ;stored VGA regs
old3c0 db	16 dup(0)

storevideo PROC NEAR
	mov	cx,16
	xor	bx,bx
sw3:	push	bx
	mov	dx,3dah
	in	al,dx
	mov	dx,3c0h
	mov	al,bl
	out	dx,al
	inc	dx
	in	al,dx
	mov	cs:old3c0[bx],al
	pop	bx
	inc	bx
	loop	sw3
	mov	ah,0fh
	int	10h
	mov	cs:oldvmode,al
	mov	ah,03h
	int	10h
	mov	cs:oldcurpos,dx
	mov	cs:oldcurtype,cx
	mov	ax,cs:vram
	mov	ds,ax
	mov	ax,cs
	mov	es,ax
	mov	di,OFFSET oldscrn
	mov	si,0
	cld
	mov	cx,4000/2
	rep	movsw
	mov	ax,cs
	mov	es,ax
	mov	di,OFFSET oldregs
	call	fromvgaregs
	ret
storevideo ENDP

restorevideo PROC NEAR
	mov	ax,cs:vram
	mov	es,ax
	mov	ax,cs
	mov	ds,ax
	mov	si,OFFSET oldscrn
	mov	di,0
	cld
	mov	cx,4000/2
	rep	movsw
	xor	ah,ah
	mov	ah,0
	mov	al,cs:oldvmode
	or	al,128
	int	10h
	mov	ax,cs
	mov	ds,ax
	mov	si,OFFSET oldregs
	call	tovgaregs
	mov	ah,1
	mov	cx,cs:oldcurtype
	int	10h
	mov	ah,2
	mov	dx,cs:oldcurpos
	int	10h
	mov	al,0
	call	setpal
	mov	cx,16
	xor	bx,bx
rw3:	push	bx
	mov	dx,3dah
	in	al,dx
	mov	dx,3c0h
	mov	al,bl
	out	dx,al
	mov	al,cs:old3c0[bx]
	out	dx,al
	pop	bx
	inc	bx
	loop	rw3
	mov	al,32
	out	dx,al
	ret
restorevideo ENDP

initvideo PROC NEAR
	mov	ax,3+128
	int	10h
	mov	dx,3dah
	in	al,dx
	mov	dx,3c0h
	mov	al,0
	mov	cx,16
iv1:	out	dx,al
	out	dx,al
	inc	al
	loop	iv1
	mov	al,32
	out	dx,al
	mov	dx,3c8h
	mov	al,0
	out	dx,al
	inc	dx
	mov	bx,0
	mov	cx,16*3
iv2:	mov	al,cs:defpal[bx]
	inc	bx
	out	dx,al
	loop	iv2
	
	mov	ax,cs:vram
	mov	es,ax
	cld
	mov	di,0
	mov	ax,0720h
	mov	cx,4000/2
	rep	stosw
	;no flash
	mov	dx,3dah
	in	al,dx
	mov	dx,3c0h
	mov	al,10h
	out	dx,al
	mov	al,0+0+4+0+0+0+0+0
	out	dx,al
	mov	al,32
	out	dx,al
	ret
initvideo ENDP

fromvgaregs PROC NEAR
	cld
	mov	cx,19h
	mov	ah,0
	mov	dx,3d4h
fvr1:	mov	al,ah
	out	dx,al
	inc	dx
	in	al,dx
	stosb
	dec	dx
	inc	ah
	loop	fvr1

	mov	cx,15h
	mov	ah,0
fvr2:	mov	dx,3dah
	in	al,dx
	mov	dx,3c0h
	mov	al,ah
	out	dx,al
	inc	dx
	in	al,dx
	dec	dx
	stosb
	inc	ah
	loop	fvr2
	mov	dx,3dah
	in	al,dx
	mov	dx,3c0h
	mov	al,32
	out	dx,al

	mov	cx,9h
	mov	ah,0
	mov	dx,3ceh
fvr3:	mov	al,ah
	out	dx,al
	inc	dx
	in	al,dx
	stosb
	dec	dx
	inc	ah
	loop	fvr3

	mov	cx,4h
	mov	ah,1 ;0/bit 1=syncr. clear (0=halt)
	mov	dx,3c4h
fvr4:	mov	al,ah
	out	dx,al
	inc	dx
	in	al,dx
	stosb
	dec	dx
	inc	ah
	loop	fvr4
	
	;Misc output 1: outp:3C2h inp:3CCh
	mov	dx,3cch
	in	al,dx
	stosb

	ret
fromvgaregs ENDP

tovgaregs PROC NEAR
	cld
	mov	dx,3c4h
	mov	ax,1*256+0
	out	dx,ax

	mov	dx,3d4h
	mov	al,11h
	mov	ah,ds:[si+11h]
	and	ah,127
	out	dx,ax

	mov	cx,19h
	mov	ah,0
	mov	dx,3d4h
tvr1:	mov	al,ah
	out	dx,al
	inc	dx
	lodsb
	out	dx,al
	dec	dx
	inc	ah
	loop	tvr1

	mov	cx,15h
	mov	ah,0
tvr2:	mov	dx,3dah
	in	al,dx
	mov	dx,3c0h
	mov	al,ah
	out	dx,al
	lodsb
	out	dx,al
	inc	ah
	loop	tvr2
	mov	dx,3dah
	in	al,dx
	mov	dx,3c0h
	mov	al,32
	out	dx,al

	mov	cx,9h
	mov	ah,0
	mov	dx,3ceh
tvr3:	mov	al,ah
	out	dx,al
	inc	dx
	lodsb
	out	dx,al
	dec	dx
	inc	ah
	loop	tvr3

	mov	cx,4h
	mov	ah,1 ;0/bit 1=syncr. clear (0=halt)
	mov	dx,3c4h
tvr4:	mov	al,ah
	out	dx,al
	inc	dx
	lodsb
	out	dx,al
	dec	dx
	inc	ah
	loop	tvr4
	
	;Misc output 1: outp:3C2h inp:3CCh
	lodsb
	mov	dx,3c2h
	out	dx,al

	mov	dx,3c4h
	mov	ax,3*256+0
	out	dx,ax
	ret
tovgaregs ENDP

printl	PROC	NEAR
	push	ax
	push	bx
	push	cx
	push	ds
	push	es
	push	di
	push	si
	push	cs
	pop	ds
	mov	es,cs:vram
	mov	di,bx
	and	di,255
	shl	di,1
	mov	bl,bh
	xor	bh,bh
	shl	bx,1
	add	di,cs:rows[bx]
	cld
	mov	ah,al
prl2:	lodsb
	stosw
	loop	prl2
prl1:	pop	si
	pop	di
	pop	es
	pop	ds
	pop	cx
	pop	bx
	pop	ax
	ret
printl	ENDP

print	PROC	NEAR
	push	ax
	push	bx
	push	ds
	push	es
	push	di
	push	si
	push	cs
	pop	ds
	mov	es,cs:vram
	mov	di,bx
	and	di,255
	shl	di,1
	mov	bl,bh
	xor	bh,bh
	shl	bx,1
	add	di,cs:rows[bx]
	cld
	mov	ah,al
prn2:	lodsb
	cmp	al,0
	je	prn1
	stosw
	jmp	prn2
prn1:	pop	si
	pop	di
	pop	es
	pop	ds
	pop	bx
	pop	ax
	ret
print	ENDP

print2	PROC	NEAR
	push	ax
	push	bx
	push	dx
	push	ds
	push	es
	push	di
	push	si
	push	cs
	pop	ds
	mov	dx,ax
	mov	es,cs:vram
	mov	di,bx
	and	di,255
	shl	di,1
	mov	bl,bh
	xor	bh,bh
	shl	bx,1
	add	di,cs:rows[bx]
	cld
prn22:	mov	ah,dh
	lodsb
	cmp	al,0
	je	prn21
	jl	prn23
	mov	ah,dl
prn23:	stosw
	jmp	prn22
prn21:	pop	si
	pop	di
	pop	es
	pop	ds
	pop	dx
	pop	bx
	pop	ax
	ret
print2	ENDP

print3	PROC	NEAR
	push	ax
	push	bx
	push	dx
	push	ds
	push	es
	push	di
	push	si
	push	cs
	pop	ds
	mov	dx,ax
	mov	es,cs:vram
	mov	di,bx
	and	di,255
	shl	di,1
	mov	bl,bh
	xor	bh,bh
	shl	bx,1
	add	di,cs:rows[bx]
	cld
prnr22:	mov	ah,dl
	lodsb
	cmp	al,0
	je	prnr21
	cmp	al,32
	jne	prnr23
	mov	ah,dh
prnr23:	stosw
	jmp	prnr22
prnr21:	pop	si
	pop	di
	pop	es
	pop	ds
	pop	dx
	pop	bx
	pop	ax
	ret
print3	ENDP

printhex16 PROC NEAR
	push	ax
	mov	al,ah
	call	printhex
	pop	ax
	call	printhex
	ret
printhex16 ENDP

hextxt	db	'0123456789ABCDEFabcdef'
hexnum	db	0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,10,11,12,13,14,15

printhex PROC NEAR
	push	ax
	push	cx
	push	dx
	push	es
	mov	es,cs:vram
	push	ax
	mov	cl,4
	shr	al,cl
	mov	ah,dl
	mov	bl,al
	xor	bh,bh
	mov	al,cs:hextxt[bx]
	stosw
	pop	ax
	and	al,15
	mov	ah,dl
	mov	bl,al
	xor	bh,bh
	mov	al,cs:hextxt[bx]
	stosw
	pop	es
	pop	dx
	pop	cx
	pop	ax
	ret
printhex ENDP

getmousepnt PROC NEAR
	;returns ES:DI to mouse position
	mov	es,cs:vram
	mov	di,cs:mousey
	shl	di,1
	mov	di,cs:rows[di]
	add	di,cs:mousex
	add	di,cs:mousex
	ret
getmousepnt ENDP

		;0         1         2         3         4         5         6         7         
		;01234567890123456789012345678901234567890123456789012345678901234567890123456789
titlebar db	" Future-Multi-Grabber-Menu V1.0   Copyright (C) 1991 ÀÅÙ/FC        I'm alive:   ",0
hexdump1 LABEL BYTE
	db	"ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß",0
hexdump2 LABEL BYTE
	db	"                                                                           Ú",24,24,"¿ "
	db	"                                                                           ÀÄÄÙ "
	db	"                                                                           ÚGO¿ "
	db	"                                                                           ÀTOÙ "
	db	"                                                                           ÚFI¿ "
	db	"                                                                           ÀNDÙ "
	db	"                                                                           ÚÄÄ¿ "
	db	"                                                                           À",25,25,"Ù "
	db	0
keyboard0 LABEL BYTE
	db	"ÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜ",0
editstr LABEL BYTE
	db	" String:                                                                        ",0
keyboard1 LABEL BYTE
	db	" ÚÄÄÄÂÄÄÄÂÄÄÄÂÄÄÄ¿ÚÄÄÄÂÄÄÄÂÄÄÄÂÄÄÄÂÄÄÄÂÄÄÄÂÄÄÄÂÄÄÄÂÄÄÄÂÄÄÄ¿    ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿ "
	db	" ³ 7 ³ 8 ³ 9 ³ 0 ³³ Q ³ W ³ E ³ R ³ T ³ Y ³ U ³ I ³ O ³ P ³    ³*1 Exit FMGM  ³ "
	db	" ÃÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄ´ÀÂÄÄÁÂÄÄÁÂÄÄÁÂÄÄÁÂÄÄÁÂÄÄÁÂÄÄÁÂÄÄÁÂÄÄÁÂÂÄÁÄ¿  ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´ "
	db	" ³ 4 ³ 5 ³ 6 ³<BS³ ³ A ³ S ³ D ³ F ³ G ³ H ³ J ³ K ³ L ³³CLR³  ³*2 Keys ON/OFF³ "
	db	" ÃÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄ´ ÀÂÄÄÁÂÄÄÁÂÄÄÁÂÄÄÁÂÄÄÁÂÄÄÁÂÄÄÁÂÂÄÁÄÄÄ´ÃÄÄÄ´  ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´ "
	db	" ³ 1 ³ 2 ³ 3 ³###³  ³ Z ³ X ³ C ³ V ³ B ³ N ³ M ³³SHIFT³³   ³  ³*3 GraphInfo  ³ "
	db	" ÀÄÄÄÁÄÄÄÁÄÄÄÁÄÄÄÙ  ÀÄÄÄÁÄÄÄÁÄÄÄÁÄÄÄÁÄÄÄÁÄÄÄÁÄÄÄÙÀÄÄÄÄÄÙÀÄÄÄÙ  ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ "
	db	0
keyboard2 LABEL BYTE
	db	" ÚÄÄÄÂÄÄÄÂÄÄÄÂÄÄÄ¿ÚÄÄÄÂÄÄÄÂÄÄÄÂÄÄÄÂÄÄÄÂÄÄÄÂÄÄÄÂÄÄÄÂÄÄÄÂÄÄÄ¿    ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿ "
	db	" ³ 7 ³ 8 ³ 9 ³ 0 ³³ q ³ w ³ e ³ r ³ t ³ y ³ u ³ i ³ o ³ p ³    ³*1 Exit FMGM  ³ "
	db	" ÃÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄ´ÀÂÄÄÁÂÄÄÁÂÄÄÁÂÄÄÁÂÄÄÁÂÄÄÁÂÄÄÁÂÄÄÁÂÄÄÁÂÂÄÁÄ¿  ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´ "
	db	" ³ 4 ³ 5 ³ 6 ³<BS³ ³ a ³ s ³ d ³ f ³ g ³ h ³ j ³ k ³ l ³³CLR³  ³*2 Keys ON/OFF³ "
	db	" ÃÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄ´ ÀÂÄÄÁÂÄÄÁÂÄÄÁÂÄÄÁÂÄÄÁÂÄÄÁÂÄÄÁÂÂÄÁÄÄÄ´ÃÄÄÄ´  ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´ "
	db	" ³ 1 ³ 2 ³ 3 ³###³  ³ z ³ x ³ c ³ v ³ b ³ n ³ m ³³SHIFT³³   ³  ³*3 GraphInfo  ³ "
	db	" ÀÄÄÄÁÄÄÄÁÄÄÄÁÄÄÄÙ  ÀÄÄÄÁÄÄÄÁÄÄÄÁÄÄÄÁÄÄÄÁÄÄÄÁÄÄÄÙÀÄÄÄÄÄÙÀÄÄÄÙ  ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ "
	db	0
keyboard3 LABEL BYTE
	db	"                                                               ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿ "
	db	"  Computer's external keyboard active. Just press the keys     ³*1 Exit FMGM  ³ "
	db	"  to write (weird eh?). If the program interrupted now         ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´ "
	db	"  crashes, it was your fault, not mine! Got it? Good!          ³*2 Keys ON/OFF³ "
	db	"  It's also possible that the machine doesn't hang             ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´ "
	db	"  but the keyboard won't work. But then use the mouseone!      ³*3 GraphInfo  ³ "
	db	"                                                               ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ "
	db	0
	
string2hex PROC NEAR
	mov	si,0
	mov	bx,0
	mov	cx,4
	
s2h1:	push	bx
	push	cx
	mov	al,cs:string[bx]
	cmp	al,0
	jne	s2h4
	pop	cx
	pop	bx
	jmp	s2h5
s2h4:	shl	si,1
	shl	si,1
	shl	si,1
	shl	si,1
	xor	bx,bx
	mov	cx,21
s2h3:	cmp	al,cs:hextxt[bx]
	je	s2h2
	inc	bx
	loop	s2h3
s2h2:	mov	bl,cs:hexnum[bx]
	add	si,bx
	pop	cx
	pop	bx
	inc	bx
	loop	s2h1
	
s2h5:	mov	ax,si
	ret
string2hex ENDP

redrawstr PROC NEAR
	mov	si,OFFSET string
	mov	bx,(25-8)*256+9
	mov	al,07h
	mov	cx,70
	call	printl
	mov	ah,2
	mov	dx,(25-8)*256+9
	mov	bx,0
	add	dx,cs:strpnt
	int	10h
	ret	
redrawstr ENDP
		
redraw	PROC	NEAR
	mov	si,OFFSET titlebar
	mov	bx,0*256+0
	mov	al,4eh
	call	print
	mov	si,OFFSET editstr
	mov	bx,(25-8)*256+0
	mov	al,80h
	call	print
	call	redrawstr
	mov	si,OFFSET keyboard0
	mov	bx,(25-9)*256+0
	mov	al,08h
	call	print
	mov	si,OFFSET hexdump1
	mov	bx,7*256+0
	mov	al,08h
	call	print
	mov	si,OFFSET hexdump2
	mov	bx,8*256+0
	mov	ax,87h
	call	print3
	call	redhexdump
redkeyboardentry:
	mov	bx,cs:keymode
	shl	bx,1
	mov	si,cs:keymodes[bx]
	mov	bx,(25-7)*256+0
	mov	ax,80h*256+87h
	call	print2
	ret
redraw	ENDP

redhexdump PROC NEAR
	mov	es,cs:vram
	mov	cx,8
	mov	di,8*160
	mov	ax,cs:memseg
	mov	bp,0
rhd2:	push	ax
	push	di
	push	cx
	push	bp
	mov	ds,ax
	mov	dl,02h
	call	printhex16
	mov	ax,'0'+02h*256
	stosw
	mov	ax,':'+02h*256
	stosw
	mov	ax,' '+02h*256
	stosw
	mov	si,0
	mov	cx,8
rhd1:	mov	dl,03h
	cmp	bp,cs:mempos
	jne	rhd11
	mov	dl,1fh
rhd11:	lodsb
	call	printhex	
	mov	ax,' '+02h
	stosw
	inc	bp
	loop	rhd1
	mov	ax,' '+02h
	stosw
	mov	cx,8
rhd4:	mov	dl,03h
	cmp	bp,cs:mempos
	jne	rhd41
	mov	dl,83h
rhd41:	lodsb
	call	printhex	
	mov	ax,' '+02h
	stosw
	inc	bp
	loop	rhd4
	mov	ax,' '+02h
	stosw
	sub	bp,16
	xor	si,si
	mov	cx,16
	mov	ah,02h
rhd3:	mov	ah,02h
	cmp	bp,cs:mempos
	jne	rhd31
	mov	ah,1fh
rhd31:	lodsb
	stosw
	inc	bp
	loop	rhd3
	pop	bp
	pop	cx
	pop	di
	pop	ax
	inc	ax
	add	bp,16
	add	di,160
	loop	rhd2
	ret
redhexdump ENDP

subkey	dw	0
subkeyc	dw	-1

mousekeys PROC	NEAR
	call	getmousepnt
	mov	cx,80
mke12:	cmp	byte ptr es:[di],128
	ja	mke1
	sub	di,2
	loop	mke12
mke1:	add	di,2
	mov	al,es:[di]
	mov	ah,es:[di+2]
keyentry:
	cmp	al,' '
	jne	mke2
	cmp	cs:subkeyc,-1
	je	mke31
	mov	bl,ah
	xor	bh,bh
	sub	bx,'0'
	mov	ax,cs:subkey
	add	ax,bx
	dec	cs:subkeyc
	cmp	cs:subkeyc,-1
	je	mke20y
	mov	cx,10
	mul	cx
	mov	cs:subkey,ax
mke20x:	jmp	mke20
mke20y:	mov	ah,al
mke31:	mov	bx,cs:strpnt
	mov	cs:string[bx],ah
	inc	bx
	cmp	bx,strmaxlen
	jl	mke3
	mov	bx,strmaxlen-1
mke3:	mov	cs:strpnt,bx	
	call	redrawstr
	ret
mke2:	;special key
	cmp	al,'<'
	jne	mke21
	;<BS
	mov	bx,cs:strpnt
	cmp	bx,0
	je	mke20x
	dec	bx
	mov	cs:string[bx],0
	mov	cs:strpnt,bx
	jmp	mke20
mke21:	cmp	al,'*'
	jne	mke22
	cmp	ah,'3'
	jne	mke22a
	call	graphinfo
	jmp	mke20
mke22a:	cmp	ah,'1'
	jne	mke221
	mov	cs:exitmenu,1
	jmp	mke20
mke221:	cmp	ah,'2'
	jne	mke222
	xor	cs:keymode,2
	test	cs:keymode,2
	jz	mke2221
mke2222: mov	ah,1
	int	16h
	jz	mke2221
	mov	ah,0
	int	16h
	jmp	mke2222
mke2221: call	redkeyboardentry
	jmp	mke20
mke222:	cmp	ah,'3'
	jne	mke20
	
	jmp	mke20
mke22:	cmp	al,'#'
	jne	mke23
	mov	cs:subkey,0
	mov	cs:subkeyc,2
	jmp	mke20
mke23:	cmp	al,'S'
	jne	mke24
	xor	cs:keymode,1
	call	redkeyboardentry
	jmp	mke20
mke24:	cmp	al,'C'
	jne	mke25
	mov	cs:strpnt,0
	mov	cx,strmaxlen
	mov	bx,0
mke241:	mov	cs:string[bx],0
	inc	bx
	loop	mke241
	jmp	mke20
mke25:
mke20:	call	redrawstr
	ret
mousekeys ENDP

checkbox MACRO x1,y1,x2,y2
	local	l1,l2
	cmp	cs:mousex,x1
	jb	l1
	cmp	cs:mousex,x2
	ja	l1
	cmp	cs:mousey,y1
	jb	l1
	cmp	cs:mousey,y2
	ja	l1
	stc
	jmp	l2
l1:	clc
l2:	ENDM

checkmouse PROC NEAR
	cmp	cs:mouseb,1
	je	chm2x
	jmp	chm2
chm2x:	;**LEFT button pressed
	cmp	cs:mousey,25-7
	jb	chm1
	call	mousekeys
	call	waitrelease
	jmp	chm0
chm1:	;not keyboard
	checkbox 76,8,78,9
	jnc	chm31
	dec	cs:memseg
	call	redhexdump
	jmp	chm0
chm31:	checkbox 76,10,78,11
	jnc	chm32
	call	string2hex
	mov	cs:memseg,ax
	call	redhexdump
	jmp	chm0
chm32:	checkbox 76,12,78,13
	jnc	chm33
	call	findstring
	jmp	chm0
chm33:	checkbox 76,14,78,15
	jnc	chm34
	inc	cs:memseg
	call	redhexdump
	jmp	chm0
chm34:
	jmp	chm0

chm2:	cmp	cs:mouseb,2
	jne	chm5

	checkbox 76,8,78,9
	jnc	chm41
	sub	cs:memseg,8
	call	redhexdump
	jmp	chm0
chm41:	checkbox 76,14,78,15
	jnc	chm42
	add	cs:memseg,8
	call	redhexdump
	jmp	chm0
chm42:

chm5:
chm0:	ret
checkmouse ENDP

checkkeyb PROC NEAR
	test	cs:keymode,2
	jnz	ckb2
	ret
ckb2:	mov	ah,1
	int	16h
	jz	ckb1
	mov	ah,0
	int	16h
	cmp	al,27
	jne	ckb21
	mov	ax,'*'+'1'*256
	jmp	keyentry
ckb21:	cmp	al,8
	jne	ckb22
	mov	ax,'<'+'B'*256
	jmp	keyentry
ckb22:	cmp	al,13
	jne	ckb23
	call	string2hex
	mov	cs:memseg,ax
	call	redhexdump
	mov	ax,'C'+'L'*256
	jmp	keyentry
ckb23:	cmp	al,10
	jne	ckb25
	call	findstring
 	ret
ckb25:	cmp	al,0
	jne	ckb24
	cmp	ah,83
	jne	ckb241
	mov	ax,'C'+'L'*256
	jmp	keyentry
ckb241:
ckb24:	mov	ah,al
	mov	al,' '
	jmp	keyentry
ckb1:	ret
checkkeyb ENDP

findstring PROC NEAR
	cmp	cs:strpnt,0
	jne	fst4
	mov	ax,cs:memseg
	mov	dx,cs:mempos
	ret
fst4:	cld
	mov	es,cs:vram
	mov	di,8*160
	mov	cx,8*160/2
	mov	ax,0720h
	rep	stosw
	mov	ax,cs
	mov	es,ax
	mov	cx,65535
	mov	ax,cs:memseg
	inc	ax
fst1:	push	cx
	push	ax
	mov	ds,ax
	xor	si,si
	mov	cx,16
fst2:	push	cx
	push	si
	mov	di,OFFSET string
	mov	cx,cs:strpnt
	repe	cmpsb
	jnz	fst3
	pop	si
	pop	cx
	pop	ax
	pop	cx
	mov	dx,si
	mov	cs:memseg,ax
	mov	cs:mempos,dx
	call	redraw
	ret
fst3:	pop	si
	pop	cx
	inc	si
	loop	fst2
	pop	ax
	pop	cx
	inc	ax
	loop	fst1
	call	redraw
	ret
findstring ENDP

drawbox PROC NEAR
	push	di
	push	cx
	push	ax
	mov	ah,al
	mov	cx,8
drb1:	mov	es:[di+0],ax
	mov	es:[di+2],ax
	mov	es:[di+4],ax
	mov	es:[di+6],ax
	add	di,320
	loop	drb1
	pop	ax
	pop	cx
	pop	di
	ret
drawbox ENDP

graphinfo PROC FAR
	mov	ax,13h ;320x200x256
	int	10h
	;draw palette boxes
	mov	ax,0a000h
	mov	es,ax
	mov	cx,16
	xor	ax,ax
	mov	di,321
gi1:	push	cx
	mov	cx,16
gi2:	call	drawbox
	add	di,9
	inc	al
	inc	ah
	loop	gi2
	add	di,320-9*16+8*320
	pop	cx
	loop	gi1
	;draw 3c0 palette boxes
	mov	di,620
	mov	cx,16
	xor	bx,bx
gi3:	push	bx
	mov	al,cs:old3c0[bx]
	call	drawbox
	add	di,9*320
	pop	bx
	inc	bx
	loop	gi3
	;set palette
	mov	al,0
	call	setpal
	;wait
	call	waitmouseb
	call	initvideo
	call	redraw
	ret
graphinfo ENDP

waitmouseb PROC NEAR
waimb1:	cmp	cs:mouseb,0
	jne	waimb1
waimb2:	cmp	cs:mouseb,0
	je	waimb2
	ret
waitmouseb ENDP

scrollscrn PROC NEAR
	mov	cs:insidemenu,1
	sti
	mov	ax,4
	mov	cx,0
	mov	dx,0
	int33
	mov	dx,3dah
	in	al,dx
	mov	dx,3c0h
	mov	al,11h+32
	out	dx,al
	mov	al,1
	out	dx,al
ss1:	cmp	cs:mouseb,0
	jne	ss1
ss2:	;main loop
	call	waitborder
	mov	ax,cs:mousegy
	mov	cx,320
	mul	cx
	add	ax,cs:mousegx
	mov	bx,ax
	mov	dx,3d4h
	mov	al,0ch
	mov	ah,bh
	out	dx,ax
	mov	al,0dh
	mov	ah,bl
	out	dx,ax
	cmp	cs:mouseb,0
	je	ss2
	mov	dx,3dah
	in	al,dx
	mov	dx,3c0h
	mov	al,11h+32
	out	dx,al
	mov	al,0
	out	dx,al
	mov	cs:insidemenu,0
	ret
scrollscrn ENDP

code	ENDS
	END


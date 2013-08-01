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
	db	"ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป",13,10
	db	"บ   Future-Ansi-Grabber V1.0      Copyright (C) 1992 ภลู / The Future Crew    บ",13,10
	db	"วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ",13,10
	db	"บ Press the mouse RIGHT-button to grab a TheDraw BIN (PIC???.BIM).            บ",13,10
	db	"วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ",13,10
	db	"บ In error conditions the screen (border) color is flashed:                   บ",13,10
	db	"บ BLUE: Disk(full) error  GREEN: Invalid videomode  RED: Unknown problems :-) บ",13,10
	db	"ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ",13,10
	db	'$'
txt2	db	13,10,'Future-Ansi-Grabber already installed!',13,10,'$'
txt3	db	13,10,'MouseDriver not found!',13,10,'$'
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
zsavemem: mov	cs:whattodo,1
	jmp	trytodoit
zrunmenu: mov	cs:whattodo,1
	jmp	trytodoit
zscroll: mov	cs:whattodo,1
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
	;call	runmenu
	jmp	dos0
dos3:	cmp	al,4
	jne	dos4
	;call	scrollscrn
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
filename db	'PIC000.BIN',0

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
	
	sti
	mov	cs:filename[3],'0'
	mov	cs:filename[4],'0'
	mov	cs:filename[5],'1'
	mov	ah,0fh
	int	10h
	cmp	al,4
	jb	dispok
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
gsr0:	;row si => es:prow ()
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
	mov	ax,cs
	mov	ds,ax
	mov	ax,0b800h
	mov	es,ax
	xor	si,si
	mov	cx,25
slb2:	push	cx
	
	mov	cx,80
	xor	bx,bx
slb1:	mov	ax,es:[si]
	mov	word ptr ds:prow[bx],ax
	add	si,2
	add	bx,2
	loop	slb1
	
	;save row to file
	mov	bx,cs:handle
	mov	ah,40h
	mov	dx,OFFSET prow
	mov	cx,160
	int	21h

	pop	cx
	loop	slb2
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

code	ENDS
	END


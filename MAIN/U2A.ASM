;** ùUùNùRùEùAùLù2ù  Copyright (C) 1993 The Future Crew
;**------------------------------------------------------
;**  Assembler utilities. Most of the useful stuff,
;**  called from the main routine. This file is included
;**  to U2.ASM

;±±±±±±±±±±±±±±±± Dos macros ±±±±±±±±±±±±±±±±

DOS_OPEN equ	3dh
DOS_CLOSE equ	3eh
DOS_READ equ	3fh
DOS_ALLOC equ	48h
DOS_FREE equ	49h
DOS_REALLOC equ	4ah

dosint	MACRO	function
	mov	ah,function
	int	21h
	ENDM
	
fatalerror PROC NEAR
	mov	cs:errorcode,8
	mov	cs:notextmode,0
	mov	ax,3
	int	10h
	jmp	fatalexit00
	push	ax
	push	bx
	push	si
	push	di
	push	ds
	push	es
	mov	si,cx
	mov	ds,dx
	mov	bx,ax
	add	bl,'0'
	mov	ax,1
	int	10h
	mov	ax,0b800h
	mov	es,ax
	test	bx,100h
	jz	@@2
	mov	di,0
	mov	cx,80
	mov	ah,4fh
@@4:	mov	al,ds:[si]
	or	al,al
	jz	@@2
	inc	si
	mov	es:[di],ax
	add	di,2
	loop	@@4
@@2:	mov	di,160
	mov	cx,2000/8
	mov	ah,4eh
@@1:	mov	al,'E'
	mov	es:[di+0],ax
	mov	al,'R'
	mov	es:[di+2],ax
	mov	al,'R'
	mov	es:[di+4],ax
	mov	al,bl
	mov	es:[di+6],ax
	add	di,8
	loop	@@1
	mov	ah,0
	int	16h
	mov	ax,13h
	int	10h
	pop	es
	pop	ds
	pop	di
	pop	si
	pop	bx
	pop	ax
	ret
fatalerror ENDP
	
;±±±±±±±±±±±±±±±± Memory management ±±±±±±±±±±±±±±±±

;°°°°°°°°°°°°°°°° getmem °°°°°°°°°°°°°°°°
;entry:	BX=paras
; exit: AX=segment
;saves: SEGS/SI/DI/BP
;descr: allocates memory
getmem	PROC NEAR
	dosint	DOS_ALLOC
	jc	@@err
	;ax=seg
	ret
@@err:	mov	ax,1
	call	fatalerror
	xor	ax,ax
	ret
getmem	ENDP

;°°°°°°°°°°°°°°°° getmemall °°°°°°°°°°°°°°°°
;entry:	-
; exit: AX=segment,BX=size
;saves: SEGS/SI/DI/BP
;descr: allocates biggest free block ofmemory
getmemall PROC NEAR
	mov	bx,0ffffh
	dosint	DOS_ALLOC
	push	bx
	dosint	DOS_ALLOC
	pop	bx
	jc	@@err
	;ax=seg
	ret
@@err:	mov	ax,2
	call	fatalerror
	xor	ax,ax
	ret
getmemall ENDP

;°°°°°°°°°°°°°°°° getmemup °°°°°°°°°°°°°°°°
;entry:	BX=paras
; exit: AX=segment
;saves: SEGS/SI/DI/BP
;descr: allocates memory from the top of the memory pool
getmemup PROC NEAR
	push	es
	mov	dx,bx
	mov	bx,0ffffh
	dosint	DOS_ALLOC
	sub	bx,dx
	dec	bx
	dosint	DOS_ALLOC
	mov	bx,dx
	mov	dx,ax
	dosint	DOS_ALLOC
	jc	@@err
	mov	es,dx
	mov	dx,ax
	dosint	DOS_FREE
	mov	ax,dx ;ax=seg
	pop	es
	ret
@@err:	pop	es
	mov	ax,3
	call	fatalerror
	xor	ax,ax
	ret
getmemup ENDP

;°°°°°°°°°°°°°°°° freemem °°°°°°°°°°°°°°°°
;entry:	AX=segment
; exit: -
;saves: SEGS/SI/DI/BP
;descr: frees memory
freemem	PROC NEAR
	mov	es,ax
	dosint	DOS_FREE
	jc	@@err
	;ax=seg
	ret
@@err:	mov	ax,4
	call	fatalerror
	xor	ax,ax
	ret
freemem	ENDP

;°°°°°°°°°°°°°°°° regetmem °°°°°°°°°°°°°°°°
;entry:	AX=segment,BX=paras
; exit: -
;saves: SEGS/SI/DI/BP
;descr: resets memory block size
regetmem PROC NEAR
	mov	es,ax
	dosint	DOS_REALLOC
	jc	@@err
	;ax=seg
	ret
@@err:	mov	ax,5
	call	fatalerror
	xor	ax,ax
	ret
regetmem ENDP

;±±±±±±±±±±±±±±±± DATAloader: file routines ±±±±±±±±±±±±±±±±

;°°°°°°°°°°°°°°°° openfile °°°°°°°°°°°°°°°°
;entry:	DS:SI=file name
; exit: AX=file handle (-1=error)
;saves: SEGS/SI/DI/BP
;descr: opens the specified file, wherever it is. 
openfile PROC NEAR
	mov	dx,si
	mov	al,0 ;read
	dosint	DOS_OPEN
	jc	@@1
	ret
@@1:	;error
	mov	ax,-1
	ret
openfile ENDP

;°°°°°°°°°°°°°°°° closefile °°°°°°°°°°°°°°°°
;entry:	AX=file id
; exit: -
;saves: SEGS/SI/DI/BP
;descr: closes the selected file.
closefile PROC NEAR
	mov	bx,ax
	dosint	DOS_CLOSE
	ret
closefile ENDP

;°°°°°°°°°°°°°°°° readfile °°°°°°°°°°°°°°°°
;entry:	AX=file id, CX=count, ES:DI=destination
; exit: -
;saves: SEGS/SI/DI/BP
;descr: reads data.
readfile PROC NEAR
	push	si
	push	ds
	mov	bx,ax
	mov	ax,es
	mov	ds,ax
	mov	dx,di
	dosint	DOS_READ
	pop	ds
	pop	si
	ret
readfile ENDP

;±±±±±±±±±±±±±±±± EXEloader: loadexe/runexe ±±±±±±±±±±±±±±±±

ALIGN 2
exeldr_pspseg	dw	0 ;loaded programs psp [USED BY DISINT.ASM FOR SETPID]
exeldr_ownpsp	dw	0 ;exeloaders psp      [USED BY DISINT.ASM FOR SETPID]
exeldr_oktorun	dw	0
exeldr_handle	dw	0
exeldr_pspsi	dw	0
exeldr_base	dw	0 ;codewise segment 0
exeldr_hdrseg	dw	0
exeldr_exelen	dw	0 ;in paras
exeldr_highflag	dw	0 ;1=load high as possible
exeldr_ownss	dw	0
exeldr_ownsp	dw	0
exeldr_cs	dw	0
exeldr_ip	dw	0
exeldr_ss	dw	0
exeldr_sp	dw	0
exeldr_goodexe	dw	0

;°°°°°°°°°°°°°°°° loadexe °°°°°°°°°°°°°°°°
;entry:	AX=file handle pointing to exefile
; exit: (exeldr_* variables)
;saves: none
;descr: loads the specified exe file to memory, does relocations
;       and creates psp. Also sets exeldr_* variables for 
;       subsequent call to runexe. NOTE, that at worst, all memory
;	will be reserved after this call for the just loaded exe.
;	NOTE: There are two entry points:
;	loadexe - loads exe normally
;	loadexehigh - loads exe as high in memory as minparasneeded
;                     allows (empty space left BEFORE exe)
loadexeboth PROC NEAR
loadexehigh: ;**ENTRY**
	mov	dx,1
	jmp	@@cont
loadexe: ;**ENTRY**
	xor	dx,dx
@@cont:	mov	cs:exeldr_highflag,dx
	mov	cs:exeldr_oktorun,0
	mov	cs:exeldr_handle,ax
	
	mov	ah,51h
	int	21h ;get current PSP
	mov	cs:exeldr_ownpsp,bx
	
	mov	bx,32/16
	call	getmem
	mov	es,ax
	xor	di,di
	mov	cx,32
	mov	ax,cs:exeldr_handle
	call	readfile
	mov	bx,es:[8h] ;size in paras
	call	getmemup
	mov	cs:exeldr_hdrseg,ax
	mov	cx,32
	mov	ax,es
	mov	ds,ax
	xor	si,si
	mov	es,cs:exeldr_hdrseg
	xor	di,di
	call	memcpy
	mov	ax,ds
	call	freemem
	;load rest of header
	mov	es,cs:exeldr_hdrseg
	mov	cx,es:[8h]
	shl	cx,4
	sub	cx,32
	cmp	cx,0
	jl	@@6
	mov	es,cs:exeldr_hdrseg
	mov	di,32
	mov	ax,cs:exeldr_handle
	call	readfile
@@6:	
	;header now in es:[], no other memory reserved
	mov	ax,es:[0]
	mov	cs:exeldr_goodexe,1
	cmp	ax,0faebh
	je	@@goodexe
	mov	cs:exeldr_goodexe,0
	cmp	ax,5a4dh
	jne	@@badexe
@@goodexe:
	mov	ax,es:[10h]
	mov	cs:exeldr_sp,ax
	mov	ax,es:[0eh]
	mov	cs:exeldr_ss,ax
	mov	ax,es:[14h]
	mov	cs:exeldr_ip,ax
	mov	ax,es:[16h]
	mov	cs:exeldr_cs,ax
	mov	ax,es:[04h]
	shl	ax,5
	mov	cs:exeldr_exelen,ax
	;allocate psp & segment for exe
	cmp	cs:exeldr_highflag,0
	je	@@norm
	mov	bx,es:[0ah] ;minimum paras needed
	add	bx,cs:exeldr_exelen
	add	bx,dx
	push	bx
	call	getmemup
	pop	bx
	jmp	@@con2
@@norm:	call	getmemall ;bx=size of seg allocated
@@con2:	mov	cs:exeldr_pspseg,ax
	add	ax,10h
	mov	cs:exeldr_base,ax
	;create new psp
	mov	dx,cs:exeldr_pspseg
	mov	si,es:[0ch] ;maximum paras needed
	add	si,cs:exeldr_exelen
	jc	@@1b
	cmp	si,bx
	jb	@@1
@@1b:	mov	si,bx
@@1:	mov	ax,es:[0ah] ;minimum paras needed
	add	ax,cs:exeldr_exelen
	cmp	si,ax
	jb	@@badexe2
	add	si,dx
	mov	cs:exeldr_pspsi,si
	
	;load exe data
	mov	si,cs:exeldr_exelen
	mov	bp,cs:exeldr_pspseg
	mov	di,100h
@@4:	mov	cx,si
	cmp	cx,1024
	jb	@@2
	mov	cx,1024
@@2:	push	cx
	shl	cx,4
	mov	ax,cs:exeldr_handle
	mov	es,bp
	call	readfile
	pop	cx
	add	bp,cx
	sub	si,cx
	jz	@@3
	jnc	@@4
@@3:
	;do relocations
	mov	dx,cs:exeldr_base
	mov	ds,cs:exeldr_hdrseg
	mov	si,ds:[18h] ;offset to first relocation item
	mov	cx,ds:[6h] ;number of reloc items
	jcxz	@@7
@@5:	mov	di,ds:[si]
	mov	ax,ds:[si+2]
	add	ax,dx
	mov	es,ax
	add	es:[di],dx
	add	si,4
	loop	@@5
@@7:
	;free hdr data
	mov	ax,cs:exeldr_hdrseg
	call	freemem
	
	mov	cs:exeldr_oktorun,1
	ret
@@badexe:
	mov	ax,6
	call	fatalerror
	ret
@@badexe2:
	mov	ax,7
	call	fatalerror
	ret
loadexeboth ENDP

;°°°°°°°°°°°°°°°° runexe °°°°°°°°°°°°°°°°
;entry:	(exeldr_* variables)
; exit: -
;saves: none
;descr: runs the exe file loaded previously with loadexe. 
;       returns after exe executed, and frees the memory
;	it uses.
runexe	PROC NEAR
	cmp	cs:exeldr_oktorun,0
	jne	@@0
	ret
@@0:	
	mov	cs:exeldr_ownss,ss
	mov	cs:exeldr_ownsp,sp

	mov	si,cs:exeldr_pspsi
	mov	dx,cs:exeldr_pspseg
	mov	ah,55h
	int	21h ;create psp

	mov	ds,cs:exeldr_pspseg
	mov	bx,cs:exeldr_ownpsp
	mov	ds:[16h],bx ;parents address
	mov	word ptr ds:[0ah],OFFSET @@return
	mov	ds:[0ch],cs ;terminate address

	mov	ah,50h
	mov	bx,cs:exeldr_pspseg
	int	21h

	cli
	mov	ax,cs:exeldr_cs
	add	ax,cs:exeldr_base
	mov	word ptr cs:@@fjmp[3],ax
	mov	ax,cs:exeldr_ip
	mov	word ptr cs:@@fjmp[1],ax
	mov	ax,cs:exeldr_ss
	add	ax,cs:exeldr_base
	mov	ss,ax
	mov	sp,cs:exeldr_sp
	xor	ax,ax
	push	ax
	xor	bx,bx
	mov	cx,0ffh
	mov	dx,cs:exeldr_pspseg
	mov	ds,dx
	mov	es,dx
	mov	bp,0fcfch ;in dos, ?
	mov	si,0fcfch ;in dos, ?
	mov	di,010h
	sti
@@fjmp:	db	0eah,0,0,0,0 ;run exe (far jmp)
	
@@return: ;returns here after int 4c00h
	cli
	mov	ss,cs:exeldr_ownss
	mov	sp,cs:exeldr_ownsp
	sti
	mov	ah,50h
	mov	bx,cs:exeldr_ownpsp
	int	21h
	;free memory
	mov	ax,cs:exeldr_pspseg
	call	freemem
	ret
runexe	ENDP

flash	PROC NEAR
	push	ax
	push	dx
	mov	dx,3dah
	in	al,dx
	mov	dx,3c8h
	xor	al,al
	out	dx,al
	mov	al,ah
	out	dx,al
	xor	al,al
	out	dx,al
	out	dx,al
	pop	dx
	pop	ax
	ret
flash	ENDP

;°°°°°°°°°°°°°°°° execute °°°°°°°°°°°°°°°°
;entry:	DS:SI=offset to filename
; exit: -
;saves: none
;descr: executes the selected exe file. Returns after it has terminates.
execute PROC NEAR
	cld
	IF TESTF
	mov	ah,63
	call	flash
	ENDIF
	call	openfile
	cmp	ax,-1
	je	@@err
	push	ax
	call	loadexe
	pop	ax
	call	closefile
	IF TESTF
	mov	ah,0
	call	flash
	ENDIF
	call	runexe
	IF TESTF
	mov	ah,32
	call	flash
	ENDIF
@@err:	ret
execute	ENDP

;°°°°°°°°°°°°°°°° executehigh °°°°°°°°°°°°°°°°
;entry:	DS:SI=offset to filename
; exit: -
;saves: none
;descr: executes the selected exe file. Returns after it has terminates.
;	The difference to normal execute is, that the exe is loaded to
;	as high in memory as minparasneeded allows. Used for loading
;	music loaders and stuff.
executehigh PROC NEAR
	cld
	call	openfile
	push	ax
	call	loadexehigh
	pop	ax
	call	closefile
	call	runexe
	ret
executehigh ENDP

;°°°°°°°°°°°°°°°° flushkbd °°°°°°°°°°°°°°°°
flushkbd PROC NEAR
	mov	cx,256
@@2:	mov	ah,1
	int	16h
	jz	@@1
	mov	ah,0
	int	16h
	loop	@@2
@@1:	ret
flushkbd ENDP

;±±±±±±±±±±±±±±±±±±±± Misc utils ±±±±±±±±±±±±±±±±±±
;all misc utils preserve all regs except ones in which values are returned
;these routines are not optimized for optimum performance, but are easy to use

;entry:	DS:SI=string
; exit:	AX=length
strlen	PROC NEAR
	xor	ax,ax
	push	si
@@2:	cmp	byte ptr ds:[si],0
	je	@@1
	inc	ax
	inc	si
	jmp	@@2
@@1:	pop	si
	ret
strlen	ENDP

;entry:	DS:SI=source, ES:DI=destination, CX=bytes to copy
; exit:	-
memcpy	PROC NEAR
	push	si
	push	di
	shr	cx,1
	rep	movsw
	adc	cx,cx
	rep	movsb
	pop	di
	pop	si
	ret
memcpy	ENDP

;entry:	DS:SI=source, ES:DI=destination (byte copied=length of source (strlen)
; exit:	-
strcpy	PROC NEAR
	push	si
	push	di
	push	ax
	call	strlen
	mov	cx,ax
	shr	cx,1
	rep	movsw
	adc	cx,cx
	rep	movsb
	pop	ax
	pop	di
	pop	si
	ret
strcpy	ENDP

;±±±±±±±±±±±±±±±±±±±± File system ±±±±±±±±±±±±±±±±±±

ALIGN 16
file_oldint dd	0
keyb_oldint dd	0
keyb9_oldint dd	0

file_setint PROC NEAR
	xor	ax,ax
	mov	ds,ax
	mov	ax,word ptr ds:[021h*4+0]
	mov	bx,word ptr ds:[021h*4+2]
	mov	word ptr cs:file_oldint[0],ax
	mov	word ptr cs:file_oldint[2],bx
	mov	word ptr ds:[021h*4+0],OFFSET file_int
	mov	word ptr ds:[021h*4+2],cs
	mov	ax,word ptr ds:[016h*4+0]
	mov	bx,word ptr ds:[016h*4+2]
	mov	word ptr cs:keyb_oldint[0],ax
	mov	word ptr cs:keyb_oldint[2],bx
	mov	word ptr ds:[016h*4+0],OFFSET keyb_int
	mov	word ptr ds:[016h*4+2],cs
	mov	word ptr ds:[03h*4+0],OFFSET just_iret
	mov	word ptr ds:[03h*4+2],cs
        IF FINAL AND NOT KEYBON
	mov	ax,word ptr ds:[09h*4+0]
	mov	bx,word ptr ds:[09h*4+2]
	mov	word ptr cs:keyb9_oldint[0],ax
	mov	word ptr cs:keyb9_oldint[2],bx
	mov	word ptr ds:[09h*4+0],OFFSET keyb9_int
	mov	word ptr ds:[09h*4+2],cs
	mov	word ptr ds:[417h],0
        ENDIF
	ret
file_setint ENDP

file_resetint PROC NEAR
	xor	ax,ax
	mov	ds,ax
	mov	ax,word ptr cs:file_oldint[0]
	mov	bx,word ptr cs:file_oldint[2]
	mov	word ptr ds:[021h*4+0],ax
	mov	word ptr ds:[021h*4+2],bx
	mov	ax,word ptr cs:keyb_oldint[0]
	mov	bx,word ptr cs:keyb_oldint[2]
	mov	word ptr ds:[016h*4+0],ax
	mov	word ptr ds:[016h*4+2],bx
	IF FINAL
	mov	ax,word ptr cs:keyb9_oldint[0]
	mov	bx,word ptr cs:keyb9_oldint[2]
	mov	word ptr ds:[09h*4+0],ax
	mov	word ptr ds:[09h*4+2],bx
	ENDIF
	ret
file_resetint ENDP

chk1	db	'SECOND.EXE',0
chk2	db	'REALITY.FC',0
dummy	db	16 dup(0fch)

file_checkfiles PROC NEAR
	xor	ax,ax
	mov	ds,ax
	mov	cx,ds:[46ch]
	and	cx,15
	inc	cx
@@aga:	push	cx
	mov	ax,cs
	mov	ds,ax
	mov	dx,OFFSET chk1
	mov	ax,3d00h
	int	21h
	jc	@@err
	mov	bx,ax
	mov	ax,3e00h
	int	21h
	mov	dx,OFFSET chk2
	mov	ax,3d00h
	int	21h
	jc	@@err
	mov	bx,ax
	mov	ax,3f00h
	mov	cx,16
	mov	dx,OFFSET dummy
	int	21h
	mov	ax,3e00h
	int	21h
	pop	cx
	loop	@@aga
	ret
@@err:	pop	cx
	mov	cs:errorcode,7
	ret
file_checkfiles ENDP

exe_path db	'SECOND.EXE',0
	db	64 dup(0)
secondpath db	'SECOND.EXE',0
exe_base dd	0
exe_signature dw 0
seek_base dd	0
seek_end dd	0
seek_special dw	0

file_path dw	OFFSET file_name-5
	db	100 dup(0)
	db	'DATA\'
file_name db	32 dup(0),0
file_acc dw	0

keyb_int PROC FAR ;interrupt 016h server
	call	forcebreakcheck
	jmp	cs:keyb_oldint
keyb_int ENDP

keyb9_int PROC FAR ;interrupt 09h server
	cmp	cs:normalkbd,0
	jne	@@1
	push	ax
	in	al,60h
	cmp	al,1
	jne	@@2
	mov	cs:ctrlwasdown,1
@@2:
        in      al,61h
        mov     ah,al
        or      al,80h
        out     61h,al
        xchg    ah,al
        out     61h,al

	mov	al,20h
	out	20h,al
	pop	ax
just_iret:
	iret
@@1:	jmp	cs:keyb9_oldint
keyb9_int ENDP

	db	'INTT'
ALIGN 16
inttable LABEL BYTE
	db	256 dup(0)
	
ALIGN 2
read_poslow dw	0
read_adr dw	0
read_cnt dw	0
read_buf dw	0,0

file_int PROC FAR ;interrupt 021h server
	sti
	
	IF DEBUG
	push	bx
	mov	bl,ah
	xor	bh,bh
	or	cs:inttable[bx],1
	pop	bx
	ENDIF

	cmp	ah,42h ;seek file
	je	@@s
	cmp	ah,3dh ;open file
	je	@@1
	cmp	ah,3fh ;read file
	je	@@rr
	cmp	ah,30h ;version
	je	@@xit
IF DEBUG
	cmp	ah,48h ;alloc
	je	@@mem
ENDIF
@@nn:	jmp	cs:file_oldint
@@xit:	mov	al,5
	mov	ah,0
	iret

IF DEBUG	
@@mem:	cmp	bx,0ffffh
	je	@@nn
	pushf
	call	cs:file_oldint
	call	recheckmem
	ret	2
ENDIF

@@rr:	cmp	cs:seek_special,0
	je	@@nn
	push	ax
	push	cx
	push	dx
	mov	ax,4201h
	mov	cx,0
	mov	dx,0
	pushf	
	call	cs:file_oldint
	mov	cs:read_poslow,ax
	pop	dx
	pop	cx
	pop	ax
	mov	cs:read_buf[0],dx
	mov	cs:read_buf[2],ds
	mov	cs:read_cnt,cx
	pushf
	call	cs:file_oldint
	pusha
	push	ds
	mov	ax,cs:read_poslow
	mov	bx,cs:read_adr
	mul	bx
	lds	si,dword ptr cs:read_buf
	mov	cx,cs:read_cnt
	mov	dl,0ffh
@@rr1:	cmp	ax,0f000h
	jae	@@rr2
	xor	ds:[si],dl
@@rr2:	inc	si
	add	ax,bx
	dec	cx
	jnz	@@rr1
	pop	ds
	popa
	clc ;no errors, sure :-)
	ret 	2

@@s:	cmp	cs:seek_special,0
	je	@@nn
	push	cx
	cmp	al,1 ;current
	je	@@sgo
	cmp	al,0
	jne	@@sm1
	add	dx,word ptr cs:seek_base[0]
	adc	cx,word ptr cs:seek_base[2]
@@sm1:	cmp	al,2
	jne	@@sm2
	add	dx,word ptr cs:seek_end[0]
	adc	cx,word ptr cs:seek_end[2]
@@sm2:	mov	ax,4200h
@@sgo:	pushf
	call	cs:file_oldint
	sub	ax,word ptr cs:seek_base[0]
	sbb	dx,word ptr cs:seek_base[2]
	pop	cx
	ret	2
	
@@1:	;special open?
	push	bx
	mov	bx,dx
	cmp	byte ptr ds:[bx],'*'
	jne	@@nosp
	;remove '*', use normal dos open
	inc	dx
	pop	bx
	jmp	cs:file_oldint
@@nosp: pop	bx
	;
	cmp	al,1
	je	@@nn ;writemode normally
	push	bx
	push	dx
	push	ds
	mov	cs:seek_base,0
	mov	cs:file_acc,ax
	cmp	cs:exe_signature,0fc0h
	je	@@p
@@try:	mov	cs:seek_special,0
	mov	bx,dx
	pushf
	call	cs:file_oldint
	jnc	@@3
	zzz=0
	REPT	32/2
	mov	ax,ds:[bx+zzz]
	mov	word ptr cs:file_name[zzz],ax
	zzz=zzz+2
	ENDM
	mov	ax,cs
	mov	ds,ax
	mov	dx,cs:file_path
	mov	ax,cs:file_acc
	pushf
	call	cs:file_oldint
	jnc	@@3
	pop	ds
	pop	dx
	pop	bx
	
	push	cx
	push	dx
	mov	cx,cs:file_path
	mov	dx,cs
	mov	ax,9+100h
	call	fatalerror
	pop	dx
	pop	cx
	
	mov	ax,2
	stc
	ret	2

@@3:	pop	ds
	pop	dx
	pop	bx
	ret	2
	
@@p:	mov	cs:seek_special,1
	;packed file, process
	;calculate checksums
	push	si
	push	di
	mov	si,dx
	xor	ah,ah
	mov	cx,1111h
	mov	dx,1111h
@@p2:	mov	al,ds:[si]
	or	al,al
	jz	@@p1
	and	al,not 20h
	xor	cx,ax
	rol	cx,1
	add	dx,ax
	inc	si
	jmp	@@p2
@@p1:	mov	si,cx
	mov	di,dx
	;seek list
	mov	bx,OFFSET filedir
	mov	cx,cs:[bx]
	jcxz	@@p4
	add	bx,2
@@p5:	cmp	cs:[bx+2],si
	jne	@@p6
	cmp	cs:[bx+6],di
	je	@@p3
@@p6:	add	bx,12
	loop	@@p5
@@p4:	pop	di
	pop	si
	pop	ds
	pop	dx
	pop	bx
	mov	ax,cs:file_acc
	push	bx
	push	dx
	push	ds
	jmp	@@try
@@p3:	;found!
	mov	dx,cs:[bx+2]
	add	dx,cs:[bx+6]
	mov	cs:read_adr,dx
	mov	dx,cs:[bx+0]
	mov	cx,cs:[bx+4]
	add	dx,word ptr cs:exe_base[0]
	adc	cx,word ptr cs:exe_base[2]
	mov	word ptr cs:seek_base[0],dx
	mov	word ptr cs:seek_base[2],cx
	push	cx
	push	dx
	add	dx,cs:[bx+8]
	adc	cx,cs:[bx+10]
	mov	word ptr cs:seek_end[0],dx
	mov	word ptr cs:seek_end[2],cx
	mov	ax,cs
	mov	ds,ax
	mov	dx,OFFSET exe_path
	mov	ax,3d00h
	pushf
	call	cs:file_oldint
	pop	dx
	pop	cx
	push	ax
	mov	bx,ax
	mov	ax,4200h
	pushf
	call	cs:file_oldint
	pop	ax
	pop	di
	pop	si
	pop	ds
	pop	dx
	pop	bx
	ret	2
file_int ENDP

;entry: DS=PSP
file_getexepath PROC NEAR
	ret
	mov	ax,ds:[2ch]
	mov	ds,ax ;ds=enviroment
	xor	si,si
	xor	ax,ax
	mov	cx,4096
@@1:	cmp	ds:[si],ax
	je	@@2
	inc	si
	loop	@@1
	ret
@@2:	add	si,4
	mov	ax,cs
	mov	es,ax
	call	strlen
	mov	di,OFFSET exe_path
	mov	cx,64
	call	memcpy
	push	si
	add	si,ax
	inc	ax
@@3:	dec	si
	dec	ax
	cmp	byte ptr ds:[si],'\'
	jne	@@3
	pop	si
	mov	di,OFFSET file_name-5 ;leave DATA\
	sub	di,ax
	mov	cs:file_path,di
	mov	cx,ax
	call	memcpy
	ret
file_getexepath ENDP

file_initpacking PROC NEAR
	mov	ax,cs
	mov	ds,ax
	mov	dx,OFFSET exe_path
	mov	ax,3d00h
	int	21h
	mov	bx,ax
	jc	@@err
	mov	ax,4202h
	mov	cx,-1
	mov	dx,-4
	int	21h
	jc	@@err
	mov	ah,3fh
	mov	cx,4
	mov	dx,OFFSET exe_base
	int	21h
	jc	@@err
	xor	dword ptr ds:exe_base,012345678h
	mov	ax,4200h
	mov	cx,word ptr ds:exe_base[2]
	mov	dx,word ptr ds:exe_base[0]
	int	21h
	jc	@@err
	mov	ah,3fh
	mov	cx,2
	mov	dx,OFFSET exe_signature
	int	21h
	jc	@@err
	mov	ax,ds:exe_signature
	add	dword ptr ds:exe_base,2
	ret
@@err:	mov	ax,8
	call	fatalerror
	ret
file_initpacking ENDP

;±±±±±±±±±±±±±±±± ±±±±±±±±±±±±±±±±

inittwk PROC FAR
	push	bp
	mov	bp,sp
	push	si
	push	di
	push	ds
	;clear palette
;	mov	dx,3c8h
;	xor	al,al
;	out	dx,al
;	inc	dx
;	mov	cx,768
;@@1:	out	dx,al
;	loop	@@1
	;400 rows
	mov	dx,3d4h
	mov	ax,00009h
	out	dx,ax
	;tweak
	mov	dx,3d4h
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
	pop	ds
	pop	di
	pop	si
	pop	bp
	ret
inittwk ENDP

;±±±±±±±±±±±±±±±± ±±±±±±±±±±±±±±±±


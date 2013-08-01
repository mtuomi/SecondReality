NOJUMPS

calccrc MACRO	start,end
	local	l1
	mov	cx,OFFSET end-OFFSET start
	mov	si,OFFSET start
l1:	add	dl,cs:[si]
	adc	dh,0
	rol	dx,1
	inc	si
	loop	l1
	ENDM
uncryptxor MACRO beg,key
	local	l1
	mov	bx,OFFSET beg
	mov	ax,key
	mov	cx,(OFFSET _unpack_endcrypt-OFFSET beg)/2
l1:	xor	ds:[bx],ax
	add	bx,2
	dec	cx
	jnz	l1
	ENDM
uncryptadd MACRO beg,key ;actually sub, uncrypts add-crypting
	local	l1
	mov	bx,OFFSET beg
	mov	ax,key
	mov	cx,(OFFSET _unpack_endcrypt-OFFSET beg)/2
l1:	sub	ds:[bx],ax
	add	bx,2
	dec	cx
	jnz	l1
	ENDM

unp_text SEGMENT para public 'CODE'
	ASSUME cs:unp_text
	PUBLIC	_unpack_start,_unpack_end
	PUBLIC	_unpack_data,_unpack_dataend,_unpack_data2,_unpack_code
	PUBLIC	_unpack_stack0,_unpack_stack1
	PUBLIC	_unpack_crc0,_unpack_crc1
	PUBLIC	_unpack_crypt1	;add 5f27h - INC
	PUBLIC	_unpack_crypt2	;xor 4493h - INC
	PUBLIC	_unpack_crypt3	;add 0073h - INC
	PUBLIC	_unpack_crypt4	;xor 4536h - none
	PUBLIC	_unpack_crypt10	;xor 0666h - none
	PUBLIC	_unpack_crypt11	;add 0656h - DEC
	PUBLIC	_unpack_crypt12	;xor 0466h - DEC
	PUBLIC	_unpack_crypt13	;add 0663h - none
	PUBLIC	_unpack_crypt14	;add DX - none
	PUBLIC	_unpack_crypt15	;xor 5cach - none
	PUBLIC	_unpack_crypt16	;add 3fach - INC
	PUBLIC	_unpack_endcrypt
_unpack_start LABEL BYTE

	db	'FCP/IV'	;Future Protector IV 

_unpack_code LABEL BYTE
startup	PROC NEAR
	mov	sp,OFFSET _unpack_stack1
io21data LABEL BYTE
	ret
pntds2	LABEL WORd
	jmp	cs:[farjump+2]
startup ENDP
int_iret PROC FAR
	iret
int_iret ENDP

ALIGN 2
_unpack_stack0 LABEL BYTE
	dw	50 dup(5555h)
_unpack_stack1 LABEL BYTE
	dw	OFFSET step1
	dw	0	;popped by step 1 => DS
	dw	OFFSET step2
	dw	OFFSET step3
	dw	OFFSEt step4
stck1:	dw	OFFSET stepx1	;the cracker blew it :-)
	dw	0fff0h,0000h
disk0	dw	0
disk1	dw	0
vram	dw	01234h
hboottxt db	'.;*',13,0
pntds	dw	0

_unpack_crc0 LABEL BYTE

.386P
step1	PROC NEAR
	in	al,21h
	mov	cs:io21data,al
	mov	al,255
	out	21h,al
	cli
	mov	cs:pntds,ds
	pop	ds
	cld
	;disk detect
	mov	ah,08h
	mov	dl,128
	mov	dh,0
	int	13h
	cmp	dh,0
	je	jjj1
	mov	cs:disk0,cx
	mov	cs:disk1,dx
jjj1:	;video detect
	mov	ah,0fh
	int	10h
	cmp	al,7
	je	ismda
	mov	cs:vram,0800h+1234h
ismda:	add	cs:vram,0b000h-01234h
	;processor detect, at return: BP=0=real BP=1=protected mode
	push	sp			; 86/186 will push SP-2;
	pop	ax			; 286/386 will push SP.
	cmp	ax, sp
	jz	not86			; If equal, SP was pushed
	;8086/80186
noprot:	mov	bp,0
	ret
not86:	smsw	cx			; Protected?  Machine status -> CX
	ror	cx,1			; Protection bit -> carry flag
	jnc	noprot			; Real mode if no carry
	mov	bp,1
	ret
step1	ENDP
.8086

step3	PROC	NEAR
	;DS=CS now (in step 2)
	;uncode crap
	mov	byte ptr cs:int1code,40h ;INC AX
	int	3	;clc set at step2's end
	uncryptadd _unpack_crypt1,5f27h
	ret
step3	ENDP

ints	PROC FAR	;SingleStep/DEBUG
int_01:	stc
int_03:	push	bp
	mov	bp,sp
	jnc	int2
	push	bx
	mov	bx,ss:[bp+2] ;ip
	cmp	byte ptr cs:[bx],49h ;inc cx
	jne	int1
int1code: inc	bx
int1:	pop	bx
int3:	pop	bp
	iret
int2:	xor	ss:[bp+6],256
	mov	bp,cs:stck1to
	mov	word ptr cs:[stck1],bp
	jmp	int3
ints000:
ints	ENDP

_unpack_crc1 LABEL BYTE

itable	LABEL WORD
itablelen equ	4
	dw	OFFSET int_iret,0f000h	;int 0
	dw	OFFSET int_01,0f000h	;int 1
	dw	OFFSET int_iret,0f000h	;int 2
	dw	OFFSET int_03,0f000h	;int 3
;Stack contents on ints:
; [bp+6]: flags
; [bp+4]: cs
; [bp+2]: ip
; [bp+0]: bp

_startzeroing LABEL BYTE

step2	PROC NEAR	;WARNING! ROUTINE OVERWRITTEN BY INNER STACK
	mov	cs:hboottxt[0],'b'
	;set interrupts
	mov	bx,'FG'
	mov	si,OFFSET itable
	xor	di,di
	mov	cx,itablelen
	mov	dx,'JM'
	mov	cs:hboottxt[1],'C'
st2a:	segcs
	lodsw
	xchg	ds:[di],ax
	mov	cs:[si-2],ax
	mov	ax,cs
	xchg	ds:[di+2],ax
	mov	cs:[si],ax
	add	si,2
	add	di,4
	loop	st2a
	;carry=0 for di won't rotate
	mov	di,dx	;='JM'
	mov	dx,OFFSET hboottxt
	mov	ax,cs
	mov	ds,ax
	mov	si,bx	;='FG'	;if this enabled, ICE will reboot
	mov	ax,0911h
	ret
step2	ENDP

stepx1	PROC NEAR
	mov	ax,0f000h
	retf	;reboots
stepx1	ENDP

_unpack_crypt1	LABEL BYTE

step4	PROC	NEAR
	;DS=CS
	mov	ax,cs:pntds
	mov	cs:pntds2,ax
	uncryptxor _unpack_crypt2,4493h
_unpack_crypt2 LABEL BYTE
	uncryptadd _unpack_crypt3,0073h
_unpack_crypt3 LABEL BYTE
	jmp	stp4o
stck1to dw	OFFSET step5
hboottxt2 db	'Bc*',13,0
stp4o:	sub	ax,ax
	mov	si,'FG'
	mov	di,'JM'
	mov	ax,0911h
	mov	dx,OFFSET hboottxt2
	clc
	int	3
	uncryptxor _unpack_crypt4,4536h
	ret	;goes to step5
step4	ENDP

_unpack_crypt4 LABEL BYTE

innerstack LABEL BYTE
	dw	OFFSET stepx1

step5	PROC	NEAR
	;enter inner stack
	mov	sp,OFFSET innerstack
	uncryptxor _unpack_crypt10,0666h
_unpack_crypt10 LABEL BYTE
i1c0:	mov	dx,0656h
	sub	ax,ax
	mov	es,ax
	mov	ax,es:[6]
	mov	bx,cs
	cmp	ax,bx
	je	i1c1
	mov	word ptr cs:[i1c0+1],0CA3Fh
	mov	dx,3FC1H
i1c1:	cmp	word ptr es:[4],OFFSET int_01
	je	i1c2
	mov	word ptr cs:[i1c0+1],0CA3Fh
	mov	dx,3CF2H
i1c2:	mov	byte ptr cs:int1code,48h ;DEC AX
	sub	ax,ax
	int	3
	uncryptadd _unpack_crypt11,dx
_unpack_crypt11 LABEL BYTE
	uncryptxor _unpack_crypt12,0466h
_unpack_crypt12 LABEL BYTE
	mov	byte ptr cs:int1code,40h ;INC AX
	sub	ax,ax
	int	3
	uncryptadd _unpack_crypt13,0663h
_unpack_crypt13 LABEL BYTE
	;calc checksum of exe start
	xor	dx,dx
	mov	byte ptr cs:int1code,43h ;INC BX
	calccrc	_unpack_crc0,_unpack_crc1
	uncryptadd _unpack_crypt14,0 ;dx
_unpack_crypt14 LABEL BYTE
	uncryptxor _unpack_crypt15,5CACh
_unpack_crypt15 LABEL BYTE
	mov	byte ptr cs:int1code,40h ;INC AX
	sub	ax,ax
	int	3
	uncryptadd _unpack_crypt16,3FACh
_unpack_crypt16 LABEL BYTE
	mov	word ptr cs:innerstack,OFFSET execute
	sub	ax,ax
	int	3
	ret
step5	ENDP

ALIGN 2

execute	PROC NEAR
	;unpack data
	mov	dx,cs:pntds2
	add	dx,10h
	xor	si,si
	xor	di,di
	cmp	cs:mcrcon,0
	je	exec11
	;calc machine crc
	mov	ax,0ff00h
	mov	ds,ax
	mov	bx,0
	mov	cx,1024
mcrc1:	add	si,ds:[bx]
	add	di,ds:[bx+2]
	rol	si,1
	add	bx,4
	loop	mcrc1
	add	si,cs:disk0
	add	di,cs:disk1
exec11:	add	si,cs:regcs
	add	di,cs:regip
	add	si,cs:regsp
	add	di,cs:regss
	add	si,cs:regkey
	add	di,cs:regkey
	mov	cx,cs:regcs0
exec1:	mov	ds,dx
	sub	ds:[0],si
	sub	ds:[2],di
	xor	si,5354h
	xor	di,1525h
	sub	ds:[4],si
	sub	ds:[6],di
	add	si,1234h
	ror	si,1
	add	di,4321h
	rol	di,1
	sub	ds:[8],si
	sub	ds:[10],di
	xor	si,5354h
	xor	di,1525h
	sub	ds:[12],si
	sub	ds:[14],di
	add	si,1234h
	ror	si,1
	add	di,4321h
	rol	di,1
	inc	dx
	add	si,ds:[0]
	add	di,ds:[8]
	add	si,ds:[2]
	add	di,ds:[10]
	add	si,ds:[4]
	add	di,ds:[12]
	add	si,ds:[6]
	add	di,ds:[14]
	loop	exec1
	mov	bp,OFFSET errormsg1
	cmp	si,cs:keychk0
	jne	error
	cmp	di,cs:keychk1
	jne	error
	cld
_endzeroing LABEL BYTE ;may overflow a byte, but who cares
	mov	dx,cs:pntds2
	mov	ax,cs
	mov	es,ax
	mov	cx,(OFFSET _endzeroing-OFFSET _startzeroing+1)/2
	mov	di,OFFSET _startzeroing
	mov	ax,'..'
	rep	stosw
	call	resetints
	mov	al,cs:io21data
	out	21h,al
	mov	ax,cs
	mov	es,ax
	mov	cx,OFFSET _startzeroing-OFFSET _unpack_code
	mov	di,OFFSET _unpack_code
	mov	al,'.'
	rep	stosb
	;load stack and cs:ip for the actual program and run it.
	mov	ds,dx
	mov	es,dx
	mov	bx,cs
	mov	cx,cs:regcs0
	mov	ax,cs:regss
	add	ax,bx
	sub	ax,cx
	mov	ss,ax
	mov	sp,cs:regsp
	add	cs:regcs,bx
	sub	cs:regcs,cx
	push	cs:regcs
	push	cs:regip
	mov	cs:regcs,ax
	mov	cs:regip,ax
	mov	cs:regss,ax
	mov	cs:regcs0,ax
	mov	cs:keychk1,ax
	mov	cs:regkey,ax
	sti
	jmp	endprg2
	
_endzeroing2 LABEL BYTE
error:	;destroy and exit with error at OFFSET BP
	cld
	mov	dx,cs:pntds2
	mov	ax,cs
	mov	es,ax
	mov	cx,(OFFSET _endzeroing2-OFFSET _startzeroing+1)/2
	mov	di,OFFSET _startzeroing
	mov	ax,'..'
	rep	stosw
	call	resetints
	mov	al,cs:io21data
	out	21h,al
	mov	ax,cs
	mov	es,ax
	mov	di,OFFSET _unpack_code
	mov	cx,OFFSET _startzeroing-OFFSET _unpack_code
	mov	al,'.'
	rep	stosb
	mov	cs:regcs,ax
	mov	cs:regip,ax
	mov	cs:regss,ax
	mov	cs:regcs0,ax
	mov	cs:keychk1,ax
	mov	cs:regkey,ax
	mov	sp,OFFSET _unpack_data
	sti
	jmp	endprg1
execute ENDP

resetints PROC NEAR
	;reset interrupts
	xor	ax,ax
	mov	es,ax
	mov	si,OFFSET itable
	xor	di,di
	mov	cx,itablelen*2
st20a:	segcs
	lodsw
	stosw
	loop	st20a
	ret
resetints ENDP

	ALIGN	2
_unpack_data LABEL BYTE
farjump LABEL DWORD
regcs	dw	0
regip	dw	0
regss	dw	0
regsp	dw	0
regcs0	dw	0
errormsg1 db	'File corrupted!$'
_unpack_data2 LABEL BYTE
regkey	dw	0
mcrcon	dw	0	;1=machine crc enabled
keychk0 dw	0
keychk1 dw	0
_unpack_dataend LABEL BYTE

	ALIGN	2
	
endprg1: ;****************
	mov	dx,bp
	mov	ax,cs
	mov	ds,ax
	mov	ah,9h
	int	21h
	mov	ax,4c03h
	int	21h
endprg2:  ;****************
	retf

	ALIGN	2
_unpack_endcrypt LABEL BYTE
	db	000h,0fch
_unpack_end LABEL BYTE
unp_text ENDS
	END
	
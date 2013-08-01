	.386p
	ifndef	??version
?debug	macro
	endm
publicdll macro	name
	public	name
	endm
	endif
	?debug	V 300h
	?debug	S "MAIN.C"
	?debug	C E9C789EC1A064D41494E2E43
	?debug	C E94019CA1815473A5C42435C494E434C5544455C535444494F2E48
	?debug	C E94019CA1815473A5C42435C494E434C5544455C5F444546532E48
	?debug	C E94019CA1816473A5C42435C494E434C5544455C5F4E46494C452E+
	?debug	C 48
	?debug	C E94019CA1815473A5C42435C494E434C5544455C5F4E554C4C2E48
	?debug	C E94019CA1814473A5C42435C494E434C5544455C4D4154482E48
	?debug	C E94019CA1815473A5C42435C494E434C5544455C434F4E494F2E48
	?debug	C E94019CA1813473A5C42435C494E434C5544455C444F532E48
	?debug	C E93587EB1A102E2E5C545745414B5C545745414B2E48
MAIN_TEXT	segment byte public use16 'CODE'
MAIN_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:MAIN_TEXT,ds:DGROUP
_DATA	segment word public use16 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public use16 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
_DATA	segment word public use16 'DATA'
_vmem	label	dword
	db	0
	db	0
	db	0
	db	160
	?debug	C E947A5DC1A08505441552E505245
_ptau	label	byte
	db	0
	db	1
	db	1
	db	1
	db	1
	db	1
	db	2
	db	2
	db	3
	db	3
	db	4
	db	5
	db	6
	db	7
	db	8
	db	9
	db	10
	db	11
	db	12
	db	13
	db	14
	db	16
	db	17
	db	18
	db	20
	db	21
	db	23
	db	24
	db	25
	db	27
	db	28
	db	30
	db	31
	db	33
	db	35
	db	36
	db	38
	db	39
	db	40
	db	42
	db	43
	db	45
	db	46
	db	47
	db	49
	db	50
	db	51
	db	52
	db	53
	db	54
	db	55
	db	56
	db	57
	db	58
	db	59
	db	60
	db	60
	db	61
	db	61
	db	62
	db	62
	db	62
	db	62
	db	62
	db	63
	db	62
	db	62
	db	62
	db	62
	db	62
	db	61
	db	61
	db	60
	db	60
	db	59
	db	58
	db	57
	db	56
	db	55
	db	54
	db	53
	db	52
	db	51
	db	50
	db	49
	db	47
	db	46
	db	45
	db	43
	db	42
	db	40
	db	39
	db	38
	db	36
	db	35
	db	33
	db	32
	db	30
	db	28
	db	27
	db	25
	db	24
	db	23
	db	21
	db	20
	db	18
	db	17
	db	16
	db	14
	db	13
	db	12
	db	11
	db	10
	db	9
	db	8
	db	7
	db	6
	db	5
	db	4
	db	3
	db	3
	db	2
	db	2
	db	1
	db	1
	db	1
	db	1
	db	1
	db	1
	db	127 dup (0)
_l1	label	word
	db	144
	db	1
_l2	label	word
	db	44
	db	1
_l3	label	word
	db	32
	db	3
_l4	label	word
	db	100
	db	0
_k1	label	word
	db	244
	db	1
_k2	label	word
	db	44
	db	1
_k3	label	word
	db	132
	db	3
_k4	label	word
	db	158
	db	2
_m1	label	word
	db	244
	db	1
_m2	label	word
	db	200
	db	0
_m3	label	word
	db	144
	db	1
_m4	label	word
	db	132
	db	3
_n1	label	word
	db	188
	db	2
_n2	label	word
	db	244
	db	1
_n3	label	word
	db	132
	db	3
_n4	label	word
	db	214
	db	1
_DATA	ends
MAIN_TEXT	segment byte public use16 'CODE'
	?debug	C E801064D41494E2E43C789EC1A
	?debug	L 42
	assume	cs:MAIN_TEXT
_main	proc	far
	?debug	B
	push	bp
	mov	bp,sp
	sub	sp,22
	push	si
	push	di
	?debug	B
	?debug	L 45
	mov	dword ptr [bp-4],large 0
	mov	dword ptr [bp-8],large 0
	?debug	L 46
	mov	word ptr [bp-10],0
	?debug	L 48
	call	far ptr _init_plz
	?debug	L 50
	mov	ax,seg _frame_count
	mov	es,ax
	mov	word ptr es:_frame_count,0
	jmp	@1@1066
@1@58:
	?debug	L 53
	mov	ax,seg _frame_count
	mov	es,ax
	movsx	eax,word ptr es:_frame_count
	add	dword ptr [bp-4],eax
	inc	dword ptr [bp-8]
	?debug	L 54
	cmp	word ptr [bp-10],37
	jne	short @1@114
	push	large 000140014h
	push	large 000140000h
	call	far ptr _tw_setrgbpalette
	add	sp,8
@1@114:
	?debug	L 56
		mov	 dx, 3c4h
	?debug	L 57
		mov	 ax, 0a02h
	?debug	L 58
		out	 dx, ax
	?debug	L 60
	push	word ptr DGROUP:_k4
	push	word ptr DGROUP:_k3
	push	word ptr DGROUP:_k2
	push	word ptr DGROUP:_k1
	call	far ptr _setplzparas
	add	sp,8
	?debug	L 61
	xor	si,si
	mov	word ptr [bp-16],00000A000h
@1@254:
	?debug	L 62
	push	word ptr [bp-16]
	push	si
	call	far ptr _plzline
	add	sp,4
	?debug	L 61
	add	word ptr [bp-16],12
	add	si,2
	cmp	si,350
	jl	short @1@254
	?debug	L 63
	push	word ptr DGROUP:_l4
	push	word ptr DGROUP:_l3
	push	word ptr DGROUP:_l2
	push	word ptr DGROUP:_l1
	call	far ptr _setplzparas
	add	sp,8
	?debug	L 64
	mov	si,1
	mov	word ptr [bp-18],00000A006h
	jmp	short @1@506
@1@422:
	?debug	L 65
	push	word ptr [bp-18]
	push	si
	call	far ptr _plzline
	add	sp,4
	?debug	L 64
	add	word ptr [bp-18],12
	add	si,2
@1@506:
	cmp	si,350
	jl	short @1@422
	?debug	L 68
		mov	 dx, 3c4h
	?debug	L 69
		mov	 ax, 0502h
	?debug	L 70
		out	 dx, ax
	?debug	L 72
	push	word ptr DGROUP:_k4
	push	word ptr DGROUP:_k3
	push	word ptr DGROUP:_k2
	push	word ptr DGROUP:_k1
	call	far ptr _setplzparas
	add	sp,8
	?debug	L 73
	mov	si,1
	mov	word ptr [bp-20],00000A006h
	jmp	short @1@758
@1@674:
	?debug	L 74
	push	word ptr [bp-20]
	push	si
	call	far ptr _plzline
	add	sp,4
	?debug	L 73
	add	word ptr [bp-20],12
	add	si,2
@1@758:
	cmp	si,350
	jl	short @1@674
	?debug	L 75
	push	word ptr DGROUP:_l4
	push	word ptr DGROUP:_l3
	push	word ptr DGROUP:_l2
	push	word ptr DGROUP:_l1
	call	far ptr _setplzparas
	add	sp,8
	?debug	L 76
	xor	si,si
	mov	word ptr [bp-22],00000A000h
@1@842:
	?debug	L 77
	push	word ptr [bp-22]
	push	si
	call	far ptr _plzline
	add	sp,4
	?debug	L 76
	add	word ptr [bp-22],12
	add	si,2
	cmp	si,350
	jl	short @1@842
	?debug	L 79
	cmp	word ptr [bp-10],37
	jne	short @1@1010
	push	large 0
	push	large 0
	call	far ptr _tw_setrgbpalette
	add	sp,8
@1@1010:
	?debug	L 83
	call	far ptr _kbhit
	or	ax,ax
	je	short @1@1066
	call	far ptr _getch
	mov	word ptr [bp-10],ax
@1@1066:
	?debug	L 51
	cmp	word ptr [bp-10],27
	je short	@@4
	jmp	@1@58
@@4:
	?debug	L 85
	call	far ptr _close_copper
	?debug	L 86
	call	far ptr _tw_closegraph
	?debug	L 88
	mov	eax,dword ptr [bp-4]
	mov	dword ptr [bp-14],eax
	fild	dword ptr [bp-14]
	mov	eax,dword ptr [bp-8]
	mov	dword ptr [bp-14],eax
	fild	dword ptr [bp-14]
	fdiv	
	sub	sp,8
	fstp	qword ptr [bp-34]
	push	ds
	push	offset DGROUP:s@
	fwait	
	call	far ptr _printf
	add	sp,12
	?debug	L 89
	pop	di
	pop	si
	leave	
	ret	
	?debug	C E60263680402F6FF0005636F756E740602F8FF00+
	?debug	C 0374696D0602FCFF00017904080192007B010406+
	?debug	C 00
	?debug	E
	?debug	E
_main	endp
	?debug	L 91
	assume	cs:MAIN_TEXT
_init_plz	proc	far
	?debug	B
	push	bp
	mov	bp,sp
	sub	sp,2
	push	si
	?debug	B
	?debug	L 141
	call	far ptr _tw_opengraph
	?debug	L 142
	push	-17536
	call	far ptr _tw_setstart
	add	sp,2
	?debug	L 144
	mov		dx, 3d4h
	?debug	L 145
	mov		ax, 4009h
	?debug	L 146
	out		dx, ax
	?debug	L 147
	mov		ax, 3013h
	?debug	L 148
	out		dx, ax
	?debug	L 149
	mov		ax, a018h		
	?debug	L 150
	out		dx, ax
	?debug	L 151
	mov		ax, 0f07h
	?debug	L 152
	out		dx, ax			
	?debug	L 155
	call	far ptr _init_copper
	?debug	L 158
	mov	word ptr [bp-2],0
@2@338:
	mov	al,byte ptr DGROUP:_ptau
	cbw	
	mov	si,ax
	push	ax
	push	ax
	mov	bx,word ptr [bp-2]
	mov	al,byte ptr DGROUP:_ptau[bx]
	cbw	
	push	ax
	push	bx
	call	far ptr _tw_setrgbpalette
	add	sp,8
	inc	word ptr [bp-2]
	cmp	word ptr [bp-2],64
	jl	short @2@338
	?debug	L 159
	mov	word ptr [bp-2],0
@2@450:
	mov	bx,word ptr [bp-2]
	mov	al,byte ptr DGROUP:_ptau[bx]
	cbw	
	push	ax
	mov	al,byte ptr DGROUP:_ptau
	cbw	
	push	ax
	mov	bx,63
	sub	bx,word ptr [bp-2]
	mov	al,byte ptr DGROUP:_ptau[bx]
	cbw	
	push	ax
	mov	ax,word ptr [bp-2]
	add	ax,64
	push	ax
	call	far ptr _tw_setrgbpalette
	add	sp,8
	inc	word ptr [bp-2]
	cmp	word ptr [bp-2],64
	jl	short @2@450
	?debug	L 160
	mov	word ptr [bp-2],0
@2@562:
	mov	bx,63
	sub	bx,word ptr [bp-2]
	mov	al,byte ptr DGROUP:_ptau[bx]
	cbw	
	push	ax
	mov	bx,word ptr [bp-2]
	mov	al,byte ptr DGROUP:_ptau[bx]
	cbw	
	push	ax
	mov	al,byte ptr DGROUP:_ptau
	cbw	
	push	ax
	mov	ax,word ptr [bp-2]
	add	ax,128
	push	ax
	call	far ptr _tw_setrgbpalette
	add	sp,8
	inc	word ptr [bp-2]
	cmp	word ptr [bp-2],64
	jl	short @2@562
	?debug	L 161
	mov	word ptr [bp-2],0
@2@674:
	mov	bx,word ptr [bp-2]
	mov	al,byte ptr DGROUP:_ptau[bx]
	cbw	
	push	ax
	mov	al,byte ptr DGROUP:_ptau+63
	cbw	
	push	ax
	mov	al,byte ptr DGROUP:_ptau[bx]
	cbw	
	push	ax
	mov	ax,word ptr [bp-2]
	add	ax,192
	push	ax
	call	far ptr _tw_setrgbpalette
	add	sp,8
	inc	word ptr [bp-2]
	cmp	word ptr [bp-2],64
	jl	short @2@674
	?debug	L 185
	pop	si
	leave	
	ret	
	?debug	C E601610402FEFF00
	?debug	E
	?debug	E
_init_plz	endp
	?debug	C E9
	?debug	C FA15000000
MAIN_TEXT	ends
_DATA	segment word public use16 'DATA'
s@	label	byte
	db	'%le'
	db	10
	db	0
_DATA	ends
MAIN_TEXT	segment byte public use16 'CODE'
MAIN_TEXT	ends
	public	_init_plz
	public	_main
	public	_n4
	public	_n3
	public	_n2
	public	_n1
	public	_m4
	public	_m3
	public	_m2
	public	_m1
	public	_k4
	public	_k3
	public	_k2
	public	_k1
	public	_l4
	public	_l3
	public	_l2
	public	_l1
	public	_ptau
	public	_vmem
	extrn	_setplzparas:far
	extrn	_plzline:far
	extrn	_frame_count:word
	extrn	_close_copper:far
	extrn	_init_copper:far
	extrn	_tw_setstart:far
	extrn	_tw_setrgbpalette:far
	extrn	_tw_closegraph:far
	extrn	_tw_opengraph:far
	extrn	_kbhit:far
	extrn	_getch:far
	extrn	_printf:far
_s@	equ	s@
	?debug	C EA010C
	?debug	C E31800000023040400
	?debug	C EC095F696E69745F706C7A181800
	?debug	C E31900000023040400
	?debug	C EC055F6D61696E191800
	?debug	C EC035F6E34040000
	?debug	C EC035F6E33040000
	?debug	C EC035F6E32040000
	?debug	C EC035F6E31040000
	?debug	C EC035F6D34040000
	?debug	C EC035F6D33040000
	?debug	C EC035F6D32040000
	?debug	C EC035F6D31040000
	?debug	C EC035F6B34040000
	?debug	C EC035F6B33040000
	?debug	C EC035F6B32040000
	?debug	C EC035F6B31040000
	?debug	C EC035F6C34040000
	?debug	C EC035F6C33040000
	?debug	C EC035F6C32040000
	?debug	C EC035F6C31040000
	?debug	C E31A0000011A02
	?debug	C EC055F707461751A0000
	?debug	C E31C0052001A04
	?debug	C E31B000400161C00
	?debug	C EC055F766D656D1B0000
	?debug	C E31D00000023040400
	?debug	C EB0C5F736574706C7A70617261731D00
	?debug	C E31E00000023040400
	?debug	C EB085F706C7A6C696E651E00
	?debug	C EB0C5F6672616D655F636F756E740400
	?debug	C E31F00000023040400
	?debug	C EB0D5F636C6F73655F636F707065721F00
	?debug	C E32000000023040400
	?debug	C EB0C5F696E69745F636F707065722000
	?debug	C E32100000023010400
	?debug	C EB0C5F74775F73657473746172742100
	?debug	C E32200000023010400
	?debug	C EB115F74775F73657472676270616C6574746522+
	?debug	C 00
	?debug	C E32300000023010400
	?debug	C EB0E5F74775F636C6F736567726170682300
	?debug	C E32400000023010400
	?debug	C EB0D5F74775F6F70656E67726170682400
	?debug	C E32500000023040400
	?debug	C EB065F6B626869742500
	?debug	C E32600000023040400
	?debug	C EB065F67657463682600
	?debug	C E32700000023040401
	?debug	C EB075F7072696E74662700
	?debug	C E60666706F735F740606000673697A655F740A06+
	?debug	C 00
	end

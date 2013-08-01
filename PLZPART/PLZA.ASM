	IDEAL
	MODEL large
	P386

SEGMENT lerssicode para PUBLIC 'CODE'

PUBLIC C jmp_tau
LABEL jmp_tau WORD
IRP ccc, <0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199>
dw	OFFSET plz_y&ccc
ENDM

LABEL start_mask BYTE
REPT	200
	db	1111b, 1110b, 1100b, 1000b
ENDM
LABEL end_mask BYTE
REPT	200
	db	0001b, 0011b, 0111b, 1111b
ENDM

PUBLIC C xx, C yy
PUBLIC C yy1, C xx1, C yy2, C xx2
PUBLIC C ay1, C ay2, C ax1, C ax2
PUBLIC C txx1, C txx2, C txy1, C txy2
PUBLIC C tax1, C tax2, C tay1, C tay2
PUBLIC C to, C from, C dseg, C ctau

xx	dw	?
yy	dw	?
yy1	dd	?
xx1	dd	?
yy2	dd	?
xx2	dd	?
ay1	dd	?
ax1	dd	?
ay2	dd	?
ax2	dd	?
txx1	dd	?
txy1	dd	?
txx2	dd	?
txy2	dd	?
tax1	dd	?
tay1	dd	?
tax2	dd	?
tay2	dd	?
to	dd	0a0000000h
from	dd	?
dseg	dw	0
ctau	dd	?


linecount	dw	?

ASSUME cs:lerssicode, ds:nothing, es:nothing

PUBLIC c do_block

PROC C do_block FAR
	ARG	ycount:word

	push	es ds si di bp

	mov	ax, [ycount]
	mov	[linecount], ax
	cmp	ax, 0d
	je	@@end
	lds	bx, [from]
	les	di, [to]
	lgs	si, [ctau]
	mov	fs, [dseg]

@@doline:
	cmp	[yy], 0d
	jl	@@endline
	cmp	[yy], 134d
	jge	@@end				; y-clip

	mov	bp, [word ctau]
	mov	ax, [Word xx2+2]
	cmp	ax, [gs:bp]
	ja	@@l7
	mov	[gs:bp], ax			; x1..
@@l7:	shr	ax, 2d
	add	di, ax
	mov	si, [Word xx1+2]
	cmp	si, [gs:bp+2]
	jb	@@l8
	mov	[gs:bp+2], si			; x2..
@@l8:	shr	si, 2d
	sub	si, ax			; si = bytes to copy
	jb	@@endline
	jz	@@singlebyte
	mov	bp, si
	dec	si
	jz	@@twobyte

	mov	dx, 3c4h
	mov	ax, 0f02h
	out	dx, ax

	mov	ebx, [txy1-2]		; xlo
	mov	ecx, [txx1-2]		; ylo

	movsx	esi, si
	mov	eax, [txy2]
	sub	eax, [txy1]
	cdq
	idiv	esi                   	; addy
	mov	ecx, eax
	mov	eax, [txx2]
	sub	eax, [txx1]
	cdq
	idiv   	esi			; addx

	mov	edx, ecx      		; look reg table
	rol	edx, 16d		; y_add
	mov	dh, dl			; yah
	rol	eax, 16d		; x_add
	mov	dl, al			; xah
	test	dl, 80h
	jz	@@l2
	dec	dh
@@l2:
	mov	bh, [Byte txy1+2]       ; txt y
	mov	bl, [Byte txx1+2]	; txt x
	test	si, 1d
	jz	@@l1

	movzx	si, [fs:bx]		; out odd byte
	add	ecx, eax
	mov	al, [ds:bx+si]
	adc	ebx, edx
	adc	bh, 0
	mov	[es:bp+di-1],al
	jmp	[cs:jmp_tau+bp-2]

@@l1:	jmp	[cs:jmp_tau+si]		; and jump to rept

;		e	h	l
;	ax	x_add	data	data
;	bx	ylo	ty	tx
;	cx	xlo	-	-
;	dx	y_add	yah	xah
;	di	-
;	si	sinus inc

	ALIGN	4
IRP ccc, <200,199,198,197,196,195,194,193,192,191,190,189,188,187,186,185,184,183,182,181,180,179,178,177,176,175,174,173,172,171,170,169,168,167,166,165,164,163,162,161,160,159,158,157,156,155,154,153,152,151,150,149,148,147,146,145,144,143,142,141,140,139,138,137,136,135,134,133,132,131,130,129,128,127,126,125,124,123,122,121,120,119,118,117,116,115,114,113,112,111,110,109,108,107,106,105,104,103,102,101,100,99,98,97,96,95,94,93,92,91,90,89,88,87,86,85,84,83,82,81,80,79,78,77,76,75,74,73,72,71,70,69,68,67,66,65,64,63,62,61,60,59,58,57,56,55,54,53,52,51,50,49,48,47,46,45,44,43,42,41,40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1>

plz_y&ccc&:

	movzx	si, [fs:bx]
	add	ecx, eax
	mov	ah, [ds:bx+si]
	adc	ebx, edx
	adc	bh, 0

	movzx	si, [fs:bx]
	add	ecx, eax
	mov	al, [ds:bx+si]
	adc	ebx, edx
	adc	bh, 0			; immediate = y_hi

	mov	[es:di+ccc*2-1], ax	; output byte
ENDM
plz_y0:

@@twobyte:
	mov	bh, [Byte txy2+2]       ; txt y
	mov	bl, [Byte txx2+2]	; txt x
	movzx	si, [fs:bx]
	mov	cl, [ds:bx+si]
	mov	bh, [Byte txy1+2]       ; txt y
	mov	bl, [Byte txx1+2]	; txt x
	movzx	si, [fs:bx]
	mov	ch, [ds:bx+si]

	mov	bx, [Word xx2+2]
	and	bx, 3d
	mov	ah, [start_mask+bx]
	mov	al, 02
	mov	dx, 3c4h
	out	dx, ax
	mov	[es:di], cl

	mov	bx, [Word xx1+2]
	and	bx, 3d
	mov	ah, [end_mask+bx]
	out	dx, ax
	mov	[es:bp+di], ch

@@endline:
	add	[Word to], 160d		; next line
	mov	di, [word to]
	inc	[yy]
	add	[word ctau], 4d

	mov	eax, [ax1]
	add	[xx1], eax
	mov	eax, [ax2]
	add	[xx2], eax
	mov	eax, [tay1]
	add	[txy1], eax
	mov	eax, [tax1]
	add	[txx1], eax
	mov	eax, [tay2]
	add	[txy2], eax
	mov	eax, [tax2]
	add	[txx2], eax		; increment txt and line indexes

	dec	[linecount]
	jnz	@@doline

@@end:
	pop	bp di si ds es
	ret


@@singlebyte:
	mov	bh, [Byte txy2+2]       ; txt y
	mov	bl, [Byte txx2+2]	; txt x
	movzx	si, [fs:bx]
	mov	cl, [ds:bx+si]

	mov	al, 02
	mov	dx, 3c4h
	mov	bx, [Word xx2+2]
	and	bx, 3d
	mov	ah, [start_mask+bx]
	mov	bx, [Word xx1+2]
	and	bx, 3d
	and	ah, [end_mask+bx]
	out	dx, ax
	mov	[es:di], cl

	add	[Word to], 160d		; next line
	mov	di, [word to]
	inc	[yy]
	add	[word ctau], 4d

	mov	eax, [ax1]
	add	[xx1], eax
	mov	eax, [ax2]
	add	[xx2], eax
	mov	eax, [tay1]
	add	[txy1], eax
	mov	eax, [tax1]
	add	[txx1], eax
	mov	eax, [tay2]
	add	[txy2], eax
	mov	eax, [tax2]
	add	[txx2], eax		; increment txt and line indexes

	dec	[linecount]
	jnz	@@doline
	pop	bp di si ds es
	ret
ENDP


PUBLIC C shadepal

PROC C shadepal

	ARG	fpal:dword, ppal:dword, shd:word

	push	si di ds es

	lds	si, [ppal]
	les	di, [fpal]
	mov     dx, [shd]
	mov	cx, 192/16d
@@loop:
	REPT	16
	lodsb
	mul	dl
	shr	ax, 6d
	stosb
	ENDM
	loop	@@loop

	pop	es ds di si
	ret

ENDP


ycnt	dw	0

PUBLIC C do_clear

PROC C do_clear

	ARG	vmem:dword, otau:dword, ntau:dword

	push	si di ds es bp

	mov	dx, 3c4h
	mov	ax, 0f02h
	out	dx, ax

	les	dx, [vmem]
	lds	bx, [otau]		; dx:bx-> otau
	lds	si, [ntau]
	sub	si, bx                  ; dx:bx+si-> ntau
	mov	dl, [es:0]		; fill latches
	mov	[ycnt], 134d
	xor	ax, ax

@@lineloop:
	cmp	[Word ds:bx], 640d
	je	@@nextline

	mov	di, [Word ds:bx]
	shr	di, 2d
	mov	cx, [word ds:bx+si]
	shr	cx, 2d
	sub	cx, di
	jb	@@l1
	add	di, dx
	rep	stosb
@@l1:
	mov	di, [Word ds:bx+si+2]
	shr	di, 2d
	mov	cx, [word ds:bx+2]
	shr	cx, 2d
	sub	cx, di
	jb	@@nextline
	add	di, dx
	inc	di
	rep	stosb

@@nextline:
	mov	[Word ds:bx], 640d
	mov	[Word ds:bx+2], 0d

	add	dx, 160d
	add	bx, 4d
	sub	[ycnt], 1d
	jae	@@lineloop

; Maskaa p„„t...

	les	di, [vmem]
	lds	si, [ntau]		; dx:bx-> ntau
	mov	cx, 134d
	mov	dx, 3c4h
	mov	ax, 0f02h
@@maskloop:
	mov	bx, [ds:si]
	dec	bx
	mov	ah, [end_mask+bx]
	out	dx, ax
	shr	bx, 2d
	mov	[es:bx+di], ch

	mov	bx, [ds:si+2]
	inc	bx
	mov	ah, [start_mask+bx]
	out	dx, ax
	shr	bx, 2d
	mov	[es:bx+di], ch

@@nextmask:
	add	si,4d
	add	di, 160d
	loop	@@maskloop

	pop	bp es ds di si
	ret
ENDP
ENDS

END

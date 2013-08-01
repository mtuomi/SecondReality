	IDEAL
	MODEL large
	P386

EXTRN C l1:word, C l2:word, C l3:word, C l4:word, C k1:word, C k2:word, C k3:word, C k4:word
EXTRN C il1:word, C il2:word, C il3:word, C il4:word, C ik1:word, C ik2:word, C ik3:word, C ik4:word

CODESEG

PUBLIC  C frame_count, C init_copper, C close_copper, C cop_drop, C cop_pal, C do_pal
PUBLIC	C cop_scrl, C cop_start, C cop_plz

frame_count     dw      0
cop_drop	dw	0
cop_pal	    	dd	0
do_pal		dw	0
cop_start	dw	0
cop_scrl	dw	0
cop_plz		dw	1

PROC	close_copper

	mov	bx, 7
	mov	ax, 0
	mov	cx, 0
	mov	dx, 0
	int	0fch

	mov	bx, 7
	mov	ax, 1
	mov	cx, 0
	mov	dx, 0
	int	0fch

	mov	bx, 7
	mov	ax, 2
	mov	cx, 0
	mov	dx, 0
	int	0fch

	ret
ENDP

PROC	init_copper

	mov	bx, 7
	mov	ax, 1
	mov	cx, OFFSET copper1
	mov	dx, SEG copper1
	int	0fch

	mov	bx, 7
	mov	ax, 2
	mov	cx, OFFSET copper2
	mov	dx, SEG copper2
	int	0fch

	mov	bx, 7
	mov	ax, 0
	mov	cx, OFFSET copper3
	mov	dx, SEG copper3
	int	0fch

	ret
ENDP

PROC	copper1

	mov	dx, 03d4h
	mov	al, 0dh
	mov	ah, [Byte cs:cop_start]
	out	dx, ax
	mov	al, 0ch
	mov	ah, [Byte cs:cop_start+1]
	out	dx, ax

	mov	dx, 3c0h
	mov	al, 33h
	out	dx, al
	mov	ax, [cs:cop_scrl]
	out	dx, al

	retf
ENDP

PROC	copper2

	pusha
	push	ds

	inc	[cs:frame_count]

	cmp	[cs:do_pal], 0d
	je	@@no_pal
	lds	si, [cs:cop_pal]
	mov	cx, 768d
	mov	dx, 3c8h
	mov	al, 0d
	out	dx, al
	inc	dx
	rep	outsb
	mov	[cs:do_pal], 0d
@@no_pal:
	cmp	[cs:cop_plz], 0d
	je	@@l3
	call    pompota
	call    moveplz
@@l3:
	cmp	[cs:cop_drop], 0d
	je	@@l1
	call	do_drop
@@l1:
	pop	ds
	popa
	retf
ENDP

PROC 	copper3
	retf
ENDP

pompi   db      0

PUBLIC C pompota

PROC    pompota

	mov	[cs:cop_scrl], 4d
	mov	dx, 3d4h
	mov	ax, 3c18h
	out	dx, ax
	mov	ax, 0f07h
	out	dx, ax			; 8th bit
	mov	ax, 3c18h
	inc     [cs:pompi]
	test    [cs:pompi], 1d
	jz      @@l1
	mov	[cs:cop_scrl], 0d
	mov	ax, 3d18h
@@l1:
	out	dx, ax
	ret
ENDP

PUBLIC C moveplz

PROC moveplz

        push    ds
	push    SEG k1
	pop     ds

	add     [ds:k1], -3d
	and     [ds:k1], 4095d
	add     [ds:k2], -2d
	and     [ds:k2], 4095d
	add     [ds:k3], 1d
	and     [ds:k3], 4095d
	add     [ds:k4], 2d
	and     [ds:k4], 4095d

	add     [ds:l1], -1d
	and     [ds:l1], 4095d
	add     [ds:l2], -2d
	and     [ds:l2], 4095d
	add     [ds:l3], 2d
	and     [ds:l3], 4095d
	add     [ds:l4], 3d
	and     [ds:l4], 4095d

	pop     ds
	ret
ENDP

LABEL	dtau Word
ccc=0
cccc=0
REPT	65				; 43=dy*512/dtý
	dw	ccc*ccc/4*43/128+60
ccc=ccc+1
ENDM

PUBLIC C fadepal
fadepal		db	768*2 dup(?)

PUBLIC C cop_fadepal
cop_fadepal	dd	0

PROC	do_drop

	inc	[cs:cop_drop]
	cmp	[cs:cop_drop], 64d
	ja	@@over

	push	bx
	mov	bx, [cs:cop_drop]
	shl	bx, 1d
	add	bx, OFFSET dtau

	mov	bx, [cs:bx]
	mov	dx, 3d4h
	mov	al, 18h		; linecompare
	mov	ah, bl
	out	dx, ax
	mov	al, 07h
	mov	ah, bh
	shl	ah, 4d
	and	ah, 10h
	or	ah, 0fh
	out	dx, ax			; 8th bit

	pop	bx
	ret

@@over:
	cmp	[cs:cop_drop], 256d
	jae	@@end
	cmp	[cs:cop_drop], 128
	jae	@@lll
	cmp	[cs:cop_drop], 64+32d
	ja	@@end

@@lll:	mov	[Word cs:cop_pal], OFFSET fadepal
	mov	[Word cs:cop_pal+2], SEG fadepal
	mov	[do_pal], 1d

	cmp	[cs:cop_drop], 65
	je	@@l5

	mov	dx, 3d4h
	mov	ax, 3c18h
	out	dx, ax
	mov	ax, 0f07h
	out	dx, ax			; 8th bit

	push	ds si di cx

	lds	si, [cs:cop_fadepal]
	mov	di, OFFSET fadepal
	mov	cx, 768/16d
@@l4:
	ccc=0
	REPT 	16
	mov	ax, [ds:si+ccc*2]
	add	[cs:di+ccc+768], al
	adc	[cs:di+ccc], ah
	ccc=ccc+1
	ENDM
	add	di, 16d
	add	si, 32d
	dec	cx
	jnz	@@l4

	pop	cx di si ds
	ret
@@l5:
	mov	dx, 3d4h
	mov	ax, 9018h
	out	dx, ax
	mov	ax, 1f07h
	out	dx, ax			; 8th bit

	call	initpparas
	ret
@@end:
	mov	[cs:cop_drop], 0d
	ret
ENDP

PROC	initpparas
	push	ax
	push    ds
	push    SEG k1
	pop     ds

	mov	ax, [ds:il1]
	mov	[ds:l1], ax
	mov	ax, [ds:il2]
	mov	[ds:l2], ax
	mov	ax, [ds:il3]
	mov	[ds:l3], ax
	mov	ax, [ds:il4]
	mov	[ds:l4], ax

	mov	ax, [ds:ik1]
	mov	[ds:k1], ax
	mov	ax, [ds:ik2]
	mov	[ds:k2], ax
	mov	ax, [ds:ik3]
	mov	[ds:k3], ax
	mov	ax, [ds:ik4]
	mov	[ds:k4], ax

	pop	ds
	pop	ax
	ret
ENDP

END


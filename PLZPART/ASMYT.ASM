	IDEAL
	MODEL large
        P386

EXTRN C l1:word, C l2:word, C l3:word, C l4:word, C k1:word, C k2:word, C k3:word, C k4:word
EXTRN C m1:word, C m2:word, C m3:word, C m4:word, C n1:word, C n2:word, C n3:word, C n4:word

SEGMENT poro para private 'CODE'

ASSUME cs:poro

PUBLIC C plzline, C setplzparas, C psini, C lsini4, C lsini16

LABEL psini BYTE
INCLUDE 'psini.inc'
;       db      16384 dup(?)

LABEL lsini4 WORD
INCLUDE 'lsini4.inc'
;       db      16384 dup(?)

LABEL lsini16 WORD
INCLUDE 'lsini16.inc'
;       db      16384 dup(?)

PROC    C plzline

        ARG     y1:word, vseg:word

        push    ds es si di

        push    [vseg]
        pop     es
        push    cs
        pop     ds
        mov     si, [y1]
;       and     si, 0fffeh
        shl     si, 1d
	mov     di, si

        IRP ccc, <3,2,1,0,7,6,5,4,11,10,9,8,15,14,13,12,19,18,17,16,23,22,21,20,27,26,25,24,31,30,29,28,35,34,33,32,39,38,37,36,43,42,41,40,47,46,45,44,51,50,49,48,55,54,53,52,59,58,57,56,63,62,61,60,67,66,65,64,71,70,69,68,75,74,73,72,79,78,77,76,83,82,81,80>

IF (ccc AND 1) EQ 1

lc2_&ccc=$+2
        mov     bx, [ds:si+0c200h]
lc1_&ccc=$+2
        mov     ah, [ds:ccc*32 + bx + 0c100h]
lc4_&ccc=$+2
        mov     bx, [ds:ccc*64 + di + 0c400h]
lc3_&ccc=$+2
        add     ah, [ds:bx + di + 0c300h]
ELSE
lc2_&ccc=$+2
        mov     bx, [ds:si+0c200h]
lc1_&ccc=$+2
        mov     al, [ds:ccc*32 + bx + 0c100h]
lc4_&ccc=$+2
        mov     bx, [ds:ccc*64 + di + 0c400h]
lc3_&ccc=$+2
        add     al, [ds:bx + di + 0c300h]
ENDIF
IF (ccc AND 3) EQ 2
        shl     eax, 16d
ENDIF
IF (ccc AND 3) EQ 0
        mov     [es:ccc], eax
ENDIF
        ENDM
        pop     di si es ds
        ret
ENDP

PROC    C setplzparas

        ARG     c1:word, c2:word, c3:word, c4:word

        IRP ccc, <0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83>
        mov     ax, [c1]
	add     ax, OFFSET psini+ccc*8
        mov     [word (cs:lc1_&ccc)], ax
        mov     ax, [c2]
	shl     ax, 1d
	add     ax, OFFSET lsini16-ccc*8+80*8
	mov     [word (cs:lc2_&ccc)], ax
	mov     ax, [c3]
	add     ax, OFFSET psini-ccc*4+80*4
	mov     [word (cs:lc3_&ccc)], ax
	mov     ax, [c4]
	shl     ax, 1d
	add     ax, OFFSET lsini4+ccc*32
	mov     [word (cs:lc4_&ccc)], ax
	ENDM
	ret
ENDP

PUBLIC C set_plzstart

PROC	C set_plzstart

	ARG	start:word

	mov	dx, 3d4h
	mov	al, 18h		; linecompare
	mov	ah, [Byte start]
	out	dx, ax
	mov	al, 07h
	mov	ah, [Byte start+1]
	shl	ah, 4d
	and	ah, 10h
	or	ah, 0fh
	out	dx, ax			; 8th bit
	ret

ENDP


ENDS
END

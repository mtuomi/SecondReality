	P386
	IDEAL

no_debug        =       1

SEGMENT koodi para public
ASSUME  cs:koodi, ds:nothing
INCLUDE 'run1.inc'
run1start=$-2

	db      12h,34h,56h,78h

PROC cstart

	mov     bx, 0d
	xor     si,si
	mov     cx, OFFSET cstart
	mov     eax, 22a53341h
@@l1:
	add     eax, 0a753cd3dh
	add     bx, 2*4
	and     bx, 3ffh
	add     eax, [dword cs:bx+randtau]
	add     bx, 23*4
	and     bx, 3ffh
	add     eax, [dword cs:bx+randtau]
	sub     bx, 24*4
	and     bx, 3ffh
	xor     [cs:si],al
	mov     [dword cs:bx+randtau], eax
	inc     si
	loop    @@l1
	jmp     run1start
ENDP
rptr    dw      0

LABEL randtau WORD
	db      12h,34h,56h,78h

;---------------------------------------------------------------------------

ORG OFFSET cstart+768d
mdd     dw      55a2h

start:
	cli
	xor     ax, ax
	mov     ds, ax
	mov     [word ds:4], OFFSET int01
	mov     [word ds:4+2], cs

	mov     [word ds:4*3], OFFSET int03
	mov     [word ds:4*3+2], cs

	mov     ax, cs
	mov     ss, ax
	mov     sp, OFFSET apino

PROC    int01
	pop     [r_ip]                  ; remove ip
	add     sp, 2d                  ; remove cs
	pusha                           ; = 8*2 bytes
	push    ds es                   ; + 2*2 bytes = total 20 bytes

ASSUME ds:koodi

	mov     ax, cs
	mov     ds, ax

	mov     bx, [pptr]
	mov     [sptau+bx], sp          ; save SP
	mov     ax, [r_ip]
	sub     ax, OFFSET runrun
	add     [iptau+bx], ax          ; save IP
	add     bx, 2d
	cmp     bx, 8d
	jbe     @@l1
	mov     bx, 0d
@@l1:   mov     [pptr], bx
	mov     sp, [sptau+bx]          ; get SP
	mov     ax, [iptau+bx]          ; get IP

	add     ax, 29787d
	mov     bp, 15553d
	mul     bp
	lea     si, [runrun]
	mov     cx, 8d
@@l2:
	mov     bx, ax
	and     bx, 511d
	mov     dl, [BYTE bx+cstart+256]
	xor     dl, 77h
	mov     [si], dl
	inc     bx
	inc     si
	add     ax, bp
	loop    @@l2

ASSUME ds:NOTHING

	pop     es ds
	popa
	push    cs
	push    OFFSET runrun
int03:
	iret
ENDP
	db      12h,34h,56h,78h                 ; end of randtau
;----------------------------------------------------------------------------

IF      randtau+1024 LT $
ERR
ENDIF

ORG     OFFSET randtau+1024

r_cnt   dw      1
r_ip    dw      ?
pptr    dw      7*2

;------------------------------------------------------------------------------

sptau   dw      OFFSET pino1, OFFSET pino2, OFFSET pino5, OFFSET pino3, OFFSET pino4

ORG     sptau+8*2                                                                   
iptau   dw      OFFSET rout1, OFFSET rout2, OFFSET rout5, OFFSET rout3, OFFSET rout4
ORG     iptau+8*2
										   

;       dw      10 dup(?)               ; free space
pino1:  dw      2 dup(?)                ; es, ds
	dw      8 dup(?)                ; di si bp sp bx dx cx ax
	dw      0100h

;       dw      10 dup(?)                ; free space
pino2:  dw      2 dup(?)                ; es, ds
	dw      8 dup(?)                ; di si bp sp bx dx cx ax
	dw      0100h

;       dw      10 dup(?)               ; free space
pino3:  dw      2 dup(?)                ; es, ds
	dw      8 dup(?)                ; di si bp sp bx dx cx ax
	dw      0100h

;       dw      10 dup(?)               ; free space
pino4:  dw      2 dup(?)                ; es, ds
	dw      8 dup(?)                ; di si bp sp bx dx cx ax
	dw      0100h

;       dw      10 dup(?)               ; free space
pino5:  dw      2 dup(?)                ; es, ds
	dw      8 dup(?)                ; di si bp sp bx dx cx ax
	dw      0100h


	dw      10h dup(?)              ; reserved
apino:  dw      OFFSET runrun
	dw      0
	dw      0


PROC    runrun
	REPT    4
	nop
	ENDM

	mov     ds, ax
	mov     es, ax
	mov     ax, 13h
	int     10h
	mov     si, OFFSET runrun
	mov     di, 0d
	rep     movsb
	mov     si, bx

	and     si, 8191
	mov     eax, [cs:bx]
	xor     eax, [cs:si]
	mov     [cs:(dword runrun)], eax
	mov     eax, [cs:bx+4]
	xor     eax, [cs:si+4]
	mov     [cs:(dword runrun+4)], eax

	int     1

	loop    runrun
ENDP

;---------------

cmp_dt  db      12 dup(?)
doexit  dw      4
nnull   dd      0

;------------------------------------------------------------------------------
	jmp     start                  
;------------------------------------------------------------------------------


MACRO scall     l1
LOCAL l2
	push    OFFSET l2
	jmp     l1
l2:
ENDM       

MACRO sret
LOCAL l2
	pop     bx
	sub     bx, OFFSET l2 - OFFSET runrun
l2:     jmp     bx        
ENDM


; This code is crypted at randtau of crypter.
; maximum length 1024 bytes.
;

	db      12h,34h,56h,78h
crypt_start:

PROC rout1
	mov     bx, OFFSET cstart
	mov     cx, 256d
@@l1:   add     ax, 7a33h
	imul    ax, 2345h
	xor     [cs:bx], al
	inc     bx
IFDEF no_debug
	int     1
ENDIF
	loop    @@l1
ENDP

PROC rout2
	mov     bx, OFFSET cstart
	mov     cx, 256d
@@l1:   add     ax, 455ch
	imul    ax, 825dh
	xor     [cs:bx], al
	inc     bx
IFDEF no_debug
	int     1
ENDIF
	loop    @@l1
ENDP

PROC    kill_sice

	REPT    8
	xor     si, si
	ENDM

	sgdt    [pword cs:cmp_dt]
	REPT    8
	xor     si, si
	ENDM
	add     eax, 23f50000h
	lea     ebp, [ebx+esi+0001fffah]
	REPT    7
	nop
	ENDM        
	sidt    [pword cs:cmp_dt+6]

	mov     cl, [cs:cmp_dt+6]        
	sub     [cs:cmp_dt], cl
	mov     cl, [cs:cmp_dt+6+1]        
	sbb     [cs:cmp_dt+1], cl
	REPT    8
	xor     si, si
	ENDM
	mov     cl, [cs:cmp_dt+6+2]        
	sbb     [cs:cmp_dt+2], cl
	mov     cl, [cs:cmp_dt+6+3]        
	sbb     [cs:cmp_dt+3], cl
	mov     cl, [cs:cmp_dt+6+4]        
	sbb     [cs:cmp_dt+4], cl
	mov     cl, [cs:cmp_dt+6+5]        
	sbb     [cs:cmp_dt+5], cl

	cmp     [word cs:cmp_dt+4], 0000h
	jne     rout3
	mov     edx, 0805f8c9h
	cmp     [dword cs:cmp_dt], edx
	je      @@dumpsice
	mov     edx, 0805f899h               ; 99f80508 0000
	cmp     [dword cs:cmp_dt], edx
	jne     @@skip
	REPT    8
	xor     si, si
	ENDM
@@dumpsice:        
IFDEF no_debug
	add     [cs:mdd],0a54dh
	add     ax, [cs:mdd]
ENDIF        
@@skip:

ENDP

PROC rout3
	mov     bx, OFFSET cstart
	mov     cx, 256d
@@l1:   add     ax, 0aa4bh
;       imul    ax, 0de88h
	xor     [cs:bx], al
	inc     bx
IFDEF no_debug
	int     1
ENDIF        
	loop    @@l1
ENDP       

PROC rout4
	dec     [doexit]
	jz      @@l2
@@l3:   
	xor     ax, ax
	dw      10 dup(0)
@@l4:   dw      10 dup(0)
	mov     si, 'FG'
	mov     di, 'JM'
	dw      5 dup(0)
	mov     ax, 0911h
IFDEF no_debug
	int     3h    
ENDIF
	jmp     @@l3

@@l2:   
	REPT    20
	xor     si, si
	ENDM
	mov     [word ds:3*4+0], OFFSET cstart
	mov     [word ds:3*4+2], cs               
IFNDEF no_debug
	int     3
ENDIF        
	jmp     rout5

	mov     ax, 4c00h
	int     21h
ENDP


PROC rout5

IFDEF no_debug
	int     01
ENDIF        
	dw      5 dup(0)
	add     [byte es:bx], 0d
	inc     bx
	dw      50 dup(0)
	jmp     rout5
ENDP

IF ($-crypt_start) GT 1000d
ERR
ENDIF

ENDS
END   


	

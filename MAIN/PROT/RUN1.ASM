	IDEAL
	P386
       
SEGMENT lerssicode

ASSUME  cs:lerssicode,ds:nothing,es:nothing

LABEL body BYTE
INCLUDE 'exebody.inc'
LABEL header BYTE
INCLUDE 'exehead.inc'

PROC    start   NEAR

	mov     ax, cs
	add     [word cs:header+0eh], ax
	add     [word cs:header+16h], ax
	sub     ax, 10h
	mov     es, ax
	mov     ds, ax

	mov     ss, [word cs:header+0eh]
	mov     sp, [word cs:header+10h]
	jmp     [dword cs:header+14h]
ENDP

	jmp     start
ENDS
END     body

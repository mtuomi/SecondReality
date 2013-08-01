	IDEAL
	P386
	
SEGMENT koodi
ASSUME  cs:koodi

ORG     100h
	
start:
	sgdt    [pword cs:erkki]
	sidt    [pword cs:erkki+6]

	mov     al, [cs:erkki+6]        
	sub     [cs:erkki], al
	mov     al, [cs:erkki+6+1]        
	sbb     [cs:erkki+1], al
	mov     al, [cs:erkki+6+2]        
	sbb     [cs:erkki+2], al
	mov     al, [cs:erkki+6+3]        
	sbb     [cs:erkki+3], al
	mov     al, [cs:erkki+6+4]        
	sbb     [cs:erkki+4], al
	mov     al, [cs:erkki+6+5]        
	sbb     [cs:erkki+5], al

	int     3

	ret
	
erkki   db      16 dup (0)


ENDS
END start

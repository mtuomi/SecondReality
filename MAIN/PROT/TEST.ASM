	IDEAL

SEGMENT lerssicode para private

string  db 'Aja t„m„ eri booteilla!',10,13,'$'
	dw      10000 dup(?)

start:
	mov     dx, 3c8h
	xor     al, al
	out     dx, al
	inc     dx
	dec     al
	out     dx, al
	out     dx, al
	out     dx, al

	mov     ah, 2       
	mov     dl, 7
	int     21h
       
	mov     dx, 3c8h
	xor     al, al
	out     dx, al
	inc     dx
	out     dx, al
	out     dx, al
	out     dx, al

	mov     ah, 9h
	mov     dx, OFFSET string
	push    cs
	pop     ds
	int     21h        

	mov     ax, 1
	int     16h       

	mov     ax, 4c00h
	int     21h

	dw      1
ENDS

END start

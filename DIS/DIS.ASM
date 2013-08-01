code 	SEGMENT para public 'CODE'
	ASSUME cs:code

rtext:	db	13,10
	db	'Demo Int Server (DIS) V1.0   Copyright (C) 1993 The Future Crew',13,10
	include disdate.inc
	db	13,10,'Installed (int fc).',13,10
	db	"NOTE: This DIS server doesn't support copper or music syncronization!",13,10
	db	'$',26

rstart:	mov	ax,cs
	mov	ds,ax
	mov	dx,OFFSET rtext
	mov	ah,9
	int	21h
	call	dis_setint
	mov	ax,3100h
	mov	dx,(rend-rstart+600)/16
	int	21h

include disint.asm

rend	LABEL BYTE
code 	ENDS
	END rstart
	

PUBLIC newdata1
data_new1 SEGMENT para public 'FARDATA'
newdata1 db 0fff0h dup(0)
data_new1 ENDS

PUBLIC background,_background
data_new2 SEGMENT para public 'FARDATA'
_background LABEL BYTE
background db 0fff0h dup(0)
data_new2 ENDS

data_v SEGMENT para public 'DATA'

PUBLIC rows
rows	dw	512 dup(0)

PUBLIC wminx,wminy,wmaxx,wmaxy
wminx	dw	0
wminy	dw	0
wmaxx	dw	100
wmaxy	dw	100

PUBLIC video,vram,rowsadd,framerate10,truevram,truerowsadd
video	dd	16 dup(0) ;far offsets to vid routines
vram	dw	0
rowsadd	dw	0
framerate10 dw	0
truevram dw	0
truerowsadd dw	0

ALIGN 4
PUBLIC projxmul,projymul,projxadd,projyadd,projminz,projminzshr
projxmul dd	0
projymul dd	0
projxadd dw	0
projyadd dw	0
projminz dd	0
projminzshr dw	0

PUBLIC color,color1,color2
color	LABEL WORD
color1	db	0
color2	db	0

ALIGN 4
PUBLIC xadd,yadd,zadd
xadd	dd	0
yadd	dd	0
zadd	dd	0

ALIGN 16 ;TEMPORARY WORK AREA FOR:
PUBLIC borders
borders LABEL WORD ;polygon drawing
	db	8192 dup(0)
workoverflow dw 00fch

data_v ENDS
	END

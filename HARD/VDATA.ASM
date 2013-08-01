
data_new1 SEGMENT para public 'FARDATA'
PUBLIC newdata1
newdata1 LABEL WORD
	 db 0fff0h dup(0)
data_new1 ENDS

data_v SEGMENT para public 'DATA'

PUBLIC rows
rows	LABEL WORD
	dw	512 dup(0)

PUBLIC wminx,wminy,wmaxx,wmaxy
wminx	dw	0
wminy	dw	0
wmaxx	dw	100
wmaxy	dw	100

PUBLIC video,vram,rowsadd,framerate10,truevram,truerowsadd
video	LABEL DWORD
	dd	16 dup(0) ;far offsets to vid routines
vram	dw	0a2d0h
rowsadd	dw	320
truevram dw	0a000h
truerowsadd dw	320
framerate10 dw	70

ALIGN 4
PUBLIC projxmul,projymul,projxadd,projyadd,projminz,projminzshr
projxmul dd	240
projymul dd	200
projxadd dw	160
projyadd dw	64
projminz dd	512
projminzshr dw	9

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

	IDEAL
	MODEL large
	P386

SEGMENT kakka2 para use16 private 'FAR_DATA'
PUBLIC C kuva1
LABEL kuva1 WORD
	db	16384 dup(?)
ENDS

SEGMENT kakka6 para use16 private 'FAR_DATA'
PUBLIC C dist1
LABEL dist1 BYTE
	dw	16384 DUP (?)
ENDS



SEGMENT kakka3 para use16 private 'FAR_DATA'
PUBLIC C kuva2
LABEL kuva2 WORD
	db	16384 dup(?)
ENDS

SEGMENT kakka4 para use16 private 'FAR_DATA'
PUBLIC C kuva3
LABEL kuva3 WORD
	db	16384 dup(?)
ENDS

SEGMENT kakka5 para use16 private 'FAR_DATA'

PUBLIC C sinit, C kosinit
LABEL sinit WORD
INCLUDE 'sinit.inc'
kosinit=sinit+512

ENDS

END
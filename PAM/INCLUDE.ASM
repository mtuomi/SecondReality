	IDEAL
	MODEL large
	P386

PUBLIC C memblock, C pal

SEGMENT kakka2 para use16 private 'FAR_DATA'
LABEL memblock WORD
INCLUDE 'out.in0'
ENDS

SEGMENT kakka3 byte use16 private 'FAR_DATA'
LABEL multable128 WORD
INCLUDE 'out.in1'
ENDS

SEGMENT kakka4 byte use16 private 'FAR_DATA'
INCLUDE 'out.in2'
ENDS

SEGMENT kakka5 byte use16 private 'FAR_DATA'
INCLUDE 'out.in3'
ENDS

SEGMENT kakka6 byte use16 private 'FAR_DATA'
INCLUDE 'out.in4'
ENDS

SEGMENT kakka8 byte use16 private 'FAR_DATA'
LABEL pal BYTE
INCLUDE 'pal.inc'
	db	768*64 dup(?)
ENDS
END
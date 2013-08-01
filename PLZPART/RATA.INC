; spline dataa...

;dw     0,0,10000,512,0,0,0,0
;dw     0,0,10000,512,-2000,0,0,0
;dw     0,0,5000,512,1000,0,0,0

;	dx, dy, dz, kx, ky, kz, l_kx, l_ky

kkk=100

dw      0,2000,500,	kkk*0 ,kkk*4 ,kkk*6 ,	0,0
dw      0,2000,500,	kkk*1 ,kkk*5 ,kkk*7 ,	0,0
dw      0,2000,500,	kkk*2 ,kkk*6 ,kkk*8 ,	0,0
dw      0,2000,500,	kkk*3 ,kkk*7 ,kkk*7 ,	0,0
dw      0,2000,500,	kkk*4 ,kkk*8 ,kkk*6 ,	0,0
dw      0,-150,500,	kkk*5 ,kkk*7 ,kkk*5 ,	0,0
dw      0,0,500,	kkk*6 ,kkk*6 ,kkk*4 ,	0,0
dw      0,0,500,	kkk*7 ,kkk*5 ,kkk*3 ,	0*32,0
dw      0,0,500,	kkk*8 ,kkk*4 ,kkk*2 ,	1*32,0
dw      0,0,500,	kkk*7 ,kkk*3 ,kkk*1 ,	2*32,0
dw      0,0,500,	kkk*6 ,kkk*2 ,kkk*0 ,	3*32,0
dw      0,0,450,	kkk*5 ,kkk*1 ,kkk*1 ,	4*32,0
dw      0,0,400,	kkk*4 ,kkk*0 ,kkk*2 ,	5*32,0
dw      0,0,350,	kkk*3 ,kkk*1 ,kkk*3 ,	6*32,0
dw      0,0,300,	kkk*2 ,kkk*2 ,kkk*4 ,	7*32,0
dw      0,0,300,	kkk*1 ,kkk*3 ,kkk*5 ,   8*32,0

dw      0,0,300,	kkk*0 ,kkk*4 ,kkk*6 ,	256,0
dw      0,0,300,	kkk*1 ,kkk*5 ,kkk*7 ,	256,0
dw      0,0,300,	kkk*2 ,kkk*6 ,kkk*8 ,	256,0
dw      0,0,300,	kkk*3 ,kkk*7 ,kkk*7 ,	256,0
dw      0,0,300,	kkk*4 ,kkk*8 ,kkk*6 ,	256,0
dw      0,0,300,	kkk*5 ,kkk*7 ,kkk*5 ,	256,0
dw      0,0,300,	kkk*6 ,kkk*6 ,kkk*4 ,	256,0
dw      0,0,300,	kkk*7 ,kkk*5 ,kkk*3 ,	256,0
dw      0,0,300,	kkk*8 ,kkk*4 ,kkk*2 ,	256,0
dw      0,0,300,	kkk*7 ,kkk*3 ,kkk*1 ,	256,0
dw      0,0,300,	kkk*6 ,kkk*2 ,kkk*0 ,	256,0
dw      0,0,300,	kkk*5 ,kkk*1 ,kkk*1 ,	256,0
dw      0,0,300,	kkk*4 ,kkk*0 ,kkk*2 ,	256,0
dw      0,0,300,	kkk*3 ,kkk*1 ,kkk*3 ,	256,0
dw      0,0,300,	kkk*2 ,kkk*2 ,kkk*4 ,	256,0
dw      0,0,300,	kkk*1 ,kkk*3 ,kkk*5 ,	256,0

dw      0,0,350,	kkk*0 ,kkk*4 ,kkk*4 ,	0*64,128
dw      0,0,400,	kkk*1 ,kkk*3 ,kkk*3 ,	1*64,256
dw      0,0,450,	kkk*2 ,kkk*2 ,kkk*2 ,	2*64,384
dw      0,0,500,	kkk*1 ,kkk*1 ,kkk*1 ,	3*64,512

REPT	100
dw      0,0,500,	kkk*0 ,kkk*0 ,kkk*0 ,	256,512
ENDM

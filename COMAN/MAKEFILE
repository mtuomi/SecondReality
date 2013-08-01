all : doloop.exe c.exe 

objs=main.obj asm.obj theloop.obj ..\dis\disc.obj 
dobjs=_w1dta1.obj _w1dta2.obj _w2dta1.obj _w2dta2.obj

asm_f	= /ML /m2 /s /JJUMPS
#c_f	= /AL /c /W1 /qc 
c_f	= /AL /c /W1 /Oxaz

.asm.obj : 
	tasm $(asm_f) $<
	
.c.obj : 
	cl $(c_f) $<

c.exe : $(objs)
	link /E $(objs)+$(dobjs),c.exe,nul;
	copy c.exe ..\main\data\3dsinfld.exe

doloop.exe : doloop.c
	mkc doloop
	doloop
	
theloop.asm : theloop.inc

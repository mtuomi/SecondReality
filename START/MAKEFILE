all : s.exe 

objs=main.obj _hzpic.obj asm.obj

asm_f	= /ML /m2 /s /JJUMPS
c_f	= /AL /c /W2 /Oxaz

.asm.obj : 
	tasm $(asm_f) $<
	
.c.obj : 
	cl $(c_f) $<

s.exe : $(objs)
	link /E $(objs),s.exe,nul;



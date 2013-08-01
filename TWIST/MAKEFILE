all : t.exe 

objs=main.obj twist.obj asm.obj ..\dis\disc.obj

asm_f	= /ML /m2 /s /JJUMPS
#c_f	= /AL /c /W1 /qc 
c_f	= /AL /c /W1 /Oxaz

.asm.obj : 
	tasm $(asm_f) $<
	
.c.obj : 
	cl $(c_f) $<

t.exe : $(objs)
	link /E $(objs),t.exe,nul;

twist.asm : twstloop.inc

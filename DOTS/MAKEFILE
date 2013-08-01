all : dots.exe face.exe

face.exe : face.c
	cl /qc /AL face.c
	face > face.inc

objs=main.obj asm.obj ..\dis\disc.obj

asm_f	= /ML /m2 /s /JJUMPS
c_f	= /AL /c /W3

.asm.obj : 
	tasm $(asm_f) $<
	
.c.obj : 
	cl /qc $(c_f) $<

dots.exe : $(objs)
	link /E $(objs),dots.exe,nul;
	copy dots.exe ..\main\data\minvball.exe
	
asm.asm : face.inc



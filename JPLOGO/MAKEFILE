all : jp.exe 

# objects

objs=jp.obj asm.obj

asm.asm : zoom.inc

# option flags

asm_f	= /ML /m9 /s /JJUMPS
c_f	= /AL /c /W3 /Oxaz

# general compile orders

.asm.obj : 
	tasm $(asm_f) $<
	
.c.obj : 
	cl $(c_f) $<

jp.exe : $(objs)
	link /E $(objs)+_pic.obk+..\dis\disc.obj,jp.exe,nul;
	copy jp.exe ..\main\data\jplogo.exe


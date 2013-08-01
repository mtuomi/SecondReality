all : end.exe 

# objects

objs=end.obj asm.obj _pic.obk

# option flags

asm_f	= /ML /m9 /s /JJUMPS
c_f	= /AL /c /W3

# general compile orders

.asm.obj : 
	tasm $(asm_f) $<
	
.c.obj : 
	cl /qc $(c_f) $<

end.exe : $(objs)
	link /E $(objs)+..\dis\disc.obj,end.exe,nul;
	copy end.exe ..\main\data\endlogo.exe
	


all : beg.exe 

# objects

objs=beg.obj asm.obj

# option flags

asm_f	= /ML /m9 /s /JJUMPS
c_f	= /AL /c /W3

# general compile orders

.asm.obj : 
	tasm $(asm_f) $<
	
.c.obj : 
	cl /qc $(c_f) $<

beg.exe : $(objs)
	link /E $(objs)+_pic.obk+..\dis\disc.obj,beg.exe,nul;
	copy beg.exe ..\main\data\beglogo.exe
	


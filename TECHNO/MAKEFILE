all : koe.exe 

# objects

objs=koe.obj koea.obj koeb.obj
data=_circle.obk _circle2.obk 
#_pic1.obk _pic2.obk

# option flags

asm_f	= /ML /m9 /s /JJUMPS
c_f	= /AL /c /W3

# general compile orders

.asm.obj : 
	tasm $(asm_f) $<
	
.c.obj : 
	cl /qc $(c_f) $<

koe.exe : $(objs)
	link /E $(objs)+$(data)+..\dis\disc.obj,koe.exe,koe.map;
	copy koe.exe ..\main\data\techno.exe
	


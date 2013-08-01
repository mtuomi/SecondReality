all : v.exe edge.exe

objs=zoomer.obj main.obj vid.obj zoom.obj adata.obj asm.obj vec.obj math.obj _fc.obk ..\dis\disc.obj

asm_f	= /ML /m2 /s /JJUMPS
c_f	= /AL /c /Oxaz /W2

.asm.obj : 
	tasm $(asm_f) $<
	
.c.obj : 
	cl /qc $(c_f) $<

v.exe : $(objs)
	link /E $(objs),v.exe,v.map,gl10.lib;
	copy v.exe ..\main\data\glenz.exe

vid.asm : vidpoly.asm vidinit.asm vidmisc.asm vidtwe.asm vidnrm.asm
vec.asm : veccmd.asm
vid.asm : new.asm
main.c : zoomer.c

edge.exe : edge.c
	cl /qc /AL edge.c

zoom.asm : zoomloop.inc

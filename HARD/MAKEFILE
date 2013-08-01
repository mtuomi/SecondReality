all : b.exe edge.exe bdo.exe

objs1=asm.obj vec.obj
objs2=b.obj ba.obj

objs=main.obj $(objs1)

odata=_dots1.obj _dots2.obj _dots3.obj _dots4.obj _grid1.obj

asm_f	= /ML /m2 /s /JJUMPS
c_f	= /AL /c /G2

.asm.obj : 
	tasm $(asm_f) $<
	
.c.obj : 
	cl /qc $(c_f) $<

v.exe : $(objs)
	link /E $(objs),v.exe,nul,gl10.lib;
	
b.exe : $(objs2) $(objs1)
	@echo $(objs1) + > link.tmp
	@echo $(objs2) + >> link.tmp
	@echo $(odata) ..\dis\disc.obj,b.exe,nul; >> link.tmp
	link /E @link.tmp
	
bdo.exe : bdo.c
	cl /QC /AL bdo.c
	
odata : bdo.exe
	bdo 
	doobj _dots1.tmp _dots1 _dots1.obj
	doobj _dots2.tmp _dots2 _dots2.obj
	doobj _dots3.tmp _dots3 _dots3.obj
	doobj _dots4.tmp _dots4 _dots4.obj
	del *.tmp

b.c : bp.c

vec.asm : vnew.asm vmath.asm vdata.asm vasm.inc

edge.exe : edge.c
	cl /qc /AL edge.c
	
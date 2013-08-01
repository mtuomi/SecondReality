all : p.exe

objs=main.obj asm.obj lineblit.obj

odata=_eye.obk

lineblit.asm : lineblit.inc

################################################################

asm_f	= /ML /m2 /s /JJUMPS
c_f	= /AL /c /G2

.asm.obj : 
	tasm $(asm_f) $<
	
.c.obj : 
	cl /qc $(c_f) $<

p.exe : $(objs)
	@echo $(objs) + > link.tmp
	@echo $(odata) ..\dis\disc.obj,p.exe,nul; >> link.tmp
	link /E @link.tmp

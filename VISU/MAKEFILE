all : visu.lib

# objects

aobjs=amain.obj adata.obj acalc.obj avid.obj adraw.obj

objs=visux.obj visu.obj $(aobjs)

# option flags

asm_f	= /ML /m9 /s /JJUMPS /Zi
c_f	= /AL /c /W3 /Zi

# general compile orders

.asm.obj : 
	tasm $(asm_f) $<
	
.c.obj : 
	cl /qc $(c_f) $<

visu.lib : $(objs)
	del visu.lib
	lib visu.lib +$(objs) , visu.lst

# dependencies

avid.asm : avidm1.asm avidm2.asm atext.asm avidfill.asm

adraw.asm : adrawclp.asm

adata.asm : adatasin.inc afilldiv.inc

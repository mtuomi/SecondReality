all : lens.exe calc.exe

#after lensexb, there must be 4K of data (filler) for 64K overflow
data1=_lensexb.obk _filler.obk _lensex1.obk _lensex0.obk 
data2=_lensex2.obk _lensex3.obk _lensex4.obk _lensexp.obk
code1=main.obj asm.obj ..\dis\disc.obj
objs=$(code1) $(data1) $(data2)

asm_f	= /ML /m2 /s /JJUMPS
#c_f	= /AL /c /W1 /qc
c_f	= /AL /c /W1 /Oxaz

.asm.obj :
	tasm $(asm_f) $<
	
.c.obj :
	cl $(c_f) $<

lens.exe : $(objs)
	echo /E + > link.tmp 
	echo $(code1) + >> link.tmp
	echo $(data1) + >> link.tmp
	echo $(data2) >> link.tmp
	echo ,lens.exe,lens.map; >> link.tmp
	link @link.tmp
	copy lens.exe ..\main\data\lns&zoom.exe

calc.exe : calc.c
	cl /W3 /AL /qc calc.c /link /E

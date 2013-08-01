all : u2.exe data\start.exe data\startmus.exe 

objs1=u2.obj stmik.300

objs2=start.obj menu.obj

objs3=startmus.obj 

u2.asm : u2a.asm ..\dis\disint.asm copper.asm packing.inc vmode.asm

startmus.c : startems.c

asm_f	= /ML /m9 /s /JJUMPS
c_f	= /AL /c /W1

################################################################


################################################################

.asm.obj : 
	tasm $(asm_f) /dFINAL=1 $<
	
.c.obj : 
	cl /qc $(c_f) $<
	
u2.exe : $(objs1)
	link /E /CP:1 $(objs1),u2.exe,u2.map;

final : $(objs1)
	cd data
	pklite *
	cd ..
	tasm /ML /m9 /s /JJUMPS /dFINAL=1 u2.asm
	link /E /CP:1 $(objs1),u2.exe,u2.map;
	lzexe u2.exe
	fcprot u2.exe -1
	touch u2.exe
	copy u2.exe prot\test.exe
	cd prot
	f:\ta\make
	cd ..
	copy prot\prot.exe u2.exe
	lzexe u2.exe
	fcprot u2.exe -1
	dir u2.exe

data\start.exe : $(objs2)
	link /E /CP:1 $(objs2)+..\dis\disc.obj,data\start.exe;

data\startmus.exe : $(objs3)
	link /E /CP:1 $(objs3)+..\dis\disc.obj,data\startmus.exe;


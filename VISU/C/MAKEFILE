all : c.exe cplay.exe city.exe srvec.exe u2a.exe u2e.exe 
#all : u2e.exe

# objects

objs=main.obj

main.c : readvue.c readinf.c readmat.c readasc.c util.c opt.c save.c osort.c

# option flags

asm_f	= /ML /m9 /s /JJUMPS
cdeb_f	= /AL /c /W2 /Zi
c_f	= /AL /Oxaz /W2 /c

# general compile orders

.asm.obj : 
	tasm $(asm_f) $<
	
.c.obj : 
	cl /qc $(c_f) $<

c.exe : $(objs) ..\visu.lib
	link /CO /E /ST:8192 $(objs),c.exe,nul,..\visu.lib;

cplay.obj : cplay.c
	cl $(c_f) cplay.c

cplay.exe : cplay.obj ..\visu.lib
	link /CO /E /ST:2048 cplay.obj,cplay.exe,nul,..\visu.lib;
	
city.exe : city.obj ..\visu.lib
	link /CO /E /ST:2048 city.obj,city.exe,nul,..\visu.lib;

srvec.exe : srvec.obj ..\visu.lib
	link /CO /E /ST:2048 srvec.obj,srvec.exe,nul,..\visu.lib;

u2a.exe : u2a.obj ..\visu.lib
	link /E /ST:2048 u2a.obj+_bg.obk+..\..\dis\disc.obj,u2a.exe,nul,..\visu.lib;
	copy u2a.exe ..\..\main\data

u2e.exe : u2e.obj ..\visu.lib
	link /E /ST:2048 u2e.obj ..\..\dis\disc.obj,u2e.exe,nul,..\visu.lib;
	copy u2e.exe ..\..\main\data


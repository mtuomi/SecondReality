all : dis.exe distest.exe

objs=

asm_f	= /ML /m2 /s /JJUMPS
c_f	= /AL /c /W3

.asm.obj : 
	tasm $(asm_f) $<
	
.c.obj : 
	cl /qc $(c_f) $<
	
dis.obj : dis.asm disint.asm
	..\util\dodate "db 'BETA VERSION - Compiled:" "'" > disdate.inc
	tasm $(asm_f) dis.asm 

dis.exe : dis.obj
	link dis.obj,dis.exe;
	
distest.exe : distest.obj disc.obj
	link /E distest.obj+disc.obj,distest.exe;

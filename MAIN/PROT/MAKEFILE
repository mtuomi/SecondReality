prot.exe: run3.obj
	tlink run3.obj,prot.exe

run3.obj: run3.asm run2.inc
	tasm run3.asm

run2.inc: run2.obj crypt.exe
	tlink /t run2.obj,run2.bin
	-crypt run2.bin
	-incbin run2.bin run2.inc

run2.obj: run2.asm run1.inc
	tasm /m9 /l run2.asm             

run1.inc: run1.obj
	tlink /t run1.obj,run1.bin
	-incbin run1.bin run1.inc
	
run1.obj: run1.asm exebody.inc exehead.inc
	tasm /m9 run1.asm
	
exebody.inc exehead.inc: test.exe split.exe
	-split test.exe
	-incbin exebody.dat exebody.inc
	-incbin exehead.dat exehead.inc
	
test.exe: test.asm
	tasm test
	tlink test

crypt.exe: crypt.c asmyt.asm 
	bcc crypt.c asmyt.asm
	
split.exe: split.c
	bcc split.c


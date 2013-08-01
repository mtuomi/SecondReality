k.exe : koe.obj 
	link /E ..\dis\disc.obj+_textpic.obk+koe.obj,k.exe,koe.map;
	copy k.exe ..\main\data\ddstars.exe

koe.obj : koe.asm
	tasm /ML /m9 koe.asm

koe.asm : stars.asm

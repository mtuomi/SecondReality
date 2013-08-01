#include <stdio.h>

char *vram=(char *)0xa0000000L;

main()
{
	unsigned int seed;
	_asm mov ax,13h
	_asm int 10h
	_asm int 3
	rand();
	while(!kbhit())
	{
		vram[seed]++;
		_asm
		{
			mov	ax,seed
			shl	ax,1
			jnc	l1
			add	ax,0f7ffh
		l1:	mov	seed,ax
		}
	}
	getch();
	_asm mov ax,3h
	_asm int 10h
}
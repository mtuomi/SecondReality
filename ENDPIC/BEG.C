#include <stdio.h>
#include "..\dis\dis.h"

#include "readp.c"

extern char pic[];

char *vram=(char *)0xa0000000L;

char	pal2[768];
char	palette[768];
char	rowbuf[640];

main()
{
	int	a,b,c,y;
	dis_partstart();
	_asm mov ax,13h
	_asm int 10h	
	inittwk();
	_asm
	{
		mov	dx,3c0h
		mov	al,11h
		out	dx,al
		mov	al,255
		out	dx,al
		mov	al,20h
		out	dx,al
	}

	readp(palette,-1,pic);
	for(y=0;y<400;y++)
	{
		readp(rowbuf,y,pic);
		lineblit(vram+(unsigned)y*80U,rowbuf);
	}
	
	for(c=0;c<=128;c++)
	{
		for(a=0;a<768-3;a++) pal2[a]=((128-c)*63+palette[a]*c)/128;
		dis_waitb();
		setpalarea(pal2,0,255);
	}
	for(a=0;a<300 && !dis_exit();a++)
	{
		dis_waitb();
		if(dis_muscode(0xf0)==0xf0) break;
	}
}

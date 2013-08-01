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
	outp(0x3c4,2);
	outp(0x3c5,15);
	memset(vram,15,32768);
	memset(vram+32768,15,32768);
	//_asm mov ax,80h+13h
	//_asm int 10h
	for(a=0;a<32;a++) dis_waitb();
	outp(0x3c8,0);
	for(a=0;a<255;a++)
	{
		outp(0x3c9,63);
		outp(0x3c9,63);
		outp(0x3c9,63);
	}
	outp(0x3c9,0);
	outp(0x3c9,0);
	outp(0x3c9,0);
	inp(0x3da);
	outp(0x3c0,0x11);
	outp(0x3c0,255);
	outp(0x3c0,0x20);
	//inittwk();
	_asm
	{
		mov	dx,3d4h
		mov	ax,000ch
		out	dx,ax
		mov	ax,000dh
		out	dx,ax
		mov	al,9
		out	dx,al
		inc	dx
		in	al,dx
		and	al,not 80h
		and	al,not 31
		out	dx,al
		mov	dx,3c0h
		mov	al,11h
		out	dx,al
		mov	al,0
		out	dx,al
		mov	al,32
		out	dx,al
	}
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
		setpalarea(pal2,0,254);
	}
	setpalarea(palette,0,254);
}

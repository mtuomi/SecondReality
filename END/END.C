#include <stdio.h>
#include "..\dis\dis.h"

#include "readp.c"

extern char pic[];

char *vram=(char *)0xa0000000L;

char *scroller[]={
"      S e c o n d  R e a l i t y     ",
"                                     ",
"     has entirely been created by    ",
"       the Future Crew in 1993       ",
"                                     ",
"         First presented at          ",
"       Assembly 1993 - Finland       ",
/*"                                     ",
"                                     ",
"          credits...                 ",
"                                     ",
"      (this is surely the best       ",
"       endscroller ever!)            ",
"                                     ",
"                                     ",
"                                     ",
"                                     ",
"                                     ",
"                                     ",
"                                     ",
"                                     ",
"                                     ",
"                                     ",
"                                     ",
"                                     ",
"                                     ",
"                                     ",
"                                     ",
"                                     ",
"                                     ",
*/};

char	pal2[768];
char	palette[768];
char	rowbuf[640];

main()
{
	int	a,b,c,y;
	dis_partstart();
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
	dis_waitb();
	outp(0x3c8,0);
	for(a=0;a<768-3;a++) outp(0x3c9,63);
//	_asm mov ax,13h+80h
//	_asm int 10h	
//	inittwk();
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
	dis_waitb();
	outp(0x3c8,0);
	for(a=0;a<768-3;a++) outp(0x3c9,63);
	for(a=0;a<32;a++) dis_waitb();

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
	for(a=0;a<5000 && !dis_exit();a++)
	{
		dis_waitb();
		if(dis_musplus()>-16) break;
	}
	for(c=63;c>=0;c--)
	{
		for(a=0;a<768-3;a++) pal2[a]=(palette[a]*c)/64;
		dis_waitb();
		setpalarea(pal2,0,255);
	}
	/*
	_asm mov ax,4
	_asm int 10h
	outp(0x3c8,0); outp(0x3c9,0); outp(0x3c9,0); outp(0x3c9,0);
	for(a=0;a<768-3;a++) outp(0x3c9,40);
	for(b=0;b<25;b++) printf("\n");
	for(a=0;a<24 && !kbhit();a++)
	{
		printf("%s\n",scroller[a]);
		for(b=0;b<20;b++) dis_waitb();
	}
	*/
}

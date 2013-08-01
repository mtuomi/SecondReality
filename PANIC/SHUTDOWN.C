#include <stdio.h>
#include <conio.h>
#include <fcntl.h>
#include <dos.h>
#include <math.h>
#include "i:\u2\dis\dis.h"

extern void tw_opengraph();
extern void tw_closegraph();
extern void tw_putpixel(int x, int y, int color);
extern int  tw_getpixel(int x, int y);
extern void tw_setpalette(void far *pal);
extern void tw_setpalarea(void far *pal,int start,int cnt);
extern void tw_setrgbpalette(int pal, int r, int g, int b);
extern void tw_setstart(int start);
extern void tw_pictovmem(void far *pic, int to, int len);
extern void tw_crlscr();

#define	_LOADER_

extern copyline(char far *from, char far *to, int count);

#ifndef _LOADER_CALLS
char	far kuva[65000];
#endif
char	far kuvapal[768];
char	far pal[768];
char	fadepals[64][768];

main()  {
	int	fff,x,y;

	dis_partstart();

#ifndef _LOADER_
	tw_opengraph();
	tw_setstart(80);
	fff=open("monster.u",O_BINARY|O_RDONLY);
	read(fff,kuva,64000);
	close(fff);
	fff=open("monster.pal",O_BINARY|O_RDONLY);
	read(fff,kuvapal,768);
	close(fff);

	for(y=0;y<200;y++) for(x=0;x<320;x++)
		{
		tw_putpixel(x+320,y*2,kuva[y*320+x]);
		tw_putpixel(x+320,y*2+1,kuva[y*320+x]);
		}
	tw_setpalette(kuvapal);
#else
	getpal(kuvapal);
#endif
	shutdown();
	}

shutdown()
	{
	int	x,y,a,b;

	for(a=0;a<320;a++) tw_putpixel(a,0,0);
	for(a=0;a<64;a++) for(b=3;b<768;b++) fadepals[a][b]=(a*63+kuvapal[b]*(64-a))/64;
	for(y=0;y<100;y++) for(x=0;x<320;x++)
		tw_putpixel(x,y+150,tw_getpixel(x+320,y*4));
	tw_setstart(100*160);
	dis_waitb();
	asm {
		mov	dx, 3d4h
		mov	ax, 4109h
		out	dx, ax

		mov	dx, 03ceh
		mov	ax, 4105h
		out	dx, ax			// latch transfer mode

		mov	dx, 03c4h
		mov	ax, 0f02h		// map mask 1111
		out	dx, ax
		}
	tw_setpalette(fadepals[3]);
	tw_setrgbpalette(0,63,63,63);
	tw_setstart(0);
	dis_waitb();
	tw_setpalette(fadepals[20]);
	asm {
		mov	dx, 3d4h
		mov	ax, 0a013h
		out	dx, ax
		}
	dis_waitb();
	for(a=32;a>2;a=a*5/6)
		{
		dis_waitb();
		tw_setpalette(fadepals[63-a]);
		for(b=a/2;b<=a;b++)
			{
			copyline(MK_FP(0x0a000,0),MK_FP(0x0a000,200*160-(b*320)),80);
			copyline(MK_FP(0x0a000,0),MK_FP(0x0a000,200*160+(b*320)),80);
			}
		for(b=0;b<a;b++)
			copyline(MK_FP(0x0a000,80+(400*b/a)*160),MK_FP(0x0a000,200*160+(b-a/2)*320),80);
		}

	copyline(MK_FP(0x0a000,0),MK_FP(0x0a000,202*160),80);
	copyline(MK_FP(0x0a000,0),MK_FP(0x0a000,198*160),80);
	asm {
		mov	dx, 03ceh
		mov	ax, 4005h
		out	dx, ax			// latch transfer mode

		mov	dx, 03c4h
		mov	ax, 0f02h		// map mask 1111
		out	dx, ax
		}

	for(x=20;x<=160;x+=3)
		{
		dis_waitb();
		tw_putpixel(x,200,0);
		tw_putpixel(320-x,200,0);
		tw_putpixel(x+1,200,0);
		tw_putpixel(319-x,200,0);
		tw_putpixel(x+2,200,0);
		tw_putpixel(318-x,200,0);
		tw_putpixel(x+3,200,0);
		tw_putpixel(317-x,200,0);
		}

	tw_putpixel(160,200,1);
	for(a=0;a<60;a++)
		{
		dis_waitb();
		b=cos(a/120.0*3*2*3.1415926535)*31.0+32;
		tw_setrgbpalette(1,b,b,b);
		}
	sleep(1);
	}

getpal(char far *pal)
	{
	asm {
		push	di
		push	es

		les	di, pal
		mov	cx, 768d
		mov	dx, 3c7h
		mov	al, 0d
		out	dx, al
		inc	dx
		inc	dx
		rep	insb

		pop	es
		pop	di
		}
	}

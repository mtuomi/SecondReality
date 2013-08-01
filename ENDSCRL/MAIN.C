#include <dos.h>
#include <io.h>
#include <fcntl.h>
#include "i:\u2\dis\dis.h"

#define FONAY 30

extern	void waitvr(void);
extern	void setstart(int);
extern	void setrgbpalette(int p, int r, int g, int b);

extern char far font[31][1500];
char	*fonaorder="ABCDEFGHIJKLMNOPQRSTUVWXabcdefghijklmnopqrstuvwxyz0123456789!?,.:èè()+-*='";
int	fonap[256];
int	fonaw[256];

char	tbuf[FONAY][640];

main()  {
	dis_partstart();
	dis_waitb();
	asm	mov	ax, 0eh
	asm   	int 	10h	      	// 640x350 16c
	asm	mov	ax, 9h
	asm	mov	dx, 3d4h
	asm	out	dx, ax
	setrgbpalette(1,20,20,20);
	setrgbpalette(2,40,40,40);
	setrgbpalette(3,60,60,60);
	setrgbpalette(4,60,60,60);
	setrgbpalette(5,60,60,60);
	setrgbpalette(6,60,60,60);
	setrgbpalette(7,60,60,60);
	setrgbpalette(8,60,60,60);
	setrgbpalette(9,60,60,60);
	setrgbpalette(10,60,60,60);
	setrgbpalette(11,60,60,60);
	setrgbpalette(12,60,60,60);
	setrgbpalette(13,60,60,60);
	setrgbpalette(14,60,60,60);
	setrgbpalette(15,60,60,60);

	init();

	while(!dis_exit())
		{
		if(dis_waitb()==1) dis_waitb();
		do_scroll();
		}
	}

char	far text[64000]="Testing Testing\0PIXEL SUCKS\0ABCDEFGHIJKLMNOPQRSTUVWXYZ\0abcdefghijklmnopqrsuvwxyz\0Testing Testing\0PIXEL SUCKS\0ABCDEFGHIJKLMNOPQRSTUVWXYZ\0abcdefghijklmnopqrsuvwxyz\0Testing Testing\0PIXEL SUCKS\0ABCDEFGHIJKLMNOPQRSTUVWXYZ\0abcdefghijklmnopqrsuvwxyz\0Testing Testing\0PIXEL SUCKS\0ABCDEFGHIJKLMNOPQRSTUVWXYZ\0abcdefghijklmnopqrsuvwxyz\0Testing Testing\0PIXEL SUCKS\0ABCDEFGHIJKLMNOPQRSTUVWXYZ\0abcdefghijklmnopqrsuvwxyz\0Testing Testing\0PIXEL SUCKS\0ABCDEFGHIJKLMNOPQRSTUVWXYZ\0abcdefghijklmnopqrsuvwxyz\0";
char	*tptr=text;
int	tstart=0,chars=0;
int	mtau[8]={128,64,32,16,8,4,2,1};

char	textline[100];
char	scanbuf[4][80];

do_scroll()
	{
	static int yscrl=0;
	static int line=0;
	int	a,b,c,x,y,m;

	if(line==0)
		{
		for(a=0,tstart=0,chars=0;*tptr!='\n';a++,chars++)
			{
			textline[a]=*tptr;
			tstart+=fonaw[*tptr++]+2;
			}
		textline[a]=*tptr++; tstart=(639-tstart)/2;
		}
	memset(scanbuf,0,80*4);

	for(a=0,x=tstart;a<chars;a++,x+=2) for(b=0;b<fonaw[textline[a]];b++,x++)
		{
		m=mtau[x&7];
		if(font[line][fonap[textline[a]]+b]&1)
			scanbuf[0][x/8]^=m;
		if(font[line][fonap[textline[a]]+b]&2)
			scanbuf[1][x/8]^=m;
		if(font[line][fonap[textline[a]]+b]&4)
			scanbuf[2][x/8]^=m;
		if(font[line][fonap[textline[a]]+b]&8)
			scanbuf[3][x/8]^=m;
		}
	outport(0x3c4,0x0102); memcpy(MK_FP(0x0a000,80*yscrl),scanbuf[0],80); memcpy(MK_FP(0x0a000,80*(yscrl+401)),scanbuf[0],80);
	outport(0x3c4,0x0202); memcpy(MK_FP(0x0a000,80*yscrl),scanbuf[1],80); memcpy(MK_FP(0x0a000,80*(yscrl+401)),scanbuf[1],80);
	outport(0x3c4,0x0402); memcpy(MK_FP(0x0a000,80*yscrl),scanbuf[2],80); memcpy(MK_FP(0x0a000,80*(yscrl+401)),scanbuf[2],80);
	outport(0x3c4,0x0802); memcpy(MK_FP(0x0a000,80*yscrl),scanbuf[3],80); memcpy(MK_FP(0x0a000,80*(yscrl+401)),scanbuf[3],80);
	yscrl=(yscrl+1)%401;
	line=(line+1)%FONAY;
	setstart(yscrl*80+80*1);
	}
init()
	{
	int	x,y,a,b;

	a=open("endscrol.txt",O_RDONLY); read(a,text,60000); close(a);

	for(x=0;x<1500 && *fonaorder;)
	{
		while(x<1500)
		{
			for(y=0;y<FONAY;y++) if(font[y][x]) break;
			if(y!=FONAY) break;
			x++;
		}
		b=x;
		while(x<1500)
		{
			for(y=0;y<FONAY;y++) if(font[y][x]) break;
			if(y==FONAY) break;
			x++;
		}
		//printf("%c: %i %i\n",*fonaorder,b,x-b);
		fonap[*fonaorder]=b;
		fonaw[*fonaorder]=x-b;
		fonaorder++;
	}
	fonap[32]=1500-20;
	fonaw[32]=16;
	}

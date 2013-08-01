#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include "..\dis\dis.h"

static char *vram=(char *)0xa0000000L;

static char *bg;

extern void zoom(char *to,char *from,int factor);

extern int sin1024[];

static char pal1[768];
static char pal2[768];

void	zoomer2(char *pic)
{
	int	a,b,c,y;
	char *v;
	int	frame=0;
	int	zly,zy,zya;
	int	zly2,zy2;

	_asm
	{
		mov	dx,3c4h
		mov	ax,0f02h
		out	dx,ax
	}	
	outp(0x3c7,0);
	for(a=0;a<768;a++) pal1[a]=inp(0x3c9);

	zy=0; zya=0; zly=0;
	zy2=0; zly2=0;
	frame=0;
	while(!kbhit())
	{
		if(zy==260) break;
		zly=zy;
		zya++;
		zy+=zya/4;
		if(zy>260) zy=260;
		v=vram+zly*80;
		for(y=zly;y<=zy;y++)
		{
			memset(v,255,80);
			v+=80;
		}
		zly2=zy2;
		zy2=125*zy/260;
		v=vram+(399-zy2)*80;
		for(y=zly2;y<=zy2;y++)
		{
			memset(v,255,80);
			v+=80;
		}
		c=frame;
		if(c>32) c=32;
		b=32-c;
		for(a=0;a<128*3;a++)
		{
			pal2[a]=(pal1[a]*b+30*c)>>5;
		}
		frame++;
		dis_waitb();
		setpalarea(pal2,0,128);
	}
	v=vram+(194)*80;
	outp(0x3c8,0);
	outp(0x3c9,0);
	outp(0x3c9,0);
	outp(0x3c9,0);
	v=vram+(0)*80;
	for(y=0;y<=399;y++)
	{
		if(y<=274 || y>=260) memset(v,0,80);
		v+=80;
	}
}

void	zoomer1(char *pic)
{
	int	dist=320,q;
	int	y1,y2,z1,z2,yd;
	int	y,z,f=0,frame=0;
	int	rot;
	bg=pic;
	for(y=199;y>=0;y--)
	{
		memmove(bg+326*y,bg+320*y,320);
		memset(bg+326*y+320,0,4);
	}
	for(q=0;q<70;q++) dis_waitb();
	for(q=0;q<320;q++) 
	{
		bg[200*326+q]=7;
	}
	while(!kbhit() && frame<=128)
	{
		q=(int)((long)frame*(long)frame/128L)*2;
		dist=320-(320-245)*frame/128;
		rot=768-q;
		y1=102+sin1024[rot&1023]*25/64;
		z1=400+sin1024[(rot+256)&1023]*25/64;
		y2=102+sin1024[(rot+512)&1023]*25/64;
		z2=400+sin1024[(rot+768)&1023]*25/64;
		// convert z to scale
		z1=(int)((long)dist*400L/(long)z1);
		z2=(int)((long)dist*400L/(long)z2);
		if(y1<y2)
		{
			yd=y2-y1;
			q=frame/20;
			y1-=q;
			for(y=y1-5;y<y1;y++) if(y>=0 && y<=199) 
			{
				memset(vram+y*320,0,320);
			}
			for(y=0;y<q;y++) 
			{
				if(y1>=0) zoom(vram+(y1)*320,bg+200*326,z1);
				y1++;
			}
			for(y=0;y<yd;y++)
			{
				z=((long)y*(long)z2+((long)yd-(long)y)*(long)z1)/(long)yd;
				q=(200L*(long)y/(long)yd);
				zoom(vram+(y+y1)*320,bg+q*326,z);
			}
			for(y=y2;y<y2+5;y++)
			{
				if(y>=0 && y<=199) memset(vram+y*320,0,320);
			}
		}
		if(f<35) f++;
		frame+=dis_waitb();
	}
}


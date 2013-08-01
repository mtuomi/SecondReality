#include <stdio.h>
#include <string.h>
#include "..\dis\dis.h"

FILE	*f1;

#include "readp.c"

extern char pic[];

char *vram=(char *)0xa0000000L;

char	rowdata1[200][184];
char	rowdata2[200][184];
char	*row[400];

char	pal2[768];
char	palette[768];
char	rowbuf[640];

extern int sin1024[];

int	framey1[512];
int	framey2[512];
int	lasty[400];
int	lasts[400];

void	scrolly(int y)
{
	int	a;
	a=y*80;
	_asm
	{
		mov	dx,3d4h
		mov	al,0dh
		mov	ah,byte ptr a[0]
		out	dx,ax
		mov	dx,3d4h
		mov	al,0ch
		mov	ah,byte ptr a[1]
		out	dx,ax
	}
}

char	*shiftstatus=(char *)0x0417;
int	waitb()
{
	return(dis_waitb());
}

int	calc(int y,int c)
{
	_asm
	{
		mov	ax,y
		sub	ax,400
		add	ax,c
		mov	dx,400
		imul	dx
		mov	cx,c
		idiv	cx
	} 
}

void	doit(void)
{
	int	frame=0,halt=0,storea=0,ysb=0;
	int	a,b,c,y,ysz,ysza,xsc,spd=10,la,y1,y2;
	while(!dis_exit() && frame<700)
	{
		if(*shiftstatus&16) setborder(0);
		c=waitb();
		if(*shiftstatus&16) setborder(127);
		frame+=c;
		if(frame>511) c=400;
		else 
		{
			y1=framey1[frame]/16;
			y2=framey2[frame]/16;
		}
		xsc=(400-(y2-y1))/8;
		for(y=0;y<400;y++)
		{
			if(y<y1 || y>=y2)
			{
				linezoom(vram+y*80,NULL,0);
			}
			else 
			{
				b=(long)(y-y1)*400L/(long)(y2-y1);
				a=184+(sin1024[b*32/25]*xsc+32)/64;
				a&=~1;
				if(lasty[y]!=b || lasts[y]!=a)
				{
					linezoom(vram+y*80,row[b],a);
					lasty[y]=b;
					lasts[y]=a;
				}
			}
		}
	}
}

main()
{
	int	frame,halt=0,storea=0;
	int	a,b,c,y,ya,ysz,ysza,xsc,spd=10,la;
	int	y1,y2,y1a,y2a,ly1a;
	dis_partstart();

	for(a=0;a<200;a++) row[a]=rowdata1[a];
	for(a=0;a<200;a++) row[a+200]=rowdata2[a];

	frame=0;
	ysz=400*16; ysza=-460/6;
	y=0;
	y1=0; y1a=500;
	y2=399*16; y2a=500;
	for(frame=0;frame<512;frame++)
	{	
		if(!halt)
		{
			y1+=y1a;
			y2+=y2a;
	
			y2a+=9;
			if(y2>400*16)
			{
				y2-=y2a;
				y2a=-y2a*5/8;
			}
	
			y1a+=9;
			
			la=a;
			a=(y2-y1)-400*16;
			if((a&0x8000)^(la&0x8000))
			{
				y1a=y1a*5/8;
			}
			y1a+=a/20;
			y2a-=a/20;
		}
		
		if(frame>97) 
		{
			if(y2>=399*16) 
			{
				y2=400*16;
				halt=1;
			}
			y1=y2-400*16;
		}

		framey1[frame]=y1;
		framey2[frame]=y2;
	}

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
		memcpy(row[y],rowbuf+70,184);
	}
	setpalarea(palette,0,256);
	
	for(y=0;y<400;y++) lasty[y]=lasts[y]=-1;
	dis_waitb();
	scrolly(400);
	dis_waitb();
	for(y=0;y<400;y++)
	{
		linezoom(vram+y*80,row[y],184);
	}
	a=64; y=400*64;
	while(y>0)
	{
		y-=a;
		a+=6;
		if(y<0) y=0;
		scrolly(y/64);
		dis_waitb();
	}
	storea=a;
	dis_waitb();
	doit();
	//_asm mov ax,3
	//_asm int 10h
	//printf("%i\n",storea);
	return(0);
}

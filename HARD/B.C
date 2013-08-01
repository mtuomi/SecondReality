/* blue screen dots */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <malloc.h>
#include <string.h>
#include "..\dis\dis.h"

#define NEXTMODE { count=-1; mode++; }

extern int rows[200];
extern int dotxyz[];
extern int dodots(char *,char *);
extern int sin1024[];

unsigned char far *vram=(char far *)0xa2d00000L;
unsigned char far *vram0=(char far *)0xa0000000L;

char pal[768];
char pal2[768];
char defpal[768];

char	*background;

int	mode=0,count=-1,border=0;

extern int grid1[]; /* _grid1.obj */
extern char dots1[]; /* _dots1.obj */
extern char dots2[]; /* _dots1.obj */
extern char dots3[]; /* _dots1.obj */
extern char dots4[]; /* _dots1.obj */
char	*dotspnt;
int	dotsp;

int	setborder(int c)
{
	if(!border) return(0);
	_asm
	{
		mov	dx,3dah
		in	al,dx
		mov	dx,3c0h
		mov	al,11h+20h
		out	dx,al
		mov	al,byte ptr c
		out	dx,al
	}
}

int	waitb(void)
{
	dis_waitb();
	#if 0
	_asm
	{
		mov	dx,3dah
	l1:	in	al,dx
		test	al,8
		jz	l1
	l2:	in	al,dx
		test	al,8
		jnz	l2
	}
	#endif
}

int	loadpal(char *pal,int cnt)
{
	_asm
	{
		push	ds
		lds	si,pal
		mov	dx,3c8h
		xor	al,al
		out	dx,al
		inc	dx
		mov	cx,cnt
		rep	outsb
		pop	ds
	}
}

void	adddot(int a,int b,int x,int y,int c)
{
	static int doti=0,sc=0;
	if(!c) dotxyz[doti*6+5]=dotxyz[doti*6+4]=0x100;
	else
	{
		dotxyz[doti*6+0]=x*64+32-(a-x);
		dotxyz[doti*6+1]=y*64+32-(b-y);
		dotxyz[doti*6+2]=(a-x);
		dotxyz[doti*6+3]=(b-y);
		dotxyz[doti*6+4]=c+(66)*256;
		dotxyz[doti*6+5]=x+y*320;
	}
	doti++; doti&=2047; /* 2048 */
}

int	doit1(int x,int y,int col)
{
	int	a,x2,y2;
	static int flag=0;
	flag^=1;
	if(flag&1) { x2=x*4; y2=y*4; }
	else  { x2=x*3; y2=y*3; }
	x2+=(rand()&31)-15;
	y2+=(rand()&31)-15;
	adddot(x+160,y+64,x2+160,y2+64,col);
}

int	doit2(int x,int y,int col)
{
	int	a,x2,y2;
	static int gx=0;
	x2=grid1[gx+0];
	y2=grid1[gx+1];
	gx+=2;
	if(gx>=200) gx=0;
	adddot(x2,y2,x+160,y+64,col);
}

#include "bp.c"

main(int argc,char *argv[])
{
	int	endcnt=0;
	FILE	*f1;
	unsigned u;
	int	a,b,c,d,e,x,y,x2,y2,y1;
	dis_partstart();
	{
		_asm mov ax,13h
		_asm int 10h
	}
	waitb();
	memset(vram0,15,64000);
	while(dis_muscode(1)!=1 && !dis_exit());
	waitb();
	waitb();
	waitb();
	waitb();
	for(x=319;x>=0;x--)
	{
		vram0[x+100*320]=0;
		if(!(x&15)) waitb();
	}
	y1=100; a=100*64; b=0;
	while(y1<200)
	{
		b+=16;
		a+=b;
		y2=a/64;
		if(y2>200) y2=200;
		for(y=y1;y<y2;y++)
		{
			memset(vram0+y*320,0,320);
		}
		y1=y2;
		waitb();
	}
	for(a=0;a<70 && !dis_exit();a++) dis_waitb();
	
	memset(defpal,0,768);
	loadpal(defpal,768);
	outp(0x3c0,0x11+0x20);
	outp(0x3c0,255);
	memset(vram0+0*320,255,35*320);
	memset(vram0+35*320,254,1*320);
	memset(vram0+36*320,0,128*320);
	memset(vram0+164*320,254,1*320);
	memset(vram0+165*320,255,35*320);
	defpal[0*3+0]=0;
	defpal[0*3+1]=0;
	defpal[0*3+2]=20;
	defpal[254*3+0]=45;
	defpal[254*3+1]=45;
	defpal[254*3+2]=45;
	defpal[255*3+0]=0;
	defpal[255*3+1]=0;
	defpal[255*3+2]=0;
	loadpal(defpal,768);
	for(a=0;a<200;a++) rows[a]=a*320;
	background=halloc(16384L,4L);
	memset(background,0,64000);
	dotspnt=dots1;
	if(argc==2) 
	{
		switch(*argv[1])
		{
		case '1' : dotspnt=dots4; break;
		case '2' : mode=5; break;
		case '3' : mode=6; break;
		}
		for(a=0;a<256;a++) vram0[a]=vram0[a+320]=a;
		border=1;
	}
	x=y=c=0;
	
	for(a=0;a<256;a++) vram0[a]=vram0[a+320]=255;
	border=0;
	
	while(!dis_exit() && mode!=-1)
	{
		count++;
		setborder(255);
		waitb();
		if(mode==4 && count>0) loadpal(pal,768);
		if(mode==5 && count>500-17) loadpal(pal,768-6);
		if(mode==6 && count>0 && count<20) loadpal(pal,768-6);
		setborder(0);
		if(mode<4) dodots(background,vram);
		else if(mode==4) dodots2(background,vram);
		switch(mode)
		{
		case 0:
			if(count>32) 
			{ 
				NEXTMODE; 
			}
			break;
		case 1:
			if(!count) 
			{
				x=0;
				part1init();
			}
			else part1();
			break;
		case 2:
			{ 
				NEXTMODE;
				if(dotspnt==dots1) { mode=1; dotspnt=dots2; }
				else if(dotspnt==dots2) { mode=1; dotspnt=dots3; }
				else if(dotspnt==dots3) { mode=1; dotspnt=dots4; }
				else for(a=0;a<2048;a++) adddot(0,0,0,0,0);
			}
			break;
		case 3:
			if(count>40) NEXTMODE;
			break;
		case 4:
			if(count>400) 
			{ 
				NEXTMODE; 
			}
			if(!count)
			{
				pal1colp=1;
				memset(background,0,64000);
				for(dotsp=a=0;a<2048;a++)
				{
					dotsp+=4;
					switch(dotspnt[dotsp])
					{
					case 1 :
					case 2 :
					case 3 : doit2(dotspnt[dotsp+2],dotspnt[dotsp+3],254); break;
					default : dotsp=0; break;
					}
				}
				for(a=3;a<32*3;a+=3) 
				{
					pal[a+0]=0;
					pal[a+1]=0;
					pal[a+2]=20;
				}
			}
			a=252-pal1colp;
			d=a+66; if(d>254) d=254;
			c=63;
			for(;a<d;a++,c-=4)
			{
				if(c<0) c=0;
				b=c*c/64;
				pal[a*3+0]=b;
				pal[a*3+1]=b;
				pal[a*3+2]=b<20?20:b;
			}
			a=(254-65);
			pal[a*3+0]=63;
			pal[a*3+1]=63;
			pal[a*3+2]=63;
			pal[1*3+0]=63;
			pal[1*3+1]=63;
			pal[1*3+2]=63;
			pal1colp++;
			if(count>100) 
			{
				for(a=0;a<2048;a++) adddot(0,0,0,0,0);
				NEXTMODE;
			}
			break;
		case 5:
			if(!count) 
			{
				for(u=0;u<128*320;u++)
				{
					if(vram[u]==254-65) vram[u]=1;
					else vram[u]=0;
				}
				part2init();
			}
			else part2();
			break;
		case 6:
			if(!count) 
			{
				part3init();
			}
			else part3();
			break;
		case 7:
			NEXTMODE;
			if(count>100)
			{ 
				NEXTMODE; 
			}
			break;
		default : 
			mode=-1;
			break;
		}
	}
	if(!dis_indemo())
	{
		_asm mov ax,3h
		_asm int 10h
	}
	/*
	for(a=0;a<16;a++)
	{
		printf("%i: %i %i %i\n",a,
			points3[2+a*8+0],
			points3[2+a*8+1],
			points3[2+a*8+4]);
	}
	*/
}

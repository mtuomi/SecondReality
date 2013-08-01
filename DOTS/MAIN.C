#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <malloc.h>
#include <math.h>
#include "..\dis\dis.h"

extern int face[];

extern int bpmin,bpmax;

extern char *bgpic;
extern int rotsin,rotcos;
extern int rows[];
extern long depthtable1[];
extern long depthtable2[];
extern long depthtable3[];
extern long depthtable4[];

extern int dotnum;

extern void drawdots(void);

char far *vram=(char far *)0xa0000000L;

char	pal[768];
char	pal2[768];

extern sin1024[];

int	isin(int deg)
{
	return(sin1024[deg&1023]);
}

int	icos(int deg)
{
	return(sin1024[(deg+256)&1023]);
}

extern struct
{
	int	x;
	int	y;
	int	z;
	int	old1;
	int	old2;
	int	old3;
	int	old4;
	int	yadd;
} dot[];

extern int gravity;
extern int gravitybottom;
extern int gravityd;

void setborder(int color)
{
	_asm
	{
		mov	dx,3dah
		in	al,dx
		mov	dx,3c0h
		mov	al,11h+32
		out	dx,al
		mov	al,color
		out	dx,al
	}
}

int	cols[]={
0,0,0,
4,25,30,
8,40,45,
16,55,60};
int	dottaul[1024];

main(int argc,char *argv[])
{
	int	timer=30000;
	int	dropper,repeat;
	int	frame=0;
	int	rota=-1*64;
	int	fb=0;
	int	rot=0,rots=0;
	int	a,b,c,d,i,j,mode;
	int	grav,gravd;
	int	f=0;
	dis_partstart();
	dotnum=512;
	for(a=0;a<dotnum;a++)  dottaul[a]=a;
	for(a=0;a<500;a++)
	{
		b=rand()%dotnum;
		c=rand()%dotnum;
		d=dottaul[b];
		dottaul[b]=dottaul[c];
		dottaul[c]=d;
	}
	{
		dropper=22000;
		for(a=0;a<dotnum;a++)
		{
			dot[a].x=0;
			dot[a].y=2560-dropper;
			dot[a].z=0;
			dot[a].yadd=0;
		}
		mode=7;
		grav=3;
		gravd=13;
		gravitybottom=8105;
		i=-1;
	}
	for(a=0;a<500;a++)
	{ // scramble
		b=rand()%dotnum;
		c=rand()%dotnum;
		d=dot[b].x; dot[b].x=dot[c].x; dot[c].x=d;
		d=dot[b].y; dot[b].y=dot[c].y; dot[c].y=d;
		d=dot[b].z; dot[b].z=dot[c].z; dot[c].z=d;
	}
	for(a=0;a<200;a++) rows[a]=a*320;
	_asm mov ax,13h
	_asm int 10h
	outp(0x3c8,0);
	for(a=0;a<16;a++) for(b=0;b<4;b++)
	{
		c=100+a*9;
		outp(0x3c9,cols[b*3+0]);
		outp(0x3c9,cols[b*3+1]*c/256);
		outp(0x3c9,cols[b*3+2]*c/256);
	}
	outp(0x3c8,255);
	outp(0x3c9,31);
	outp(0x3c9,0);
	outp(0x3c9,15);
	outp(0x3c8,64);
	for(a=0;a<100;a++)
	{
		c=64-256/(a+4);
		c=c*c/64;
		outp(0x3c9,c/4);
		outp(0x3c9,c/4);
		outp(0x3c9,c/4);
	}
	outp(0x3c7,0);
	for(a=0;a<768;a++) pal[a]=inp(0x3c9);
	outp(0x3c8,0);
	for(a=0;a<768;a++) outp(0x3c9,0);
	for(a=0;a<100;a++)
	{
		memset(vram+(100+a)*320,a+64,320);
	}
	for(a=0;a<128;a++)
	{
		c=a-(43+20)/2;
		c=c*3/4;
		c+=8;
		if(c<0) c=0; else if(c>15) c=15;
		c=15-c;
		depthtable1[a]=0x202+0x04040404*c;
		depthtable2[a]=0x02030302+0x04040404*c;
		depthtable3[a]=0x202+0x04040404*c;
		//depthtable4[a]=0x02020302+0x04040404*c;
	}
	bgpic=halloc(64000L,1L);
	memcpy(bgpic,vram,64000);
	a=0;
	for(b=64;b>=0;b--)
	{	
		for(c=0;c<768;c++)
		{
			a=pal[c]-b;
			if(a<0) a=0;
			pal2[c]=a;
		}
		dis_waitb();
		dis_waitb();
		outp(0x3c8,0);
		for(c=0;c<768;c++) outp(0x3c9,pal2[c]);
	}
	
	while(!dis_exit() && frame<2450)
	{
		//setborder(0);
		repeat=dis_waitb();
		if(frame>2300) setpalette(pal2);
		//setborder(1);
		if(dis_indemo())
		{
			a=dis_musplus();
			if(a>-4 && a<0) break;
		}
		while(repeat--)
		{
			frame++;
			if(frame==500) f=0;
			i=dottaul[j];
			j++; j%=dotnum;
			if(frame<500)
			{
				dot[i].x=isin(f*11)*40;
				dot[i].y=icos(f*13)*10-dropper;
				dot[i].z=isin(f*17)*40;
				dot[i].yadd=0;
			}
			else if(frame<900)
			{
				dot[i].x=icos(f*15)*55;
				dot[i].y=dropper;
				dot[i].z=isin(f*15)*55;
				dot[i].yadd=-260;
			}
			else if(frame<1700)
			{	
				a=sin1024[frame&1023]/8;
				dot[i].x=icos(f*66)*a;
				dot[i].y=8000;
				dot[i].z=isin(f*66)*a;
				dot[i].yadd=-300;
			}
			else if(frame<2360)
			{
				/*
				a=rand()/128+32;
				dot[i].y=8000-a*80;
				b=rand()&1023;
				a+=rand()&31;
				dot[i].x=sin1024[b]*a/3+(a-50)*7;
				dot[i].z=sin1024[(b+256)&1023]*a/3+(a-40)*7;
				dot[i].yadd=300;
				if(frame>1640 && !(frame&31) && grav>-2) grav--;
				*/
				dot[i].x=rand()-16384;
				dot[i].y=8000-rand()/2;
				dot[i].z=rand()-16384;
				dot[i].yadd=0;
				if(frame>1900 && !(frame&31) && grav>0) grav--;
			}
			else if(frame<2400)
			{
				a=frame-2360;
				for(b=0;b<768;b+=3)
				{
					c=pal[b+0]+a*3;
					if(c>63) c=63;
					pal2[b+0]=c;
					c=pal[b+1]+a*3;
					if(c>63) c=63;
					pal2[b+1]=c;
					c=pal[b+2]+a*4;
					if(c>63) c=63;
					pal2[b+2]=c;
				}
			}
			else if(frame<2440)
			{
				a=frame-2400;
				for(b=0;b<768;b+=3)
				{
					c=63-a*2;
					if(c<0) c=0;
					pal2[b+0]=c;
					pal2[b+1]=c;
					pal2[b+2]=c;
				}
			}
			if(dropper>4000) dropper-=100;
			rotcos=icos(rot)*64; rotsin=isin(rot)*64;
			rots+=2;
			if(frame>1900) 
			{
				rot+=rota/64;
				rota--;
			}
			else rot=isin(rots);
			f++;
			gravity=grav;
			gravityd=gravd;
		}
		drawdots();
	}
	if(!dis_indemo())
	{
		_asm mov ax,3h
		_asm int 10h
	}
	return(0);
}

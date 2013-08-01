#include <stdio.h>
#include <conio.h>
#include <malloc.h>
#include "..\dis\dis.h"

int	flip;

int wavesin[]=
#include "wave.h"

extern unsigned char *wave1;
extern unsigned char *wave2;
extern unsigned char *vbuf;
extern int cameralevel;

char far *vram=(char far *)0xa0000000L;
char far *vram2=(char far *)0xa8000000L;

extern int sin1024[];

char	palette[768];

char	*shiftstatus=(char *)0x0417;
int	waitb()
{
	if(*shiftstatus&16) setborder(0);
	while(!(inp(0x3da)&8));
	while((inp(0x3da)&8));
	if(*shiftstatus&16) setborder(127);
	return(1);
}

void	doit(void)
{
	unsigned int u,v,uc;
	int	startrise=160;
	int	a,b,x,y,xa,ya,j,xw,yw,d;
	int	rot2=0,rot=0,rsin,rcos,xwav=0,ywav=0,zwav=0;
	int	rsin2,rcos2;
	int	frame=0;
	rot=200; rot2=0;
	cameralevel=-270;
	while(!kbhit() && frame<444)
	{
		if(frame<50)
		{
			if(frame==4)
			{
				setpalarea(palette,0,256);
			}
			if(startrise>0) startrise-=4;
		}
		rsin=sin1024[rot&1023];
		rcos=sin1024[(rot+256)&1023];
		rsin2=sin1024[(rot+177)&1023];
		rcos2=sin1024[(rot+177+256)&1023];
		zwav+=1;
		rot2+=2;
		rot+=3*(sin1024[rot2&1023]+64)/256;
		xw=xwav; yw=ywav;
		for(a=0;a<160;a++)
		{
			x=(a-80);
			y=160;
			xa=(int)(((long)x*(long)rcos+(long)y*(long)rsin)/256L);
			ya=(int)(((long)y*(long)rcos2-(long)x*(long)rsin2)/256L);
			b=(a&1)*80+(a>>1)+199*160;
			docol(xw,yw,xa,ya,b);
			if(a==80)
			{
				xwav+=xa*4+ya/2;
				ywav+=ya*4;
			}
		}
		flip++; if(flip>1) flip=0;
		switch(flip)
		{
		case 0 :
			outp(0x3d4,0x0c);
			outp(0x3d5,0x80);
			frame+=waitb();
			docopy(0xa000+5*startrise);
			break;
		case 1 :
			outp(0x3d4,0x0c);
			outp(0x3d5,0x00);
			frame+=waitb();
			docopy(0xa800+5*startrise);
			break;
		}
	}
}

main()
{
	unsigned int u,v,uc;
	int	a,b,x,y,xa,ya,j,k,xw,yw;
	int	rot=0,rsin,rcos,xwav=0,ywav=0;
	dis_partstart();
	_asm mov ax,13h
	_asm int 10h
	if(!dis_indemo()) printf("\nShaded 3D sinusfield < Lanscape?\n");
	for(a=0;a<256;a++)
	{
		uc=(223-a*22/26)*3;
		b=(230-a)/4;
		if(b<0) b=0;
		if(b>63) b=63;
		palette[uc+1]=b;
		b=(255-a)/4;
		if(b>63) b=63;
		palette[uc+2]=b;
		palette[uc+0]=0;
		vram[a+640]=a;
	}
	for(a=0;a<32;a++)
	{
		uc=(255-a)*3;
		palette[uc+0]=a;
		palette[uc+1]=0;
		palette[uc+2]=0;
	}
	palette[0]=0;
	palette[1]=0;
	palette[2]=0;
	wave1=halloc(16384,4);
	wave2=halloc(16384,4);
	vbuf=halloc(16384,4);
	setpalarea(palette,0,256);
	for(b=u=v=0;b<256;b++)
	{
		for(a=0;a<256;a++)
		{
			k=((long)u*1024L*7L)/65536L;
			j=wavesin[k&1023]/8;
			k=((long)u*1024L*3L)/65536L;
			j+=wavesin[k&1023]/7;
			wave1[u]=(j/3)+120/2;
			k=((long)u*1024L*5L)/65536L;
			j=wavesin[k&1023]/6;
			k=((long)u*1024L*2L)/65536L;
			j+=wavesin[k&1023]/5;
			wave2[u]=(j/3)+120/2;
			u++;
		}
		if(!dis_indemo()) vram[b]=127;
	}
	inittwk();
	memset(vram,0,64000);
	setpalarea(palette,0,256);

	doit();
	if(kbhit()) getch();

	if(!dis_indemo())
	{	
		_asm mov ax,3h
		_asm int 10h
	}
}


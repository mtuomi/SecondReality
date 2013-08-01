#include <stdio.h>
#include <conio.h>
#include <malloc.h>
#include "..\dis\dis.h"

#define noCALCW12

char combg[16];

#ifndef CALCW12
extern int w1dta[];
extern int w2dta[];
#endif

extern char combg[];
extern char combguse[];

int	flip;

int wavesin[]=
#include "wave.h"

extern int *wave1;
extern int *wave2;
extern unsigned char *vbuf;
extern int cameralevel;

char far *vram=(char far *)0xa0000000L;
char far *vram2=(char far *)0xa8000000L;

extern int sin1024[];

char	palette[768];

char	*shiftstatus=(char *)0x0417;
int	waitb()
{
	//if(*shiftstatus&16) setborder(0);
	while(!(inp(0x3da)&8));
	while((inp(0x3da)&8));
	//if(*shiftstatus&16) setborder(127);
	return(1);
}

void	doit(void)
{
	unsigned int u,v,uc;
	int	startrise=160,frepeat=1;
	int	a,b,x,y,xa,ya,j,xw,yw,d,r;
	int	rot2=0,rot=0,rsin,rcos,xwav=0,ywav=0,zwav=0;
	int	rsin2,rcos2;
	int	frame=0;
	rot=0; rot2=0;
	cameralevel=-270;
	dis_waitb();
	while(!dis_exit() && dis_musplus()<0);
	while(!dis_exit() && frame<4444)
	{
		a=dis_musplus();
		if(a>-8 && a<0) break;
		while(frepeat--)
		{
			if(a>-30 && a<0) 
			{
				if(startrise<160) startrise+=1;
			}
			if(frame<400 && startrise>0)
			{
				if(frame==4)
				{
					dis_waitb();
					setpalarea(palette,0,256);
				}
				if(startrise>0) startrise-=1;
			}
		}
		rot2+=4;
		rot+=(sin1024[rot2&1023])/15;
		r=rot>>3;
		rsin=sin1024[r&1023];
		rcos=sin1024[(r+256)&1023];
		rsin2=sin1024[(r+177)&1023];
		rcos2=sin1024[(r+177+256)&1023];
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
				xwav+=xa*4;
				ywav+=ya*4;
			}
		}
		flip++; if(flip>1) flip=0;
		switch(flip)
		{
		case 0 :
			outp(0x3d4,0x0c);
			outp(0x3d5,0x80);
			frame+=(frepeat=dis_waitb());
			docopy(0xa000+5*startrise);
			break;
		case 1 :
			outp(0x3d4,0x0c);
			outp(0x3d5,0x00);
			frame+=(frepeat=dis_waitb());
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

	for(a=0;a<256;a++)
	{
		uc=(223-a*22/26)*3;
		b=(230-a)/4;
		b+=sin1024[a*4&1023]/32;
		if(b<0) b=0;
		if(b>63) b=63;
		palette[uc+1]=b;
		b=(255-a)/3;
		if(b>63) b=63;
		palette[uc+2]=b;
		b=a-220; if(b<0) b=-b;
		if(b>40) b=40;
		b=40-b;
		palette[uc+0]=b/3;
		//if(!dis_indemo()) vram[a+640]=a;
	}
	for(a=0;a<768-16*3;a++)
	{
		b=palette[a];
		b=b*9/6; if(b>63) b=63;
		palette[a]=b;
	}
	for(a=0;a<24;a++)
	{
		uc=(255-a)*3;
		b=a-4; if(b<0) b=0;
		palette[uc+0]=b/2;
		palette[uc+1]=0;
		palette[uc+2]=0;
	}
	palette[0]=0;
	palette[1]=0;
	palette[2]=0;
	for(x=(256-16)*3;x<768;x++)
	{
		palette[x]=combg[16+x];
	}
	vbuf=halloc(16384,4);
	for(y=0;y<90;y++)
	{
		for(x=0;x<80;x++)
		{
			combguse[x+y*160]=combg[x*4+y*320+768+16];
		}
		for(x=0;x<80;x++)
		{
			combguse[x+80+y*160]=combg[x*4+y*320+2+768+16];
		}
	}
	dis_waitb();
	setpalarea(palette,0,256);
	#ifdef CALCW12
	wave1=halloc(16384,4);
	wave2=halloc(16384,4);
	for(b=u=v=0;b<128;b++)
	{
		for(a=0;a<256;a++)
		{
			k=((long)u*1024L*7L)>>15;
			j=wavesin[k&1023]/8;
			k=((long)u*1024L*3L)>>15;
			j+=wavesin[k&1023]/7+((rand()&7)*a/256);
			wave1[u]=j*5/9;
			k=((long)u*1024L*5L)>>15;
			j=wavesin[k&1023]/5;
			k=((long)u*1024L*2L)>>15;
			j+=wavesin[k&1023]/6;
			wave2[u]=j*7/9;
			u++;
		}
		if(!dis_indemo()) vram[b*2]=127;
	}
	{
		FILE *f1;
		f1=fopen("w1dta.bin","wb");
		fwrite(wave1,2,16384,f1);
		fwrite(wave1+16384,2,16384,f1);
		fclose(f1);
		f1=fopen("w2dta.bin","wb");
		fwrite(wave2,2,16384,f1);
		fwrite(wave2+16384,2,16384,f1);
		fclose(f1);
	}
	#else
	wave1=w1dta;
	wave2=w2dta;
	#endif
	inittwk();
	memset(vram,0,64000);
	dis_waitb();
	setpalarea(palette,0,256);

	doit();

	if(!dis_indemo())
	{	
		_asm mov ax,3h
		_asm int 10h
	}
}


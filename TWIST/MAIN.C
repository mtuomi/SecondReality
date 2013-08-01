#include <stdio.h>
#include <conio.h>
#include <malloc.h>
#include "..\dis\dis.h"

int	flip;

extern char far *twistvram;
extern int twist[];
extern void twister(void);

extern char far *vram;
char far *vram1=(char far *)0xa0000000L;
char far *vram2=(char far *)0xa8000000L;

extern int sin1024[];

char	palette[768];

char	*shiftstatus=(char *)0x0417;
int	waitb()
{
	if(*shiftstatus&16) setborder(0);
	while(!(inp(0x3da)&8));
	while((inp(0x3da)&8));
	if(*shiftstatus&16) setborder(31);
	return(1);
}

void	line(int y,int w,int c)
{
	if(w<0)
	{
		leftline(y,-w,c);
		rightline(y,-w,c);
	}
	else
	{
		leftline(y,w,c);
		rightline(y,w,c);
	}
}

void	doit(void)
{
	int y,rot,r,ra,w,c;
	int frame=0,flip=0;
	rot=0; ra=0;
	while(!kbhit() && frame<4444)
	{
		twistvram=vram;
		ra++;
		r=0;
		for(y=0;y<200;y++)
		{
			w=sin1024[ ((r>>6)&511) +256]*25/64+100;
			twist[y*4+0]=w;
			r+=ra;
		}
		twister();
		flip++; if(flip>1) flip=0;
		switch(flip)
		{
		case 0 :
			outp(0x3d4,0x0c);
			outp(0x3d5,0x80);
			frame+=waitb();
			vram=vram1;
			break;
		case 1 :
			outp(0x3d4,0x0c);
			outp(0x3d5,0x00);
			frame+=waitb();
			vram=vram2;
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
	inittwk();
	initcoppers();
	memset(vram,0,64000);
	for(a=0;a<256;a++)
	{
		b=a*3;
		palette[b+0]=a/3;
		palette[b+1]=a*2/3;
		palette[b+2]=a;
		vram[a]=a;
	}	
	setpalarea(palette,0,256);

	doit();
	if(kbhit()) getch();

	if(!dis_indemo())
	{	
		_asm mov ax,3h
		_asm int 10h
	}
}


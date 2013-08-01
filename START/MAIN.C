// THIS CODE SUCKS!

#include <stdio.h>
#include "..\dis\dis.h"

extern char hzpic[];

char font[31][640];

char *vram=(char *)0xa0000000L;
char palette[768];
char palette2[768];
char rowbuf[640];

char	fade1[64][576];
char	fade2[64][576];

char	*fonaorder="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
int	fonap[256];
int	fonaw[256];

#include "readp.c"

void	prt(int x,int y,char *txt)
{
	int	x2w,x2,y2,y2w=y+32,sx,d;
	while(*txt)
	{
		x2w=fonaw[*txt]+x;
		sx=fonap[*txt];
		for(x2=x;x2<x2w;x2++)
		{
			for(y2=y;y2<y2w;y2++)
			{
				d=font[y2-y][sx];
				pset(x2,y2,pget(x2,y2)|d);
			}
			sx++;
		}
		x=x2+2;
		txt++;
	}
}

void	prtc(int x,int y,char *txt)
{
	int	w=0;
	char	*t=txt;
	while(*t) w+=fonaw[*t++]+2;
	prt(x-w/2,y,txt);
}

void	dofade2(int wait)
{
	int	x;
	for(x=0;x<64;x++)
	{
		waitb();
		setpalarea(fade2[x],64,192);
	}
	for(x=0;x<wait && !kbhit();x++) waitb();
	for(x=63;x>=0;x--)
	{
		waitb();
		setpalarea(fade2[x],64,192);
	}
}

void	dofadef(void)
{
	char	tmp[64*3];
	int	x,y;
	for(y=0;y<=64;y++)
	{
		for(x=0;x<64*3;x++)
		{
			tmp[x]=(palette[x]*y>>6);
		}
		waitb();
		setpalarea(tmp,0,64);
	}
}

void	dofade(int wait)
{
	int	x;
	for(x=0;x<64;x++)
	{
		waitb();
		setpalarea(fade1[x],64,192);
	}
	for(x=0;x<wait;x++) waitb();
	for(x=63;x>=0;x--)
	{
		waitb();
		setpalarea(fade1[x],64,192);
	}
}

main()
{
	int	a,b,x,y;
	int	zimz;
	initvideo();
	readp(palette,-1,hzpic);
	for(y=0;y<256;y++)
	{
		readp(rowbuf,y,hzpic);
		lineblit(y+64,rowbuf);
	}
	for(y=0;y<32;y++)
	{
		readp(font[y],y+300,hzpic);
		for(a=0;a<640;a++) 	
		{
			switch(font[y][a])
			{
			case 0x40 : b=0xc0; break;
			case 0x41 : b=0x80; break;
			case 0x42 : b=0x40; break;
			default : b=0;
			} 
			font[y][a]=b;
		}
	}
	for(y=0;y<768;y+=3)
	{
		if(y<64*3) ;
		else if(y<128*3) 
		{
			palette2[y+0]=palette[0x42*3+0];
			palette2[y+1]=palette[0x42*3+1];
			palette2[y+2]=palette[0x42*3+2];
		}
		else if(y<192*3) 
		{
			palette2[y+0]=palette[0x41*3+0];
			palette2[y+1]=palette[0x41*3+1];
			palette2[y+2]=palette[0x41*3+2];
		}
		else
		{
			palette2[y+0]=palette[0x40*3+0];
			palette2[y+1]=palette[0x40*3+1];
			palette2[y+2]=palette[0x40*3+2];
		}
	}
	for(y=192;y<768;y++)
	{
		palette[y]=palette[y-192];
	}
	for(x=0;x<64;x++)
	{
		for(y=0;y<576;y++)
		{
			fade1[x][y]=(palette2[y+192]*x+palette[y+192]*(63-x))/63;
			fade2[x][y]=(palette2[y+192]*x)/63;
		}
	}

	for(x=0;x<640 && *fonaorder;)
	{
		while(x<640)
		{
			for(y=0;y<32;y++) if(font[y][x]) break;
			if(y!=32) break;
			x++;
		}
		b=x;
		while(x<640)
		{
			for(y=0;y<32;y++) if(font[y][x]) break;
			if(y==32) break;
			x++;
		}
		//printf("%c: %i %i\n",*fonaorder,b,x-b);
		fonap[*fonaorder]=b;
		fonaw[*fonaorder]=x-b;
		fonaorder++;
	}
	fonap[32]=640-20;
	fonaw[32]=16;
	setpalarea(fade2[0],64,192);

	for(;;)
	{	
		prtc(160,140,"A");
		prtc(160,180,"Future Crew");
		prtc(160,220,"production");
		dofade2(300);
		if(kbhit()) break;
	
		prtc(160,140,"First presented at");
		prtc(160,220,"Assembly NoNumbers");
		dofade2(300);
		if(kbhit()) break;
		
		dofadef();
		if(kbhit()) break;
	
		prtc(100,140,"Graphics");
		prtc(100,200,"Pixel");
		prtc(100,240,"Marvel");
		dofade(300);
		if(kbhit()) break;
	
		prtc(100,140,"Music");
		prtc(100,200,"Purple Motioon");
		prtc(100,240,"Skaven");
		dofade(300);
		if(kbhit()) break;
	
		prtc(100,140,"Code");
		prtc(100,200,"Trug");
		prtc(100,240,"Wildfire");
		prtc(100,280,"Psi");
		dofade(300);
		if(kbhit()) break;
	}
	getch();
	deinitvideo();
}

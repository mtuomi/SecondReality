#include <conio.h>
#include <mem.h>
#include "h:\u2\dis\dis.h"

char *prtc(int x,int y,char *txt);

extern char pic1[];
extern char pic2[];
extern char pic3[];
extern char pic4[];
extern char pic5[];
extern char pic5b[];
extern char pic6[];
extern char pic7[];
extern char pic8[];
extern char pic9[];
extern char pic10[];
extern char pic10b[];
extern char pic11[];
extern char pic12[];
extern char pic13[];
extern char pic14[];
extern char pic14b[];
extern char pic15[];
extern char pic16[];
extern char pic17[];
extern char pic18[];

#define FONAY 32

extern char far font[FONAY][1500];
char	*fonaorder="ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789/?!:,.\"()+-";
int	fonap[256];
int	fonaw[256];

main()  {
	dis_partstart();
	init();
	tw_opengraph();
	tw_setstart(160*200);
	if(!dis_exit()) screenin(pic1,
		"GRAPHICS - MARVEL\0"
		"MUSIC - SKAVEN\0"
		"CODE - WILDFIRE\0"
		);

	if(!dis_exit()) screenin(pic2,
		"GRAPHICS - MARVEL\0"
		"MUSIC - SKAVEN\0"
		"CODE - PSI\0"
		);

	if(!dis_exit()) screenin(pic3,
		"GRAPHICS - MARVEL\0"
		"MUSIC - SKAVEN\0"
		"CODE - WILDFIRE\0"
		"ANIMATION - TRUG\0"
		);

	if(!dis_exit()) screenin(pic4,
		"\0GRAPHICS - PIXEL\0"
		);

	if(!dis_exit()) screenin(pic5,
		"GRAPHICS - PIXEL\0"
		"MUSIC - PURPLE MOTION\0"
		"CODE - PSI\0"
		);

	if(!dis_exit()) screenin(pic5b,
		"\0MUSIC - PURPLE MOTION\0"
		"CODE - TRUG\0"
		);

	if(!dis_exit()) screenin(pic6,
		"\0MUSIC - PURPLE MOTION\0"
		"CODE - PSI\0"
		);

	if(!dis_exit()) screenin(pic7,
		"\0MUSIC - PURPLE MOTION\0"
		"CODE - PSI\0"
		);

	if(!dis_exit()) screenin(pic8,
		"\0GRAPHICS - PIXEL\0"
		"MUSIC - PURPLE MOTION\0"
		);

	if(!dis_exit()) screenin(pic9,
		"GRAPHICS - PIXEL\0"
		"MUSIC - PURPLE MOTION\0"
		"CODE - TRUG\0"
		"RENDERING - TRUG\0"
		);

	if(!dis_exit()) screenin(pic10,
		"GRAPHICS - PIXEL, SKAVEN\0"
		"MUSIC - PURPLE MOTION\0"
		"CODE - PSI\0"
		);

	if(!dis_exit()) screenin(pic10b,
		"GRAPHICS - PIXEL, SKAVEN\0"
		"MUSIC - PURPLE MOTION\0"
		"CODE - PSI\0"
		);

	if(!dis_exit()) screenin(pic11,
		"\0MUSIC - PURPLE MOTION\0"
		"CODE - WILDFIRE\0"
		);

	if(!dis_exit()) screenin(pic12,
		"\0MUSIC - PURPLE MOTION\0"
		"CODE - WILDFIRE\0"
		);

	if(!dis_exit()) screenin(pic13,
		"\0MUSIC - PURPLE MOTION\0"
		"CODE - PSI\0"
		);

	if(!dis_exit()) screenin(pic14,
		"GRAPHICS - PIXEL\0"
		"MUSIC - PURPLE MOTION\0"
		"CODE - TRUG\0"
		"RENDERING - TRUG\0"
		);

	if(!dis_exit()) screenin(pic14b,
		"\0MUSIC - PURPLE MOTION\0"
		"CODE - PSI\0"
		);

	if(!dis_exit()) screenin(pic15,
		"GRAPHICS - MARVEL\0"
		"MUSIC - PURPLE MOTION\0"
		"CODE - PSI\0"
		);

	if(!dis_exit()) screenin(pic16,
		"\0MUSIC - SKAVEN\0"
		"CODE - PSI\0"
		);

	if(!dis_exit()) screenin(pic17,
		"\0GRAPHICS - PIXEL\0"
		"MUSIC - PURPLE MOTION\0"
		);

	if(!dis_exit()) screenin(pic18,
		"GRAPHICS - PIXEL\0"
		"MUSIC - PURPLE MOTION\0"
		"CODE - WILDFIRE\0"
		);

	//tw_closegraph();
	}

screenin(char (* pic)[160], char *text)
	{
	int	a,x,y,yy,v;

	tw_setsplit(400);
	tw_clrscr();
	tw_setstart(160*200);
	dis_waitb();
	tw_setpalette(&pic[0][16]);
	pic=&pic[0][784];

	y=16;while(*(text=prtc(160,y,text))) y+=FONAY+10;

	for(x=0;x<160;x++) for(y=0;y<100;y++) tw_putpixel(400+x,400+y*2,pic[y][x]+16);

	for(y=200*128;y>0;y=y*12L/13)
		{
		dis_waitb();
		tw_setsplit(y/128+200);
		yy=320-y/80;
		for(a=0;a<10000;a++);
		tw_setstart(160*200+(yy/4));
		asm	{
			mov	dx, 0x3c0
			mov	al, 0x33
			out	dx, al
			mov	ax, yy
			and	ax, 3
			shl	ax, 1
			out	dx, al
			}
		}

	for(a=0;a<200 && !dis_exit();a++) dis_waitb();

	for(y=0,v=0;y<128*200;y=y+v,v+=15)
		{
		dis_waitb();
		tw_setsplit(y/128+200);
		yy=320+y/80;
		for(a=0;a<10000;a++);
		tw_setstart(160*200+(yy/4));
		asm	{
			mov	dx, 0x3c0
			mov	al, 0x33
			out	dx, al
			mov	ax, yy
			and	ax, 3
			shl	ax, 1
			out	dx, al
			}
		}
	}

prt(int x,int y,char *txt)
{
	int	x2w,x2,y2,y2w=y+FONAY,sx,d;
	while(*txt)
	{
		x2w=fonaw[*txt]+x;
		sx=fonap[*txt];
		for(x2=x;x2<x2w;x2++)
		{
			for(y2=y;y2<y2w;y2++)
			{
				d=font[y2-y][sx];
				tw_putpixel(x2,y2,d);
			}
			sx++;
		}
		x=x2+2;
		txt++;
	}
}

char *prtc(int x,int y,char *txt)
{
	int	w=0;
	char	*t=txt;
	while(*t) w+=fonaw[*t++]+2;
	prt(x-w/2,y,txt);
	return(t+1);
}


init()
	{
	int	x,y,a,b;

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
	fonap[32]=1500-32;
	fonaw[32]=8;

	memmove(&pic1[16*3+16],&pic1[16],768-16*3);
	memmove(&pic2[16*3+16],&pic2[16],768-16*3);
	memmove(&pic3[16*3+16],&pic3[16],768-16*3);
	memmove(&pic4[16*3+16],&pic4[16],768-16*3);
	memmove(&pic5[16*3+16],&pic5[16],768-16*3);
	memmove(&pic5b[16*3+16],&pic5b[16],768-16*3);
	memmove(&pic6[16*3+16],&pic6[16],768-16*3);
	memmove(&pic7[16*3+16],&pic7[16],768-16*3);
	memmove(&pic8[16*3+16],&pic8[16],768-16*3);
	memmove(&pic9[16*3+16],&pic9[16],768-16*3);
	memmove(&pic10[16*3+16],&pic10[16],768-16*3);
	memmove(&pic10b[16*3+16],&pic10b[16],768-16*3);
	memmove(&pic11[16*3+16],&pic11[16],768-16*3);
	memmove(&pic12[16*3+16],&pic12[16],768-16*3);
	memmove(&pic13[16*3+16],&pic13[16],768-16*3);
	memmove(&pic14[16*3+16],&pic14[16],768-16*3);
	memmove(&pic14b[16*3+16],&pic14b[16],768-16*3);
	memmove(&pic15[16*3+16],&pic15[16],768-16*3);
	memmove(&pic16[16*3+16],&pic16[16],768-16*3);
	memmove(&pic17[16*3+16],&pic17[16],768-16*3);
	memmove(&pic18[16*3+16],&pic18[16],768-16*3);
	for(a=0;a<10;a++)
		{
		pic1[a*3+0+16]=pic1[a*3+1+16]=pic1[a*3+2+16]=7*a;
		pic2[a*3+0+16]=pic2[a*3+1+16]=pic2[a*3+2+16]=7*a;
		pic3[a*3+0+16]=pic3[a*3+1+16]=pic3[a*3+2+16]=7*a;
		pic4[a*3+0+16]=pic4[a*3+1+16]=pic4[a*3+2+16]=7*a;
		pic5[a*3+0+16]=pic5[a*3+1+16]=pic5[a*3+2+16]=7*a;
		pic5b[a*3+0+16]=pic5b[a*3+1+16]=pic5b[a*3+2+16]=7*a;
		pic6[a*3+0+16]=pic6[a*3+1+16]=pic6[a*3+2+16]=7*a;
		pic7[a*3+0+16]=pic7[a*3+1+16]=pic7[a*3+2+16]=7*a;
		pic8[a*3+0+16]=pic8[a*3+1+16]=pic8[a*3+2+16]=7*a;
		pic9[a*3+0+16]=pic9[a*3+1+16]=pic9[a*3+2+16]=7*a;
		pic10[a*3+0+16]=pic10[a*3+1+16]=pic10[a*3+2+16]=7*a;
		pic10b[a*3+0+16]=pic10b[a*3+1+16]=pic10b[a*3+2+16]=7*a;
		pic11[a*3+0+16]=pic11[a*3+1+16]=pic11[a*3+2+16]=7*a;
		pic12[a*3+0+16]=pic12[a*3+1+16]=pic12[a*3+2+16]=7*a;
		pic13[a*3+0+16]=pic13[a*3+1+16]=pic13[a*3+2+16]=7*a;
		pic14[a*3+0+16]=pic14[a*3+1+16]=pic14[a*3+2+16]=7*a;
		pic14b[a*3+0+16]=pic14b[a*3+1+16]=pic14b[a*3+2+16]=7*a;
		pic15[a*3+0+16]=pic15[a*3+1+16]=pic15[a*3+2+16]=7*a;
		pic16[a*3+0+16]=pic16[a*3+1+16]=pic16[a*3+2+16]=7*a;
		pic17[a*3+0+16]=pic17[a*3+1+16]=pic17[a*3+2+16]=7*a;
		pic18[a*3+0+16]=pic18[a*3+1+16]=pic18[a*3+2+16]=7*a;
		}
	}

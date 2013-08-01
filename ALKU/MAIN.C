#include <dos.h>
#include <stdio.h>
#include <conio.h>
#include "tweak.h"
#include "h:\u2\dis\dis.h"

#define SCRLF 9

extern	init_copper();
extern	close_copper();
extern	far int frame_count;
extern  far char * far cop_pal;
extern  far int do_pal;
extern  far int cop_start;
extern  far int cop_scrl;
extern	far int cop_dofade;
extern	far char * far cop_fadepal;
extern 	far char fadepal[];

extern char far hzpic[];
extern outline(char far *f, char far *t);
extern ascrolltext(int scrl, int *dtau);
char	* far vmem=(char *)0x0a0000000L;
int	mmask[4]={0x0102,0x0202,0x0402,0x0802};

char (* far vvmem)[176];
extern char far font[31][1500];
char rowbuf[1024];

char	palette[768];		// pic
char	palette2[768];		// pic & text

char	fade1[768];			// black
char	fade2[768];			// text

int	picin[768];
int	textin[768];
int	textout[768];

char	*fonaorder="ABCDEFGHIJKLMNOPQRSTUVWXabcdefghijklmnopqrstuvwxyz0123456789!?,.:èè()+-*='èô";
int	fonap[256];
int	fonaw[256];

int	far dtau[30000];
char	far tbuf[186][352];

int	a=0,p=0,tptr=0;

main()
	{
	int	aa,b,c,x,y,f;

	asm	mov	ax, 3
	asm	int	10h

	init();


	while(dis_sync()<1 && !dis_exit());

	prtc(160,120,"A");
	prtc(160,160,"Future Crew");
	prtc(160,200,"Production");
	dofade(fade1,fade2); wait(300); dofade(fade2,fade1); fonapois();

	while(dis_sync()<2 && !dis_exit());

	prtc(160,160,"First Presented");
	prtc(160,200,"at Assembly 93");
	dofade(fade1,fade2); wait(300); dofade(fade2,fade1); fonapois();

	while(dis_sync()<3 && !dis_exit());

	prtc(160,120,"in");
	prtc(160,160,"è");
	prtc(160,179,"ô");
	dofade(fade1,fade2); wait(300); dofade(fade2,fade1); fonapois();

	while(dis_sync()<4 && !dis_exit());

	memcpy(fadepal,fade1,768);
	cop_fadepal=picin;
	cop_dofade=128;
	for(a=1,p=1,f=0,frame_count=0;cop_dofade!=0 && !dis_exit();)
		do_scroll(2);

	for(f=60;a<320 && !dis_exit();)
		{
		if(f==0) {
			cop_fadepal=textin;
			cop_dofade=64;
			f+=20;
			}
		else if(f==50) {
			cop_fadepal=textout;
			cop_dofade=64;
			f++;
			}
		else if(f>50 && cop_dofade==0) {
			cop_pal=palette; do_pal=1; f++;
			memset(tbuf,0,186*320);
			switch(tptr++) {
			case 0:
				addtext(160,50,"Graphics");
				addtext(160,90,"Marvel");
				addtext(160,130,"Pixel");	// sucks
				ffonapois();
				break;
			case 1:
				faddtext(160,50,"Music");
				faddtext(160,90,  "Purple Motion");
				faddtext(160,130, "Skaven");
				ffonapois();
				break;
			case 2:
				faddtext(160,30,"Code");
				faddtext(160,70,  "Psi");
				faddtext(160,110, "Trug");
				faddtext(160,148, "Wildfire");
				ffonapois();
				break;
			case 3:
				faddtext(160,50,"Additional Design");
				faddtext(160,90, "Abyss");
//				faddtext(160,110,"Useless Design");
				faddtext(160,130, "Gore");
				ffonapois();
				break;
			case 4:
				ffonapois();
				break;
			default:
				faddtext(160,80, "BUG BUG BUG");
				faddtext(160,130, "Timing error");
				ffonapois();
				break;
				}
			while(((a&1) || dis_sync()<4+tptr) && !dis_exit() && a<319)
				do_scroll(0);
			aa=a;
			if(aa<320-12) fmaketext(aa+16);
			f=0;
			}
		else	f++;
		do_scroll(1);
		}
	if(f>63/SCRLF){
		dofade(palette2,palette);
		}
	fonapois();
	close_copper();
	}

init()	{
	int	a,b,c,x,y,p=0,f;

	dis_partstart();
	tw_opengraph();
	init_copper();
	tw_setpalette(fade1);
	memcpy(palette,hzpic+16,768);

	for(a=0;a<88;a++)
		{
		outline(MK_FP(FP_SEG(hzpic),FP_OFF(hzpic)+a*4+784), MK_FP(0x0a000,a+176*50));
		outline(MK_FP(FP_SEG(hzpic),FP_OFF(hzpic)+a*4+784), MK_FP(0x0a000,a+176*50+88));
		}

	for(y=0;y<32;y++)
	{
		for(a=0;a<1500;a++)
		{
			switch(font[y][a]&3)
			{
			case 0x1 : b=0x40; break;
			case 0x2 : b=0x80; break;
			case 0x3 : b=0xc0; break;
			default : b=0;
			}
			font[y][a]=b;
		}
	}

	for(y=0;y<768;y+=3)
	{
		if(y<64*3)
		{
		palette2[y+0]=palette[y+0];
		palette2[y+1]=palette[y+1];
		palette2[y+2]=palette[y+2];
		}
		else if(y<128*3)
		{
			palette2[y+0]=(fade2[y+0]=palette[0x1*3+0])*63+palette[y%(64*3)+0]*(63-palette[0x1*3+0])>>6;
			palette2[y+1]=(fade2[y+1]=palette[0x1*3+1])*63+palette[y%(64*3)+1]*(63-palette[0x1*3+1])>>6;
			palette2[y+2]=(fade2[y+2]=palette[0x1*3+2])*63+palette[y%(64*3)+2]*(63-palette[0x1*3+2])>>6;
		}
		else if(y<192*3)
		{
			palette2[y+0]=(fade2[y+0]=palette[0x2*3+0])*63+palette[y%(64*3)+0]*(63-palette[0x2*3+0])>>6;
			palette2[y+1]=(fade2[y+1]=palette[0x2*3+1])*63+palette[y%(64*3)+1]*(63-palette[0x2*3+1])>>6;
			palette2[y+2]=(fade2[y+2]=palette[0x2*3+2])*63+palette[y%(64*3)+2]*(63-palette[0x2*3+2])>>6;
		}
		else
		{
			palette2[y+0]=(fade2[y+0]=palette[0x3*3+0])*63+palette[y%(64*3)+0]*(63-palette[0x3*3+0])>>6;
			palette2[y+1]=(fade2[y+1]=palette[0x3*3+1])*63+palette[y%(64*3)+1]*(63-palette[0x3*3+1])>>6;
			palette2[y+2]=(fade2[y+2]=palette[0x3*3+2])*63+palette[y%(64*3)+2]*(63-palette[0x3*3+2])>>6;
		}
	}

	for(a=192;a<768;a++) palette[a]=palette[a-192];

	for(x=0;x<1500 && *fonaorder;)
	{
		while(x<1500)
		{
			for(y=0;y<32;y++) if(font[y][x]) break;
			if(y!=32) break;
			x++;
		}
		b=x;
		while(x<1500)
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
	fonap[32]=1500-20;
	fonaw[32]=16;

	for(a=0;a<768;a++)
		{
		textin[a]=(palette2[a]-palette[a])*256/64;
		textout[a]=(palette[a]-palette2[a])*256/64;
		picin[a]=(palette[a]-fade1[a])*256/128;
		}
	}

wait(int t)
	{
	while(frame_count<t && !dis_exit()); frame_count=0;
	}

fonapois()
	{
	char 	far *vvmem=MK_FP(0x0a000,0);
	unsigned a;

	outport(0x3c4,0x0102);
	outport(0x3ce,0x0004);
	for(a=160*64;a<160U*(64+256);a++) vvmem[a]=vvmem[a]&63;

	outport(0x3c4,0x0202);
	outport(0x3ce,0x0104);
	for(a=160*64;a<160U*(64+256);a++) vvmem[a]=vvmem[a]&63;

	outport(0x3c4,0x0402);
	outport(0x3ce,0x0204);
	for(a=160*64;a<160U*(64+256);a++) vvmem[a]=vvmem[a]&63;
	outport(0x3c4,0x0802);
	outport(0x3ce,0x0304);
	for(a=160*64;a<160U*(64+256);a++) vvmem[a]=vvmem[a]&63;

	}

prt(int x,int y,char *txt)
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
				tw_putpixel(x2,y2,tw_getpixel(x2,y2)|d);
			}
			sx++;
		}
		x=x2+2;
		txt++;
	}
}

prtc(int x,int y,char *txt)
{
	int	w=0;
	char	*t=txt;
	while(*t) w+=fonaw[*t++]+2;
	prt(x-w/2,y,txt);
}

dofade(char far *pal1, char far *pal2)
	{
	int	a,b,c;
	char	pal[768];

	for(a=0;a<64 && !dis_exit();a++)
		{
		for(b=0;b<768;b++) pal[b]=(pal1[b]*(64-a)+pal2[b]*a>>6);
		cop_pal=pal; do_pal=1;
		while(frame_count<1); frame_count=0;
		}
	}
char	fuckpal[768];

fdofade(char far *pal1, char far *pal2, int a)
	{
	int	b,c;

	if(a<0 || a>64) return(0);
	for(b=0;b<768;b++) fuckpal[b]=(pal1[b]*(64-a)+pal2[b]*a>>6);
	cop_pal=fuckpal; do_pal=1;
	}

addtext(int tx,int ty,char *txt)
	{
	int	a,b,c,x,y,w=0;
	char	*t=txt;

	while(*t) w+=fonaw[*t++]+2;

	t=txt; w/=2;
	while(*t)
		{
		for(x=0;x<fonaw[*t];x++)
			for(y=0;y<32;y++)
				tbuf[y+ty][tx+x-w]=font[y][fonap[*t]+x];

		tx+=fonaw[*t++]+2;
		}
	}


maketext(int scrl)
	{
	char 	far *vvmem=MK_FP(0x0a000,0);
	int	*p1=dtau;
	int	mtau[]={1*256+2,2*256+2,4*256+2,8*256+2};
	int	a,b,c,x,y,m;

	for(m=0;m<4;m++)
		{
		for(x=m;x<320;x+=4) for(y=1;y<184;y++)
			if(tbuf[y][x]!=tbuf[y][x-2]) {
				*p1++=x/4+y*176+100*176;
				*p1++=tbuf[y][x]^tbuf[y][x-2];
				}
		*p1++=-1;
		*p1++=-1;
		}

	for(x=0;x<320;x++)
		{
		outport(0x3c4,mtau[(x+scrl)&3]);
		outport(0x3ce,((x+scrl)&3)*256+4);
		for(y=1;y<184;y++)
			{
			vvmem[y*176+176*100+(x+scrl)/4]^=tbuf[y][x-1-1];
			vvmem[y*176+176*100+(x+scrl)/4+88]^=tbuf[y][x-1];
			}
		}
	}

scrolltext(int scrl)
	{
	char 	far *vvmem=MK_FP(0x0a000,0);
	int	mtau[]={1*256+2,2*256+2,4*256+2,8*256+2,1*256+2,2*256+2,4*256+2,8*256+2};
	int	*p1=dtau;
	int	x,y,a,c,m,aa;

	p1=dtau;
	for(m=0;m<4;m++)
		{
		aa=(scrl+m)/4;
		outport(0x3c4,mtau[(scrl+m)&3]);
		outport(0x3ce,((scrl+m)&3)*256+4);
		while(*p1!=-1)
			{
			a=*p1++;
			c=*p1++;
			vvmem[a+aa]^=c;
			}
		p1+=2;
		}
	}


do_scroll(int mode)
	{
	if(mode==0 && frame_count<SCRLF) return(0);
	while(frame_count<SCRLF);
	frame_count-=SCRLF;
	if(mode==1) ascrolltext(a+p*352,dtau);
	cop_start=a/4+p*88; cop_scrl=(a&3)*2;

	if((a&3)==0)
		{
		outline(MK_FP(FP_SEG(hzpic),FP_OFF(hzpic)+(a/4+86)*4+784), MK_FP(0x0a000,(a/4+86)+176*50));
		outline(MK_FP(FP_SEG(hzpic),FP_OFF(hzpic)+(a/4+86)*4+784), MK_FP(0x0a000,(a/4+86)+176*50+88));
		}
	a+=1; p^=1;
	return(1);
	}




faddtext(int tx,int ty,char *txt)
	{
	int	a,b,c,x,y,w=0;
	char	*t=txt;

	while(*t) w+=fonaw[*t++]+2;

	t=txt; w/=2;
	while(*t)
		{
		for(x=0;x<fonaw[*t];x++)
			for(y=0;y<32;y++)
				tbuf[y+ty][tx+x-w]=font[y][fonap[*t]+x];

		do_scroll(0);
		tx+=fonaw[*t++]+2;
		}
	}

fmaketext(int scrl)
	{
	char 	far *vvmem=MK_FP(0x0a000,0);
	int	*p1=dtau;
	int	mtau[]={1*256+2,2*256+2,4*256+2,8*256+2};
	int	b,c,x,y,m;

	for(m=0;m<4;m++)
		{
		for(x=m;x<320;x+=4) {
			for(y=1;y<184;y++) if(tbuf[y][x]!=tbuf[y][x-2]) {
				*p1++=x/4+y*176+100*176;
				*p1++=tbuf[y][x]^tbuf[y][x-2];
				}
			do_scroll(0);
			}
		*p1++=-1;
		*p1++=-1;
		}

	for(x=0;x<320;x++)
		{
		outport(0x3c4,mtau[(x+scrl)&3]);
		outport(0x3ce,((x+scrl)&3)*256+4);
		for(y=1;y<184;y++)
			{
			vvmem[y*176+176*100+(x+scrl)/4]^=tbuf[y][x-1-1];
			vvmem[y*176+176*100+(x+scrl)/4+88]^=tbuf[y][x-1];
			}
		do_scroll(0);
		}

	while(a<=scrl) do_scroll(0);
	}

ffonapois()
	{
	long 	far *vvmem=MK_FP(0x0a000,0);
	unsigned a;

	outport(0x3c4,0x0102);
	outport(0x3ce,0x0004);
	for(a=40*64;a<40U*(64+256+10);a++) vvmem[a]=vvmem[a]&0x3f3f3f3f;
	do_scroll(0);

	outport(0x3c4,0x0202);
	outport(0x3ce,0x0104);
	for(a=40*64;a<40U*(64+256+10);a++) vvmem[a]=vvmem[a]&0x3f3f3f3f;
	do_scroll(0);

	outport(0x3c4,0x0402);
	outport(0x3ce,0x0204);
	for(a=40*64;a<40U*(64+256+10);a++) vvmem[a]=vvmem[a]&0x3f3f3f3f;
	do_scroll(0);

	outport(0x3c4,0x0802);
	outport(0x3ce,0x0304);
	for(a=40*64;a<40U*(64+256+10);a++) vvmem[a]=vvmem[a]&0x3f3f3f3f;
	do_scroll(0);
	}

char far cfpal[768*2];
int far cop_fade;

fffade(char far *pal1, char far *pal2, int frames)
	{
	int	a,b,c;
	for(a=0;a<768;a++)
		{
		cfpal[a]=pal1[a];
		cfpal[a+768]=(pal2[a]-pal1[a])*256/frames;
		}
	cop_fade=frames;
	}

/*
dis_sync

0	= ...
1	= fc_pres
2	= first
3	= maisema
4	= gfx
5	= music
6	= code
7	= addi
8	= exit

dis_muscode=row/order

2  ekaa = mustaa
2  fc..
3  93..
4  feidaa ineen
5  music
6  gfx
7  code
8  addi

bx=6
ax=?
int fch
cx=ord
bx=row


*/
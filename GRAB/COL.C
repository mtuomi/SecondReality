#include <stdio.h>
#include <bios.h>
#include <malloc.h>
#include <memory.h>

int	colsreserved=1;

int	setback255=0;
int	filetype; /* 1=LBM */
int	backcol=255;

unsigned char far *vram=(char far *)0xa0000000;
union	REGS	reg;
unsigned char far pic[64000];
unsigned char far pic1[64000];
unsigned char far pic2[64000];
unsigned char far bigpal[8192*3];
unsigned char far bigpalhsv[8192*3];
unsigned int	  bigcnt[8192];
unsigned int	  bigcntbak[8192];
int	bigpalpnt,bigpalnum;
unsigned char     palmap[256];
unsigned char far pal1[768];
unsigned char far pal2[768];
unsigned char far palhsv[768];
unsigned char far tmphsv[768];
unsigned char far pal[768];
unsigned char far defpal[16*3]={
00,00,00,
00,00,40,
00,40,00,
00,40,40,
40,00,00,
40,00,40,
40,20,00,
40,40,40,
20,20,20,
20,20,60,
20,60,20,
20,60,60,
60,20,20,
60,20,60,
60,60,20,
60,60,60,
};

int eschit(void)
{
	return(kbhit());
}

#include "loadlbm.c"

main(int argc,char *argv[])
{
	int	a,b,c,d,x,y;
	printf("\nColorMixer V1.0   (C) 1991 Sami Tammilehto   \n\n");
	if(argc==1)
	{
		printf("usage: col [?###] <src1> <src2>\n"
			"when picture is in screen, press space to\n"
			"toggle pics/palette, ESC to exit.\n"
			"!### means reserve ### first colors for other\n"
			"purposes (leave empty/to default pal)\n");
		return(0);
	}
	if(*argv[1]=='?')
	{
		colsreserved=atoi(argv[1]+1);
		argv[1]=argv[2];
		argv[2]=argv[3];
	}
	bigpalpnt=0;
	printf("Processing, please wait...\n");
	printf("File: %s",argv[1]);
	memset(pic,0,64000);
	memset(pal,0,768);
	loadlbm(argv[1]); printf("100%%\n");
	memcpy(pic1,pic,64000);
	memcpy(pal1,pal,768);
	analyze(pic1,pal1);
	printf("File: %s",argv[2]);
	memset(pic,0,64000);
	memset(pal,0,768);
	loadlbm(argv[2]); printf("100%%\n");
	memcpy(pic2,pic,64000);
	memcpy(pal2,pal,768);
	analyze(pic2,pal2);
	bigpalnum=bigpalpnt;
	printf("Colors in big palette: %i\n",bigpalnum);
	printf("Combining and optimizing palettes...\n");
	memset(pal,0,768);
	memcpy(pal,defpal,16*3);
	createpal();
	printf("Remapping first picture...                \n");
	remap(pic1,pal1,pal);
	printf("Remapping second picture...               \n");
	remap(pic2,pal2,pal);
	reg.x.ax=0x13;
	int86(0x10,&reg,&reg);
	{
		outp(0x3c8,0);
		for(a=0;a<768;a++) outp(0x3c9,pal[a]);
	}
	for(;;)
	{
		memset(vram,0,64000);
		for(a=0;a<16;a++) for(b=0;b<16;b++)
		{
			x=a*12; y=b*10;
			for(c=0;c<11;c++) for(d=0;d<9;d++)
			{
				*(vram+x+c+(y+d)*320)=b+a*16;
			}
		}
		a=getch();
		if(a==13 || a==27) break;
		memcpy(vram,pic1,64000);
		a=getch();
		if(a==13 || a==27) break;
		memcpy(vram,pic2,64000);
		a=getch();
		if(a==13 || a==27) break;
	}
	reg.x.ax=0x3;
	int86(0x10,&reg,&reg);
}

unsigned colcnt[256];

int	analyze(unsigned char *pic,unsigned char *pal)
{
	int	a,b,c,x,y;
	printf("Generating big palette and calculating relative color importances...\n");
	memset(colcnt,0,256*2);
	for(y=0;y<200;y++) for(x=0;x<320;x++)
	{
		colcnt[pic[x+y*320]]++;
	}
	for(a=0;a<256;a++)
	{
		if(colcnt[a]!=0)
		{
			bigpal[bigpalpnt*3+0]=pal[a*3+0];
			bigpal[bigpalpnt*3+1]=pal[a*3+1];
			bigpal[bigpalpnt*3+2]=pal[a*3+2];
			bigcnt[bigpalpnt++]=colcnt[a];
		}
	}
}


int	createpal(void)
{
	int	testval,testcnt;
	int	colors=256-colsreserved,firstcol=colsreserved;
	int	xr,xb,xg,xh,xs,xv;
	int	a,b,c,d,e,f,g,h;
	int	x,y;
	int	s;

	rgb2hsv(bigpalhsv,bigpal,bigpalnum);

    for(testcnt=0;testcnt<5;testcnt++)
    {
    	switch(testcnt)
	{
		case 0 : testval=0; break;
		case 1 : testval=1; break;
		case 2 : testval=3; break;
		case 3 : testval=5; break;
		case 4 : testval=8; break;
	}
	memset(pal+firstcol*3,0,colors*3);
	memcpy(bigcntbak,bigcnt,bigpalnum*2);
	for(a=0;a<colors;)
	{
	    	printf("Processing pass %i/5:  %i%% complete  \r",testcnt+1,100*a/256);
		{
			unsigned int c;
			c=0; /* max */
			d=0; /* maxcolowner */
			for(b=0;b<bigpalnum;b++)
			{
				if(bigcnt[b]>c) { c=bigcnt[b]; d=b; }
			}
			if(c==0) { testcnt=999; break; }
			s=d; bigcnt[d]=0;
		}
		
		{
			xr=bigpal[s*3+0];
			xg=bigpal[s*3+1];
			xb=bigpal[s*3+2];
			xh=bigpalhsv[s*3+0];
			xs=bigpalhsv[s*3+1];
			xv=bigpalhsv[s*3+2];
			c=999; /* difference */
			for(b=firstcol;b<a+firstcol;b++)
			{
				f=b*3;
				g=((xr-(unsigned int)pal[f+0]));
				e=(g<0?-g:g);
				g=((xg-(unsigned int)pal[f+1]));
				e+=(g<0?-g:g);
				g=((xb-(unsigned int)pal[f+2]));
				e+=(g<0?-g:g);
				/*g=((xh-(unsigned int)palhsv[f+0]));
				e=(g<0?-g:g)*2;
				g=((xs-(unsigned int)palhsv[f+1]));
				e+=(g<0?-g:g)*2;
				g=((xv-(unsigned int)palhsv[f+2]));
				e+=(g<0?-g:g);*/
				if(e<c) { c=e; }
			}
		}
		if(c>testval)
		{
			d=a+firstcol;
			pal[d*3]=bigpal[s*3];
			pal[d*3+1]=bigpal[s*3+1];
			pal[d*3+2]=bigpal[s*3+2];
			rgb2hsv(palhsv+d*3,pal+d*3,1);
			a++;
		}
	}
	memcpy(bigcnt,bigcntbak,bigpalnum*2);
    }
	rgb2hsv(palhsv,pal,256);
}

int	remap(unsigned char *pic,unsigned char *spal,unsigned char *pal)
{
	int	xr,xb,xg,xh,xs,xv;
	int	a,b,c,d,e,f,g,h;
	memset(palmap,0,256);
	rgb2hsv(tmphsv,spal,256);
	for(a=0;a<256;a++)
	{
	    	printf("Processing:  %i%% complete  \r",100*a/256/2);
		xr=spal[a*3+0];
		xg=spal[a*3+1];
		xb=spal[a*3+2];
		xh=tmphsv[a*3+0];
		xs=tmphsv[a*3+1];
		xv=tmphsv[a*3+2];
		h=0; c=999; /* difference */
		for(b=colsreserved;b<256;b++)
		{
			f=b*3;
			/*g=(xh-(unsigned int)palhsv[f+0]);
			e=(g<0?-g:g)*2;
			g=(xs-(unsigned int)palhsv[f+1]);
			e+=(g<0?-g:g);
			g=(xv-(unsigned int)palhsv[f+2]);
			e+=(g<0?-g:g)*2;*/
			g=((xr-(unsigned int)pal[f+0]));
			e=(g<0?-g:g);
			g=((xg-(unsigned int)pal[f+1]));
			e+=(g<0?-g:g);
			g=((xb-(unsigned int)pal[f+2]));
			e+=(g<0?-g:g);
			if(e<c) { c=e; h=b; }
		}
		palmap[a]=h;
	}
	for(a=0;a<200;a++)
	{
	    	printf("Processing:  %i%% complete  \r",100*a/200/2+50);
		for(b=0;b<320;b++)
		{
			pic[a*320+b]=palmap[pic[a*320+b]];
		}
	}
}

int	minval(int v1,int v2,int v3)
{
	if(v1<v2 && v1<v3) return(v1);
	if(v2<v1 && v2<v3) return(v2);
	if(v3<v1 && v3<v2) return(v3);
	return(v1);
}

int	maxval(int v1,int v2,int v3)
{
	if(v1>v2 && v1>v3) return(v1);
	if(v2>v1 && v2>v3) return(v2);
	if(v3>v1 && v3>v2) return(v3);
	return(v1);
}

int	rgb2hsv(unsigned char *dst,unsigned char *src,int num)
{
	int	cr,cg,cb;
	int	tmp,i;
	int	xr,xg,xb,zh,zs,zv;
	for(i=0;i<num;i++)
	{
		xr=*(src++);
		xg=*(src++);
		xb=*(src++);
		/* value */
		zv=maxval(xr,xg,xb);
		/* saturation */
		tmp=minval(xr,xg,xb);
		if(zv==0) zs=0; else zs=63*(zv-tmp)/zv;
		/* hue */
		if(zs==0) zh=127;
		else
		{
			cr=100*(zv-xr)/(zv-tmp);
			cg=100*(zv-xg)/(zv-tmp);
			cb=100*(zv-xb)/(zv-tmp);
			if(xr==zv) zh=cb-cg;
			if(xg==zv) zh=200+cr-cb;
			if(xb==zv) zh=400+cg-cr;
			if(zh<0) zh+=500;
			zh/=8;
		}
		*(dst++)=zh;
		*(dst++)=zs;
		*(dst++)=zv;
	}
}


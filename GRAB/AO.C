
#include <bios.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <graph.h>
#include <memory.h>
#include <malloc.h>

int	xit=0;
int	showonly=0;
int	greycol,blackcol;
int	fcp4plane=0;
int	fcptransp=1;
int	fcporigo=0;
int	fcpsavepal=1; 
int	fcpstyle=15; /* wing commander style */
int	setback255=0;

char	filename[64];
char	showname[16][64];
int	shownum=0;
int	paletteset=0;

int	bx1,by1,bx2,by2;

int	pass;
int	picnum=0,picwrt=0;

FILE	*f1;
FILE	*f2; /* out */

union	REGS	reg;
unsigned char far *vram=(char far *)0xa0000000;

char	apubuf[512];

unsigned char pal[768];
unsigned char pic[64000];
unsigned char pic2[64000];
unsigned char picorg[64000];
int	backcol=0;

#define SUBMAX 384
long picpos[SUBMAX],pics; /* for loading FCP:s */
long wpicpos[SUBMAX],wpics; /* for loading FCP:s */

#define	pxor(xx,yy) *(vram+(xx)+(yy)*320)=255-*(vram+(xx)+(yy)*320)
#define	pset(xx,yy,zz) *(vram+(xx)+(yy)*320)=(zz)
#define	pget(xx,yy) *(vram+(xx)+(yy)*320)

#define AOLOAD

#include "lfcp.c"
#include "sfcp.c"

main(int argc,char *argv[])
{
	int	a,b,c;
	*filename=0;
	for(a=1;a<argc;a++)
	{
		if(*argv[a]=='/' || *argv[a]=='-') switch(*(argv[a]+1))
		{
			case 'h' :
			case 'H' :
			case '?' : *filename=0; a=argc; break;
			case 'o' : backcol=0; break;
			default :
				printf("Unknown option: %s\n",argv[a]);
				exit(1);
		}
		else 
		{
			if(*filename==0) strcpy(filename,argv[a]);
			else strcpy(showname[shownum++],argv[a]);
		}
	}
	if(!*filename)
	{
		printf("FCP Animation Optimizer V2.0 - input format: FCP\n"
		"usage: VP2 <destfile> <strfile(s)>            Switches:\n"
		"-h		help\n"
		"-o0		set backg. color to 0\n"
		); return(0);
	}
	else printf("FCP Animation Optimizer V2.0\n");
	
	/* do some real stuff */
	reg.x.ax=0x13;
	int86(0x10,&reg,&reg);
	{
		pass=0;
		bx1=by1=bx2=by2=0;
		memset(pic,backcol,64000);
		memset(picorg,backcol,64000);
		viewpic();
		for(b=0;b<shownum;b++)
		{
			a=loadfcp(showname[b]);
			if(a==3)
			{
				printf("\nFile not found. Program aborted.\n");
				return(0);
			}
			else if(a==1) 
	 		{
				printf("\nError loading picture. Program aborted.\n");
				return(0);
			}
		}
	}
	if(savefcp(filename)) 
	{
		printf("Save error (filename:%s)!\n",filename);
		exit(3);
	}
	reg.x.ax=0x3;
	int86(0x10,&reg,&reg);
}

int	onepicdone(void)
{
	unsigned int u;
	if(pass==0)
	{
		picnum++;
		viewpic(0);
	}
	else
	{
		#ifdef DOANALYZE
		/* analyze */
		for(u=0;u<64000;u++)
		{
			if(pic[u+0]==backcol) pic[u]=backcol;
			else picorg[u]=pic[u];
		}
		/*for(u=0;u<63999;u++)
		{
			if(pic[u]!=backcol && pic[u+1]==backcol && pic[u+2]==backcol)
				pic[u]=picorg[u];
		}*/
		#endif
		viewpic(0);
		/* save */
		wpicpos[picwrt++]=ftell(f2);
		savefcpdata(f2,bx1,by1,bx2,by2);
	}
}

int	eschit(void)
{
	int	a;
	if(kbhit()) 
	{
		a=getch();
		if(a==27 || a==32) return(1);
	}
	return(0);
}

int	findcol(int r,int g,int b)
{
	int	dif=999,a,c,e,col=0;
	for(a=0;a<256;a++)
	{
		c=r-pal[a*3+0]; if(c<0) c=-c;
		e=c;
		c=g-pal[a*3+1]; if(c<0) c=-c;
		e+=c;
		c=b-pal[a*3+2]; if(c<0) c=-c;
		e+=c;
		if(e<dif) { dif=e; col=a; }
	}
	return(col);
}

int	gotoxy(int x,int y)
{
	reg.h.ah=0x02;
	reg.h.bh=0x00;
	reg.h.dh=y;
	reg.h.dl=x;
	int86(0x10,&reg,&reg);
}

int	area(int x1,int y1,int x2,int y2,int c)
{
	int	x,y;
	if(c==-1)
	{
		for(y=y1;y<=y2;y++) for(x=x1;x<=x2;x++)
		{
			pxor(x,y);
		}
	}
	else
	{
		for(y=y1;y<=y2;y++) for(x=x1;x<=x2;x++)
		{
			pset(x,y,c);
		}
	}
}

int	xorrec(int x1,int y1,int x2,int y2)
{
	int	x,y;
	for(y=y1+1;y<y2;y++)
	{
		pxor(x1,y);
		pxor(x2,y);
	}
 	for(x=x1;x<=x2;x++)
	{
		pxor(x,y1);
		pxor(x,y2);
	}
}

int	waitborder(void)
{
	while(inp(0x3da)&8);
	while(!(inp(0x3da)&8));
}

int	viewpal(int flag)
{
	int	a;
	waitborder();
	outp(0x3c8,0);
	if(flag) 
	{ 
		for(a=0;a<3;a++) outp(0x3c9,0); 
		for(a=0;a<768-6;a++) outp(0x3c9,63-pal[a]); 
		for(a=0;a<3;a++) outp(0x3c9,pal[a+768-3]); 
	}
	else { for(a=0;a<768;a++) outp(0x3c9,pal[a]); }
}

int	viewpic(int flag)
{
	gotoxy(0,0);
	memcpy(vram,pic,64000);
}

int	cls(void)
{
	int	a;
	memset(vram,0,64000);
	/* set palettes 16 first colors to greyscale */
	waitborder();
	outp(0x3c8,0);
	for(a=0;a<8;a++)
	{
		outp(0x3c9,a*7);
		outp(0x3c9,a*7);
		outp(0x3c9,a*7);
	}
	gotoxy(0,0);
}

int	savefcp(char *fname)
{
	int	style;
	int	xo,yo;
	int	x,y,z,zm=1;
	int	a,b,c,d,w,wc;
	unsigned u;
	long	tell4,telltbl;
	long	planepos[4];
	wpics=picnum;
	strcat(fname,".FCP");
	f2=fopen(fname,"w+b");
	if(f2==NULL) return(1);
	putc('F',f2);
	putc('C',f2);
	putc('P',f2);
	putc(0x1a,f2);
	putc(0x10,f2); /* file ver */
	a=2; /* +2=AO file */
	if(fcpsavepal) a|=1;
	putc(a,f1);
	putw(wpics,f2); /* single file */
	telltbl=ftell(f2);
	fwrite(wpicpos,4,wpics,f2);
	if(fcpsavepal)
	{
		fwrite(pal,256,3,f2);
	}

	{
		pass=1;
		memset(pic,backcol,64000);
		viewpic(0);
		picwrt=0;
		for(b=0;b<shownum;b++)
		{
			a=loadfcp(showname[b]);
			if(a==3)
			{
				printf("\nFile not found. Program aborted.\n");
				return(0);
			}
			else if(a==1) 
	 		{
				printf("\nError loading picture. Program aborted.\n");
				return(0);
			}
		}
	}
	
    	fseek(f2,telltbl,SEEK_SET);
	fwrite(wpicpos,4,wpics,f2);
    	fseek(f2,0L,SEEK_END);
	fclose(f2);
	if(fcp4plane) 
	{
		memcpy(pic,pic2,64000);
	}
	return(0);
}


#include <stdio.h>
#include <graph.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>

int	scan(void);
int	doblock(FILE *f1);
int	readlbm(FILE *f1);
int	showline(int y);

int	bluecol=1;
int	backcol=15;

char	tablex[256];
char	tabley[256];
int	tablep=0;

FILE	*out;
long	f_off[256];
char	f_wid[256];
char	f_hig[256];
char	f_base[256];
long	averwid;
int	avercnt;

main(int argc,char *argv[])
{
	int	a;
	FILE	*f1;
	if(argc==1)
	{
		printf("usage: FONT <srclbm> <desttmpfont>");
		return(0);
	}
	_setvideomode(_VRES16COLOR);
	for(a=0;a<256;a++)
	{
		f_off[a]=0;
		f_wid[a]=0;
		f_hig[a]=0;
	}
	avercnt=0; averwid=0;
	out=fopen(argv[2],"wb");
	putw(0x1a1a,out);
	putw(tablep,out);
	fwrite(tablex,1,256,out);
	fwrite(tabley,1,256,out);
	f1=fopen(argv[1],"rb");
	readlbm(f1);
	{
		char buf[64];
		int x0,y0,x,y,a;
		for(y0=2;y0<7*8-2;y0+=8)
		  for(x0=0;x0<56*8;x0+=8)
		{
			for(a=x=0;x<8;x++) for(y=0;y<8;y++)
			{
				if((buf[x+y*8]=_getpixel(x0+x,y0+y))==0) a=1;
			}
			if(a)
			{
				tablex[tablep]=x0/8;
				tabley[tablep++]=(y0-2)/8;
				for(y=0;y<8;y++) 
				{
					for(a=x=0;x<8;x++)
					{
						if(!buf[x+y*8]) a+=128>>x;
					}
					putc(a,out);
				}
				_setcolor(15);
			}
			else _setcolor(8);
			_setpixel(x0,y0);
		}
	}
	getch();
	fclose(f1);
	fseek(out,0L,SEEK_SET);
	putw(0x1a1a,out);
	putw(tablep,out);
	fwrite(tablex,1,256,out);
	fwrite(tabley,1,256,out);
	fclose(out);
	_setvideomode(_DEFAULTMODE);
	printf("tablecount:%i",tablep);
}

unsigned char linebuf[1024];
unsigned char pal[768];

int	showline(int y)
{
	int	x;
	for(x=0;x<640;x++)
	{
		_setcolor(linebuf[x]);
		_setpixel(x,y);
	}
	return(0);
}

int cbits[8]={1,2,4,8,16,32,64,128};
int	lbmmode; /* 1=DPII, 2=DPIIe */
char	*errtxt;
char	lasttype[5];
char	type[5]={"----"};
long	len;
int	xsz,ysz,planes,stencil,colors,mode;
char	lbmtype[4];
int	checktype=0;

int	eschit(void)
{
	if(kbhit())
	{
		if(getch()==27) return(1);
	}
	return(0);
}

int	getaw(FILE *f1)
{
	unsigned int	a;
	a=getc(f1)*256;
	a+=getc(f1);
	return(a);
}

int	readlbm(FILE *f1)
{
	int	x,y,a,b,c;
	long	l;
	unsigned u;
	int	firstone=1;
	errtxt=NULL;
	do
	{
		memcpy(lasttype,type,5);
		/* get header */
		do
		{
			type[0]=getc(f1); 
		}
		while(type[0]==0 && !feof(f1));
		type[1]=getc(f1); type[2]=getc(f1); type[4]=0; 
		type[3]=getc(f1); len=getc(f1)*256L*65536L;
		if(firstone && memcmp(type,"FORM",4)) return(2);
		firstone=0;
		len+=getc(f1)*65536L; len+=getc(f1)*256L; len+=getc(f1);
		/* process it */
		a=doblock(f1);
		if(errtxt!=NULL)
		{
			printf("%s\n",errtxt);
			return(1);
		}
		if(a==-1)
		{
			printf("Not a Deluxepaint file!");
			return(2);
		}
		if(a==2)
		{
			printf("OLD:<%s> NEW<%s>\n",lasttype,type);
			return(1);
		}
		else if(a==1) break;
	}
	while(!feof(f1));
	return(0);
}

int	doblock(FILE *f1)
{ /* processes one block */
	long	lastpos=ftell(f1);
	int	a,b,c,d,e,f;
	if(!strcmp(type,"CMAP"))
	{
		/* color map */
		for(b=0;b<colors*3;b++)
		{
			a=getc(f1)/4;
			pal[b]=a;
		}
		/* load colors */
		for(a=0;a<16;a++) /* vga */
		{
			inp(0x3da);
			outp(0x3c0,a);
			outp(0x3c0,a);
			outp(0x3c8,a);
			outp(0x3c9,pal[a*3+0]);
			outp(0x3c9,pal[a*3+1]);
			outp(0x3c9,pal[a*3+2]);
		}
		inp(0x3da);
		outp(0x3c0,32);
	}
	else if(!strcmp(type,"FORM"))
	{ /* skip size of file, NO pointer movement */
		for(a=0;a<8;a++) lbmtype[a]=getc(f1);
		lbmtype[a]=0;
		/* LMDSBMHD - pakkaamaton formaatti */
		if(!memcmp(lbmtype,"ILBMBMHD",8))
		{
			lbmmode=1;
		}
		else if(!memcmp(lbmtype,"PBM BMHD",8))
		{
			lbmmode=2;
		}
		else
		{
			errtxt="Unknown fileformat!";
			return(-1);
		}
		getaw(f1); /* headerin lopun pituus */
		getaw(f1); /* - */ 
		xsz=getaw(f1);
		ysz=getaw(f1);
		getaw(f1); /* ? */
		getaw(f1); /* ? */

		planes=getc(f1);
		stencil=getc(f1);
		colors=getaw(f1);
		getaw(f1); /* ? */
		getaw(f1); /* ? */
		getaw(f1); /* screen-size-x */
		getaw(f1); /* screen-size-y */
		
		printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"
			"Size: %i x %i (Colors:%i)\nPlanes:%i Stencil:%i\n",
			xsz,ysz,colors,planes,stencil);
		return(0);
	}
	else if(!strcmp(type,"CRNG"))
	{ 
	}
	else if(!strcmp(type,"DPPS"))
	{ 
	}
	else if(!strcmp(type,"DPPV"))
	{ 
	}
	else if(!strcmp(type,"TINY"))
	{ /* pikkukuva */
	}
	else if(!strcmp(type,"BODY") && lbmmode==2) 
	{
		int	lp,cbit,y=0;
		while(!eschit() && y<ysz)
		{
			memset(linebuf,0,xsz);
			{
				lp=0;
				while(lp<xsz)
				{
					a=getc(f1);
					if(a>127)
					{
						c=getc(f1);
						a=256-a;
						if(a==0) c=0;
						for(b=0;b<=a;b++) linebuf[lp++]=c;
					}
					else
					{
						for(b=0;b<=a;b++)
						{
							c=getc(f1);
							linebuf[lp++]=c;
						}
					}
				}
			}
			showline(y);
			y++;
		}
		return(1);
	}
	else if(!strcmp(type,"BODY") && lbmmode==1) 
	{
		int	lp,cbit,y=0;
		while(!eschit() && y<ysz)
		{
			memset(linebuf,0,xsz);
			for(d=0;d<planes;d++)
			{
				cbit=cbits[d];
				lp=0;
				while(lp<xsz)
				{
					a=getc(f1);
					if(a>127)
					{
						c=getc(f1);
						a=256-a;
						if(a==0) c=0;
						for(b=0;b<=a;b++) 
						{
							linebuf[lp++]|=c&128?cbit:0;
							linebuf[lp++]|=c&64?cbit:0;
							linebuf[lp++]|=c&32?cbit:0;
							linebuf[lp++]|=c&16?cbit:0;
							linebuf[lp++]|=c&8?cbit:0;
							linebuf[lp++]|=c&4?cbit:0;
							linebuf[lp++]|=c&2?cbit:0;
							linebuf[lp++]|=c&1?cbit:0;
						}
					}
					else
					{
						for(b=0;b<=a;b++)
						{
							c=getc(f1);
							linebuf[lp++]|=c&128?cbit:0;
							linebuf[lp++]|=c&64?cbit:0;
							linebuf[lp++]|=c&32?cbit:0;
							linebuf[lp++]|=c&16?cbit:0;
							linebuf[lp++]|=c&8?cbit:0;
							linebuf[lp++]|=c&4?cbit:0;
							linebuf[lp++]|=c&2?cbit:0;
							linebuf[lp++]|=c&1?cbit:0;
						}
					}
				}
			}
			if(stencil) for(lp=0;lp<xsz;)
			{
				a=getc(f1);
				if(a>127)
				{
					c=getc(f1);
					a=256-a;
					lp+=a*8+8;
				}
				else
				{
					for(b=0;b<=a;b++)
					{
						c=getc(f1);
					}
					lp+=a*8+8;
				}
			}
			showline(y);
			y++;
		}
		return(1);
	}
	else if(checktype)
	{ 
		return(2);
	}
	fseek(f1,lastpos+len,SEEK_SET); 
	return(0);
}

int	letter(int chr,int x,int y)
{
	int	basey;
	int	x1,y1,x2,y2,a,c=1;
	x1=x2=x; y1=y2=y;
	/* expand box */
	while(c)
	{
		c=0;
		for(a=x1;a<=x2;a++)
		{
			if(y1>=0 && _getpixel(a,y1)!=backcol) { y1--; c=1; }
			if(y2<=479 && _getpixel(a,y2)!=backcol) { y2++; c=1; }
		}
		for(a=y1;a<=y2;a++)
		{
			if(x1>=0 && _getpixel(x1,a)!=backcol) { x1--; c=1; }
			if(x2<=639 && _getpixel(x2,a)!=backcol) { x2++; c=1; }
		}
		if(x1<=0 || y1<=0 || x2>=639 || y2>=479) exit(1);
	}
	basey=y-y1;
	/* erase blue */
	for(y=y1;y<=y2;y++)
	  for(x=x1;x<=x2;x++)
	{
		if(_getpixel(x,y)==bluecol) 
		{
			_setcolor(backcol);
			_setpixel(x,y);
		}
	}
	/* save font */
	{
		avercnt++;
		averwid+=x2-x1+1;
		f_wid[chr]=x2-x1+1;
		f_hig[chr]=y2-y1+1;
		f_base[chr]=basey;
		f_off[chr]=ftell(out);
		for(y=y1;y<=y2;y++)
		  for(x=x1;x<=x2;x++)
		{
			if(_getpixel(x,y)!=backcol) putc(1,out);
			else putc(0,out);
		}
	}
	/* erase box */
	_setcolor(backcol);
	_rectangle(_GFILLINTERIOR,x1,y1,x2,y2);
	return(0);
}

unsigned char order[256]={
"!\"#$%&'()*+,-./0123456789:;<=>?"
"@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_"
"`abcdefghijklmnopqrstuvwxyz{|}~"
"™†„”"
"אבגדהוזחטיךכלםמן"};
int	scan(void)
{
	int	y,x,a=0;
	for(y=0;y<480;y++) if((bluecol=_getpixel(0,y))!=backcol)
	{
		for(x=1;x<640;x++) if(_getpixel(x,y)!=backcol)
		{
			if(letter(order[a++],x,y)) return(1);
		}
	}
	return(0);
}

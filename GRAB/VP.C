#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <graph.h>
#include <memory.h>

/* contrast, default=100, a bit weird,
   brightness, default=0 */
int	contrast=100,brightness=0;

int	getaw(FILE *); /* amiga word */

int	checktype=0;
int	xsz,ysz,planes,stencil,colors,mode;
int	connectcol=1; /* color removed from sprites etc */

char	*errtxt;

int	origox,origoy;

int	magcolors;
int	fullscreen=0;

int	neverrll;

int	setpal(int xx);
int	doblock(void);

char	filetype[9];

char far *vram=(char far *)0xa0000000;

char	basefilename[16];
char	basefilename2[16];
char	tmpstr[64];

unsigned char far picbuf[64000];
char	filebuf[8192];

unsigned char orgpalette[768];
unsigned char palette[768];

FILE	*f1;
char	lasttype[5];
char	type[5]={"----"};
long	len;

unsigned int	maxp;
unsigned int	pp=0;

int	showonly=0,dothird=0;

main(int argc,char *argv[])
{
	int	sz1,sz2,sz3,sz4,num,mx,my;
	int	x,y,a,tx,ty,b,c,pix,xl;
	long	l,l1,l2,eob;
	int	beg,end;
	unsigned u;
	if(argc!=3) printf("Deluxe PIC viewer & Future Format Converter V1.0   (C) 1990 Sami Tammilehto\n"
		"usage: VP <fname> [/3=do 3] [/S=show pic only & return immediately]\n"
		"While viewing ? pops up a helpscreen\n\n"
		"Picture format for multiple object recoginition:\n"
		"     Background color 255, col 1 reserved. Fonts' bounding boxes must NOT touch\n"
		"     If a font has separate parts, connect them with blue line (col 1)\n"
		"     Put ONE dot to the first column on each fontrows baseline\n"
		"     the dot must have the color used for connectors.\n");
	else 
	{
		if(argv[2][1]=='3') dothird=1;
		else showonly=1;
	}
	if(argc==1) return(0);
	f1=fopen(argv[1],"rb");
	if(f1==NULL) { printf("File not found!\n"); return(1); }
	else fclose(f1);
	if(strrchr(argv[1],'\\')==NULL) strcpy(basefilename,argv[1]);
	else strcpy(basefilename,strrchr(argv[1],'\\')+1);
	*strchr(basefilename,'.')=0;
	memcpy(basefilename2,basefilename,7);
	basefilename2[7]=0;
	_setvideomode(_MRES256COLOR);
	if(!showonly)
	{
		_setcolor(7);
		for(y=0;y<256;y++)
		{
			_setcolor(y);
			_setpixel(y/16*2+320-33  ,200-33+(y&15)*2);
			_setpixel(y/16*2+320-33+1,200-33+(y&15)*2);
			_setpixel(y/16*2+320-33  ,200-33+(y&15)*2+1);
			_setpixel(y/16*2+320-33+1,200-33+(y&15)*2+1);
		}
	}
	{
		f1=fopen(argv[1],"rb");
		setvbuf(f1,filebuf,_IOFBF,8192);
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
			len+=getc(f1)*65536L; len+=getc(f1)*256L; len+=getc(f1);
			/* process it */
			a=doblock();
			if(errtxt)
			{
				_setvideomode(_DEFAULTMODE);
				printf("%s\n",errtxt);
				return(0);
			}
			if(a==2)
			{
				_setvideomode(_DEFAULTMODE);
				printf("OLD:<%s> NEW<%s>\n",lasttype,type);
				return(0);
			}
			else if(a==1) break;
		}
		while(!feof(f1) && !kbhit());
		fclose(f1);
	}
	if(showonly) return(0);
	do
	{
		if(dothird) a='3'; else a=getch();
		switch(a)
		{
			case '*' :
				for(y=0;y<256;y++)
				{
					_setcolor(y);
					_setpixel(y/16*2+320-33  ,200-33+(y&15)*2);
					_setpixel(y/16*2+320-33+1,200-33+(y&15)*2);
					_setpixel(y/16*2+320-33  ,200-33+(y&15)*2+1);
					_setpixel(y/16*2+320-33+1,200-33+(y&15)*2+1);
				}
				break;
			case '?' : 
				keyhelp();
				break;
			case '0' :
				origox=origoy=0;
				savefc();
				break; 
			case '1' :
				origox=160; origoy=100;
				savefc();
				break;
			case '8' :
				neverrll=1;
				scanfonts(301);
				neverrll=0;
				break;
			case 'M' :
			case 'm' :
				_setvideomode(_TEXTC80);
				neverrll=0;
				for(;neverrll!=99;)
				{
					printf("\n\n\nMAGazine piccysaver\n\n"
						"Mode(press X to change): %s\n\n"
						"Press F for full screen, M for multiple.\n",
						neverrll?"nopack":"rll-pack");
					switch(getch())
					{
						case 'x' :
						case 'X' :
							neverrll^=1;
							break;
						case 'f' :
						case 'F' :
							_setvideomode(_MRES256COLOR);
							showpic();
							setpal(0);
							magcolors=256;
							fullscreen=1;
							savesprite(basefilename,0,0,319,199,-1,1);
							fullscreen=0;
							neverrll=99;
							break;
						case 'm' :
						case 'M' :
							_setvideomode(_MRES256COLOR);
							showpic();
							setpal(0);
							magcolors=256;
							scanfonts(301);
							neverrll=99;
							break;
					}
				}
				neverrll=0;
				magcolors=0;
				break;
			case '3' :
				scanfonts(200);
				break;
			case '4' :
				scanfonts(300);
				break;
			case '5' :
				saveunpack(1);
				break;
			case '6' :
				saveunpack(0);
				break;
			case 's' :
			case 'S' :
				scanfonts(100);
				break;
			case 'f' :
			case 'F' :
				scanfonts(0);
				break;
			case 'g' :
			case 'G' :
				scanfonts(2);
				break;
			case 'u' :
			case 'U' :
				scanfonts(1);
				break;
			case 't' :
			case 'T' :
				setpal(1);
				savesprite(basefilename,0,0,319,199,0,0);
				setpal(0);
				break;
			case 'c' :
			case 'C' :
				setpal(1);
				savesprite(basefilename,0,0,319,199,160,100);
				setpal(0);
				break;
			case ' ' :
				setpal(1);
				showpic();
				setpal(0);
				break;
			case '+' : brightness+=2; break;
			case '-' : brightness-=2; break;
			case 'p' :
			case 'P' :
				savepal();
				break;
			case 'd' :
			case 'D' :
				dither();
				break;
		}
		if(dothird) a=27;
	}
	while(a!=27);
	_setvideomode(_DEFAULTMODE);
}

unsigned char sbuf[330];

int	savepal(void)
{
	FILE	*f1;
	strcpy(tmpstr,basefilename);
	strcat(tmpstr,".PAL");
	f1=fopen(tmpstr,"wb");
	fwrite(palette,1,768,f1);
	fclose(f1);
}

int cbits[8]={1,2,4,8,16,32,64,128};
unsigned char linebuf[320];
int	filemode; /* 1=DPII, 2=DPIIe */

int	doblock(void)
{
	long	lastpos=ftell(f1);
	int	a,b,c,d,e,f;
	if(!strcmp(type,"CMAP"))
	{
		/* color map */
		for(b=0;b<256*3;b++)
		{
			a=getc(f1)/4;
			palette[b]=a;
		}
		setpal(0);
	}
	else if(!strcmp(type,"FORM"))
	{ /* skip size of file, NO pointer movement */
		for(a=0;a<8;a++) filetype[a]=getc(f1); 
		filetype[a]=0;
		/* LMDSBMHD - pakkaamaton formaatti */
		if(!memcmp(filetype,"ILBMBMHD",8))
		{
			filemode=1;
		}
		else if(!memcmp(filetype,"PBM BMHD",8))
		{
			filemode=2;
		}
		else
		{
			errtxt="Not known format!";
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
		if(!showonly) printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"
			"Basefilename: %s\n"
			"Width:%i Height:%i\n"
			"Planes:%i Colors:%i Stencil:%i\n"
			,basefilename,xsz,ysz,planes,colors,stencil);
		getaw(f1); /* ? */
		getaw(f1); /* ? */
		getaw(f1); /* screen-size-x */
		getaw(f1); /* screen-size-y */
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
	else if(!strcmp(type,"BODY") && filemode==2) 
	{
		int	lp,cbit,y=0;
		while(!kbhit() && y<ysz)
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
			/*if(stencil) for(lp=0;lp<xsz;)
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
			}*/
			{
				_setcolor(y);
				_setpixel(0,y);
			}
			memcpy(picbuf+y*320,linebuf,xsz);
			y++;
		}
		showpic();
		return(1);
	}
	else if(!strcmp(type,"BODY") && filemode==1) 
	{
		int	lp,cbit,y=0;
		while(!kbhit() && y<ysz)
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
			{
				_setcolor(y);
				_setpixel(0,y);
			}
			memcpy(picbuf+y*320,linebuf,xsz);
			y++;
		}
		showpic();
		return(1);
	}
	else if(checktype)
	{ 
		return(2);
	}
	fseek(f1,lastpos+len,SEEK_SET); 
	return(0);
}

int	setpal(int xx) /* xx=0:normal,xx=1:inverse */
{		
	int a,b;
	outp(0x3c8,0);
	switch(xx)
	{
		case 0 :
			for(a=0;a<768;a++) outp(0x3c9,palette[a]);
			break;
		
		case 1 :
			for(a=0;a<768;a++) outp(0x3c9,63-palette[a]);
			break;
	}
}

int	getaw(FILE *f1)
{
	unsigned int	a;
	a=getc(f1)*256;
	a+=getc(f1);
	return(a);
}

/*----CUT HERE----CUT HERE----*/

int	fontx1[256]; 
int	fontx2[256]; 
int	fonty1[256]; 
int	fonty2[256]; 
int	fonty[256]; /* baseline y */
int	fp;
int	fpnt[256];
int	fwid[256];
char	frow[320];

int	noblueremove;

unsigned char	asciitxt[256]={
"!\"#$%&'()*+,-./0123456789:;<=>?@"
"ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_"
"abcdefghijklmnopqrstuvwxyz{|}"
"ÜÑîèéô     "};

int	scanfonts(int pack)
{
	int	maxup=0,maxdown=0,maxwidth=0;
	int	up,down;
	int	a,b,c,x,y,yb,y1,y2,x1=-1,x2,tx,ty;
	int	d,e,f;
	long	l;
	int	ascii=0,asciipnt=0;
	FILE	*f1;
	if(pack==2) { pack=1; ascii=1; }
	if(pack==200)
	{
		strcpy(tmpstr,basefilename);
		strcat(tmpstr,".UNS");
		f1=fopen(tmpstr,"wb");
	}
	setpal(0);
	fp=0;
	noblueremove=0;
	for(y=0;y<200 && !kbhit();y++) if(_getpixel(0,y)!=255)
	{
		connectcol=_getpixel(0,y);
		x=1;
		for(;x<320 && !kbhit();x++)
		{
			b=99;
			if(_getpixel(x,y)!=255) b=0;
			if(_getpixel(x,y-1)!=255) b=1;
			if(b!=99)
			{
				x1=x2=x; y1=y2=y-b;
				if(!ascii && pack<99) setpal(1);
				/* test box */
				for(c=1;c!=0 && !kbhit();)
				{
					c=0;
					for(a=x1;a<=x2;a++)
					{
						if(_getpixel(a,y1)!=255) { y1--; c=1; }
						if(_getpixel(a,y2)!=255) { y2++; c=1; }
					}
					for(a=y1;a<=y2;a++)
					{
						if(_getpixel(x1,a)!=255) { x1--; c=1; }
						if(_getpixel(x2,a)!=255) { x2++; c=1; }
					}
				}
				if(!ascii && pack<99) setpal(0);
				y1++; y2--; x1++; x2--;
				up=y-y1; down=y2-y;
				if(up>maxup) maxup=up;
				if(down>maxdown) maxdown=down;
				x=x2+1;
				if(x2-x1+1>maxwidth) maxwidth=x2-x1+1;
				
				fontx1[fp]=x1;
				fontx2[fp]=x2;
				fonty[fp]=y;
				fonty1[fp]=y1;
				fonty2[fp++]=y2;
				if(!ascii) rectangle(x1,y1,x2,y2);
				if(pack==100) 
				{
					char str[3];
					str[1]=0;
					str[0]=getch();
					if(str[0]==27) 
					{	
						showpic();
						return(0);
					}
					strcpy(tmpstr,basefilename2);
					strcat(tmpstr,str);
					rectangle(x1,y1,x2,y2);
					savesprite(tmpstr,x1,y1,x2,y2,x1,y1);
				}
				if(pack==300 || pack==301) 
				{
					char str[3];
					str[1]=0;
					str[0]=getch();
					if(str[0]==27) 
					{	
						showpic();
						return(0);
					}
					strcpy(tmpstr,basefilename2);
					strcat(tmpstr,str);
					rectangle(x1,y1,x2,y2);
					savesprite(tmpstr,x1,y1,x2,y2,-1,pack-300);
				}
				if(pack==200) 
				{
					int	tx,tu;
					rectangle(x1,y1,x2,y2);
					for(ty=y1;ty<y1+32;ty++)
					  for(tx=x1;tx<x1+32;tx++)
					{
						putc(_getpixel(tx,ty),f1);
					}
				}
			}
		}
		_setcolor(1);
		_setpixel(0,y);
	}
	setpal(1);
	setpal(0);
	if(pack==100 || pack==300 || pack==301) return(0);
	if(pack==200) { fclose(f1); return(0); }
	if(pack==0) 
	{ /* packed */
		strcpy(tmpstr,basefilename);
		strcat(tmpstr,".fcf");
		f1=fopen(tmpstr,"wb");
		fprintf(f1,"FCF100\x1aXXX");
		putw(maxwidth,f1);
		putw(maxup+maxdown+1,f1);
		putw(maxup,f1);
		memset(fpnt,0,2*256);
		memset(fwid,1,2*256);
		fseek(f1,512L+256L,SEEK_CUR); /* skip fonttable */
		for(a=0;a<fp;a++)
		{
			x1=fontx1[a];
			x2=fontx2[a];
			y1=fonty1[a];
			y2=fonty2[a];
			rectangle(x1,y1,x2,y2);
			y=fonty[a];
			for(ty=y1;ty<=y2;ty++)
			  for(tx=x1;tx<=x2;tx++) 
			{
				b=_getpixel(tx,ty);
				if(b==connectcol) b=255; /* remove blue 'connectors' */
				_setcolor(255-b);
				_setpixel(tx,ty);
			}
			y1=y-maxup; y2=y+maxdown;
			l=ftell(f1);
			if(l&15) 
			{
				for(b=15;b>=(l&15);b--) putc('X',f1);
				l=ftell(f1);
			}
			b=getch();
			fpnt[b]=(unsigned)(l/16L);
			fwid[b]=x2-x1+1;
			for(ty=y1;ty<fonty1[a];ty++) putc(0,f1);
			for(ty=fonty1[a];ty<=fonty2[a];ty++)
			{
				c=1;
				f=0;
				for(tx=x1;tx<=x2;tx++) 
				{
					b=255-_getpixel(tx,ty);
					if(b!=255)
					{ /* stuff! */
						f++;
						d=0; e=c; c+=2;
						frow[e]=tx-x1; /* x */
						for(;tx<=x2;tx++) 
						{
							b=255-_getpixel(tx,ty);
							_setcolor(b);
							_setpixel(tx,ty);
							if(b==255) break;
							d++; frow[c++]=b;
						}
						frow[e+1]=d; /* count */
					}
					else
					{
						_setcolor(b);
						_setpixel(tx,ty);
					}
				}
				frow[0]=f;
				if(f>0) fwrite(frow,1,c,f1);
				else putc(0,f1);
			}
			for(ty=fonty2[a];ty<=y2;ty++) putc(0,f1);
			putc(0,f1);
		}
		fseek(f1,16L,SEEK_SET); /* fonttable */
	 	for(a=0;a<256;a++) putw(fpnt[a],f1);
		for(a=b=c=0;a<256;a++) if(fwid[a]!=0x101) { b+=fwid[a]; c++; }
		if(c==0) b=1; else b/=c; 
		fwid[32]=8; /*b;*/
		for(a=0;a<256;a++) putc(fwid[a],f1);
		close(f1);
	}
	if(pack==1) 
	{ /* unpacked */
		strcpy(tmpstr,basefilename);
		strcat(tmpstr,".uff");
		f1=fopen(tmpstr,"wb");
		fprintf(f1,"UFF100\x1aXXX");
		putw(maxwidth,f1);
		putw(maxup+maxdown+1,f1);
		putw(maxup,f1);
		memset(fpnt,0,2*256);
		memset(fwid,1,2*256);
		fseek(f1,512L+256L,SEEK_CUR); /* skip fonttable */
		for(a=0;a<fp;a++)
		{
			x1=fontx1[a];
			x2=fontx2[a];
			y1=fonty1[a];
			y2=fonty2[a];
			if(!ascii) rectangle(x1,y1,x2,y2);
			y=fonty[a];
			for(ty=y1;ty<=y2;ty++)
			  for(tx=x1;tx<=x2;tx++) 
			{
				b=_getpixel(tx,ty);
				if(b==connectcol) b=255; /* remove blue 'connectors' */
				_setcolor(b);
				_setpixel(tx,ty);
			}
			y1=y-maxup; y2=y+maxdown;
			l=ftell(f1);
			if(l&15) 
			{
				for(b=15;b>=(l&15);b--) putc('X',f1);
				l=ftell(f1);
			}
			if(ascii) 
			{
				b=asciitxt[asciipnt++];
			}
			else b=getch();
			fpnt[b]=(unsigned)(l/16L);
			fwid[b]=x2-x1+1;
			for(ty=y1;ty<fonty1[a];ty++) for(tx=x1;tx<=x2;tx++) putc(255,f1);
			for(ty=fonty1[a];ty<=fonty2[a];ty++)
			{
				c=1;
				f=0;
				_setcolor(255);
				for(tx=x1;tx<=x2;tx++) 
				{
					putc(_getpixel(tx,ty),f1);
					_setpixel(tx,ty);
				}
			}
			for(ty=fonty2[a];ty<=y2;ty++) for(tx=x1;tx<=x2;tx++) putc(255,f1);
		}
		fseek(f1,16L,SEEK_SET); /* fonttable */
	 	for(a=0;a<256;a++) putw(fpnt[a],f1);
		for(a=b=c=0;a<256;a++) if(fwid[a]!=0x101) { b+=fwid[a]; c++; }
		if(c==0) b=1; else b/=c; 
		fwid[32]=b;
		for(a=0;a<256;a++) putc(fwid[a],f1);
		close(f1);
	}
	if(ascii && asciitxt[asciipnt]!=' ')
	{
		_setvideomode(_TEXTC80);
		printf("Not all letters found! (last:%c)\n",asciitxt[asciipnt]);
		printf("\nPress a key\n");
		getch();
		_setvideomode(_MRES256COLOR);
		showpic();
	}
}

int	rectangle(int x1,int y1,int x2,int y2)
{
	int	a,tx,ty;
	for(ty=y1+1;ty<y2;ty++)
	{
		a=_getpixel(x1,ty);
		_setcolor(255-a);
		_setpixel(x1,ty);
		a=_getpixel(x2,ty);
		_setcolor(255-a);
		_setpixel(x2,ty);
	}
	for(tx=x1;tx<=x2;tx++) 
	{
		a=_getpixel(tx,y1);
		_setcolor(255-a);
		_setpixel(tx,y1);
		a=_getpixel(tx,y2);
		_setcolor(255-a);
		_setpixel(tx,y2);
	}
}

unsigned char far rllbuf[64000];

int	savesprite(char *fname,int x1,int y1,int x2,int y2,int ox,int oy)
{
	int	a,b,c,tx,ty,d,e,f,bitplane=0,bc;
	unsigned int u;
	FILE	*f1;
	if(ox==-1) bitplane=oy+1;
	setpal(1);
	if(magcolors==0) strcat(fname,".4SR");
	else 
	{
		strcat(fname,".MAG");
		magazine(x1,y1,x2,y2,magcolors);
	}
	f1=fopen(fname,"wb");
	if(!fullscreen)
	{
		for(ty=y1;ty<=y2;ty++)
		  for(tx=x1;tx<=x2;tx++)
		{
			b=_getpixel(tx,ty);
			if(b==connectcol) b=255; /* remove blue 'connectors' */
			_setcolor(b);
			_setpixel(tx,ty);
		}
	}
	if(!bitplane)
	{
		putw(x2-x1+1,f1);
		putw(y2-y1+1,f1);
		putw(oy-y1,f1);
	}
	else if(bitplane!=2)
	{
		a=x2-x1+1;
		a=(a+3)&(65535-3);
		x2=x1+a;
	}
	if(bitplane==1) 
	{
		putw(y2-y1+1,f1);
		putw(a/4,f1);
		for(bc=0;bc<4;bc++,x1++)
		{
			for(ty=y1;ty<=y2;ty++)
			{
				for(tx=x1;tx<x2;tx+=4) 
				{
					b=_getpixel(tx,ty);
					putc(b,f1);
				}
			}
		}
	}
	else if(bitplane==2)
	{ /* MAGAZINE */
		if(magcolors!=0)
		{
			putw(magcolors+(neverrll?0:512),f1);
			for(b=0;b<magcolors*3;b++) 
			{ 
				putc(palette[b],f1);
			}
			memcpy(palette,orgpalette,768);
		}
		putw(y2-y1+1,f1);
		putw(x2-x1+1,f1);
		u=0;
		if(neverrll)
		{
			for(ty=y1;ty<=y2;ty++)
			{
				for(tx=x1;tx<=x2;tx++) 
				{
					putc(_getpixel(tx,ty),f1);
				}
			}
		}
		else
		{
			for(ty=y1;ty<=y2;ty++)
			{
				for(tx=x1;tx<=x2;tx++) 
				{
					rllbuf[u++]=_getpixel(tx,ty);
				}
			}
			{
				unsigned int a,b,c,d,e,f;
				for(d=c=b=0;b<u;b++)
				{
					f=rllbuf[b];
					if(f==rllbuf[b+1] && f==rllbuf[b+2] && b<(u-2))
					{
						if(c>0)
						{
							putc(c,f1);
							fwrite(rllbuf+d,c,1,f1);
						}
						c=b+126; if(c>u) c=u;
						for(e=b;e<c && rllbuf[e]==f;e++);
						c=e-b;
						b=e;
						d=b;
						putc(c+128,f1);
						putc(f,f1);
						c=0;
						b--;
					}
					else c++;
					if(c>126)
					{
						putc(c,f1);
						fwrite(rllbuf+d,c,1,f1);
						d=b;
						c=0;
					}
				}
			}
		}
	}
	else
	{
		for(ty=y1;ty<=y2;ty++)
		{
			c=2;
			f=0;
			for(tx=x1;tx<=x2;tx++) 
			{
				b=_getpixel(tx,ty);
				if(b!=255)
				{ /* stuff! */
					f++;
					d=0; e=c; c+=4;
					frow[e]=(tx-ox)&255; /* x */
					frow[e+1]=(tx-ox)/256; /* x */
					for(;tx<=x2;tx++) 
					{
						b=_getpixel(tx,ty);
						_setcolor(b);
						_setpixel(tx,ty);
						if(b==255) break;
						d++; frow[c++]=b;
					}
					frow[e+2]=d&255; /* count */
					frow[e+3]=d/256; /* count */
				}
				else
				{
					_setcolor(b);
					_setpixel(tx,ty);
				}
			}
			frow[0]=f&255;
			frow[1]=f/256;
			if(f>0) fwrite(frow,1,c,f1);
			else putw(0,f1);
		}
	}
	fclose(f1);
	while(kbhit()) getch();
	setpal(0);
}

int	showpic(void)
{
	setpal(0);
	memcpy(vram,picbuf,64000);
}

int	savefc(void)
{ /* color 255 considered transparent */
	int	x,y,a,b,c,d,e,f;
	int	leftx=origox,rightx=origox,upy=origoy,downy=origoy;
	FILE	*f1;
	long	sizepos;
	setpal(1);
	strcpy(tmpstr,basefilename);
	strcat(tmpstr,".FCP");
	f1=fopen(tmpstr,"wb");
	
	fprintf(f1,"FCP1.000");
	putw(0,f1); /* file type: 0=single pictures, 1=differential animation */
	putw(1,f1); /* number of pictures */
	putw(-1,f1); /* unused */
	putw(-1,f1); /* unused */
	putw(16+4,f1); putw(0,f1); /* far pointer(s) to picture(s) */
	
	/* picture header, 16 bytes */
	putc(1,f1); /* screen mode, 1=320x200x256 */
	putc(1,f1); /* flags, incl. palette */
	sizepos=ftell(f1);
	putw(-1,f1); /* leftmost x */
	putw(-1,f1); /* uppermost y */
	putw(-1,f1); /* rightmost x */
	putw(-1,f1); /* bottommost y */
	putw(-1,f1); /* unused */
	putw(-1,f1); /* unused */
	putw(-1,f1); /* unused */
	
	for(a=0;a<768;a++) putc(palette[a],f1);
	
	for(y=0;y<200;y++)
	{
		for(x=0;x<320;)
		{
			if((a=_getpixel(x,y))!=255)
			{
				if(y<upy) upy=y;
				if(y>downy) downy=y;
				if(x<leftx) leftx=x;
				putw(x-origox,f1); /* x */
				putw(y-origoy,f1); /* y */
				b=0;
				do sbuf[b++]=a; while((a=_getpixel(++x,y))!=255 && x<320);
				if(x>rightx) rightx=x;
				sbuf[b]=255;
				/* entire continuos part read */
				for(d=a=0;a<b;)
				{
					if(sbuf[a]==sbuf[a+1] && sbuf[a]==sbuf[a+2]
					&& sbuf[a]==sbuf[a+3] && sbuf[a]==sbuf[a+4])
					{ /* at least 5 continuos */
						if(a>d)
						{
							putw(a-d,f1);
							for(e=d;e<a;e++) putc(sbuf[e],f1);
						}
						c=1; a++; f=sbuf[a];
						while(f==sbuf[a])
						{
							c++;
							a++;
						}
						putw(-c,f1);
						putc(f,f1);
						d=a;
					}
					else
					{
						a++;
					}
				}
				if(a>d)
				{
					putw(a-d,f1);
					for(e=d;e<a;e++) putc(sbuf[e],f1);
				}
				putw(0,f1); /* e-o-c-part*/
			}
			else x++;
		}
	}
	putw(32767,f1); /* pic terminator */
	fseek(f1,sizepos,SEEK_SET);
	putw(leftx-origox,f1);
	putw(upy-origoy,f1);
	putw(rightx-origox,f1);
	putw(downy-origoy,f1);
	fclose(f1);
	setpal(0);
}

char	temp[768];
int	dithermatrix[4][4]={
0,8,2,10,
12,4,14,6,
3,11,1,9,
15,7,13,5};

int	dither(void)
{
	int	a,b,c,x,y;
	unsigned u;
	memcpy(temp,palette,768);
	for(a=0;a<64;a++)
	{
		palette[a*3+0]=a;
		palette[a*3+1]=a;
		palette[a*3+2]=a;
	}
	setpal(0);
	memcpy(palette,temp,768);
	for(u=0;u<64000;u++)
	{
		if(!(u&127)) if(kbhit()) break;
		a=picbuf[u];
		b=(palette[a*3]*30+palette[a*3+1]*11+palette[a*3+2]*59)/100;
		vram[u]=b;
	}
	for(u=y=0;y<200 && !kbhit();y++) 
	{
		for(x=0;x<320;x++,u++)
		{ /* dither */
			vram[u]=(((int)vram[u]+contrast*dithermatrix[x&3][y&3]/25))<(64+brightness)?0:63;
		}
	}
}

int	saveunpack(int bpm)
{
	int	a,b,c;
	unsigned int u;
	FILE	*f1;
	setpal(1);
	if(bpm==1)
	{ /* 4-bitplane */
		strcpy(tmpstr,basefilename);
		strcat(tmpstr,".4BM");
		f1=fopen(tmpstr,"wb");
		if(f1==NULL) exit(3);
		for(a=0;a<4;a++) for(u=0;u<64000;u+=4)
		{
			putc(picbuf[u+a],f1);
		}
		fclose(f1);
	}
	if(bpm==0)
	{ /* 1-bitplane */
		strcpy(tmpstr,basefilename);
		strcat(tmpstr,".1BM");
		f1=fopen(tmpstr,"wb");
		if(f1==NULL) exit(3);
		for(u=0;u<64000;u++)
		{
			putc(picbuf[u],f1);
		}
		fclose(f1);
	}
	setpal(0);
}

int	magazine(int x1,int y1,int x2,int y2,int max)
{
	int	a,b,c;
	memcpy(orgpalette,palette,768);
	return(0);
}

int	keyhelp(void)
{
	int	x,y,a;
	_setvideomode(_TEXTC80);
	printf("Keys availible:\n"
		"ESC..Exit\n"
		"*....Display palette\n"
		"6....64K unpacced piccy\n"
		"5....64K unpacced piccy in 4-bit-plane-mode.\n"
		"3....Save 32x32 sprites unpacked\n"
		"4....Save partially transparent 4-bitplane 256 color sprites\n"
		"8....Save standard 256 color unp. sprites\n"
		"M....256 color MAGazine sprites (then press 1..8 for 2^x colors)\n"
		"0....Save FCP file with origo in 0,0\n"
		"1....Save centered FCP file with origo in 160,100\n"
		"F....Create fontfile.\n"
		"G....Create unpacked fontfile (ASCII ordering).\n"
		"U....Create unpacked fontfile.\n"
		"S....Save sprites (identified as fonts).\n"
		"T....Save sprite (whole screen, 0,0=center).\n"
		"C....Save sprite (whole screen, 160,100=center).\n"
		"P....Save palette.\n"
		"D....Dither.\n"
		"SPC..Restore pic.\n"
		"-/+..Brightness.\n"
		"\nPress a key...\n");
	getch();
	_setvideomode(_MRES256COLOR);
	showpic();
}


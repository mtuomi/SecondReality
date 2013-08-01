#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <graph.h>

/* conversion palette to DEFPAL */
int wc2conv[]={0,42,120,57,186,90,152,21,26,37,116,52,181,196,130,16
,24,208,225,210,211,212,213,214,215,216,217,218,220,221,222,223
,32,33,34,35,36,37,38,40,40,41,43,44,45,46,47,0
,32,33,67,68,70,71,72,73,74,75,75,76,77,78,47,47
,128,241,242,243,244,245,167,183,184,185,186,187,188,189,190,191
,48,177,178,179,180,181,182,183,242,254,253,117,120,122,124,127
,16,161,145,212,148,149,150,151,152,152,153,154,155,156,141,158
,16,161,162,227,228,229,230,231,232,233,234,235,236,237,238,0
,16,176,162,163,164,165,166,167,168,169,170,234,235,236,238,0
,16,32,49,50,51,52,53,55,55,56,57,59,60,61,63,0
,16,17,18,19,20,21,22,23,24,26,27,28,29,30,31,0
,208,162,179,181,181,199,200,201,202,203,204,204,205,206,207,0
,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
,112,97,98,99,100,101,102,103,104,105,106,106,108,108,109,110
,208,210,212,255,217,219,220,157,151,231,232,170,171,234,188,239
,249,250,251,252,122,123,124,125,47,0,0,0,0,0,0,255};

unsigned char far pic[64000];
int	fcporigo=2,fcporigox,fcporigoy;
int	backcol=255;
int	savegfx=0;
int	savegfx1=0;
FILE	*gfx;

#include "gfxsave.h"

char	palette[768];

int	origox,origoy;

unsigned int	maxp;
unsigned int	pp=0;

int	getcol(void);

unsigned char far buf[64010];

long	table[1024];

char	*fname=NULL;
int	picnum=1;
int	showpal=0;
int	origox=0,origoy=0,origoreset=0;
char	*palfile="palette.wc2";
int	longanim=0;
int	savelbm=0;

char far *vram=(char far *)0xa0000000;

unsigned picposu[256],pn=0;

main(int argc,char *argv[])
{
	int	filenum;
	int	o;
	char	fstr[80];
	FILE	*f1;
	int	sz1,sz2,sz3,sz4,num,mx,my;
	int	x,y,a,tx,ty,b,c,pix;
	long	l,l1,l2,eob;
	int	beg,end;
	unsigned u;
	printf("Origin PIC viewer V3.0\n");
	if(argc==1) 
	{
		printf("usage: SP <options> <fname> [pic number,none=all]"
			"available options:\n"
			"-l	long animation (.1,.2,.3...)\n"
			"-s	show palette\n"
			"-1	use wing commmander 1 palette\n"
			"-o	set origox to 160,100 (default: 0,0)\n"
			"-ox#	set origox\n"
			"-oy#	set origoy\n"
			"-or	reset origo for long anims pics >1\n"
			"-f	save with FutureGrabber to LBM:s\n"
			"-h	save as 1-bitplane-packed\n"
			"-g	save as GFX\n"
			"-G	save as GFX with DEFAULT palette\n"
			);
		return(0);
	}
	for(a=1;a<=argc;a++)
	{
		if(*argv[a]=='-' || *argv[a]=='/') switch(*(argv[a]+1))
		{
			case 'l' : longanim=1; break;
			case 'o' : switch(*(argv[a]+2))
			{
				case 'x' :
					origox=atoi(argv[a]+3);
					break;
				case 'y' :
					origoy=atoi(argv[a]+3);
					break;
				case 'r' :
					origoreset=1;
					break;
				default :
					origox=160; origoy=100;
					break;
			} break;
			case 's' : showpal=1; break;
			case '1' : palfile="palette.wc1"; break;
			case 'f' : savelbm=1; break;
			case 'g' : savegfx=1; break;
			case 'G' : savegfx=2; break;
			case 'h' : savegfx1=1; break;
		}
		else
		{
			if(fname==NULL) 
			{
				fname=argv[a];
				if(strchr(fname,'.')==NULL) longanim=1;
			}
			else picnum=atoi(argv[a]);
		}
	}
	if(!longanim)
	{
		printf("Scanning... ");
		f1=fopen(fname,"rb");
		if(f1==NULL) { printf("File '%s' not found!\n",fname); return(1); }
		fread(table,4,1024,f1);
		fclose(f1);
		num=table[1]/4-1;
		table[num+1]=table[0];
		printf("File contains %i pictures.\n",num);
	}
	else num=9999;
	a=picnum;
	if(a<0 || a>num) { printf("Number must be between 0 and %i!\n",num); return(3); }
	_setvideomode(_MRES256COLOR);
	f1=fopen(palfile,"rb");
	for(b=0;b<768;b++) palette[b]=getc(f1);
	fclose(f1);
	palette[765]=2;
	palette[766]=3;
	palette[767]=4;
	outp(0x3c8,0); for(b=0;b<256*3;b++)
	{
		outp(0x3c9,palette[b]);
	}
	if(showpal) 
	{
		for(y=0;y<256;y++)
		{
			_setcolor(y);
			_setpixel(y/16*2+320-33  ,(y&15)*2);
			_setpixel(y/16*2+320-33+1,(y&15)*2);
			_setpixel(y/16*2+320-33  ,(y&15)*2+1);
			_setpixel(y/16*2+320-33+1,(y&15)*2+1);
		}
		getch();
	}
	if(a==0) { beg=1; end=num; } /* animate */
	else beg=end=a;
	_setcolor(255);
	for(b=0;b<200;b++)
	{
		_moveto(0,b);
		_lineto(319,b);
	}
	if(savegfx && longanim)
	{
		printf("Can't save a long animation in one part!\n");
		return(1);
	}
	if(savegfx)
	{
		gfx=fopen("gfx.gfx","wb");
		putc('G',gfx);
		putc('F',gfx);
		putc('X',gfx);
		putc('1',gfx);
		putc('0',gfx);
		putc(0x1a,gfx);
		putw(end-beg+1,gfx);
		fwrite(picposu,2,end-beg+1,f1);
	}
 for(filenum=1;;filenum++)
 {
  if(longanim)
  {
  	sprintf(fstr,"%s.%i",fname,filenum);
	f1=fopen(fstr,"rb");
	if(f1==NULL) break;
	fread(table,4,1024,f1);
	fclose(f1);
	num=table[1]/4-1;
	table[num+1]=table[0];
	end=num;
	beg=1;
  }
  else strcpy(fstr,fname);
  for(a=beg;a<=end;a++)
  {
  	if(savegfx)
	{
		b=16-((ftell(gfx))&15); if(b!=16) for(c=0;c<b;c++) putc(0,gfx);
		picposu[pn++]=ftell(gfx)>>4;
	}
	f1=fopen(fstr,"rb");
	fseek(f1,table[a],SEEK_SET);
	eob=table[a+1];
	sz3=getw(f1);
	sz1=-getw(f1);
	sz2=-getw(f1);
	sz4=getw(f1);
	mx=origox; my=origoy;
  	if(savegfx)
	{
		fcporigoy=-sz2;
		fcporigox=-sz1;
	}
	for(;;)
	{
		int	mode;
		pix=getc(f1); pix+=getc(f1)*256; 
		if(feof(f1) || ftell(f1)>eob) break;
		tx=getc(f1); tx+=getc(f1)*256; tx+=mx;
		ty=getc(f1); ty+=getc(f1)*256; ty+=my;
		mode=pix&1; pix>>=1;
		u=pix;
		if(mode==0) for(b=0;b<u;b++)
		{
			_setcolor(getc(f1));
			_setpixel(tx++,ty);
		}
		else while(pix>0) 
		{
			u=getc(f1);
			mode=u&1;
			u>>=1;
			if(u==0) break;
			switch(mode)
			{
				case 0 :
					o=tx+ty*320;
					pix-=u;
					tx+=u;
					for(b=0;b<u;b++)
					{
						*(vram+o++)=getc(f1);
					}
					break;
				case 1 :
					memset(vram+tx+ty*320,getc(f1),u);
					pix-=u;
					tx+=u;
					break;
			}
		}
	}
	fclose(f1);
	if(savegfx)
	{
		int	x1,y1,x2,y2,a;
		x1=origox+sz1;
		x2=origox+sz3;
		y1=origoy+sz2;
		y2=origoy+sz4;
		if(savegfx==2)
		{
			unsigned int u;
			for(u=0;u<64000;u++) pic[u]=wc2conv[(unsigned char)vram[u]];
		}
		else memcpy(pic,vram,64000);
		x1--; y1--; x2++; y2++;
		for(a=x1;a<=x2;a++) vram[y1*320+a]=15;
		for(a=x1;a<=x2;a++) vram[y2*320+a]=15;
		for(a=y1;a<=y2;a++) vram[a*320+x1]=15;
		for(a=y1;a<=y2;a++) vram[a*320+x2]=15;
		x1++; y1++; x2--; y2--;
		if(savegfx1) savegfxdata1(gfx,x1,y1,x2,y2);
		else savegfxdata(gfx,x1,y1,x2,y2);
	}
	if(savelbm) _asm
	{
		mov	ax,0fcfch
		mov	bx,01h
		int	33h
	}
	else
	{
		do
		{
			b=getch();
		}
		while(b!=32 && b!=27 && b!=13 && b!='c');
		if(b=='c')
		{
			origox=160; origoy=100;
			savefc();
		}
	 	if(b==32)
		{
			_setcolor(255);
			for(b=0;b<200;b++)
			{
				_moveto(0,b);
				_lineto(319,b);
			}
		}
		if(b==27) break;
	}
 }
 if(!longanim) break;
 if(origoreset) origox=origoy=0;
 if(b==27) break;
}
	if(savegfx)
	{
	    	fseek(gfx,8L,SEEK_SET);
		fwrite(picposu,2,pn,gfx);
		fclose(gfx);
	}
	_setvideomode(_DEFAULTMODE);
	printf("File contained %i pictures.\n",num);
}

unsigned char sbuf[330];

int	savefc(void)
{ /* color 255 considered transparent */
	int	x,y,a,b,c,d,e,f;
	int	leftx=origox,rightx=origox,upy=origoy,downy=origoy;
	FILE	*f1;
	long	sizepos;
	f1=fopen("pic.fcp","wb");
	
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
}


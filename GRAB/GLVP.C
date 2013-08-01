#define VPSOFT

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

char	*palettefname;
char	filename[64];
char	basefname[16]={""}; /* 8 chars max */
char	basefname1[16]={""}; /* 7 chars max */

union	REGS	reg;
unsigned char far *vram=(char far *)0xa0000000;

char	apubuf[512];

int	leftsidedot=0; /* switch for marker */

int	subpicframe=0;

unsigned char pal[768]={
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
unsigned char far pic[64000];
unsigned char far pic2[64000];
unsigned char far picorg[64000];
int	filetype; /* 1=LBM */
int	backcol=255;

/* for marktile */
int	customwid=16;
int	customhig=16;

int	setback255=0;
int	fcp4plane=0;
int	fcptransp=1;
int	fcporigo=0,fcporigox,fcporigoy;
int	fcpsavepal=0; 
int	fcpstyle=15; /* wing commander style */
char	*fcpstyletxt[16]={
"0",
"1",
"2",
"3",
"4",
"5",
"6",
"7",
"8",
"9",
"10",
"11",
"12",
"13",
"14",
"FastWing(transp)",
};

int	savetype=3;
char	*savetypetxt[4]={
"0..9",
"A..Z",
"0..9,A..Z",
"KeyPress"};
char	savenumber[12]={"0123456789*"};
char	saveletter[32]={"ABCDEFGHIJKLMNOPQRSTUVWXYZ*"};
char	saveboth[48]={"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ*"};

int	markthis=-9;
int	savepnt;
int	subpics=0;
#define SUBMAX 384
int	subx1[SUBMAX];
int	suby1[SUBMAX];
int	subx2[SUBMAX];
int	suby2[SUBMAX];

long picpos[SUBMAX],pics; /* for loading FCP:s */
unsigned picposu[SUBMAX]; /* for loading FCP:s */

int	flashycount=0;

int	gfxsubfile;

int	cmdpnt=-1;
char	cmdstr[64];

int	loadformat=0;

main(int argc,char *argv[])
{
	int	a,b,c;
	*filename=0; *basefname=0;
	palettefname=NULL;
	for(a=1;a<argc;a++)
	{
		if(*argv[a]=='/' || *argv[a]=='-') switch(*(argv[a]+1))
		{
			case 'h' :
			case 'H' :
			case '?' : *filename=0; a=argc; break;
			case 's' :
			case 'S' : savetype=*(argv[a]+2)-48; break;
			case 'f' : 
				loadformat=toupper(*(argv[a]+2))-'0'; 
				if(loadformat>9) loadformat+='0'-'A'+11;
				break;
			case 'o' : switch(*(argv[a]+2))
			{
				case '4' : fcp4plane=1; break;
				case 'p' : fcpsavepal=1; break;
				case '0' : backcol=0; break;
				case '9' : backcol=255; break;
			} break;
			case 't' : switch(*(argv[a]+2))
			{
				case 'w' : customwid=atoi(argv[a]+3); break;
				case 'h' : customhig=atoi(argv[a]+3); break;
			} break;
			case 'x' : strcpy(cmdstr,argv[a]+2); cmdpnt=0; break;
			case 'c' : setback255=1; break;
			case 'p' : palettefname=argv[a]+2; break;
			case 'b' : strcpy(basefname,argv[a]+2); break;
			case 'g' : gfxsubfile=atoi(argv[a]+2); break;
			default :
				printf("Unknown option: %s\n",argv[a]);
				exit(1);
		}
		else strcpy(filename,argv[a]);
	}
	if(!*filename)
	{
		printf("Great Library View Picture V1.0   Copyright (C) 1991 Sami Tammilehto\n"
		"usage: GLVP <inputfilename[.LBM']> [switch(es)]          Switches:\n"
		"-h		Help\n"
		"-b<fname>	Set basefilename\n"
		"-s#		Sets savetype (numbers,letter,both,keywait)\n"
		"-o4		Set 4-bitplane to default\n"
		"-o0		Set transp. color to 0\n"
		"-o9		Set transp. color to 255\n"
		"-op		Set palettesaving to default\n"
		"-c		Floodfills background to 255 before loading the pic\n"
		"-x<keys>	Execute keypresses\n"
		"-p<fname>	Load a palette from standard palette file\n"
		"-tw#		Custom tilewidth (mark command 2)\n"
		"-th#		Custom tileheigth (mark command 2)\n"
		"-f#		Specify fileformat: 0=LBM,1=FCP,2=UH,A=CMP(accolade),B=BM,C=4BM\n"
		"-g#		Specify number of subfile to load\n"
		); return(0);
	}
	else printf("Great Library View Picture V1.0\n");
	/* make (base)fname(1) & filename */
	for(a=strlen(filename)-1;a>=0 && filename[a]!='\\';a--) if(filename[a]=='.') { a=-9; break; }
	if(a!=-9) strcat(filename,".LBM");
	strupr(filename);
	if(*basefname==0)
	{
		char	*p=filename;
		if(p[1]==':') p+=2;
		if(strrchr(p,'\\')==NULL) strcpy(basefname,p);
		else strcpy(basefname,strrchr(p,'\\')+1);
		*strchr(basefname,'.')=0;
	}
	basefname[8]=0;
	strcpy(basefname1,basefname);
	basefname1[7]=0;
	
	for(a=0;a<16;a++)
	{
		pal[(a+16)*3+0]=63-a*4;
		pal[(a+16)*3+1]=(63-a*4)*3/4;
		pal[(a+16)*3+2]=(63-a*4)*3/4;
	}
	for(a=2;a<16;a++)
	{
		memcpy(pal+a*16*3,pal+16*3,16*3);
	}
	pal[255*3+0]=24;
	pal[255*3+1]=24;
	pal[255*3+2]=32;
	if(palettefname!=NULL)
	{
		FILE	*f1;
		f1=fopen(palettefname,"rb");
		if(f1==NULL)
		{
			printf("Palette not found. Program aborted.\n");
			exit(1);
		}
		fread(pal,3,256,f1);
		fclose(f1);
	}
	/* do some real stuff */
	if(setback255) memset(pic,255,64000);
	for(a=2,b=loadformat;b<99 && a==2;b++)
	{
		switch(b)
		{
			case 0 : a=loadlbm(filename); break;
			case 1 : a=loadfcp(filename); break;
			case 2 : a=loaduh(filename); break;
			case 3 : a=loadgfx(filename); break;
			default :
				printf("\nUnknown fileformat! Program aborted.\n");
				return(1);
			case 11 : a=loadcmp(filename); break;
			case 12 : a=loadu(filename); break;
			case 13 : a=loadu4(filename); break;
		}
		if(a==3)
		{
			printf("\nFile '%s' not found. Program aborted.\n",filename);
			return(0);
		}
		else if(a==1) 
		{
			printf("\nError loading picture. Program aborted.\n");
			return(0);
		}
	}
	reg.x.ax=0x13;
	int86(0x10,&reg,&reg);
	viewpal(0);
	viewpic(0);
	greycol=findcol(15,15,24);
	blackcol=findcol(0,0,0);
	memcpy(picorg,pic,64000);
	commands();
	reg.x.ax=0x3;
	int86(0x10,&reg,&reg);
}

int	printhelp(void)
{
	cls();
	printf("Help screen:\n");
	printf(
	" n	unmark all pics\n"
	" m	mark all separate pics\n"
	" M	mark with left-side-dot-remove\n"
	" N	mark with blue->backcol\n"
	" 6	mark 64x64 pics\n"
	" 3	mark 32x32 pics\n"
	" 2	mark custom x custom pics\n"
	" 1	mark 16x16 pics\n"
	" 8	mark 8x8 pics\n"
	"\n"
	" v	save unpacked (with pal+hdr)\n"
	" u	save unpacked (no header)\n"
	" y	save unpacked (with header)\n"
	" f	save FCP (fastwing)\n"
	" F	save all in one FCP\n"
	" g	save all in one GFX (4)\n"
	" G	save all in one GFX (4) w/ pal\n"
	" h	save all in one GFX (1)\n"
	" H	save all in one GFX (1) w/ pal\n"
	" r	save all in one GFX (R)\n"
	" R	save all in one GFX (R) w/ pal\n"
	" x	save all in one GFX (X)\n"
	" X	save all in one GFX (X) w/ pal\n"
	" p	save palette\n"
	"\n--more--");
	getch();
	cls();
	printf(
	" ESC/!	exit program\n"
	" ?	this help screen\n"
	" i	picture information\n"
	" I	picture palette\n"
	"\n"
	" T	FLAGS; transcol is transparent\n"
	" O	FLAGS; origo\n"
	" A     FLAGS; automatic origo (col 1)\n"
	" P	FLAGS; savepal\n"
	" s	toggle savename determinations\n"
	" 0/9	set background color to 0/255\n"
	" D	select visual subpiccy-frame\n"
	"\n"
	);
	printf("\nPress any key to return.");
	getch();
}

int	commands(void)
{
	int	x,y,a,b,c;
	long	l;
	unsigned u;
	while(!xit)
	{
		flashycount=0;
		if(subpicframe) viewpic(0);
		while(!kbhit() && cmdpnt==-1)
		{
			waitborder();
			flashycount++;
			flashycount&=63;
			if(!subpicframe)
			{
				if(flashycount==0) viewpic(0);
				if(flashycount==32) viewpic(1);
			}
		}
		if(cmdpnt!=-1)
		{
			a=cmdstr[cmdpnt++];
			if(a==0) cmdpnt=-1;
		}
		else
		{
			a=getch(); if(a==0) a=1000+getch();
		}
		switch(a)
		{
			case '?' :
				printhelp();
				viewpic(0);
				viewpal(0);
				break;
			case 'D' :
				subpicframe^=1;
				break;
			case 'O' : fcporigo^=1;
				cls();
				printpicinfo();
				viewpal(0);
				break;
			case 'A' : fcporigo=3;
				cls();
				printpicinfo();
				viewpal(0);
				break;
			case '4' : fcp4plane^=1;
				cls();
				printpicinfo();
				viewpal(0);
				break;
			case 'P' : fcpsavepal^=1;
				cls();
				printpicinfo();
				viewpal(0);
				break;
			case 's' : savetype++;
				if(savetype>3) savetype=0;
				cls();
				printpicinfo();
				viewpal(0);
				break;
			case 'i' :
				cls();
				printpicinfo();
				getch();
				viewpal(0);
				viewpic(0);
				break;
			case 'I' :
				viewpal(0);
				viewpic(0);
				{
					char far *p;
					int x,y,a;
					for(x=0;x<16;x++) for(y=0;y<16;y++)
					{
						a=x*16+y;
						p=vram+320-48+x*3+(200-48+y*3)*320;
						*(p)=a;
						*(p+1)=a;
						*(p+2)=a;
						*(320+p)=a;
						*(320+p+1)=a;
						*(320+p+2)=a;
						*(640+p)=a;
						*(640+p+1)=a;
						*(640+p+2)=a;
					}
				}
				getch();
				viewpal(0);
				viewpic(0);
				break;
			case 32 :
				memcpy(pic,picorg,64000);
 				viewpal(0);
				viewpic(0);
				break;
			case 27 : 
			case '!' :
				xit=1;
				break;
				
			case 'n' :
				subpics=0;
				viewpic(0);
				break;
			case 'm' :
				markthis=-9;
				viewpic(1);
				markall();
				break;
			case 'M' :
				leftsidedot=1;
				markthis=-9;
				viewpic(1);
				markall();
				leftsidedot=0;
				break;
			case 'N' :
				leftsidedot=2;
				markthis=-9;
				viewpic(1);
				markall();
				leftsidedot=0;
				break;
			case '8' :
				viewpic(1);
				marktiles(8,8);
				break;
			case '1' :
				viewpic(1);
				marktiles(16,16);
				break;
			case '2' :
				viewpic(1);
				marktiles(customwid,customhig);
				break;
			case '3' :
				viewpic(1);
				marktiles(32,32);
				break;
			case '6' :
				viewpic(1);
				marktiles(64,64);
				break;
			case '+' :
				markthis++;
				if(markthis<0) markthis=0;
				viewpic(1);
				markall();
				break;
			case '-' :
				markthis--;
				if(markthis<0) markthis=0;
				viewpic(1);
				markall();
				break;
				
			case '0' : backcol=0; break;
			case '9' : backcol=255; break;
			
			case 'p' :
				{
					FILE	*f1;
					strcpy(apubuf,basefname);
					strcat(apubuf,".PAL");
					f1=fopen(apubuf,"wb");
					fwrite(pal,3,256,f1);
					fclose(f1);
				}
				break;
			case 'u' : /* unpacked w/o header */
			case 'y' : /* unpacked with cheader */
			case 'v' : /* unpacked with palette / header */
			case 'f' : /* fastwing */
				viewpic(1);
				savepnt=0;
				if(subpics==0)
				{
					savepnt=-1;
					save(a,0,0,319,199);
					viewpic(0);
				}
				else for(b=0;b<subpics;b++)
				{
					c=save(a,subx1[b],suby1[b],subx2[b],suby2[b]);
					if(c==-1) break;
				}
				viewpic(0);
				break;
			case 'F' :
				if(subpics==0) break;
				viewpic(1);
				savepnt=-1;
				save('f',-1,-1,-1,-1);
				viewpic(0);
			case 'G' :
			case 'g' :
			case 'H' :
			case 'h' :
			case 'R' :
			case 'r' :
			case 'X' :
			case 'x' :
				viewpic(1);
				if(subpics==0)
				{
					savepnt=-1;
					subpics=1;
					subx1[0]=0;
					suby1[0]=0;
					subx2[0]=319;
					suby2[0]=199;
					save(a,-1,-1,-1,-1);
					subpics=0;
					break;
				}
				savepnt=-1;
				save(a,-1,-1,-1,-1);
				viewpic(0);
				break;
		}
	}
}

/* picture loaders:
** LBM
** FCP
** CMP
** 
*/
#include "vpl.c"

int	printpicinfo()
{
	printf( "Picture information:\n"
		"Basefilename: %s\n"
		"Transp.color: %i\n"
		"MarkedBlocks: %i\n"
		"Savenametype: %s\n"
		"\n"
		"FuturePictureParameters:\n"
		"     Packing: %s\n"
		"Save palette: %s\n"
		"       Origo: %s\n"
		" Transparent: %s\n"
		" 4-bit-plane: %s\n"
		"\n",
		basefname,backcol,subpics,
		savetypetxt[savetype],
		fcpstyletxt[fcpstyle],
		fcpsavepal?"yes":"no",
		fcporigo?"center":"topleft",
		fcptransp?"yes":"no",
		fcp4plane?"yes":"no");
	if(filetype==1)
	{ /* lbm */
		printf( "Loaded file: Deluxepaint/LBM\n"
			"Width:%i Height:%i\n"
			"Planes:%i Colors:%i Stencil:%i\n"
			,basefname,xsz,ysz,planes,colors,stencil);
	}
	if(filetype==1)
	{ /* fcp */
		printf( "Loaded file: FuturePicture (FCP)\n"
			);
	}
}

int	eschit(void)
{
	int	a;
	if(kbhit()) 
	{
		a=getch();
		if(a==27) return(1);
	}
	return(0);
}

#define	pxor(xx,yy) *(vram+(xx)+(yy)*320)=*(vram+(xx)+(yy)*320)^0xe1
#define	pset(xx,yy,zz) *(vram+(xx)+(yy)*320)=(zz)
#define	pget(xx,yy) *(vram+(xx)+(yy)*320)

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
	if(!flag) viewsubpics();
}

int	viewsubpics(void)
{
	int	a;
	if(subpicframe) for(a=0;a<subpics;a++)
	{
		area(subx1[a]-1,suby1[a]-1,subx2[a]+1,suby2[a]+1,-1);
	}
	else for(a=0;a<subpics;a++)
	{
		if(subx1[a]==subx2[a] && suby1[a]==suby2[a]) 
		{
			pxor(subx1[a],suby1[a]);
		}
		else xorrec(subx1[a],suby1[a],subx2[a],suby2[a]);
	}
}

int	cls(void)
{
	int	a;
	memset(vram,0,64000);
	/* set palettes 16 first colors to greyscale */
	waitborder();
	outp(0x3c8,7);
	outp(0x3c9,44);
	outp(0x3c9,20);
	outp(0x3c9,4);
	gotoxy(0,0);
}

int	markall(void)
{
	int	leftsidecol;
	int	a,b,c,mcc=0;
	int	x1,y1,x2,y2,x,y;
	int	ox1,oy1,ox2,oy2;
	viewpal(1);
	subpics=0;
	memset(apubuf,backcol,320);
	b=0;
	for(a=0;a<320;a++) if(pget(a,0)!=backcol) { b++; break; }
	for(a=0;a<320;a++) if(pget(a,199)!=backcol) { b++; break; }
	for(a=0;a<200;a++) if(pget(0,a)!=backcol) { b++; break; }
	for(a=0;a<200;a++) if(pget(319,a)!=backcol) { b++; break; }
	if(b==4)
	{
		subx1[subpics]=0;
		suby1[subpics]=0;
		subx2[subpics]=319;
		suby2[subpics]=199;
		subpics++;
		viewpal(0);
		viewpic(0);
		return(0);
	}
	for(y=0;y<200 && !eschit();y++) if((leftsidedot!=1 && memcmp(apubuf,pic+y*320,320)) || (leftsidedot==1 && pic[y*320]!=backcol))
	{
		if(leftsidedot==1) { leftsidecol=pic[y*320]; pic[y*320]=backcol; pset(0,y,backcol); }
		for(x=0;x<320;x++) if(pget(x,y)!=backcol)
		{
			if(leftsidedot==2) { leftsidecol=pget(x,y); }
			x1=x2=x;
			y1=y2=y;
			/* test box */
			for(c=1;c!=0 && !eschit();)
			{
				c=0;
				for(a=x1;a<=x2;a++)
				{
					if(y1>=0 && pget(a,y1)!=backcol) { y1--; c=1; }
					if(y2<=199 && pget(a,y2)!=backcol) { y2++; c=1; }
				}
				for(a=y1;a<=y2;a++)
				{
					if(x1>=0 && pget(x1,a)!=backcol) { x1--; c=1; }
					if(x2<=319 && pget(x2,a)!=backcol) { x2++; c=1; }
				}
				if(x1==0 && y1==0 && x2==319 && y2==199)
				{
					x1=-1; y1=-1; x2=320; y2=200;
					c=0;
				}
			}
			if(kbhit()) break;
			y1++; y2--; x1++; x2--;
			if(leftsidedot)
			{
				int	x,y;
				for(x=x1;x<=x2;x++) for(y=y1;y<=y2;y++)
				  if(pic[x+y*320]==leftsidecol)
				  {
				  	pic[x+y*320]=backcol;
					pset(x,y,backcol);
				  }
			}
			area(x1,y1,x2,y2,backcol);
			if(markthis==mcc || markthis==-9)
			{
				if(leftsidedot==2)
				{
					x1++; y1++;
					x2--; y2--;
				}
				subx1[subpics]=x1;
				subx2[subpics]=x2;
				suby1[subpics]=y1;
				suby2[subpics]=y2;
				subpics++;
			}
			mcc++;
			if(subpics>SUBMAX) 
			{
				printf("OVERFLOW!");
				subpics=0;
				return(1);
			}
		}
		if(y>1)
		{
			pset(0,y-2,greycol);
			pset(319,y-2,greycol);
			pset(1,y-2,blackcol);
			pset(318,y-2,blackcol);
		}
	}
	else 
	{
		if(y>1)
		{
			pset(0,y-2,greycol);
			pset(319,y-2,greycol);
			pset(1,y-2,blackcol);
			pset(318,y-2,blackcol);
		}
	}
	if(kbhit()) subpics=0;
	if(subpics==0 && markthis!=-9) markthis=0; 
	viewpal(0);
	viewpic(0);
	return(0);
}

int	marktiles(int wid,int hig)
{
	int	x,y,x1,y1;
	viewpal(1);
	subpics=0;
	for(y=0;y<=200-hig && !kbhit();y+=hig) for(x=0;x<=320-wid && !eschit();x+=wid)
	{
		for(y1=y;y1<y+wid;y1++)	
		{
			for(x1=x;x1<x+wid;x1++) 
			{
				if(pget(x1,y1)!=backcol) { y1=998; break; }
			}
		}
		if(y1==999)
		{
			area(x,y,x+wid-1,y+hig-1,backcol);
			subx1[subpics]=x;
			subx2[subpics]=x+wid-1;
			suby1[subpics]=y;
			suby2[subpics]=y+hig-1;
			subpics++;
			if(subpics>SUBMAX)
			{
				printf("OVERFLOW!");
				subpics=0;
				return(1);
			}
		}
	}
	if(kbhit()) subpics=0;
	viewpal(0);
	viewpic(0);
}


int	save(int cmd,int x1,int y1,int x2,int y2)
{
	int	a,b;
	char	fname[64];
	char	*savestr;
	switch(savetype)
	{
		case 0 : savestr=savenumber; break;
		case 1 : savestr=saveletter; break;
		case 2 : savestr=saveboth; break;
		case 3 : savestr=NULL; break;
	}
	if(savepnt==-1) strcpy(fname,basefname);
	else 
	{
		strcpy(fname,basefname1);
		if(savestr==NULL) 
		{
			while(!kbhit())
			{
				xorrec(x1,y1,x2,y2);
				for(a=0;a<8 && !kbhit();a++) waitborder();
				xorrec(x1,y1,x2,y2);
				for(a=0;a<8 && !kbhit();a++) waitborder();
			}
			a=getch();
		}
		else
		{
			a=savestr[savepnt++];
		}
		if(a=='*' || a==27 || a==32) return(-1);
		fname[b=strlen(fname)]=a;
		fname[b+1]=0;
	}
	switch(cmd)
	{
		case 'u' :
			if(saveunp(fname,x1,y1,x2,y2)) return(1);
			break;
		case 'y' :
			if(saveunpheader(fname,x1,y1,x2,y2)) return(1);
			break;
		case 'v' :
			if(saveunpheaderpal(fname,x1,y1,x2,y2)) return(1);
			break;
		case 'f' :
			if(savefcp(fname,x1,y1,x2,y2)) return(1);
			break;
		case 'g' :
			if(savegfx(0,fname,x1,y1,x2,y2)) return(1);
			break;
		case 'G' :
			if(savegfx(1/*incl.pal*/,fname,x1,y1,x2,y2)) return(1);
			break;
		case 'h' :
			if(savegfx1(0,fname,x1,y1,x2,y2)) return(1);
			break;
		case 'H' :
			if(savegfx1(1/*incl.pal*/,fname,x1,y1,x2,y2)) return(1);
			break;
		case 'r' :
			if(savegfx3(0,fname,x1,y1,x2,y2)) return(1);
			break;
		case 'R' :
			if(savegfx3(1/*incl.pal*/,fname,x1,y1,x2,y2)) return(1);
			break;
		case 'x' :
			if(savegfxx(0,fname,x1,y1,x2,y2)) return(1);
			break;
		case 'X' :
			if(savegfxx(1/*incl.pal*/,fname,x1,y1,x2,y2)) return(1);
			break;
		default :
			printf("INTERNAL ERROR: cmd unknown!\n");
			exit(1);
			break;
	}
	area(x1,y1,x2,y2,backcol);
	return(0);
}

int	saveunp(char *fname,int x1,int y1,int x2,int y2)
{
	FILE	*f1;
	int	x,y;
	strcat(fname,".U");
	f1=fopen(fname,"wb");
	if(f1==NULL) return(1);
	for(y=y1;y<=y2;y++)
	{
		for(x=x1;x<=x2;x++)
		{
			putc(pic[x+y*320],f1);
		}
		pxor(x1,y);
		pxor(x2,y);
	}
	fclose(f1);
}

int	saveunpheader(char *fname,int x1,int y1,int x2,int y2)
{
	FILE	*f1;
	int	x,y;
	strcat(fname,".UH");
	f1=fopen(fname,"wb");
	if(f1==NULL) return(1);
	putw('U'+'H'*256,f1); /* magic */
	putw(1,f1); /* number of pics */
	putw(x2-x1+1,f1); /* width */
	putw(y2-y1+1,f1); /* heigth */
	putw(0,f1); 
	putw(0,f1);
	putw(0,f1);
	putw(0,f1);
	for(y=y1;y<=y2;y++)
	{
		for(x=x1;x<=x2;x++)
		{
			putc(pic[x+y*320],f1);
		}
		pxor(x1,y);
		pxor(x2,y);
	}
	fclose(f1);
}

int	saveunpheaderpal(char *fname,int x1,int y1,int x2,int y2)
{
	FILE	*f1;
	int	x,y;
	strcat(fname,".UH");
	f1=fopen(fname,"wb");
	if(f1==NULL) return(1);
	putw('U'+'h'*256,f1); /* magic */
	putw(49,f1); /* paraoffset to pic */
	putw(x2-x1+1,f1); /* width */
	putw(y2-y1+1,f1); /* heigth */
	putw(0,f1); 
	putw(0,f1);
	putw(0,f1);
	putw(0,f1); // palette
	fwrite(pal,1,768,f1);
	for(y=y1;y<=y2;y++)
	{
		for(x=x1;x<=x2;x++)
		{
			putc(pic[x+y*320],f1);
		}
		pxor(x1,y);
		pxor(x2,y);
	}
	fclose(f1);
}

int	savefcp(char *fname,int x1,int y1,int x2,int y2)
{
	int	style;
	int	tx1=x1,ty1=y1,tx2=x2,ty2=y2;
	int	xl=y2-y1+1,yl=y2-y1+1;
	int	xo,yo;
	int	x,y,z,zm=1;
	int	a,b,c,d,w,wc;
	unsigned u;
	long	tell4,telltbl;
	long	planepos[4];
	FILE	*f1;
	if(x1!=-1) pics=1;
	else pics=subpics;
	strcat(fname,".FCP");
	f1=fopen(fname,"wb");
	if(f1==NULL) return(1);
	putc('F',f1);
	putc('C',f1);
	putc('P',f1);
	putc(0x1a,f1);
	putc(0x10,f1); /* file ver */
	a=0;
	if(fcpsavepal) a|=1;
	putc(a,f1); /* single */
	putw(pics,f1); /* single file */
	telltbl=ftell(f1);
	fwrite(picpos,4,pics,f1);
	if(fcpsavepal)
	{
		fwrite(pal,256,3,f1);
	}

	if(x1==-1) 
	{
		for(a=0;a<pics;a++)
		{
			picpos[a]=ftell(f1);
			savefcpdata(f1,subx1[a],suby1[a],subx2[a],suby2[a]);
			area(subx1[a],suby1[a],subx2[a],suby2[a],backcol);
		}
	}
	else
	{
		picpos[0]=ftell(f1);
		savefcpdata(f1,x1,y1,x2,y2);
	}
	
    	fseek(f1,telltbl,SEEK_SET);
	fwrite(picpos,4,pics,f1);
    	fseek(f1,0L,SEEK_END);
	fclose(f1);
	return(0);
}

#include "sfcp.c"

int	savegfx(int flags,char *fname,int x1,int y1,int x2,int y2)
{
	FILE	*f1;
	long	fpos1,fpos2;
	int	pn=0;
	int	x,y,a,b,c;
	strcat(fname,".GFX");
	f1=fopen(fname,"wb");
	if(f1==NULL) return(1);
	if(x1!=-1) pics=1;
	else pics=subpics;
	
	putc('G',f1);
	putc('F',f1);
	putc('X',f1);
	putc('1',f1);
	putc('0',f1);
	putc(0x1a,f1);
	
	putw(pics+(flags&1),f1);
	fwrite(picposu,2,pics,f1);

	if(flags&1) /* save palette */
	{
		b=16-((ftell(f1))&15); if(b!=16) for(c=0;c<b;c++) putc(0,f1);
		picposu[pn++]=ftell(f1)>>4;
		putc(0xfe,f1);
		fwrite(pal,1,768,f1);
	}
	for(a=0;a<pics;a++)
	{
		b=16-((ftell(f1))&15); if(b!=16) for(c=0;c<b;c++) putc(0,f1);
		picposu[pn++]=ftell(f1)>>4;
		savegfxdata(f1,subx1[a],suby1[a],subx2[a],suby2[a]);
		area(subx1[a],suby1[a],subx2[a],suby2[a],backcol);
	}
	
    	fseek(f1,8L,SEEK_SET);
	fwrite(picposu,2,pn,f1);
	fclose(f1);
}

int	savegfx1(int flags,char *fname,int x1,int y1,int x2,int y2)
{
	FILE	*f1;
	long	fpos1,fpos2;
	int	pn=0;
	int	x,y,a,b,c;
	strcat(fname,".GFX");
	f1=fopen(fname,"wb");
	if(f1==NULL) return(1);
	if(x1!=-1) pics=1;
	else pics=subpics;
	
	putc('G',f1);
	putc('F',f1);
	putc('X',f1);
	putc('1',f1);
	putc('0',f1);
	putc(0x1a,f1);
	
	putw(pics+(flags&1),f1);
	fwrite(picposu,2,pics,f1);

	if(flags&1) /* save palette */
	{
		b=16-((ftell(f1))&15); if(b!=16) for(c=0;c<b;c++) putc(0,f1);
		picposu[pn++]=ftell(f1)>>4;
		putc(0xfe,f1);
		fwrite(pal,1,768,f1);
	}
	for(a=0;a<pics;a++)
	{
		b=16-((ftell(f1))&15); if(b!=16) for(c=0;c<b;c++) putc(0,f1);
		picposu[pn++]=ftell(f1)>>4;
		savegfxdata1(f1,subx1[a],suby1[a],subx2[a],suby2[a]);
		area(subx1[a],suby1[a],subx2[a],suby2[a],backcol);
	}
	
    	fseek(f1,8L,SEEK_SET);
	fwrite(picposu,2,pn,f1);
	fclose(f1);
}

int	savegfx3(int flags,char *fname,int x1,int y1,int x2,int y2)
{ // rotated
	FILE	*f1;
	long	fpos1,fpos2;
	int	pn=0;
	int	x,y,a,b,c;
	strcat(fname,".GFX");
	f1=fopen(fname,"wb");
	if(f1==NULL) return(1);
	if(x1!=-1) pics=1;
	else pics=subpics;
	
	putc('G',f1);
	putc('F',f1);
	putc('X',f1);
	putc('1',f1);
	putc('0',f1);
	putc(0x1a,f1);
	
	putw(pics+(flags&1),f1);
	fwrite(picposu,2,pics,f1);

	if(flags&1) /* save palette */
	{
		b=16-((ftell(f1))&15); if(b!=16) for(c=0;c<b;c++) putc(0,f1);
		picposu[pn++]=ftell(f1)>>4;
		putc(0xfe,f1);
		fwrite(pal,1,768,f1);
	}
	for(a=0;a<pics;a++)
	{
		b=16-((ftell(f1))&15); if(b!=16) for(c=0;c<b;c++) putc(0,f1);
		picposu[pn++]=ftell(f1)>>4;
		savegfxdata3(f1,subx1[a],suby1[a],subx2[a],suby2[a]);
		area(subx1[a],suby1[a],subx2[a],suby2[a],backcol);
	}
	
    	fseek(f1,8L,SEEK_SET);
	fwrite(picposu,2,pn,f1);
	fclose(f1);
}

int	savegfxx(int flags,char *fname,int x1,int y1,int x2,int y2)
{ // optimized x sprite
	FILE	*f1;
	long	fpos1,fpos2;
	int	pn=0;
	int	x,y,a,b,c;
	strcat(fname,".GFX");
	f1=fopen(fname,"wb");
	if(f1==NULL) return(1);
	if(x1!=-1) pics=1;
	else pics=subpics;
	
	putc('G',f1);
	putc('F',f1);
	putc('X',f1);
	putc('1',f1);
	putc('0',f1);
	putc(0x1a,f1);
	
	putw(pics+(flags&1),f1);
	fwrite(picposu,2,pics,f1);

	if(flags&1) /* save palette */
	{
		b=16-((ftell(f1))&15); if(b!=16) for(c=0;c<b;c++) putc(0,f1);
		picposu[pn++]=ftell(f1)>>4;
		putc(0xfe,f1);
		fwrite(pal,1,768,f1);
	}
	for(a=0;a<pics;a++)
	{
		b=16-((ftell(f1))&15); if(b!=16) for(c=0;c<b;c++) putc(0,f1);
		picposu[pn++]=ftell(f1)>>4;
		savegfxdatax(f1,subx1[a],suby1[a],subx2[a],suby2[a]);
		area(subx1[a],suby1[a],subx2[a],suby2[a],backcol);
	}
	
    	fseek(f1,8L,SEEK_SET);
	fwrite(picposu,2,pn,f1);
	fclose(f1);
}

#include "gfxsave.h"



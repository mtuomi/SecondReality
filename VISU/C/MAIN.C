// 3DS converter V2.0
#include <stdio.h>
#include <stdarg.h>
#include <conio.h>
#include <ctype.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <math.h>

#include "..\cd.h"
#include "..\c.h"

void	vDraw(void);
void	vDeinit(void);
void	vClear(void);
void	vNext(void);
void	vInit(void);
void	vPalette(void);
int	drawpass=0;

void	doscene(int scene);

int	addvx(long x,long y,long z);
int	addnr(long x,long y,long z);

char far *vram=(char far *)0xa0000000L;

char	fill_color[64000];
char	fill_object[64000];
char	fill_curobj;

//----------------------------------------------------------------

// filenames
char	*inname="NoName";
char	ascname[64];
char	vuename[64];
char	tmpname[64];
char	scenename[64];

// flags
int	cityflag=0;
int	debug=0;
int	printon=0;
int	reporton=1;
int	playloop=0;
int	materials=0;
int	vuedone=0;

// variables
float	fxadd,fxmul;
float	fyadd,fymul;
float	fzadd,fzmul;
float	scale=100.0F;

#define BIGLONG 2000000000L
#define MAXMAT 32
#define MAXFACE 16 // in a polygon
#define MAXOBJ 256
#define MAXVX 500
#define MAXNR 500
#define MAXFC 500
#define NORMALSIZE UNIT // length of normals

//----------------------------------------------------------------

// globals
rmatrix camera;
FILE	*in,*in2;
FILE	*outb,*outb2,*outb3;
FILE	*freport=NULL;
FILE	*fdeb;
int	debcount=0;
char	palette[768];

// prototypes
void	readvue(void);
void	readasc(void);

// world structs
struct s_cobject
{
	char	name[18];
	int	index;
	int	duplicate;
	int	original;
	int	lastused;
	int	f1,f2,n1,n2,v1,v2,ng;
	long	size;
	char	fname[16];
	object	*o;
	long	dist;
	int	on;
	int	last_on;
	long	last_x,last_y,last_z;
	long	last_m[9];
} co[MAXOBJ];
int	conum=0;

struct s_material
{
	char	name[32];
	int	flags;
	int	color;
	int	colorlen;
} mat[MAXMAT];
int	matnum=0;
char	matpal[768];
int	matpalenabled=0;

int	usefov;

object	camobject;

// other c code included
#include "save.c"
#include "util.c"
#include "osort.c"
#include "readasc.c"
#include "readvue.c"
#include "readmat.c"
#include "readinf.c"

// main

main(int argc,char *argv[])
{
	char	tmpfname[64];
	int	pause=0;
	int	a;
	int	totalfaces=0,totalvertices=0,totalnormals=0;
	
	fdeb=fopen("debug.tmp","wt");
	
	if(argc==1)
	{
		printf("\n"
			"usage: C [options] <scenename(no extensions)>\n"
			"-c	Set Cityflag\n"
			"-w	Set Runflag\n"
			"-r	Generate report (to file REPORT)\n"
			"-p	Print report to screen as well\n"
			"-l	Loop the animation phase until key pressed\n"
			"-s###	Specify scale factor (default 100.0)\n"
			);
		return(0);
	}
	
	reporton=0;
	for(a=1;a<argc;a++) if(argv[a][0]=='-') switch(argv[a][1])
	{
	case 'c' :
	case 'C' : cityflag=1; break;
	case 'w' :
	case 'W' : cityflag=2; break;
	case 'r' :
	case 'R' : reporton=1; break;
	case 'p' :
	case 'P' : reporton=printon=1; break;
	case 's' :
	case 'S' : sscanf(argv[a]+2,"%f",&scale); break;
	case 'l' :
	case 'L' : playloop=1; break;
	}
	else strcpy(inname,argv[a]);

	// defaults
	strupr(inname);
	strcpy(ascname,inname);
	strcat(ascname,".ASC");
	strcpy(vuename,inname);
	strcat(vuename,".VUE");
	strcpy(scenename,inname);
	
	fxmul=scale; fxadd=0.0F;
	fymul=scale; fyadd=0.0F;
	fzmul=scale; fzadd=0.0F;
	
	in=fopen(ascname,"rt");
	if(!in)
	{
		printf("%s not found!\n",ascname);
		exit(3);
	}
	fclose(in);

	if(reporton) freport=fopen("report","wt");

	// blue screen
	printf( "\n");
	printf(	"Analyzing and converting (while screen is blue)\n");
	printf( "  Scene name: %s\n",inname);
	printf( "Scale factor: %f\n",scale);
	if(pause)
	{
		printf("Press any key to start.\n");
		getch();
	}
	else printf("\n");
	outp(0x3c8,0); outp(0x3c9,0); outp(0x3c9,0); outp(0x3c9,32);

	strcpy(co[0].name,"CAMERA");
	co[0].on=1; // camera is always on
	co[0].index=-1;
	co[0].duplicate=1; // let's lie
	co[0].original=0;
	co[0].lastused=0;
	co[0].o=&camobject;
	camobject.r=&camera;
	camobject.r0=&camera;
	conum=1; 		
	
	strcpy(tmpname,inname);
	strcat(tmpname,".INF");
	in=fopen(tmpname,"rt");
	if(in) fclose(in);
	else 
	{
		printf("%s.INF file not found!\n",inname);
		exit(3);
	}

	strcpy(tmpname,inname);
	strcat(tmpname,".MAT");
	in=fopen(tmpname,"rt");
	if(in)
	{
		readmat();
		fclose(in);
	}
	
	strcpy(tmpname,inname);
	strcat(tmpname,".PAL");
	in=fopen(tmpname,"rb");
	if(in)
	{
		matpalenabled=1;
		fread(matpal,768,1,in);
		fclose(in);
	}
	else 
	{
		for(a=0;a<32;a++)
		{
			matpal[a*3+0]=a*2;
			matpal[a*3+1]=a*2;
			matpal[a*3+2]=a*2;
		}
	}

	sprintf(tmpfname,"%s.00M",scenename);
	outb3=fopen(tmpfname,"wb");
	putw(0,outb3);
	putw(0,outb3);
	putw(0,outb3);
	putw(0,outb3);
	putw(0,outb3);
	putw(0,outb3);
	putw(0,outb3);
	putw(0,outb3);
	fwrite(matpal,768,1,outb3);

	in=fopen(ascname,"rt");
	if(!in)
	{
		printf("%s not found!\n",ascname);
		exit(3);
	}
	readasc();
	fclose(in);
	
	vInit();
	
	printon=0;

	for(a=1;a<conum;a++)
	{
		objectsort(co[a].o);
	}

	_asm mov ax,13h
	_asm int 10h
	vPalette();

	sprintf(tmpfname,"%s.0AA",scenename);
	outb2=fopen(tmpfname,"wb");
	strcpy(tmpname,inname);
	strcat(tmpname,".INF");
	in=fopen(tmpname,"rt");
	readinf();
	fclose(in);
	putw(0xffff,outb2);
	putw(0xffff,outb2);
	fclose(outb2);

	{ // init stuff for script
		int coi;
		long l,l2;
		struct s_cobject *cop;
		l=ftell(outb3);
		putw(conum,outb3);
		for(coi=1;coi<conum;coi++)
		{
			cop=co+coi;
			putw(cop->index,outb3);
		}
		fseek(outb3,0L,SEEK_SET);
		putc('F',outb3);
		putc('C',outb3);
		putc(0xfc,outb3);
		putc(0x1a,outb3);
		putl(l,outb3);
		putw(0,outb3);
		putw(0,outb3);
		putw(0,outb3);
		putc(0,outb3);
		if(cityflag==1) putc('C',outb3);
		else if(cityflag==2) putc('R',outb3);
		else putc(0,outb3);
	}

	if(!kbhit()) vuedone=1;
	if(kbhit()) getch();
	vDeinit();
	
	{
		if(vuedone) printf("Saving scenery: (%s.00O,%s.00M,%s.0$$)\n\n",scenename,scenename,scenename);
		else printf("Saving objects: (%s.00M)\n\n",scenename);
		for(a=1;a<conum;a++)
		{
			if(!co[a].duplicate)
			{
				printf("Object: %s",co[a].name);
				saveobject(co+a); // save object to disk
				printf("      File: %s    Size:%li\n",co[a].fname,co[a].size);
				printf("      Faces:%i=>%i  Vertices:%i=>%i  Normals:%i  Gouraudnormals:%i\n",
					co[a].f1,co[a].f2,
					co[a].v1,co[a].v2,
					co[a].n2,co[a].ng-co[a].n2);
			}
			totalfaces+=co[co[a].index].f1;
			totalvertices+=co[co[a].index].v1;
			totalnormals+=co[co[a].index].n2;
		}
	}
	print("Conversion finished (or escaped)!\n");
	printf("Total sum of stuff in animation:\n"
		"   faces: %i\n"
		"vertices: %i\n"
		" normals: %i\n",
		totalfaces,
		totalvertices,
		totalnormals);
	if(freport) fclose(freport);	
	fclose(fdeb);
	return(0);
}

void	doscene(int scene)
{
	int	a;
	char	tmpfname[64];
	sprintf(tmpfname,"%s-%i.vue",inname,scene);
	in=fopen(tmpfname,"rt");
	if(!in)
	{
		print("Scene %s not found.\n",tmpfname);
		return;
	}
	vuedone=1;
	putw(scene,outb2);
	putw(0,outb2);
	sprintf(tmpfname,"%s.0%c%c",inname,scene/10+'A',scene%10+'A');
	outb=fopen(tmpfname,"wb");
	readvue();
	fclose(in);
	fclose(outb);
}
	
void	vPalette(void)
{
	int	a,b;
	if(matpalenabled) 
	{
		vid_setpal(matpal);
		return;
	}
	for(a=0;a<256;a++)
	{
		palette[a*3+0]=(char)((a&15)*3+((a/16)&3)*0);
		palette[a*3+1]=(char)((a&15)*3+((a/16)&3)*2);
		palette[a*3+2]=(char)((a&15)*3+((a/16)&3)*4);
	}
	for(a=0;a<32;a++)
	{
		palette[16*3+a*3+0]=(char)(63-a*2);
		palette[16*3+a*3+1]=(char)(63-a*2);
		palette[16*3+a*3+2]=(char)(63-a*2);
	}
	palette[2]=32;
	palette[255*3+0]=63;
	palette[255*3+1]=32;
	palette[255*3+2]=16;
	vid_setpal(palette);
}

void	vInit(void)
{
	vid_init(1);
	vPalette();
}

void	vDeinit(void)
{
	vid_deinit();
}

void	vNext(void)
{
	vid_switch();
	vid_waitb();
	vid_clear();
}

void	vClear(void)
{
	vid_clear();
}

#pragma check_stack(off)
void _loadds cfill(int *fd)
{
	int	color,y,a,x,cnt;
	long	lx,la,rx,ra;
	char	*p,*pp;
	a=*fd++;
	if(a!=0) return; // not flat shading!
	color=*fd++;
	y=*fd++;
	for(;;)
	{
		a=*fd++;
		if(a<0) break;
		if(a)
		{
			lx=(unsigned)(*fd++);
			lx|=(long)(*fd++)<<16;
			la=(unsigned)(*fd++);
			la|=(long)(*fd++)<<16;
		}
		a=*fd++;
		if(a<0) break;
		if(a)
		{
			rx=(unsigned)(*fd++);
			rx|=(long)(*fd++)<<16;
			ra=(unsigned)(*fd++);
			ra|=(long)(*fd++)<<16;
		}
		a=*fd++;
		while(a--)
		{
			lx+=la;
			rx+=ra;
			if(lx<rx)
			{
				x=lx>>16;
				cnt=(rx>>16)-x;
			}
			else
			{
				x=rx>>16;
				cnt=(lx>>16)-x;
			}
			p=fill_color+x+y*320;
			pp=fill_object+x+y*320;
			#if 0
			p[0]=color;
			p[cnt-1]=color;
			#else
			while(cnt--) 
			{
				*p++=color;
				*pp++=fill_curobj;
			}
			#endif
			y++;
		}
	}
}
#pragma check_stack(on)

int	order[MAXOBJ],ordernum;
void	vDraw(void)
{
	rmatrix cam;
	rmatrix tmp;
	object 	*o;
	long	dis;
	int	a,b,c;
	if(!drawpass) 
	{
		vid_waitb();
		memcpy(vram,fill_color,64000);
		memset(fill_color,0,64000);
		memset(fill_object,0,64000);
		draw_setfillroutine(cfill);
	}
	else 
	{
		vNext();
		draw_setfillroutine(NULL);
	}
	memcpy(&cam,&camera,sizeof(rmatrix));
	{
		ordernum=0;
		for(a=1;a<conum;a++)
		{
			order[ordernum++]=a;
			o=co[a].o;
			memcpy(o->r,o->r0,sizeof(rmatrix));
			calc_applyrmatrix(o->r,&cam);
			b=o->pl[0][1]; // center vertex
			if(co[a].name[1]=='_') co[a].dist=100000000L;
			else co[a].dist=calc_singlez(b,o->v0,o->r);
		}

		for(a=0;a<ordernum;a++) 
		{
			dis=co[c=order[a]].dist;
			for(b=a-1;b>=0 && dis>co[order[b]].dist;b--)
				order[b+1]=order[b];
			order[b+1]=c;
		}
		
		if(!drawpass)
		{
			print("[Zsort order: (conum=%i)]\n",conum);
		}
		for(a=0;a<ordernum;a++)
		{
			int	x,y;
			o=co[order[a]].o;
			fill_curobj=order[a];
			vis_drawobject(o);
			co[order[a]].on=!o->vf;
			if(!drawpass)
			{
				print("[%s(%i): %li (v%i)]\n",co[order[a]].name,order[a],co[order[a]].dist,o->pl[0][1]);
				if(!o->vf)
				{
					x=o->pv[o->pl[0][1]].x;
					y=o->pv[o->pl[0][1]].y;
					x=x+y*320;
					vram[x]=255;
					vram[x-1]=255;
					vram[x+1]=255;
					vram[x-320]=255;
					vram[x+320]=255;
				}
			}
		}
		if(!drawpass)
		{
			print("\n");
		}
	}
}

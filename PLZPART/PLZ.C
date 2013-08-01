#include <stdio.h>
#include <math.h>
#include <conio.h>
#include <dos.h>
#include "tweak.h"

//#define DO_TABLES
//#define DPII (3.1415926535*2.0)

#define LINELEN 41
#define MAXY 280
#define YADD 0
#define XADD 0
#define SINMASK 4095
#define PLZSINI(p1,p2,p3,p4)  *ptr++ = (psini[x*32+lsini[y*2+p2]*16+p1] + psini[y*4+lsini[x*64+p4]*4+p3]) + (psini[x*32+16+lsini[y*2+p2]*16+p1] + psini[y*4+lsini[x*64+32+p4]*4+p3])*256;

extern int init_copper();
extern int close_copper();
extern int far frame_count;
extern int far cop_drop;
extern int far cop_plz;
extern int far cop_start;
extern char far * far cop_fadepal;
extern char far fadepal[768];
extern  far char * far cop_pal;
extern  far int do_pal;

extern int plzline(int y, int vseg);
extern int setplzparas(int c1, int c2, int c3, int c4);
extern int set_plzstart(int y);
extern char far psini[16384];
extern int far lsini4[8192];
extern int far lsini16[8192];

//int (* vmem)[LINELEN]=MK_FP(0x0a000,0);
//char	psini[16384]=
//#include "psini.pre"
//char	lsini[16384]=
//#include "lsini.pre"

char	ptau[256]=
#include "ptau.pre"

int	pals[6][768];
int	curpal=0;
int	timetable[10]={64*6*2-45,64*6*4-45,64*6*5-45,64*6*6-45,64*6*7+90,0};
int	ttptr=0;

int	l1=1000, l2=2000, l3=3000, l4=4000;
int	k1=3500, k2=2300, k3=3900, k4=3670;

int	il1=1000, il2=2000, il3=3000, il4=4000;
int	ik1=3500, ik2=2300, ik3=3900, ik4=3670;

int	inittable[10][8]={{1000,2000,3000,4000,3500,2300,3900,3670},
			  {1000,2000,4000,4000,1500,2300,3900,1670},
			  {3500,1000,3000,1000,3500,3300,2900,2670},
			  {1000,2000,3000,4000,3500,2300,3900,3670},
			  {1000,2000,3000,4000,3500,2300,3900,3670},
			  {1000,2000,3000,4000,3500,2300,3900,3670}};

plz(){
	register int x,y;
	int	*ptr;
	long	tim=0,count=0;
	int	ch=0,sync=2;

	while(dis_musplus()<0 && !dis_exit());
	dis_setmframe(0);

	init_plz();
	cop_drop=128;
	cop_fadepal=pals[curpal++];

	frame_count=0;
	while(!dis_exit())
		{
		tim+=frame_count; frame_count=0; count++;
		if(dis_getmframe()>timetable[ttptr])
			{
			memset(fadepal,0,768);
			cop_drop=1;
			cop_fadepal=pals[curpal++];
			ttptr++;
			il1=inittable[ttptr][0];
			il2=inittable[ttptr][1];
			il3=inittable[ttptr][2];
			il4=inittable[ttptr][3];
			ik1=inittable[ttptr][4];
			ik2=inittable[ttptr][5];
			ik3=inittable[ttptr][6];
			ik4=inittable[ttptr][7];
			}
		if(curpal==5 && cop_drop>64) break;

		asm	mov dx, 3c4h
		asm	mov ax, 0a02h
		asm	out dx, ax

		setplzparas(k1,k2,k3,k4);
		for(y=0;y<MAXY;y+=2)
			plzline(y,0x0a000+y*6+YADD*6);
		setplzparas(l1,l2,l3,l4);
		for(y=1;y<MAXY;y+=2)
			plzline(y,0x0a000+y*6+YADD*6);


		asm	mov dx, 3c4h
		asm	mov ax, 0502h
		asm	out dx, ax

		setplzparas(k1,k2,k3,k4);
		for(y=1;y<MAXY;y+=2)
			plzline(y,0x0a000+y*6+YADD*6);
		setplzparas(l1,l2,l3,l4);
		for(y=0;y<MAXY;y+=2)
			plzline(y,0x0a000+y*6+YADD*6);
		}
	cop_drop=0; frame_count=0; while(frame_count==0);
	set_plzstart(500);
	cop_plz=0;
	}

init_plz()
	{
	int	a,b,c,z;
	int	*pptr=pals;

#ifdef	DO_TABLES
	{
	FILE	*f1,*f2,*f3,*f4;
	f1=fopen("lsini4.inc","wb");
	f2=fopen("lsini16.inc","wb");
	f3=fopen("psini.inc","wb");
	f4=fopen("ptau.inc","wb");

	for(a=0;a<1024*16;a++)
		{
		if(a<1024*8)
			{
			lsini4[a]=(sin(a*DPII/4096)*55+sin(a*DPII/4096*5)*8+sin(a*DPII/4096*15)*2+64)*8;
			lsini16[a]=(sin(a*DPII/4096)*55+sin(a*DPII/4096*4)*5+sin(a*DPII/4096*17)*3+64)*16;
			}
		psini[a]=sin(a*DPII/4096)*55+sin(a*DPII/4096*6)*5+sin(a*DPII/4096*21)*4+64;
		if((a&15)==0)
			{
			if(a<1024*8)
				{
				fprintf(f1,"\ndw	%4d",lsini4[a]);
				fprintf(f2,"\ndw	%4d",lsini16[a]);
				}
			fprintf(f3,"\ndb	%4d",psini[a]);
			}
		else	{
			if(a<1024*8)
				{
				fprintf(f1,",%4d",lsini4[a]);
				fprintf(f2,",%4d",lsini16[a]);
				}
			fprintf(f3,",%4d",psini[a]);
			}
		}

	fprintf(f4,"{\n%d",ptau[0]=0);
	for(a=1;a<=128;a++)
		{
		fprintf(f4,",%3d",ptau[a]=cos(a*DPII/128+3.1415926535)*31+32);
		if(!(a&15)) fputc('\n',f4);
		}
	fputc('}',f4); fputc(';',f4);

	fclose(f1); fclose(f2); fclose(f3); fclose(f4);
	}
#endif
	tw_opengraph2();
	cop_start=96*(682-400);
	set_plzstart(60);
	init_copper();
	for(a=0;a<256;a++) tw_setrgbpalette(a,63,63,63);

//	RGB
	pptr=&pals[0][3];
	for(a=1;a<64;a++) *pptr++=ptau[a   ],*pptr++=ptau[0   ],*pptr++=ptau[0   ];
	for(a=0;a<64;a++) *pptr++=ptau[63-a],*pptr++=ptau[0   ],*pptr++=ptau[0   ];
	for(a=0;a<64;a++) *pptr++=ptau[0   ],*pptr++=ptau[0   ],*pptr++=ptau[a];
	for(a=0;a<64;a++) *pptr++=ptau[a   ],*pptr++=ptau[0   ],*pptr++=ptau[63-a];

//	RB-black
	pptr=&pals[1][3];
	for(a=1;a<64;a++) *pptr++=ptau[a   ],*pptr++=ptau[0   ],*pptr++=ptau[0   ];
	for(a=0;a<64;a++) *pptr++=ptau[63-a],*pptr++=ptau[0   ],*pptr++=ptau[a   ];
	for(a=0;a<64;a++) *pptr++=ptau[0   ],*pptr++=ptau[a   ],*pptr++=ptau[63-a];
	for(a=0;a<64;a++) *pptr++=ptau[a   ],*pptr++=ptau[63  ],*pptr++=ptau[a   ];

//	RB-white
	pptr=&pals[3][3];
	for(a=1;a<64;a++) *pptr++=ptau[a   ],*pptr++=ptau[0   ],*pptr++=ptau[0   ];
	for(a=0;a<64;a++) *pptr++=ptau[63  ],*pptr++=ptau[a   ],*pptr++=ptau[a   ];
	for(a=0;a<64;a++) *pptr++=ptau[63-a],*pptr++=ptau[63-a],*pptr++=ptau[63  ];
	for(a=0;a<64;a++) *pptr++=ptau[0   ],*pptr++=ptau[0   ],*pptr++=ptau[63  ];

//	white
	pptr=&pals[2][3];
	for(a=1;a<64;a++) *pptr++=ptau[0   ]/2,*pptr++=ptau[0   ]/2,*pptr++=ptau[0   ]/2;
	for(a=0;a<64;a++) *pptr++=ptau[a   ]/2,*pptr++=ptau[a   ]/2,*pptr++=ptau[a   ]/2;
	for(a=0;a<64;a++) *pptr++=ptau[63-a]/2,*pptr++=ptau[63-a]/2,*pptr++=ptau[63-a]/2;
	for(a=0;a<64;a++) *pptr++=ptau[0   ]/2,*pptr++=ptau[0   ]/2,*pptr++=ptau[0   ]/2;

//	white II
	pptr=&pals[4][3];
	for(a=1;a<75;a++) *pptr++=ptau[63-a*64/75],*pptr++=ptau[63-a*64/75],*pptr++=ptau[63-a*64/75];
	for(a=0;a<106;a++)*pptr++=0,*pptr++=0,*pptr++=0;
	for(a=0;a<75;a++) *pptr++=ptau[a*64/75]*8/10,*pptr++=ptau[a*64/75]*9/10,*pptr++=ptau[a*64/75];

	pptr=pals;
	for(a=0;a<768;a++,pptr++) *pptr=(*pptr-63)*2;
	for(a=768;a<768*5;a++,pptr++) *pptr*=8;
	}


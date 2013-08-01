#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <math.h>
#include <io.h>
#include <fcntl.h>
#include "tweak.h"

#define SX sinit[kx]
#define SY sinit[ky]
#define SZ sinit[kz]
#define CX kosinit[kx]
#define CY kosinit[ky]
#define CZ kosinit[kz]

extern	far char (* far vmem)[160];
extern char far pal[768];

extern clear();
extern init();
extern shadepal(char far *fpal, char far *ppal, int shade);
extern	init_copper();
extern	close_copper();
extern 	int far cop_rotatev;
extern	far int frame_count;
extern  far char * far cop_pal;
extern  far int do_pal;
extern  far int cop_start;
extern  far int cop_scrl;

extern int do_poly();
extern int getspl(int where);
extern int far sinit[1024];
extern int far kosinit[1024];

char 	far fpal[768];

struct	object {
	char	name[100];

	int	pnts;
	struct	points_3d {
		int	x;
		int	y;
		int	z;
		int	xx;
		int	yy;
		int	zz;
		int	xxx;
		int	yyy;
		} point[256];

	int	faces;
	struct	polygon {
		int	p1;
		int	p2;
		int	p3;
		int	p4;
		int	p5;
		int	p6;
		int	n;
		int	color;
		} pg[256];

	int	lines;
	struct	lines {
		int	p1;
		int	p2;
		int	n;
		int	col;
		} lin[256];
	} object={
		"Cube",
		8,				// points
		{
		{125,125,125},
		{125,-125,125},
		{-125,-125,125},
		{-125,125,125},
		{125,125,-125},
		{125,-125,-125},
		{-125,-125,-125},
		{-125,125,-125},
		},
		6,			// faces
		{
		{1,2,3,0,0,0,0,0},
		{7,6,5,4,0,0,0,0},
		{0,4,5,1,0,0,0,1},
		{1,5,6,2,0,0,0,2},
		{2,6,7,3,0,0,0,1},
		{3,7,4,0,0,0,0,2}}
		};

struct 	polygons_to_draw {
		int	p;
		int	dis;
		} ptodraw[256];
int	polys=0;

int	light_src[6]={0};
int	lls[6]={0};

int	cxx, cxy, cxz, cyx, cyy, cyz, czx, czy, czz;
int  	kx=0,ky=0,kz=0,dis=320,tx=0,ty=-50;
int	ls_kx=0,ls_ky=0,ls_kz=0,ls_x=0,ls_y=0,ls_z=128;
int 	page=0;
int	frames=0;

vect()
	{
	int	c=0,a;

	tw_opengraph();

	while(dis_musplus()<13 && !dis_exit()); frame_count=0;
	while(!dis_exit())
		{
		a=dis_musplus();
		if(a>=-4 && a<0) break;
		swappage();
		while(frame_count<1); frames+=frame_count; frame_count=0;
		cop_pal=fpal; do_pal=1;

		calculate(1);
		draw(15);
		clear();
		}
//	tw_closegraph();
	}

calculate(int k)
	{
	int	a;
	static int px=0,py=256;

	getspl(4*256+frames*4);
        kx=kx&1023;
	ky=ky&1023;
	kz=kz&1023;
	ls_kx=ls_kx&1023;
	ls_ky=ls_ky&1023;

	ls_y=kosinit[ls_kx]>>8;
	ls_x=(sinit[ls_kx]>>8)*(sinit[ls_ky]>>8)>>7;
	ls_z=(sinit[ls_kx]>>8)*(kosinit[ls_ky]>>8)>>7;

	count_const();
	rotate();
	sort_faces();
	}

count_const()
	{
	//matrix equations:
	//X Y Z -> nX
	//X Y Z -> nY
	//X Y Z -> nZ
	//
	// 0=Ycos*Zcos		 2=Ycos*Zsin		 4=-Ysin
	// 6=Xsin*Zcos*Ysin	 8=Xsin*Ysin*Zsin	10=Ycos*Xsin
	//   -Xcos*Zsin		   +Xcos*Zcos
	//12=Xcos*Zcos*Ysin	14=Xcos*Ysin*Zsin	16=Ycos*Xcos
	//   +Xsin*Zsin		   -Xsin*Zcos

	cxx=(long)CY*(long)CZ>>15+7;
	cxy=(long)CY*(long)SZ>>15+7;
	cxz=-(long)SY>>7;

	cyx=((long)SX*(long)CZ+16384L>>15)*(long)SY - (long)CX*(long)SZ>>15+7;
	cyy=((long)SX*(long)SY+16384L>>15)*(long)SZ + (long)CX*(long)CZ>>15+7;
	cyz=(long)CY*(long)SX>>15+7;

	czx=((long)CX*(long)CZ+16384L>>15)*(long)SY + (long)SX*(long)SZ>>15+7;
	czy=((long)CX*(long)SY+16384L>>15)*(long)SZ - (long)SX*(long)CZ>>15+7;
	czz=(long)CY*(long)CX>>15+7;

	}

rotate()
	{
	int	a,b,x,y,z,xx,yy,zz;

	for(a=0;a<object.pnts;a++)
		{
		x=object.point[a].x; y=object.point[a].y; z=object.point[a].z;

		object.point[a].xx=xx=((x*cxx>>1) + (y*cxy>>1) + (z*cxz>>1)>>7)+tx;
		object.point[a].yy=yy=((x*cyx>>1) + (y*cyy>>1) + (z*cyz>>1)>>7)+ty;
		object.point[a].zz=zz=((x*czx>>1) + (y*czy>>1) + (z*czz>>1)>>7)+dis;

/*
		1000,1000,1000

		x*256, y*213, / (
*/

		object.point[a].xxx=(xx*256L)/zz+160+160;
		object.point[a].yyy=(yy*142L)/zz+66;
		}
	}

sort_faces()
	{
	int 	a=0,b,c,x,y,z,p=0;
	long	ax,ay,az,bx,by,bz,kx,ky,kz,nx,ny,nz,s,l;

	while(a<object.faces)
		{
		x=object.point[object.pg[a].p1].xx;
		y=object.point[object.pg[a].p1].yy;
		z=object.point[object.pg[a].p1].zz;

		ax=object.point[object.pg[a].p2].xx-x;
		ay=object.point[object.pg[a].p2].yy-y;
		az=object.point[object.pg[a].p2].zz-z;

		bx=object.point[object.pg[a].p3].xx-x;
		by=object.point[object.pg[a].p3].yy-y;
		bz=object.point[object.pg[a].p3].zz-z;

		nx = ay * bz - az * by;
		ny = az * bx - ax * bz;
		nz = ax * by - ay * bx;			// normal

		kx = -x;
		ky = -y;
		kz = -z;				// view_vector

		s = kx*nx + ky*ny + kz*nz;		// skalaaritulo

		if(s>0)  { a++; continue; }

		s=(ls_x*nx+ls_y*ny+ls_z*nz)/250000+32;
		light_src[p]=s;
		c=object.pg[a].color;
		if(lls[p]!=light_src[p])
			{
			shadepal(&fpal[c*64*3],&pal[c*64*3], light_src[p]);
			lls[p]=light_src[p];
			}

		ptodraw[p++].p=a++;
		}
	polys=p;
	}


draw()
	{
	int 	a=0,b,c,f,x,y,z;
	long	ax,ay,az,bx,by,bz,kx,ky,kz,nx,ny,nz,s;

	for(a=0;a<polys;a++)
		{
		c=object.pg[ptodraw[a].p].color;
		do_poly(object.point[object.pg[ptodraw[a].p].p1].xxx+(page&1)*2, object.point[object.pg[ptodraw[a].p].p1].yyy,
			object.point[object.pg[ptodraw[a].p].p2].xxx+(page&1)*2, object.point[object.pg[ptodraw[a].p].p2].yyy,
			object.point[object.pg[ptodraw[a].p].p3].xxx+(page&1)*2, object.point[object.pg[ptodraw[a].p].p3].yyy,
			object.point[object.pg[ptodraw[a].p].p4].xxx+(page&1)*2, object.point[object.pg[ptodraw[a].p].p4].yyy,
			c, frames&63);
		}
	}

swappage()
	{
	page=(page+1)%6;
	if(page==0){
		vmem=MK_FP(0x0a000,0x0000);
		cop_start=0xaa00+40;
		cop_scrl=4;
		}
	else if(page==1){
		vmem=MK_FP(0x0a000,0x5500);
		cop_start=0x0000+40;
		cop_scrl=0;
		}
	else if(page==2){
		vmem=MK_FP(0x0a000,0xaa00);
		cop_start=0x5500+40;
		cop_scrl=4;
		}
	else if(page==3){
		vmem=MK_FP(0x0a000,0x0000);
		cop_start=0xaa00+40;
		cop_scrl=0;
		}
	else if(page==4){
		vmem=MK_FP(0x0a000,0x5500);
		cop_start=0x0000+40;
		cop_scrl=4;
		}
	else if(page==5){
		vmem=MK_FP(0x0a000,0xaa00);
		cop_start=0x5500+40;
		cop_scrl=0;
		}
	}


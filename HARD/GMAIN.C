#include <stdio.h>
#include <math.h>
#include <conio.h>
#include <dos.h>
#include <stdlib.h>

char far *vram=(char far *)0xa0000000L;

extern char lightshift;

extern int _ndebug1;

extern char asmtestmode;

extern int csetmatrix(int *,long,long,long);
extern int crotlist(long *,long *);
extern int cclipedges(int *,int *,long *); // modifies given point list
extern int cprojlist(long *,long *);
extern int cdrawpolylist(int *);
extern int cmatrix_yxz(int,int,int,int *);
extern int cpolylist(int *polylist,int *polys,int *edges,long *points3);
extern int ceasypolylist(int *polylist,int *polys,long *points3);

long	cubepoints[]={8,
-666,-666,-666,
666,-666,-666,
666,666,-666,
-666,666,-666,
-666,-666,666,
666,-666,666,
666,666,666,
-666,666,666
};
int	cubeepolys[]={
4,0x4002,0,1,2,3,
4,0x4004,4,5,6,7,
4,0x4006,0,1,5,4,
4,0x4008,1,2,6,5,
4,0x4003,2,3,7,6,
4,0x4005,3,0,4,7,
0};

long	points[64]={14,
-1000,-1000,-1000,
1000,-1000,-1000,
1000,1000,-1000,
-1000,1000,-1000,
-1000,-1000,1000,
1000,-1000,1000,
1000,1000,1000,
-1000,1000,1000,
0,0,-1700,
0,0,1700,
1700,0,0,
-1700,0,0,
0,1700,0,
0,-1700,0};
long	pointsb[64]={14,
-600,-600,-600,
600,-600,-600,
600,600,-600,
-600,600,-600,
-600,-600,600,
600,-600,600,
600,600,600,
-600,600,600,
0,0,-900,
0,0,900,
900,0,0,
-900,0,0,
0,900,0,
0,-900,0};
long	points2[256];
int	points3[256*2];
int	edges[64]={12,0,
0,1,0,0,
1,2,0,0,
2,3,0,0,
3,0,0,0,
4,5,0,0, // 4
5,6,0,0,
6,7,0,0,
7,4,0,0,
0,4,0,0, // 8
1,5,0,0,
2,6,0,0,
3,7,0,0};
int	edges2[64];
int	polys[64]={
4,0x4010,0,1,2,3,
4,0x4008,0,8,4,9,
4,0x4010,1,9,5,10,
4,0x4020,2,10,6,11,
4,0x4040,3,11,7,12,
4,0x4080,4,5,6,7,
0};
int	epolys[]={
3,0x4008,0,1,8,
3,0x4010,1,2,8,
3,0x4018,2,3,8,
3,0x4020,3,0,8,

3,0x4028,2,1,10,
3,0x4030,1,5,10,
3,0x4038,5,6,10,
3,0x4040,6,2,10,

3,0x4048,2,6,12,
3,0x4050,6,7,12,
3,0x4058,7,3,12,
3,0x4060,3,2,12,

3,0x4068,0,3,11,
3,0x4070,3,7,11,
3,0x4078,7,4,11,
3,0x4080,4,0,11,

3,0x4088,5,1,13,
3,0x4090,1,0,13,
3,0x4098,0,4,13,
3,0x40a0,4,5,13,

3,0x40a8,5,4,9,
3,0x40b0,4,7,9,
3,0x40b8,7,6,9,
3,0x40c0,6,5,9,

0};
int	epolys1[]={
3,0x4008,0,1,8,
3,0x4010,1,2,8,
3,0x4018,2,3,8,
3,0x4020,3,0,8,
/*
3,0x4028,2,1,10,
3,0x4030,1,5,10,
3,0x4038,5,6,10,
3,0x4040,6,2,10,
*/
/*3,0x4048,2,6,12,*/
/*3,0x4050,6,7,12,*/
3,0x4058,7,3,12,
3,0x4060,3,2,12,

3,0x4068,0,3,11,
3,0x4070,3,7,11,
3,0x4078,7,4,11,
3,0x4080,4,0,11,

/*3,0x4088,5,1,13,*/
3,0x4090,1,0,13,
3,0x4098,0,4,13,
/*3,0x40a0,4,5,13,*/
/*
3,0x40a8,5,4,9,
3,0x40b0,4,7,9,
3,0x40b8,7,6,9,
3,0x40c0,6,5,9,
*/
0};
int	epolys2[]={
/*
3,0x4008,0,1,8,
3,0x4010,1,2,8,
3,0x4018,2,3,8,
3,0x4020,3,0,8,
*/
3,0x4028,2,1,10,
3,0x4030,1,5,10,
3,0x4038,5,6,10,
3,0x4040,6,2,10,

3,0x4048,2,6,12,
3,0x4050,6,7,12,
/*3,0x4058,7,3,12,*/
/*3,0x4060,3,2,12,*/
/*
3,0x4068,0,3,11,
3,0x4070,3,7,11,
3,0x4078,7,4,11,
3,0x4080,4,0,11,
*/
3,0x4088,5,1,13,
/*3,0x4090,1,0,13,*/
/*3,0x4098,0,4,13,*/
3,0x40a0,4,5,13,

3,0x40a8,5,4,9,
3,0x40b0,4,7,9,
3,0x40b8,7,6,9,
3,0x40c0,6,5,9,

0};
int	epolysb[]={
3,0x4004,0,1,8,
3,0x4002,1,2,8,
3,0x4004,2,3,8,
3,0x4002,3,0,8,

3,0x4004,2,1,10,
3,0x4002,1,5,10,
3,0x4004,5,6,10,
3,0x4002,6,2,10,

3,0x4004,2,6,12,
3,0x4002,6,7,12,
3,0x4004,7,3,12,
3,0x4002,3,2,12,

3,0x4004,0,3,11,
3,0x4002,3,7,11,
3,0x4004,7,4,11,
3,0x4002,4,0,11,

3,0x4004,5,1,13,
3,0x4002,1,0,13,
3,0x4004,0,4,13,
3,0x4002,4,5,13,

3,0x4004,5,4,9,
3,0x4002,4,7,9,
3,0x4004,7,6,9,
3,0x4002,6,5,9,

0};
int	polylist[256];
int	matrix[9];

int	testlist[]={
3,16, 190,100, 90,50, 110,150,
3,20, 10,100, 90,50, 99,150,
0};
	
main()
{
	int	a,b,c,x,y,rx,ry,rz,n=8,p1,p2,r,g,zpos=7000;
	testasm();
	//initnewgroup();
	outp(0x3c8,0);
	for(a=0;a<64;a++)
	{
		r=g=b=0;
		c=a&31;
		r=16+c;
		g=c*2;
		b=c;
		if(a&0x20) r+=20; g+=20; b+=20;
		/*
		if(a&0x01) { r+=40; }
		if(a&0x02) { g+=40; r+=40; b+=40; }
		if(a&0x04) { r+=0; }
		if(a&0x08) { g+=10; r+=10; b+=10; }
		if(a&0x10) { b+=40; }
		if(a&0x20) { g+=40; r+=40; b+=40; }
		if(a&0x40) { b+=0; }
		if(a&0x80) { g+=10; r+=10; b+=10; }
		r=r*4/6;
		g=g*4/6;
		b=b*4/6;
		*/
		if(r>63) r=63;
		if(g>63) g=63;
		if(b>63) b=63;
		r=g=b=0;
		outp(0x3c9,r);
		outp(0x3c9,g);
		outp(0x3c9,b);
	}
	for(a=0;a<256;a++) vram[a]=a;
	lightshift=8;
	rx=ry=rz=0;
	for(;;)
	{
		if(kbhit())
		{
			a=getch();
			if(a==27) break;
			if(a=='l') lightshift=2;
			if(a=='+') zpos+=200;
			if(a=='-') zpos-=200;
			if(a=='<') lightshift--;
			if(a=='>') lightshift++;
			if(a=='t') asmtestmode=1;
			if(a=='c') { if(zpos==3200) { lightshift=8; zpos=7000; } else { zpos=3200; lightshift=11; } }
			if(a=='8') ry-=16;
			if(a=='2') ry+=16;
			if(a=='4') rz-=16;
			if(a=='6') rz+=16;
		}
		asm();
		setborder(1);
		//rx+=16; ry+=9; rz+=7;
		rx+=32; ry+=8;
		rx%=3600; ry%=3600; rz%=3600;
		//if(rz==0) printf(".");
		/*
		a=0;
		while(polylist[a])
		{
			a+=polylist[a]*2+2;
		}
		polylist[a++]=4;
		polylist[a++]=19;
		polylist[a++]=160-60; polylist[a++]=100-40;
		polylist[a++]=160+60; polylist[a++]=100-40;
		polylist[a++]=160+60; polylist[a++]=100+40;
		polylist[a++]=160-60; polylist[a++]=100+40;
		polylist[a++]=0;
		*/
		cglenzinit();

		cmatrix_yxz(rx,ry,rz,matrix);
		csetmatrix(matrix,0,0,zpos);
		points2[0]=0; crotlist(points2,points);
		//cclipedges(edges2,edges,points2);
		points3[0]=0; cprojlist((long *)points3,points2);
		//cpolylist(polylist,polys,edges,points3);
		ceasypolylist(polylist,epolys,points3);

		cglenzpolylist(polylist);
/*
		cmatrix_yxz(rx,-450,900+ry,matrix);
		csetmatrix(matrix,0,0,zpos);
		points2[0]=0; crotlist(points2,points);
		//cclipedges(edges2,edges,points2);
		points3[0]=0; cprojlist((long *)points3,points2);
		//cpolylist(polylist,polys,edges,points3);
		ceasypolylist(polylist,epolys1,points3);

		cglenzpolylist(polylist);

		cmatrix_yxz(-rx,-450,900+ry,matrix);
		csetmatrix(matrix,0,0,zpos);
		points2[0]=0; crotlist(points2,points);
		//cclipedges(edges2,edges,points2);
		points3[0]=0; cprojlist((long *)points3,points2);
		//cpolylist(polylist,polys,edges,points3);
		ceasypolylist(polylist,epolys2,points3);

		cglenzpolylist(polylist);
*/

		cmatrix_yxz(-rx,-ry,-rz,matrix);
		csetmatrix(matrix,0,0,zpos);
		points2[0]=0; crotlist(points2,pointsb);
		//cclipedges(edges2,edges,points2);
		points3[0]=0; cprojlist((long *)points3,points2);
		//cpolylist(polylist,polys,edges,points3);
		ceasypolylist(polylist,epolysb,points3);
		
		cglenzpolylist(polylist);

//		//cmatrix_yxz(rx*2,-ry,rz*2,matrix);
//		//csetmatrix(matrix,0,0,zpos);
//		//points2[0]=0; crotlist(points2,cubepoints);
//		//points3[0]=0; cprojlist((long *)points3,points2);
//		//ceasypolylist(polylist,cubeepolys,points3);
		
		cglenzdone();
		//cdrawpolylist(polylist);
		//cdrawpolylist(testlist);
		//setborder(3);
		//drawnewgroup();
		#if 0
		n=edges[0];
		for(a=2,b=0;b<n;b++)
		{
			p1=edges[a++];
			p2=edges[a++];
			a+=2;
			testline(points3[p1*6+2],points3[p1*6+3],points3[p2*6+2],points3[p2*6+3],63);
		}
		#endif
	}
	_asm mov ax,3
	_asm int 10h
	#if 0
	for(a=0;a<100;)
	{
		b=polylist[a++];
		c=polylist[a++];
		if(!b) break;
		printf("s:%i c:%i v:",b,c);
		for(x=0;x<b;x++)
		{
			printf("%i,%i ",polylist[a+0],polylist[a+1]);
			a+=2;
		}
		printf("\n");
	}
	printf("points2: %Fp\n",points2);
	#endif
	printf("%i,%i,%i  Z:%i LS:%i \n",rx,ry,rz,zpos,lightshift);
}
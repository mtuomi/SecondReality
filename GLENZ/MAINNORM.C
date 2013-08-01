#include <stdio.h>

extern int csetmatrix(int *,long,long,long);
extern int crotlist(long *,long *);
extern int cclipedges(int *,int *,long *); // modifies given point list
extern int cprojlist(long *,long *);
extern int cdrawpolylist(int *);
extern int cmatrix_yxz(int,int,int,int *);
extern int cpolylist(int *polylist,int *polys,int *edges,long *points3);

long	points[32]={8,
-1000,-1000,-1000,
1000,-1000,-1000,
1000,1000,-1000,
-1000,1000,-1000,
-1000,-1000,1000,
1000,-1000,1000,
1000,1000,1000,
-1000,1000,1000};
long	points2[64];
int	points3[64*2];
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
4,0x0010,0,1,2,3,
4,0x0018,0,8,4,9,
4,0x0020,1,9,5,10,
4,0x0028,2,10,6,11,
4,0x0030,3,11,7,12,
4,0x8038,4,5,6,7,
0};
int	polylist[256];
int	matrix[9];

int	testlist[]={
3,16, 190,100, 90,50, 110,150,
3,20, 10,100, 90,50, 99,150,
0};
	
main()
{
	int	a,b,c,x,y,rx,ry,rz,n=8,p1,p2;
	printf("Starting");
	testasm();
	initnewgroup();
	initnewgroup();
	while(!kbhit())
	{
		initnewgroup();
		rz+=5; ry+=7; rz+=6;
		rx%=3600; ry%=3600; rz%=3600;
		cmatrix_yxz(rx,ry,rz,matrix);
		csetmatrix(matrix,0,0,4000);
		points2[0]=0; crotlist(points2,points);
		//cclipedges(edges2,edges,points2);
		points3[0]=0; cprojlist(points3,points2);
		cpolylist(polylist,polys,edges,points3);
		asm();
		cdrawpolylist(polylist);
		//cdrawpolylist(testlist);
		setborder(40);
		drawnewgroup();
		setborder(10);
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
}

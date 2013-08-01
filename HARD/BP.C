/* parts */

char	pal1[768];
int	pal1colp=0;

void	part1init(void)
{
	int	a,b,c;
	if(!pal1colp) memcpy(pal,defpal,768);
	for(c=(16)*3,a=0;a<64;a++) 
	{
		b=64-a; b=b*b/64;
		if(b<0) b=0; else if(b>63) b=63;
		pal1[a*3+c+0]=b;
		pal1[a*3+c+64*3+0]=b*2/3;
		pal1[a*3+c+128*3+0]=b*1/3;
		b=64-a; b=b*b/64; 
		if(b<0) b=0; else if(b>63) b=63;
		pal1[a*3+c+1]=b;
		pal1[a*3+c+64*3+1]=b*2/3;
		pal1[a*3+c+128*3+1]=b*1/3;
		b=64-a;
		b=b*4/6;
		if(b<0) b=0; else if(b>63) b=63;
		pal1[a*3+c+2]=b+20;
		pal1[a*3+c+64*3+2]=b*2/3+20;
		pal1[a*3+c+128*3+2]=b*1/3+20;
	}
	waitb();
	inp(0x3da);
	dotsp=0;
	pal1colp+=4;
}

void	part1(void)
{
	int	a,b;
	b=count-6; if(b<0) b=0; else if(b>63) b=63;
	b=b*3+16*3;
	pal[-3*4+pal1colp*3+1*3+0]=pal1[0*3+b+0];
	pal[-3*4+pal1colp*3+1*3+1]=pal1[0*3+b+1];
	pal[-3*4+pal1colp*3+1*3+2]=pal1[0*3+b+2];
	pal[-3*4+pal1colp*3+2*3+0]=pal1[64*3+b+0];
	pal[-3*4+pal1colp*3+2*3+1]=pal1[64*3+b+1];
	pal[-3*4+pal1colp*3+2*3+2]=pal1[64*3+b+2];
	pal[-3*4+pal1colp*3+3*3+0]=pal1[128*3+b+0];
	pal[-3*4+pal1colp*3+3*3+1]=pal1[128*3+b+1];
	pal[-3*4+pal1colp*3+3*3+2]=pal1[128*3+b+2];
	b=(64-(count>64?64:count))*3+16*3;
	pal[pal1colp*3+1*3+0]=pal1[0*3+b+0];
	pal[pal1colp*3+1*3+1]=pal1[0*3+b+1];
	pal[pal1colp*3+1*3+2]=pal1[0*3+b+2];
	pal[pal1colp*3+2*3+0]=pal1[64*3+b+0];
	pal[pal1colp*3+2*3+1]=pal1[64*3+b+1];
	pal[pal1colp*3+2*3+2]=pal1[64*3+b+2];
	pal[pal1colp*3+3*3+0]=pal1[128*3+b+0];
	pal[pal1colp*3+3*3+1]=pal1[128*3+b+1];
	pal[pal1colp*3+3*3+2]=pal1[128*3+b+2];
	loadpal(pal,32*3);
	if(count<8) for(a=0;a<2048/8;a++)
	{
		dotsp+=4;
		switch(dotspnt[dotsp])
		{
		case 1 : doit1(dotspnt[dotsp+2],dotspnt[dotsp+3],3+pal1colp); break;
		case 2 : doit1(dotspnt[dotsp+2],dotspnt[dotsp+3],2+pal1colp); break;
		case 3 : doit1(dotspnt[dotsp+2],dotspnt[dotsp+3],1+pal1colp); break;
		default : dotsp=0; break;
		}
	}
	else if(count>=72) NEXTMODE;
}

/*################################################################*/

extern int csetmatrix(int *,long,long,long);
extern int crotlist(long *,long *);
extern int cclipedges(int *,int *,long *); // modifies given point list
extern int cprojlist(long *,long *);
extern int cdrawpolylist(int *);
extern int cmatrix_yxz(int,int,int,int *);
extern int cpolylist(int *polylist,int *polys,int *edges,long *points3);
extern int ceasypolylist(int *polylist,int *polys,long *points3);

long	cubepoints[]={8,
-510,-510,-510,
410,-510,-510,
410,410,-510,
-510,410,-510,
-510,-510,410,
410,-510,410,
410,410,410,
-510,410,410
};
int	cubepolys[]={
4,0x4001,3,2,1,0,
4,0x4002,4,5,6,7,
4,0x4004,0,1,5,4,
4,0x4008,1,2,6,5,
4,0x4010,2,3,7,6,
4,0x4020,3,0,4,7,
0};
int	cubepolyss[]={
4,0x4040,3+8,2+8,1+8,0+8,
4,0x4040,4+8,5+8,6+8,7+8,
4,0x4040,0+8,1+8,5+8,4+8,
4,0x4040,1+8,2+8,6+8,5+8,
4,0x4040,2+8,3+8,7+8,6+8,
4,0x4040,3+8,0+8,4+8,7+8,
0};

long	gridpoints[1010*3];

int	polylist[256];
long	points2[1010*3];
int	points3[1010*8];

int	rx=0,ry=0,rz=0,zp=5600;

int	matrix[9];

extern char depthcol[];

void	part2init(void)
{
	int	a,r,g,b,x,y,z,d;
	
	for(a=0;a<3100;a++)
	{
		if(a<1000 || b>=3000) b=127;
		else b=127-(a-1000)/32;
		depthcol[a]=b;
	}

	memcpy(pal,defpal,768);
	pal[1*3+0]=63;
	pal[1*3+1]=63;
	pal[1*3+2]=63;
	pal[250*3+0]=0;
	pal[250*3+1]=0;
	pal[250*3+2]=40;
	d=64;
	for(a=0;a<64;a++)
	{
		b=a+d; if(b>63) b=63;
		pal[64*3+a*3+0]=a;
		b=a+20+d; if(b>63) b=63;
		pal[64*3+a*3+1]=b;
		b=a+40+d; if(b>63) b=63;
		pal[64*3+a*3+2]=b;
		if(d>0) d-=8;
	}
	loadpal(pal,768);
	memcpy(pal2,pal,768);

	a=1;
	for(x=0;x<10;x++) for(y=0;y<10;y++) for(z=0;z<10;z++)
		if(!x || !y || !z || x==9 || y==9 || z==9)
	{
		gridpoints[a++]=(x-5)*100;
		gridpoints[a++]=(y-5)*100;
		gridpoints[a++]=(z-5)*100;
	}
	gridpoints[0]=(a-1)/3;
	
	zp=3500;
}

void	part2(void)
{
	int	a,x,y;
	if(count>=500) NEXTMODE;
	if(count>500-16)
	{
		a=(500-16)-count;
		fadepal(pal,pal2,a*16);
	}

	setborder(0);
	cmatrix_yxz(rx,ry,rz,matrix);
	csetmatrix(matrix,0,0,zp);
	setborder(1);
	points3[0]=0; crotprojlist(points3,gridpoints);
	setborder(250);
	do3dots(points3,vram);
	#if 0
	if(count==2)
	{
		FILE	*f1;
		f1=fopen("_grid1.tmp","wb");
		for(a=0;a<points3[0];a++) if(points3[2+a*8+4]<=3000)
		{
			x=points3[2+a*8+0];
			y=points3[2+a*8+1];
			putw(x,f1);
			putw(y,f1);
		}
		fclose(f1);
	}
	#endif
	#if 0
	for(a=0;a<1000;a++)
	{
		x=points3[2+a*8+0];
		y=points3[2+a*8+1];
		vram[x+y*320]++;
	}
	#endif
	if(count>1) 
	{ 
		rx+=9; ry+=10; rz+=11; 
		if(zp>2000) zp-=10;
	}
	setborder(255);
}

/*################################################################*/


void	part3init(void)
{
	int	a,r,g,b;
	memcpy(pal,defpal,768);
	for(a=1;a<128;a++)
	{
		r=g=0; b=20;
		if(a&0x1)  { g+=20; r+=20; }
		if(a&0x2)  { g+=22; r+=22; }
		if(a&0x4)  { g+=24; r+=24; }
		if(a&0x8)  { g+=26; r+=26; }
		if(a&0x10) { g+=28; r+=28; }
		if(a&0x20) { g+=30; r+=30; }
		if(a&0x40) { b=15; }
		if(r>63) r=63;
		if(g>63) g=63;
		if(b>63) b=63;
		pal2[a*3+0]=r;
		pal2[a*3+1]=g;
		pal2[a*3+2]=b;
	}
	for(a=2;a<254;a++) 
	{
		pal[a*3+0]=0;
		pal[a*3+1]=0;
		pal[a*3+2]=20;
	}
	loadpal(pal,768);
	/*
	rx=0;
	ry=0;
	rz=0;
	zp=5600;
	*/
}

void	part3(void)
{
	int	a,b,c,x,y;
	int	liy=80,lix=190;
	static	int ct2=0;
	long	lz,z;
	if(count>=800) NEXTMODE;
	if(dis_muscode(0xff)==0xff) NEXTMODE;

	ct2+=4;
	lix=sin1024[ct2&1023]/2+160;
	liy=sin1024[(ct2+256)&1023]/2+64;
	
	setborder(0);
	cglenzinit();
	cmatrix_yxz(rx,ry,rz,matrix);
	csetmatrix(matrix,0,0,zp);
	points2[0]=0; crotlist(points2,cubepoints);
	points3[0]=0; cprojlist((long *)points3,points2);
	points3[0]+=8;
	for(a=0;a<8*8;a+=8)
	{
		x=points3[2+a+0];
		y=points3[2+a+1];
		z=points3[2+a+4];
		z+=2000;
		lz=3000+2000;
		x=(long)(x-lix)*lz/(long)z+lix;
		y=(long)(y-liy)*lz/(long)z+liy;
		points3[8*8+2+a+0]=x;
		points3[8*8+2+a+1]=y;
		points3[8*8+2+a+4]=3000;
	}
	ceasymode(0);
	cclipeasypolylist(polylist,cubepolyss,(long *)points3);
	cglenzpolylist(polylist);
	ceasymode(1);
	cclipeasypolylist(polylist,cubepolys,(long *)points3);
	cglenzpolylist(polylist);
	cglenzdone();
	setborder(255);
	rx+=9; ry+=10; rz+=11; 
	if(count<20) fadepal(pal,pal2,count*16);
}

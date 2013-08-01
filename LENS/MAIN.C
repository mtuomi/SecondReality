#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include "..\dis\dis.h"

extern char lensex0[];
extern char lensex1[];
extern char lensex2[];
extern char lensex3[];
extern char lensex4[];
extern char lensexp[];
extern char lensexb[];

#define noSAVEPATH

#ifdef SAVEPATH
FILE	*fp;
int	pathstart2;
#else 
int	*pathdata1;
int	*pathdata2;
char	pathdata[13000];
#endif

char *vram=(char *)0xA0000000L;
int *lens1,*lens2,*lens3,*lens4;
extern char *back;
char *fade,*fade2;
extern char *rotpic;
extern char *rotpic90;
int	lenswid,lenshig,lensxs,lensys;
char	palette[768];

char	*shiftstatus=(char *)0x0417;

int	waitb()
{
	if(dis_indemo())
	{
		return(dis_waitb());
	}
	if(*shiftstatus&16) setborder(0);
	while(!(inp(0x3da)&8));
	while((inp(0x3da)&8));
	if(*shiftstatus&16) setborder(24);
	return(1);
}

void	drawlens(int x0,int y0)
{
	int	y,ys,ye;
	long int u1,u2;
	u1=(x0-lensxs)+(long)(y0-lensys)*320L;
	u2=(x0-lensxs)+(long)(y0+lensys-1)*320L;
	ys=lenshig/2;
	ye=lenshig-1;
	for(y=0;y<ys;y++)
	{
		if(u1>=0 && u1<=64000)
		{
			dorow(lens1,(unsigned)u1,y,0x40);
			dorow2(lens2,(unsigned)u1,y,0x80);
			dorow2(lens3,(unsigned)u1,y,0xC0);
			dorow3(lens4,(unsigned)u1,y,0);
		}
		u1+=320;
		if(u2>=0 && u2<=64000)
		{
			dorow(lens1,(unsigned)u2,ye-y,0x40);
			dorow2(lens2,(unsigned)u2,ye-y,0x80);
			dorow2(lens3,(unsigned)u2,ye-y,0xC0);
			dorow3(lens4,(unsigned)u2,ye-y,0);
		}
		u2-=320;
	}
}

void	setvmode(int m)
{
	_asm mov ax,m
	_asm int 10h
}

int	firfade1[200];
int	firfade2[200];
int	firfade1a[200];
int	firfade2a[200];

void	part1(void)
{
	int	x,y,xa,ya;
	int	a,r,g,b,c,i;
	int	frame=0;
	char	*cp,*dp;
	frame=0;
	for(b=0;b<200;b++)
	{
		a=b;
		firfade1a[b]=(19+a/5+4)&(~7);
		firfade2a[b]=(-(19+(199-a)/5+4))&(~7);
		firfade1[b]=170*64+(100-b)*50;
		firfade2[b]=170*64+(100-b)*50;
	}
	if(dis_musplus>-30) while(!dis_exit() && dis_musplus()<-6) ;
	dis_waitb();
	dis_setmframe(0);
	while(!dis_exit() && frame<300)
	{
		if(frame<80)
		{
			a=frame*2;
			for(c=0;c<6;c++)
			{
				cp=vram; dp=back;
				for(y=0;y<200;y++)
				{
					x=firfade1[y]>>6;
					cp[x]=dp[x];
					x=firfade2[y]>>6;
					cp[x]=dp[x];
					firfade1[y]+=firfade1a[y];
					firfade2[y]+=firfade2a[y];
					cp+=320;
					dp+=320;
				}
			}
		}
		a=waitb();
		frame+=a;
	}
}

void	part2(void)
{
	int	firstbounce=1;
	int	x,y,xa,ya;
	int	a,r,g,b,c,i;
	int	frame=0,uframe=0;
	char	*cp,*dp;
	x=65*64;
	y=-50*64;
	xa=ya=64;
	uframe=frame=0;
	while(!dis_exit() && uframe<715)
	{
		if(uframe<96)
		{
			a=(uframe-32)/2;
			if(a<0) a=0;
			setpalarea(fade2+a*3*192,64,192);
		}
		#ifdef SAVEPATH
		putw(x/64,fp);
		putw(y/64,fp);
		drawlens(x/64,y/64);
		x+=xa; y+=ya;
		if(x>256*64 || x<60*64) xa=-xa;
		if(y>150*64 && frame<600) 
		{
			y-=ya;
			if(firstbounce)
			{
				ya=-ya*2/3;
				firstbounce=0;
			}
			else ya=-ya*9/10;
		}
		ya+=2;
		#else
		x=pathdata1[frame*2+0];
		y=pathdata1[frame*2+1];
		drawlens(x,y);
		#endif
		a=waitb();
		uframe+=a;
		if(a>3) a=3;
		frame+=a;
	}
	while(!dis_exit() && uframe<720)
	{
		uframe+=waitb();
	}
}

void	part3(void)
{
	int	x,y,xa,ya;
	int	a,r,g,b,c,i;
	int	frame=0;
	char	*cp,*dp;
	rotpic90=back;
	for(x=0;x<256;x++)
	{
		for(y=0;y<256;y++)
		{
			rotpic90[x+y*256]=rotpic[y+(255-x)*256];
		}
	}
	waitb();
	setpalarea(fade+64*64*3,0,64);
	inittwk();
	{
		double	d1,d2,d3,scale,scaleb,scalea;
		int	flag=1;
		d1=0;
		d2=0.00007654321;
		d3=0;
		scale=2;
		scalea=-0.01;
		frame=0;
		while(!dis_exit() && frame<2000)
		{	
			if(dis_musplus()>-4) break;
			#ifdef SAVEPATH
			x=70.0*sin(d1)-30;
			y=70.0*cos(d1)+60;
			d1-=.005;
			xa=-1024.0*sin(d2)*scale;
			ya=1024.0*cos(d2)*scale;
			x-=xa/16;
			y-=ya/16;
			d2+=d3;
			putw(x,fp);
			putw(y,fp);
			putw(xa,fp);
			putw(ya,fp);
			rotate(x,y,xa,ya);
			scale+=scalea;
			if(frame>25)
			{
				if(d3<.02) d3+=0.00005;
			}
			if(frame<270)
			{
				if(scale<.9)
				{
					if(scalea<1) scalea+=0.0001;
				}
			}
			else if(frame<400)
			{
				if(scalea>0.001) scalea-=0.0001;
			}
			else if(frame>1600)
			{
				if(scalea>-.1) scalea-=0.001;
			}
			else if(frame>1100)
			{
				a=frame-900; if(a>100) a=100;
				if(scalea<256) scalea+=0.000001*a;
			}
			#else
			x=pathdata2[frame*4+0];
			y=pathdata2[frame*4+1];
			xa=pathdata2[frame*4+2];
			ya=pathdata2[frame*4+3];
			rotate(x,y,xa,ya);
			#endif
			frame+=waitb();
			if(frame>2000-128)
			{
				a=frame-(2000-128);
				a/=2;
				if(a>63) a=63;
				setpalarea(fade+a*64*3,0,64);
			}
			if(frame<16)
			{
				setpalarea(fade+(64+frame)*64*3,0,64);
			}
		}
	}
	for(a=0;a<768;a++) palette[a]=63;
	setpalarea(palette,0,256);
}

main()
{
	int	x,y,xa,ya;
	int	a,r,g,b,c,i;
	int	frame=0;
	char	*cp,*dp;
	FILE	*f1;
	dis_partstart();
	rotpic=halloc(16384,4);
	if(!rotpic) exit(1);
	fade=halloc(16000,1);
	if(!fade) exit(1);
	fade2=halloc(20000,1);
	if(!fade2) exit(1);
	setvmode(0x13);
	outp(0x3c8,0);
	for(a=0;a<768;a++) outp(0x3c9,0);
	#ifndef SAVEPATH
	a=*(int *)(lensexp+2);
	pathdata1=(int *)(lensexp+4);
	pathdata2=(int *)(lensexp+4+2*a);
	#endif
	memcpy(palette,lensexb+16,768);
	back=(char *)((long)lensexb+((768+16)/16)*65536L);
	memcpy(back+64000,back+64000-1536,1536);
	lenswid=*(int *)(lensex0+0);
	lenshig=*(int *)(lensex0+2);
	cp=lensex0+4;
	lensxs=lenswid/2;
	lensys=lenshig/2;
	for(i=1;i<4;i++)
	{
		r=*cp++;
		g=*cp++;
		b=*cp++;
		for(a=0;a<64*3;a+=3)
		{
			c=r+palette[a+0];
			if(c>63) c=63;
			palette[a+i*64*3+0]=c;
			c=g+palette[a+1];
			if(c>63) c=63;
			palette[a+i*64*3+1]=c;
			c=b+palette[a+2];
			if(c>63) c=63;
			palette[a+i*64*3+2]=c;
		}	
	}
	lens1=lensex1;
	lens2=lensex2;
	lens3=lensex3;
	lens4=lensex4;
	cp=fade;
	for(x=0;x<64;x++)
	{
		for(y=0;y<64*3;y++)
		{
			a=(palette[y]*(63-x)+x*63)/63;
			*cp++=a;
		}
	}
	for(x=0;x<16;x++)
	{
		for(y=0;y<64*3;y++)
		{
			a=palette[y]+(15-x)*5;
			if(a>63) a=63;
			*cp++=a;
		}
	}
	cp=fade2;
	for(x=0;x<32;x++)
	{
		for(y=64*3;y<256*3;y++)
		{
			a=y%(64*3);
			a=(palette[y]-palette[a]*(31-x)/31);
			*cp++=a;
		}
	}
	// rotpic readymake
	for(x=0;x<256;x++)
	{
		for(y=0;y<256;y++)
		{
			a=y*10/11-36/2;
			if(a<0 || a>199) a=0;
			a=back[x+32+a*320];
			rotpic[x+y*256]=a;
		}
	}

	waitb();
	setpalarea(palette,0,256);

	#ifdef SAVEPATH
	fp=fopen("lens.exp","wb");
	putw(0,fp);
	putw(0,fp);
	#endif

	if(!dis_exit()) part1();
	while(!dis_exit() && dis_musplus()<-20) ;
	dis_waitb();
	if(!dis_exit()) part2();
	#ifdef SAVEPATH
	pathstart2=(ftell(fp)-4)/2;
	#endif
	if(!dis_exit()) part3();
	
	#ifdef SAVEPATH
	rewind(fp);
	putw(0,fp);
	putw(pathstart2,fp);
	fclose(fp);
	#endif
}

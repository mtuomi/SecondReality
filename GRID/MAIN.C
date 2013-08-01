
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "..\dis\dis.h"

char	pal[3][768];

#define RECLEN (4*4)
extern int plane;
extern char muldata[];
extern int xyzdata[];
/**/
extern void setpalette(char *);
extern void asminit(void);
extern void asmloop1(void);
extern void setborder(int);
extern char bufdata[];

unsigned char far *vram0=(char far *)0xa0000000L;

extern char eye[];

int	sin256[256];
int	sin1[1024];
int	sinus1[1024];
int	sinus2[1024];

main(int argc,char *argv[])
{
	FILE	*f1;
	char	*cp;
	int	a,b,c,d,cx,cy,sip=0,j,k;
	int	x,y,x1,y1,x2,y2;
	int	scale=50,scalea=-1;
	unsigned u;
	double	f;
	for(a=0;a<256;a++)
	{
		f=(double)a*3.141592653589*2.0/256.0;
		f=sin(f)*64;
		sin256[a]=(int)f;
	}
	for(a=0;a<1024;a++)
	{
		f=(double)a*3.141592653589*2.0/1024.0;
		f=sin(f)*256;
		sin1[a]=(int)f;
	}
	
	b=c=0;
	for(a=0;a<1024;a++)
	{
		b+=97;
		c+=87;
		d+=67;
		sinus1[a]=(sin1[(b>>3)&1023]+sin1[(c>>3)&1023]*2+sin1[(d>>3)&1023])/8;
	}
	for(a=0;a<1024;a++)
	{
		sinus2[a]=sinus1[(a+277)&1023]*7/10;
	}
	
	dis_partstart();
	{
		_asm mov ax,13h
		_asm int 10h
	}

	#if 0
	memset(vram0,8,64000);
	while(dis_muscode(2)!=2 && !dis_exit());
	for(x=319;x>=0;x--)
	{
		vram0[x+100*320]=0;
		if(!(x&15)) dis_waitb();
	}
	y1=100; a=100*64; b=0;
	while(y1<200)
	{
		b+=8;
		a+=b;
		y2=a/64;
		if(y2>200) y2=200;
		for(y=y1;y<y2;y++)
		{
			memset(vram0+y*320,0,320);
		}
		y1=y2;
		dis_waitb();
	}
	for(a=0;a<35 && !dis_exit();a++) dis_waitb();
	#endif
	
	outp(0x3c8,0);
	for(y=0;y<3;y++)
	{
		for(x=0;x<256;x++)
		{
			c=eye[16+(x>>3)*3+0]/4;
			b=eye[16+(x>>3)*3+1]/4;
			a=eye[16+(x>>3)*3+2]/4;
			if(y==0)
			{
				if(x&1) c+=40;
				if(x&4) c+=20;
				if(x&2) c+=10;
			}
			else if(y==1)
			{
				if(x&2) c+=40;
				if(x&1) c+=20;
				if(x&4) c+=10;
			}
			else
			{
				if(x&4) c+=40;
				if(x&2) c+=20;
				if(x&1) c+=10;
			}
			if(c>63) c=63;
			pal[y][x*3+0]=c;
			pal[y][x*3+1]=b;
			pal[y][x*3+2]=a;
		}
	}
	//for(x=0;x<256;x++) vram0[x]=x;
	
	asminit();
	cp=muldata;
	for(x=0;x<256;x++) for(y=0;y<256;y++)
	{
		_asm mov al,x
		_asm mul byte ptr y
		_asm mov al,ah
		_asm xor ah,ah
		_asm mov a,ax
		*(cp++)=a;
	}

	for(u=784;u<64684;u++) eye[u]<<=3;
	memcpy(bufdata,eye+784,64000);

	for(x=0;x<1024;x++)
	{
		u=x*RECLEN;
		xyzdata[u+0]=sinus1[x]+160;
		xyzdata[u+1]=sinus2[x]+100;
		vram0[xyzdata[u+0]+xyzdata[u+1]*320]=15;
		xyzdata[u+2]=0;
		xyzdata[u+3]=0;
	}
	getch();
		
	//while(dis_muscode(2)!=2 && !dis_exit());
	memcpy(vram0,eye+768+16,64000);
	j=k=0;
	while(!dis_exit())
	{
		if(dis_muscode(3)==3) break;
		c=j;
		d=sin1[k&1023]/4+256;
		d=0;
		j++;
		k++;
		for(u=0;u<1001*RECLEN;u+=RECLEN)
		{
			xyzdata[u+0]=sinus1[(c)&1023]+160;
			xyzdata[u+1]=sinus2[(d)&1023]+100;
			c++; d++;
		}
		asmloop1();
		setborder(0);
		dis_waitb();
		setborder(1);
		setpalette(pal[plane]);
	}

	if(!dis_indemo())
	{
		_asm mov ax,3h
		_asm int 10h
	}
}

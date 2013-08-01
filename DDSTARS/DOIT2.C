#include <stdio.h>
#include <math.h>

char *vram=(char *)0xa0000000L;

main()
{
	FILE	*f1;
	char	*vp;
	int	a,b,c,r,x,y;
	long	l1,l2;
	_asm mov ax,13h
	_asm int 10h
	outp(0x3c8,0);
	for(a=0;a<8;a++)
	{
		outp(0x3c9,a*8);
		outp(0x3c9,a*8);
		outp(0x3c9,a*8);
	}
	for(y=0;y<200;y++) for(x=0;x<320;x++)
	{
		l1=(long)(x-320)*(long)(x-320);
		l2=(long)(y-200)*(long)(y-200);
		r=(int)sqrt((double)l1+(double)l2);
		a=((r)/12)&1;
		vram[x+y*320]=a;
	}
	c=1;
	f1=fopen("pic2.ega","wb");
	for(y=0;y<200;y++)
	{
		for(c=1;c<=1;c<<=1)
		{
			for(x=0;x<320;x+=8)
			{
				vp=vram+x+y*320;
				a=0;
				if(vp[0]&c) a|=128;
				if(vp[1]&c) a|=64;
				if(vp[2]&c) a|=32;
				if(vp[3]&c) a|=16;
				if(vp[4]&c) a|=8;
				if(vp[5]&c) a|=4;
				if(vp[6]&c) a|=2;
				if(vp[7]&c) a|=1;
				putc(a,f1);
			}
		}
		vram[y*320]=15;
		vram[y*320+1]=0;
	}
	fclose(f1);
	getch();
	_asm mov ax,3
	_asm int 10h
	for(a=0;a<256;a++)
	{
		b=0;
		if(a&1) b|=128;
		if(a&2) b|=64;
		if(a&4) b|=32;
		if(a&8) b|=16;
		if(a&16) b|=8;
		if(a&32) b|=4;
		if(a&64) b|=2;
		if(a&128) b|=1;
		printf("%i,",b);
	}
}

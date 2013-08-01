#include <stdio.h>
#include <math.h>

char *vram=(char *)0xa0000000L;

main()
{
	FILE	*f1;
	char	*vp;
	int	a,b,c,r,x,y;
	double	l1,l2;
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
		a=((long)((x-160)*(long)(320-y)+1600L)/3200L)&1;
		vram[x+y*320]=a;
	}
	c=1;
	f1=fopen("pic3.ega","wb");
	for(y=0;y<200;y++)
	{
		for(c=1;c<=4;c<<=1)
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
}

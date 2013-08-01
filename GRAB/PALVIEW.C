#include <stdio.h>
#include <bios.h>
#include <stdlib.h>

#define	pset(xx,yy,cc) *(vram+(xx)+(yy)*320)=(cc);

union	REGS	reg;

main(int argc,char *argv[])
{
	int	minr=999,ming=999,minb=999;
	int	maxr=0,maxg=0,maxb=0;
	int	a,b,c;
	int	xr,xg,xb;
	int	div=1;
	int	x,y;
	long	start=0;
	char far *vram=(char far *)0xa0000000;
	FILE	*f1;
	if(argc==1)
	{
		printf("usage: VIEWPAL <filename> [byte to start] [4 to divide]\n");
		return(0);
	}
	f1=fopen(argv[1],"rb");
	if(f1==NULL) return(0);
	if(argc>=3) start=atol(argv[2]);
	if(argc>=4) { div=4; }
	fseek(f1,start,SEEK_SET);
	reg.x.ax=0x13;
	int86(0x10,&reg,&reg);
	
	for(y=0;y<16;y++) for(x=0;x<16;x++)
	{
		a=x*16+y;
		for(b=0;b<4;b++) for(c=0;c<4;c++)
		{
			pset(x*4+b,y*4+c,a);
		}
	}
	outp(0x3c8,0);
	for(a=0;a<256;a++)
	{
		xr=getc(f1)/div;
		xg=getc(f1)/div;
		xb=getc(f1)/div;
		outp(0x3c9,xr);
		outp(0x3c9,xg);
		outp(0x3c9,xb);
		if(xr<minr) minr=xr;
		if(xg<ming) ming=xg;
		if(xb<minb) minb=xb;
		if(xr>maxr) maxr=xr;
		if(xg>maxg) maxg=xg;
		if(xb>maxb) maxb=xb;
	}
	fclose(f1);
	
	getch();
	reg.x.ax=0x3;
	int86(0x10,&reg,&reg);
	printf("Min: R:%-3i G:%-3i B:%-3i\n",minr,ming,minb);
	printf("Max: R:%-3i G:%-3i B:%-3i\n",maxr,maxg,maxb);
}

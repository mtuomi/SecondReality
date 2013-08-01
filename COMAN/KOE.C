#include <stdio.h>
#include <math.h>

int	table[1024];

char *vram=(char *)0xa0000000L;

main()
{
	int x,y;
	double f,g;
	_asm mov ax,13h
	_asm int 10h
	for(x=0;x<1024;x++)
	{
		f=(double)x*3.1415926535*3.0/1024.0;
		f=sin(f);
		g=(double)x/700;
		if(g>1) g=2-g;
		f=f*g*g;
		y=(int)(f*400.0);
		table[x]=y;
		vram[x/4+(y/4+100)*320]=9;
	}
	getch();
	_asm mov ax,3h
	_asm int 10h
	for(x=0;x<1024;x++)
	{
		if(!x) printf("{%i",table[x]);
		else printf(",%i",table[x]);
		if(!(x&15))
		{
			printf(" \\\n");
		}
	}
	printf("};\n");
}

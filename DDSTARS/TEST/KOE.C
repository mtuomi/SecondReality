#include <stdio.h>
#include "graph.h"

main()
{
	int	x,y,z,sx,sy,a;
	_setvideomode(_VRES16COLOR);
	for(a=0;a<19200;a++)
	{
		x=a%26;
		y=a/26%26;
		z=a/26/26%26;
		x=(x-13)*64;
		y=(y-13)*64;
		z=z*32+64;
		sx=100*x/z+320;
		sy=100*y/z+240;
		if(a&1) _setcolor(15);
		else _setcolor(3+8);
		_setpixel(sx,sy);
	}
	getch();
	_setvideomode(_DEFAULTMODE);
}

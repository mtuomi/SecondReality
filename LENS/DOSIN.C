#include <stdio.h>
#include <math.h>

main()
{
	int	a,b;
	double	d;
	for(a=0;a<4096;a++)
	{
		d=3.14159265358979*2.0*(double)a/4096.0;
		b=(int)(sin(d)*16384.0);
		if(!(a&7)) printf("\ndw %6i",b);
		else printf(",%6i",b);
	}
	printf("\n");
}

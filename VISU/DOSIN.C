#include <stdio.h>
#include <math.h>

main()
{
	double	d,e;
	int	r,i;
	printf( ";Sinus table for a circle with 4096 subdivision\n"
		";4096 values in range -16384..16384\n");
	for(r=0;r<4096;r++)
	{
		if(!(r&7)) printf("\ndw ");
		d=2.0*3.1415926535897932384626*(double)r/4096.0;
		e=sin(d)*16384.0+0.5;
		i=(int)e;
		if((r+1)&7) printf("%6i,",i);
		else printf("%6i",i);
	}
	printf("\n");
}

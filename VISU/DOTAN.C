#include <stdio.h>
#include <math.h>

main()
{
	int	a,b;
	double	f,g;
	printf(";Visual angle tangent table (256 words = 90 degrees)");
	for(a=0;a<256;a++)
	{
		if(a<2) b=32767;
		else
		{
			f=(double)a*2.0*3.141592653589/1024.0;
			g=1/tan(f);
			b=(int)(256*g);
		}
		if(!(a%12)) printf("\ndw ");
		else printf(",");
		printf("%5i",b);
	}
}
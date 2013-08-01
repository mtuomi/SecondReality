#include <stdio.h>

char	buf[1024];

main()
{
	FILE	*f1;
	int	a,b,c=0;
	float	x,y,z;
	f1=fopen("face.asc","rt");
	while(!feof(f1))
	{
		fgets(buf,1024,f1);
		fgets(buf,1024,f1);
		if(!memcmp(buf,"Vertex ",7))
		{
			if(c)
			{
				sscanf(buf,"Vertex %i: X: %f Y: %f Z: %f",&a,&x,&y,&z);
				printf("dw %i,%i,%i\n",(int)(x*1000),-(int)(z*1000),(int)(y*1000));
			} 
			else c=1;
		}
	}
	fclose(f1);
}

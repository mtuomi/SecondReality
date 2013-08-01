#include <stdio.h>

unsigned char logodat[64000];

main()
{
	int	x,y;
	FILE	*f1,*f2;
	unsigned u,c;
	printf("Creating movement tables for FC logo...\n");
	
	f2=fopen("_dots1.tmp","wb");
	f1=fopen("fckoe3.u","rb");
	fread(logodat,320,200,f1);
	fclose(f1);

	#if 0
	for(c=x=0;x<320;x++) for(y=0;y<200;y++)
	{
		u=(x+y*320); if(logodat[u]) c++;
	}
	#endif

	for(x=0;x<320;x++) for(y=0;y<200;y++)
	{
		u=x+y*320;
		if(logodat[u]==2 || logodat[u]==1)
		{
			putc(logodat[u],f2);
			putc(0,f2);
			putc(x-160,f2);
			putc(y-32,f2);
		}
	}
	putc(0,f2);

	fclose(f2);
}

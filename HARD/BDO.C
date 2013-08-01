#include <stdio.h>

unsigned char logodat[64000];

void	doit(char *name,int ybase)
{
	int	x,y;
	FILE	*f2;
	unsigned u,c;
	printf("%s : ",name);
	f2=fopen(name,"wb");
	for(c=x=0;x<320;x++) for(y=0;y<40;y++)
	{
		u=x+y*320+ybase*320;
		if(logodat[u]<4 && logodat[u])
		{
			c++;
			putc(logodat[u],f2);
			putc(0,f2);
			putc(x-160,f2);
			putc(y-20,f2);
		}
	}
	putc(0,f2);
	printf("%i dots.\n",c);
	fclose(f2);
}

main()
{
	FILE	*f1;
	printf("Creating movement tables for FC logo...\n");
	
	f1=fopen("fcrz.u","rb");
	fread(logodat,320,200,f1);
	fclose(f1);

	doit("_dots1.tmp",0);
	doit("_dots2.tmp",40);
	doit("_dots3.tmp",80);
	doit("_dots4.tmp",120);
}

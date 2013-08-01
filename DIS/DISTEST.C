#include <stdio.h>
#include "dis.h"

main()
{
	int	a;
	a=dis_version();
	if(!a)
	{
		printf("\nDIS not installed!\n");
		return(1);
	}
	else
	{
		printf("\nDIS version %04X installed.\n",a);
	}
	printf("\nPress any key to exit.\n");
	a=0;
	while(!dis_exit())
	{
		a+=dis_waitb();
		printf("%i frames waited.\r",a);
	}
	printf("\n\n");
	return(0);
}

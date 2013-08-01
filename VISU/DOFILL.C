#include <stdio.h>
#include <stdarg.h>
#include <string.h>

char	xf[256];
char	xi[256];

FILE	*f1;

void	P(char *str,...)
{
	char out[256];
	va_list	argp;
	va_start(argp,str);
	vsprintf(out,str,argp);
	fprintf(f1,"%s\n",out);
	va_end(argp);
}

int	buf1[1024],buf1p;
int	buf2[1024],buf2p;

main()
{
	long	dif,len,zlen,a;
	f1=fopen("afilldiv.inc","wt");
	for(zlen=0;zlen<512;zlen++)
	{
		if(!zlen) len=1; else len=zlen;
		a=65536/len;
		if(a>65535) a=65535;
		if(!(zlen&15)) fprintf(f1,"\ndw %i",a);
		else fprintf(f1,",%i",a);
	}
	fprintf(f1,"\n");
	fclose(f1);
	#if 0
	{
		int	st,le,en;
		f1=fopen("afill.inc","wt");
		P(";start jmp,end jmp");
		for(le=0;le<4;le++)
		{
			for(st=0;st<4;st++)
			{
				en=(le+st)&3;
				P("dw OFFSET afill_s%i,OFFSET afill_e%i",st,en);
			}
		}
		fclose(f1);
	}
	#endif
}

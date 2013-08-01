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
	int	a,b,c,d,e,f,g,fr,to;
	int	pmask;
	unsigned mask1,mask2,u;
	f1=fopen("avidfill.inc","wt");
	P("gf_table LABEL WORD\n");
	for(fr=0;fr<16;fr++) for(to=fr;to<16;to++)
	{
		P("dw OFFSET gf_%i_%i",fr,to);
	}
	P("");
	for(fr=0;fr<16;fr++) for(to=fr;to<16;to++)
	{
		P("\ngf_%i_%i:\n",fr,to);
		mask1=0;
		for(a=fr;a<=to;a++) mask1|=1<<a;
		mask2=mask1&0xAAAA;
		mask1=mask1&0x5555;
		if((fr&3)>=3)
		{
			u=mask1; mask1=mask2; mask2=u; break;
		}
		buf1p=buf2p=0;
		pmask=0;
		for(a=0;a<16;a+=2)
		{
			c=(mask1>>a)&3;
			if(c)
			{
				if(c!=pmask)
				{
					P("mov al,%i",pmask);
					P("out dx,al");
					pmask=c;
				}
				P("mov es:[di],ch");
				break;
			}
		}
		for(;a<16;a+=2)
		{
			c=(mask1>>a)&3;
			if(c)
			{
				if(c!=pmask)
				{
					P("mov al,%i",pmask);
					P("out dx,al");
					pmask=c;
				}
				P("mov es:[di],ch");
				break;
			}
		}
	}
	fclose(f1);
}

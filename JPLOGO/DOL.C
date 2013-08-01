#include <stdio.h>
#include <math.h>
#include <conio.h>
#include <stdarg.h>

#define BLACK -1
#define EMPTY -2

#define BEG 140
#define END 244

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


main()
{
	int	a,b,c,x,y,z,le,ri;
	int	xt[320];
	f1=fopen("zoom.inc","wt");
	P("zoomt LABEL WORD");
	for(a=0;a<320;a+=2)
	{
		y=a;
		if(!y) P("dw OFFSET @zoom0",BEG);
		else if(y<BEG) P("dw OFFSET @zoom%i",BEG);
		else if(y>END) P("dw OFFSET @zoom%i",END);
		else P("dw OFFSET @zoom%i",y);
	}
	P("@zoom0:");
	a=(160-END/2-1)&(~7);
	b=(160+END/2+8)&(~7);
	for(x=a;x<b;x+=8) P("mov es:[di+%i],ax",x/4);
	P("ret");
	for(y=BEG;y<=END;y+=2)
	{
		kbhit();
		printf("\r%i: ",y);
		P("@zoom%i:",y);
		
		le=160-(y/2);
		ri=160+(y/2);
		c=ri-le+1;
		for(x=0;x<320;x++)
		{
			if(x<le || x>ri) xt[x]=BLACK;
			else xt[x]=((long)(x-le)*185L+(c/2))/(long)c;
		}
		
		le=160-END/2-1; le&=~7;
		ri=160+END/2+8; ri&=~7;
		for(x=0;x<le;x++) xt[x]=EMPTY;
		for(x=ri;x<320;x++) xt[x]=EMPTY;
		
		for(x=0;x<320;x+=8)
		{
			for(b=x;b<x+8;b++) if(xt[b]!=BLACK) break;
			if(b==x+8)
			{
				P("mov es:[di+%i],ax",x/4);
				for(b=x;b<x+8;b++) xt[b]=EMPTY;
			}
		}
		for(x=0;x<320;x+=4)
		{
			for(b=x;b<x+4;b++) if(xt[b]!=BLACK) break;
			if(b==x+4)
			{
				P("mov es:[di+%i],al",x/4);
				for(b=x;b<x+4;b++) xt[b]=EMPTY;
			}
		}
		for(x=0;x<320;x++) if(xt[x]==BLACK) printf(" %i",x);
		printf("\n");
		for(z=0;z<4;z++)
		{
			P("mov al,%i",1<<z);
			P("out dx,al");
			for(x=z;x<320;x+=8)
			{
				a=xt[x]; b=xt[x+4];
				if(a==EMPTY && b==EMPTY) continue;
				if(a==BLACK && b==BLACK) P("xor ax,ax");
				else
				{
					if(a==BLACK) P("xor al,al");
					else if(a!=EMPTY) P("mov al,ds:[si+%i]",a);
					if(b==BLACK) P("xor ah,ah");
					else if(b!=EMPTY) P("mov ah,ds:[si+%i]",b);
				}
				if(b==EMPTY) P("mov es:[di+%i],al",x/4);
				else if(a==EMPTY) P("mov es:[di+%i],ah",x/4+1);
				else P("mov es:[di+%i],ax",x/4);
			}
		}
		P("ret");
	}
	fclose(f1);
}

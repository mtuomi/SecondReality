#include <stdio.h>
#include <math.h>
#include <conio.h>
#include <stdarg.h>

#define MAXZOOM 340
#define MINZOOM 160

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

long	weirdflip(unsigned long l)
{
	long	m,lo,hi;
	lo=l&0xffff;
	hi=l>>16;
	m=(lo<<16)+hi;
	return(m);
}

int	xt[320];

main()
{
	int	i,j,x,y,a,sk1,sk2,sk3,sk4,sfl;
	long	l;
	f1=fopen("zoomloop.inc","wt");

	P("zoom PROC FAR");	
	P(";ds:si=source bitmap row start");
	P(";es:di=destination pointer");
	P(";ax=zoom factor (desired screen width)");
	P("cmp ax,%i",MAXZOOM);
	P("jae zoom_ret");
	P("xor cx,cx");
	P("test ax,4");
	P("jnz @@1");
	P("add di,2");
	P("mov cx,1");
	P("@@1:");
	P("mov bx,ax");
	P("and bx,not 1");
	P("xor edx,edx");
	P("jmp cs:zoomloopt[bx]");
	P("zoom_ret:");
	P("ret");
	P("ALIGN 16 ;following should stay in cache");
	P("zoomloopt LABEL WORD");
	for(y=0;y<=MAXZOOM;y+=2)
	{	
		if(y<MINZOOM) P("dw OFFSET zoom_%i",MINZOOM&(~4));
		else 
		{
			P("dw OFFSET zoom_%i",y&(~4));
		}
	}
	for(y=MINZOOM;y<=MAXZOOM;y+=2)
	{	
		if(y&4) continue;
		printf("%i\r",y);
		for(x=0;x<320;x++)
		{
			a=(int)((long)(x-160)*320L/(long)y)+160;
			if(a<0 || a>319) xt[x]=-1;
			else xt[x]=a;
		}
		sk1=(int)((long)-80*(long)y/320L)+160;
		sk2=(int)((long)-40*(long)y/320L)+160;
		sk3=(int)((long)40*(long)y/320L)+160;
		sk4=(int)((long)80*(long)y/320L)+160;
		sk1&=~3;
		sk2&=~3;
		sk3&=~3;
		sk4&=~3;
		P("zoom_%i:",y);
		sfl=0;
		for(x=0;x<320;x+=2)
		{
			if(x==sk1 || x==sk2 || x==sk3 || x==sk4)
			{
				P("add si,cx");
			}
			if(xt[x]==-1 && xt[x+1]==-1 && xt[x+2]==-1 && xt[x+3]==-1 && !(x&2))
			{
				P("mov es:[di+%i],edx",x);
				x+=2;
			}
			else if(xt[x]==-1 && xt[x+1]==-1)
			{
				P("mov es:[di+%i],dx",x);
			}
			else
			{
				if(xt[x]!=-1 && xt[x+1]!=-1 && xt[x+1]==xt[x]+1)
				{
					P("mov ax,ds:[si+%i]",xt[x]);
					P("mov es:[di+%i],ax",x);
				}
				else
				{
					if(xt[x]==-1) P("mov al,dl");
					else P("mov al,ds:[si+%i]",xt[x]);
					if(xt[x+1]==xt[x]) P("mov ah,al");
					else if(xt[x+1]==-1) P("mov ah,dl");
					else P("mov ah,ds:[si+%i]",xt[x+1]);
					P("mov es:[di+%i],ax",x);
				}
			}
		}
		P("ret");
	}
	P("zoom ENDP");
	fclose(f1);
}

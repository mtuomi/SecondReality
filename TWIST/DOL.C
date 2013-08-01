#include <stdio.h>
#include <math.h>
#include <conio.h>
#include <stdarg.h>

int	from=6,to=198;

int	cent=160;

FILE	*f1;

unsigned char *vram=(char *)0xa0000000L;

void	P(char *str,...)
{
	char out[256];
	va_list	argp;
	va_start(argp,str);
	vsprintf(out,str,argp);
	fprintf(f1,"%s\n",out);
	va_end(argp);
}

int	xt[320];
char	palette[768];

main()
{
	int	i,j,x,y,a,sk1,sk2,sk3,sk4,sfl,la;
	unsigned int u;
	char	*v;
	long	l;
	
	_asm mov ax,13h
	_asm int 10h
	f1=fopen("tmp.uh","rb");
	fread(palette,1,16,f1);
	fread(palette,1,768,f1);
	fread(vram,1,64000,f1);
	for(u=1;u<64000;u++)
	{
		vram[u-1]=palette[vram[u]*3];
	}
	outp(0x3c8,0);
	for(a=0;a<64;a++)
	{
		outp(0x3c9,a);
		outp(0x3c9,a);
		outp(0x3c9,a);
	}
	fclose(f1);
	for(y=0;y<200;y++)
	{
		v=vram+y*320;
		for(x=0;x<320;x++) if(x&3)
		{
			if(v[x+4]!=0 && v[x-4]!=0) v[x]=v[x-1];
		}
	}

	f1=fopen("twstloop.inc","wt");
	P("twistt LABEL WORD");
	for(y=0;y<200;y++)
	{
		a=y;
		P("dw OFFSET twist%il,OFFSET twist%ir",a,a);
	}
	P("twist PROC NEAR");
	for(y=0;y<200;y++)
	{
		kbhit();
		P("twist%il:",y);
		P("mov dx,3c4h");
		P("mov ax,0f02h");
		P("out dx,ax");
		la=-1;
		for(x=0;x<160;x+=8)
		{
			a=vram[y*320+x];
			a|=(vram[y*320+x+4])<<8;
			if(la!=a) 
			{
				la=a;
				P("mov ax,0%04Xh",a);
			}
			P("mov ds:[di+%i],ax",x/4);
		}
		P("ret");
		P("twist%ir:",y);
		P("mov dx,3c4h");
		P("mov ax,0f02h");
		P("out dx,ax");
		la=-1;
		a=from+(long)y*(long)(to-from)/200L;
		v=vram+a*320;
		for(x=160;x<320;x+=8)
		{
			a=v[x];
			a|=(v[x+4])<<8;
			if(la!=a) 
			{
				la=a;
				P("mov ax,0%04Xh",a);
			}
			P("mov ds:[di+%i],ax",x/4);
		}
		P("ret");
		v[0]=63;
	}
	P("twist ENDP");
	fclose(f1);
	getch();
	_asm mov ax,3h
	_asm int 10h
}

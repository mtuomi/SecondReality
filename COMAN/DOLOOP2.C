#include <stdio.h>
#include <math.h>
#include <conio.h>
#include <stdarg.h>

int	horizony=70;
int	bail=192;
int	bailhalve=64;
int	group=150;
int	zwave[1024];

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

long	wflip(unsigned long l)
{
	long	m,lo,hi;
	lo=l&0xffff;
	hi=l>>16;
	m=(lo<<16)+hi;
	return(m);
}

main()
{
	int	i,j,k,jm,y,a;
	int	sina=1;
	long	l,firstraydir;
	f1=fopen("theloop.inc","wt");
	
	for(i=0;i<bail;i++)
	{
		zwave[i]=(int)(16.0*sin((double)i*3.1415926535*2.0*3.0/(double)bail));
	}

	P(";Register usage:		");
	P(";eax	low...	ray heigth 	");
	P(";ebx	-	!      		");
	P(";ecx	low...	ray direction 	(always negative)");	
	P(";edx	bail    scenary heigth	");
	P(";esi	-	xsin		");	
	P(";edi	-	ysin		");	
	P(";ebp	-	p->screen	");	
	P(";ds	->codesegment");
	P(";es	->screenbuffer");
	P(";fs	->wavetablex");
	P(";gs	->wavetabley");
	P("");
	P("ALIGN 16 ;following should stay in cache");
	P("theloop_waveseg dw 0");
	P("theloop_xsina1 dw 0");
	P("theloop_ysina1 dw 0");
	P("theloop_xsina2 dw 0");
	P("theloop_ysina2 dw 0");
	P("theloop_heigth dw 0");
	firstraydir=-(200-horizony)*2560L;
	P("");
	P(";entry: es=videbuf");
	P(";	bp=pointer to bottom of current column in videbuf");
	P(";	fs=waveXsegment");
	P(";	gs=waveYsegment");
	P(";	si=waveXpos");
	P(";	di=waveYpos");
	P(";	cx=waveXadd");
	P(";	dx=waveYadd");
	P(";	ax=camera heigth adder");
	P("theloop PROC FAR");
	P("mov cs:theloop_xsina1,cx");
	P("mov cs:theloop_ysina1,dx");
	P("add cx,cx");
	P("mov cs:theloop_xsina2,cx");
	P("add dx,dx");
	P("mov cs:theloop_ysina2,dx");
	P("mov cs:theloop_heigth,ax");
	P("mov bx,cs:theloop_xsina1");
	//P("mov bp,OFFSET theloop_raydir");
	P("mov ax,cs");
	P("mov ds,ax");
	P("mov edx,10000h*(%i/3)",-bail);
	P("xor eax,eax");
	P("xor ecx,ecx");
	for(i=199;i>=horizony;i--)
	{
		j=i;
		printf("%i   \r",j);

		// seek until scene hits row i
		P("_@seek%i:",j);
		for(k=0;k<3;k++)
		{
			P("add si,ds:theloop_xsina1");
			P("mov dx,fs:[si]");
			P("add di,ds:theloop_ysina1");
			P("add dx,gs:[di]");
			P("add dx,%i",-100);
			l=(i-horizony)*2560L;
			P("add ecx,0%08lXh",wflip(2560L));
			P("adc cx,0");
			P("sub eax,0%08lXh",wflip(l));
			P("sbb ax,0");
			P("cmp ax,dx");
			P("jl  short _@seek%im",j);
		}
		P("add edx,00010000h");
		P("jnc _@seek%i",j);
		P("ret");

		// hit, draw pixel & advance rays
		P("_@seek%im:",j);
		P("mov es:[bp+%i],dl",-(199-j)*160);
		P("add eax,ecx");
		P("adc ax,0");
		P("cmp ax,dx");
		P("jl  short _@seek%im",j-1);
	}
	P("_@seek%im:",j-1);
	P("ret");
	P("theloop ENDP");
	printf("       \r",i);
	
	fclose(f1);
}

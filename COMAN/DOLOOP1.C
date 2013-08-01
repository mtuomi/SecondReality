#include <stdio.h>
#include <math.h>
#include <conio.h>
#include <stdarg.h>

int	horizony=70;
int	bailstart=0; // must be divisible by 2
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
	int	i,j,jm,y,a;
	int	sina=1;
	long	l,firstraydir;
	f1=fopen("theloop.inc","wt");
	
	for(i=0;i<bail;i++)
	{
		zwave[i]=16.0*sin((double)i*3.1415926535*2.0*3.0/(double)bail);
	}

	P(";Register usage:		");
	P(";eax	low...	ray heigth 	");
	P(";ebx	-	p->screen	");
	P(";ecx	low...	ray direction 	(always negative)");	
	P(";edx	-       scenary heigth	");
	P(";esi	-	xsin		");	
	P(";edi	-	ysin		");	
	P(";ebp	-	-		");	
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
	P("mov bx,bp");
	P("mov cs:theloop_xsina1,cx");
	P("mov cs:theloop_ysina1,dx");
	P("add cx,cx");
	P("mov cs:theloop_xsina2,cx");
	P("add dx,dx");
	P("mov cs:theloop_ysina2,dx");
	P("mov cs:theloop_heigth,ax");
	//P("mov bp,OFFSET theloop_raydir");
	P("mov ax,cs");
	P("mov ds,ax");
	P("xor eax,eax");
	P("mov ecx,0%08lXh",wflip(firstraydir));
	for(i=0;i<bailstart/2;i++)
	{
		P("add si,cx");
		P("add si,dx");
		P("add eax,0%08lXh",wflip(firstraydir*2L));
		P("adc ax,-1");
	}
	for(i=bailstart;i<bail;i+=group)
	{
		jm=i+group; if(jm>bail) jm=bail;
		for(j=i;j<jm;j++)
		{
			printf("%i\r",j);

			/*
			if(j==bailhalve) 
			{
				P("add ecx,ecx");
				sina=2;
			}
		*/
			
			P("_@seek%i:",j);
			P("add si,ds:theloop_xsina%i",sina);
			P("mov dl,fs:[si]");
			P("xor dh,dh");
			P("add di,ds:theloop_ysina%i",sina);
			P("add dl,gs:[di]");
			P("add dx,%i",zwave[j]+30-j/4+(-260));
			//P("add dx,ds:theloop_heigth");
			P("cmp ax,dx");
			P("jge short _@seeko%i",j);
			
			l=(long)j*2560L;
			l=wflip(l);
			P("_@hit%i:",j);
			if(l)
			{
				P("add eax,0%08lXh",l);
				P("adc ax,0");
			}
			P("mov es:[bx],dl");
			P("cmp ax,dx");
			P("jge short _@seek1a%i",j);
			if(l) 
			{
				P("add eax,0%08lXh",l);
				P("adc ax,0");
			}
			P("mov es:[bx-160],dl");
			P("cmp ax,dx");
			P("jge short _@seek2a%i",j);
			if(l) 
			{
				P("add eax,0%08lXh",l);
				P("adc ax,0");
			}
			P("mov es:[bx-320],dl");
			/**/
			P("add ecx,0%08lXh",wflip((long)sina*3L*2560L));
			P("adc cx,0");
			P("sub bx,160*3");
			/**/
			P("cmp ax,dx");
			P("jge short _@seeko%i",j);
			P("jmp _@hit%i",j);
			
			P("_@seek2a%i:",j);
			P("add ecx,0%08lXh",wflip((long)sina*2560L));
			P("adc cx,0");
			P("sub bx,160");
			P("_@seek1a%i:",j);
			P("add ecx,0%08lXh",wflip((long)sina*2560L));
			P("adc cx,0");
			P("sub bx,160");
			
			P("_@seeko%i:",j);
			P("add eax,ecx");
			P("adc ax,-1");
			
			if(sina==2) j++;
		}
		if(jm>=bail) 
		{
			P("_@seek%i:",bail);
			P("_@seekr%i:",bail);
			P("ret");
		}
		else P("jmp _@seek%i",jm);
	}
	P("theloop ENDP");
	printf("       \r",i);
	
	fclose(f1);
}

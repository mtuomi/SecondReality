/* UNREAL 2 - Init / Deinit part 
*/
#include <stdio.h>
#include <dos.h>
#include <string.h>
#include <malloc.h>
#include "..\dis\dis.h"
// doshandle.h
#include <sys\types.h>
#include <sys\stat.h>
#include <fcntl.h>
#include <io.h>

// set by startems.c
int		zemspagehandle=0;
int		zemspageframe=0;
char far 	*module=NULL;

long	*lp;

int	outputmode;
#define OUTPUTGUS 2
#define OUTPUTMONO 1
#define OUTPUTNONE 7

#include "startems.c"

char far *tvram=(char far *)0xb8000000L;

int	checkgusmem(void)
{
	int	a;
	_asm
	{
		mov	bx,4
		mov	ax,103h
		int	0fch
		mov	a,ax
	}
	if(a<512) return(1);
	return(0);
}

int	checkemsmem(void)
{
	int	ht[50];
	int	a,b;
	char	e;
	union REGS reg;
	{ /* check for ems */
		FILE	*f1;
		f1=fopen("*EMMXXXX0","rb");
		if(!f1) return(1);
		fclose(f1);
	}
	_asm 
	{
		mov	ah,42h
		int	67h
		mov	a,bx
	}
	if(a<63) return(1);
	for(a=0;a<50;a++)
	{
		_asm
		{
			mov	ah,43h
			mov	bx,1
			int	67h
			mov	e,ah
			mov	b,dx
		}
		if(e) break;
		ht[a]=b;
	}
	while(a>0)
	{
		a--;
		b=ht[a];
		_asm
		{
			mov	ah,45h
			mov	dx,b
			int	67h
		}
	}
	if(e) return(2);
	return(0);
}

void	loadmodule(char *name)
{
	int	h;
	char	*p;
	long	size;
	h=open(name,O_BINARY|O_RDONLY);
	module=stmik_emsload(h);
	close(h);
	
	memcpy(lp,&module,4);
	memcpy(lp+1,&zemspageframe,2);
	memcpy(lp+2,&zemspagehandle,2);

	p=module;
	_asm
	{
		mov	bx,4
		mov	ax,101h
		mov	dx,word ptr p[2]
		int	0fch // initmodule
	}
	stmik_emsready(module);
}

void	loadmodule2(int i)
{
	int	h;
	char	*p;
	long	size;
	long	base;
	
	gusmode=0;
	if(outputmode==OUTPUTMONO) nosurround=1;
	else nosurround=0;

	memset(lp+1,0,8);	

	if(outputmode==OUTPUTNONE) 
	{
		loadmode=-1; // load no samples
	}
	else if(outputmode==OUTPUTGUS) 
	{
		loadmode=0; // all low !!!0!!!
		gusmode=1;
	}
	else
	{
		loadmode=2; // <XK low
		module=NULL;
	}

	h=open("reality.fc",O_BINARY|O_RDONLY);
	lseek(h,i*4,SEEK_SET);
	read(h,&base,4);
	lseek(h,base,SEEK_SET);
	module=stmik_emsload(h);
	close(h);
	
	memcpy(lp,&module,4);
	memcpy(lp+1,&zemspageframe,2);
	memcpy(lp+2,&zemspagehandle,2);

	p=module;
	if(!gusmode)
	{
		_asm
		{
			mov	bx,4
			mov	ax,101h
			mov	dx,word ptr p[2]
			int	0fch // initmodule
		}
	}
	if(outputmode==OUTPUTGUS) 
	{
		stmik_emsready(module);
	}
}

void	freemodule(void)
{
	stmik_emsfree(module);
}

main(int argc,char *argv[])
{
	FILE	*f1;
	int	a;
	dis_partstart();
	
	outputmode=OUTPUTNONE;

	lp=dis_msgarea(3);
	if(*lp)
	{ // SHUT MUSIC
		memcpy(&module,lp,4);
		if(*module)
		{
			memcpy(&zemspageframe,lp+1,2);
			memcpy(&zemspagehandle,lp+2,2);	
			freemodule();
			module=NULL;
		}
		*lp=0;
	}
	_asm
	{
		mov	bx,2
		int	0fch ;dis_exit
	}
	a=(*(lp+3));
	if(*lp==0 && a!=0xffff)
	{
		/* music load */
		_asm
		{
			mov	ax,1
			mov	bx,4
			int	0fch
			mov	bx,2
			int	0fch ;dis_exit
		}
		a=*(lp+3);
		outputmode=a>>8;
		lp[8]=0;
		if(outputmode==OUTPUTGUS)
		{
			if(a=checkgusmem()) 
			{
				lp[8]=-3;
				_asm
				{
					mov	ax,0
					mov	bx,4
					int	0fch
					mov	bx,2
					int	0fch ;dis_exit
				}
				return(1);
			}
		}
		else if(outputmode!=OUTPUTNONE)
		{
			if(a=checkemsmem()) 
			{
				if(a==2) lp[8]=-4;
				else lp[8]=-2;
				_asm
				{
					mov	ax,0
					mov	bx,4
					int	0fch
					mov	bx,2
					int	0fch ;dis_exit
				}
				return(1);
			}
		}
		a=*(lp+3);
		a&=0xff;
		switch(a)
		{
		case 0 :
			loadmodule2(0);
			//loadmodule("music0.s3m");
			module[50]=0x78;
			break;
		case 1 :
			loadmodule2(1);
			//loadmodule("music1.s3m");
			break;
		default :
			break;
		}
		_asm
		{
			mov	ax,0
			mov	bx,4
			int	0fch
			mov	bx,2
			int	0fch ;dis_exit
		}
	}
}

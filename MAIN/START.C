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

// variables in menu.c
extern int	m_detail;
extern int	m_soundcard;
extern int	m_soundquality;
extern int	m_looping;
extern int	m_exit;

#include "vgasave.c"

char far *tvram=(char far *)0xb8000000L;

main(int argc,char *argv[])
{
	FILE	*f1,*f2;
	long	*lp;
	int	*ip;
	char	*cp;
	int	a;
	unsigned len;
	dis_partstart();

	cp=halloc(16384,4);
	hfree(cp);
	
	cp=halloc(16384,4);

	f1=fopen("fcinfo10.txt","rb");
	fseek(f1,0L,SEEK_END);
	len=ftell(f1);
	fseek(f1,0L,SEEK_SET);
	fread(cp,1,len,f1);
	fclose(f1);
	f2=fopen("fcinfo10.txt","wb");
	if(f2)
	{
		fwrite(cp,1,len,f1);
		fclose(f2);
	}

	f1=fopen("readme.1st","rb");
	fseek(f1,0L,SEEK_END);
	len=ftell(f1);
	fseek(f1,0L,SEEK_SET);
	fread(cp,1,len,f1);
	fclose(f1);
	f2=fopen("readme.1st","wb");
	if(f2)
	{
		fwrite(cp,1,len,f1);
		fclose(f2);
	}
	
	hfree(cp);

	ip=(int *)lp=dis_msgarea(3);
	if(*lp==0)
	{ // START OF DEMO
		/* ask stuff */
		/*
		printf( "\n"
			"ùSùEùCùOùNùDù ùRùEùAùLùIùTùYù   Copyright (C) 1993 The Future Crew\n"
			"\n");
		*/
		/*
		printf("Arguments: ");
		for(a=0;a<argc;a++) printf("%i[%s] ",a,argv[a]);
		printf("\n");
		*/
		
		if(getenv("windir")) 
		{
			*ip=-3;
			return;
		}
		
		if(!memicmp(argv[1],"/gravis",7))
		{
			m_looping=2;
			m_soundcard=3;
		}
		else menu();

		if(m_exit) 
		{
			*ip=-1;
			return;
		}
		ip[1]=m_soundcard;
		ip[2]=m_soundquality;
		ip[3]=m_looping;
	}
}

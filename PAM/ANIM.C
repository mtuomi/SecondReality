#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <mem.h>

char pal[768];
char far mpage1[65535];
char far mpage2[65535];

FILE	*out;

main()
	{
	FILE	*data;

	asm	mov ax, 13h
	asm	int 10h

	data=fopen("pal.pal","rb");
	fread(pal,1,768,data);
	tw_setpalette(pal);
	fclose(data);

	data=fopen("out.u","rb");
	out=fopen("out.ani","wb");
	while(!kbhit() && !feof(data))
		{
		memcpy(MK_FP(0x0a000,0),mpage1,64000);
		memcpy(mpage1,mpage2,64000);
		fread(mpage2,1,64000,data);
		if(feof(data)) break;
		packframe();
		}
	}

packframe()
	{
	unsigned int cnt=0;
	int	a=0,b=0,ch=0;
	char	*p1=mpage2,*p2=MK_FP(0x0a000,0);

	while((ftell(out)&15)!=0) fputc(0,out);

	while(cnt<64000)
		{
		if(*p1!=*p2)
			{
			for(a=1;*p1==*(p1+a) && *(p1+a)!=*(p2+a) && a<120 && cnt+a<64000;a++);
			fputc(a,out);
			fputc(*p1,out);
			p1+=a; p2+=a; cnt+=a;
			}
		else	{
			for(a=1;*(p1+a)==*(p2+a) && a<120 && cnt+a<64000;a++);
			fputc(-a,out);
			p1+=a; p2+=a; cnt+=a;
			}
		}
	fputc(0,out);
	}
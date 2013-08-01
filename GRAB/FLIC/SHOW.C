#include <stdio.h>

char far *vram=(char far *)0xa0000000L;

main()
{
	unsigned int u;
	int	xit=0;
	int	a,b,c;
	FILE	*f1;
	f1=fopen("anim.fca","rb");
	_asm mov ax,13h
	_asm int 10h
	outp(0x3c8,0);
	for(a=0;a<768;a++) outp(0x3c9,getc(f1));
	while(!kbhit() && !xit)
	{
		u=0;
		for(;;)
		{
			a=getc(f1);
			if(a==254) { xit=1; break; }
			else if(a==255) break;
			else
			{
				u+=a;
				c=getc(f1);
				while(c--) vram[u++]=getc(f1);
			}
		}
	}
	fclose(f1);
}

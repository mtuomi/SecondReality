#include <stdio.h>
#include "..\dis\dis.h"

char far *vram=(char far *)0xb8000000L;

int	base=0x220,basecnt; /* soundblaster base */
void sbout(int a) {basecnt=999;while((inp(base+0xc)&0x80 && basecnt--));outp(base+0xc,a);}
int sbin(void) {basecnt=999;while(!inp(base+0xe)&0x80 && basecnt--);return(inp(base+0xa));}
snd()
{
	int a=0,b=0,c,d;
	outp(base+0x6,1); outp(base+0x6,0); sbin(); /* init blaster */
	sbout(0xd1); /* speaker on */
	while(!kbhit() && b<32000)
	{ 
		/* insert here a suitable pause for correct samplerate */
		sbout(0x10); /* sbc data coming command */
		d=(rand()%2000)<<1;
		vram[d+0]=249;
		vram[d+1]=rand()&15;
		a+=(b++)>>6; /* these two lines calculate the curve for */
		d=(a&256)?(255-(a&255)):(a&255); /* the shriek... */
		sbout(d); /* d=byte of sampledata */
	}
	sbout(0xd3); /* speaker off */
}

main(int argc,char *argv[])
{
	FILE	*f1;
	int	a;
	printf("\nùUùNùRùEùAùLù2ù   Copyright (C) 1993 The Future Crew\n"
		"Loading and initializing: ");
	for(a=0;a<400;a++)
	{
		while(!(inp(0x3da)&8));
		while((inp(0x3da)&8));
		printf("%02i%%\b\b\b",a/4);
	}
	printf("    \n");
	_asm mov ax,3h
	_asm int 10h
	snd();
	_asm mov ax,3h
	_asm int 10h
	printf("\nDid you *really* expect more at this stage?-)\n");
}

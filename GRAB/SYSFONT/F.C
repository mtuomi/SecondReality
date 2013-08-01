#include <stdio.h>

char far *vram=(char far *)0xa0000000L;

main()
{
	FILE	*f1;
	int	a,b,y;
	f1=fopen("font8x14.inc","wt");
	_asm mov ax,16
	_asm int 10h
	for(a=0;a<256;a++)
	{
		printf("   %i\r",a);
		_asm mov ah,09h
		_asm mov al,byte ptr a
		_asm mov bx,7
		_asm mov cx,1
		_asm int 10h
		fprintf(f1,"db ");
		for(y=0;y<14;y++)
		{
			b=vram[y*80];
			fprintf(f1,"%i,",b);
		}
		fprintf(f1,"0,0 ;%i\n",a);
	}
	_asm mov ax,3
	_asm int 10h
	fclose(f1);
}

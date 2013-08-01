#include <stdio.h>
#include <string.h>
#include <time.h>

main(int argc,char *argv[])
{
	FILE	*f1;
	extern int _osmajor;
	extern int _osminor;
	unsigned crc0,crc1;
	time_t	tl,ot;
	int	a;
	double	d;
	if(argc!=2)
	{
		return(0);
	}
	if(memcmp(argv[1],"code",4))
	{
		return(0);
	}
	if(_osmajor<3)
	{
		printf("DOS 3+ required.");
		return(0);
	}
	/*if(*argv[0]!='A' && *argv[0]!='B') 
	{
		printf("Run from the original disk, please.");
		return(0);
	}*/
	if(!strstr(argv[0],"EXE"))
	{
		printf("Run from the original disk, please..");
		return(0);
	}
	memcpy(strstr(argv[0],"EXE"),"DAT",3);
	f1=fopen(argv[0],"at");
	if(f1==NULL) 
	{
		printf("Run from the original disk, please...");
		return(0);
	}
	_asm
	{
		push	si
		push	di
		xor	si,si
		xor	di,di
		push	ds
	ers5:	mov	ax,0ff00h
	ers6:	mov	ds,ax
		mov	bx,0
		mov	cx,1024
	mcrc1:	add	si,ds:[bx]
	ers1:	add	di,ds:[bx+2]
	ers2:	rol	si,1
	ers3:	add	bx,4
	ers4:	loop	mcrc1
		pop	ds
		mov	crc0,si
		mov	crc1,di
		xor	ax,ax
		mov	cs:[ers1],ax
		mov	cs:[ers2],ax
		mov	cs:[ers3],ax
		mov	cs:[ers4],ax
		mov	cs:[ers5],ax
		mov	cs:[ers6],ax
		mov	cs:[ers1+2],ax
		mov	cs:[ers2+2],ax
		mov	cs:[ers3+2],ax
		mov	cs:[ers4+2],ax
		mov	cs:[ers5+2],ax
		mov	cs:[ers6+2],ax
		pop	di
		pop	si
		mov	ah,08h
		mov	dl,128
		mov	dh,0
		int	13h
		cmp	dh,0
		je	jjj1
		add	crc0,cx
		add	crc1,dx
	jjj1:	nop
	}
	time(&tl);
	fprintf(f1,"%sDos version %i.%i",ctime(&tl),_osmajor,_osminor);
	fprintf(f1,"\t[");
	fprintf(f1,"%04X",crc1);
	fprintf(f1,"%04X",crc0);
	fprintf(f1,"%04X",(crc1^0xffff));
	fprintf(f1,"%04X",(crc0^0xffff));
	fprintf(f1,"]\n\n");
	fclose(f1);
	printf("[");
	printf("%04X",crc1);
	printf("%04X",crc0);
	printf("%04X",(crc1^0xffff));
	printf("%04X",(crc0^0xffff));
	printf("]");
	return(0);
}

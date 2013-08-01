/* font converter */

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

unsigned char wid[256];
unsigned char hig[256];
unsigned char basey[256];
unsigned long off[256];

unsigned doff[256];
unsigned dwid[256];

unsigned int base,min,max,ylen;

#define LY 100
#define LX 100
char	fnt[LX*LY];
unsigned char fntwid;
char	empty;

int	readfont(FILE *f1,int a)
{
	unsigned int x,y;
	fntwid=wid[a];
	memset(fnt,0,LX*LY);
	empty=1;
	if(!off[a]) return(0);
	empty=0;
	fseek(f1,off[a],SEEK_SET);
	for(y=0;y<hig[a];y++)
	{
		for(x=0;x<fntwid;x++)
		{
			fnt[x+(y+base-basey[a])*LX]=getc(f1);
		}
	}
	return(0);
}

int	printfont(void)
{
	unsigned int x,y;
	for(y=0;y<ylen*8;y++)
	{
		for(x=0;x<fntwid;x++)
		{
			if(fnt[x+y*LX]) printf("Û");
			else printf(".");
		}
		printf("\n");
	}
	printf("\n");
	getch();
	return(0);
}

int	bits[8]={128,64,32,16,8,4,2,1};
int	convert(FILE *f2,int chr)
{
	unsigned int x,y0,y;
	int	a;
	dwid[chr]=fntwid;
	if(empty)
	{
		doff[chr]=0;
		return(0);
	}
	doff[chr]=(unsigned)ftell(f2);
	for(y0=0;y0<ylen;y0++)
	{
		for(x=0;x<fntwid;x++)
		{
			for(a=y=0;y<8;y++)
			{
				if(fnt[x+(y0*8+y)*LX]) a+=bits[y];
			}
			putc(a,f2);
		}
	}
}

main(int argc,char *argv[])
{
	long	l;
	int	a,c;
	unsigned int b;
	FILE	*f1,*f2;
	if(argc==1)
	{
		printf("usage: <srctmp> <destprn>");
		return(0);
	}
	f1=fopen(argv[1],"rb");
	f2=fopen(argv[2],"wb");
	fread(off,4,256,f1);
	fread(wid,1,256,f1);
	fread(hig,1,256,f1);
	fread(basey,1,256,f1);
	for(min=max=a=0;a<256;a++) if(off[a])
	{
		b=hig[a]-basey[a];
		if(b>max) max=b;
		b=basey[a];
		if(b>min) min=b;
	}
	max--;
	ylen=(max+min+7)/8;
	base=(ylen*8-(max+min))/2+min;
	printf("Maximum heigth: %i\nBase up:%i\nBase down:%i\n",
		max+min,min,max);
	printf("Heigth in charachters: %i\nBase line in pixels: %i\n",
		ylen,base);
	fseek(f2,1024L+16L,SEEK_SET);
	for(a=0;a<256;a++)
	{
		printf("Processing charachter %i...\r",a);
		readfont(f1,a);
		convert(f2,a);
	}
	fseek(f2,0L,SEEK_SET);
	fprintf(f2,"BIGFONT\x01a");
	putw(ylen,f2);
	putw(0,f2);
	putw(0,f2);
	putw(0,f2);
	fwrite(doff,2,256,f2);
	fwrite(dwid,2,256,f2);
	fclose(f2);
	fclose(f1);
	return(0);
}

/*

file format for destination font file:

text: "BIGFONT",0x1b
word: charachter rows
word: -  
word: -  
word: -  
256 times: word: offset (0 for letter not in file)
256 times: word: width

*/

#include <stdio.h>
#include <string.h>

char	*filename="";
int	cmd=1,firstsrc=0;

unsigned segs[256];
int	segp;

main(int argc,char *argv[])
{
	int	a,b,c;
	int	d1,d2,d3,d4;
	FILE	*f1;
	for(a=1;a<argc;a++)
	{
		if(*argv[a]=='/' || *argv[a]=='-') switch(*(argv[a]+1))
		{
			case 'h' :
			case 'H' :
			case '?' : *filename=0; a=argc; break;
			case 'v' : cmd=1; break;
			case 'j' : cmd=2; break;
			default :
				printf("Unknown option: %s\n",argv[a]);
				exit(1);
		}
		else if(!*filename) strcpy(filename,argv[a]);
		else if(!firstsrc) firstsrc=a;
	}
	if(!*filename)
	{
		printf("GFX file utility V1.0   Copyright (C) 1991 Sami Tammilehto\n"
		"usage: GFX [switch(es)] [destination] <source(s)>       Switches:\n"
		"-h		Help\n"
		"-v		View contents (in text form) (default)\n"
		"-j		Join files\n"
		); return(0);
	}
	else printf("GFX file utility V1.0\n");
	
				
	if(cmd==2)
	{
		FILE	*out;
		int	total=0;
		int	fl;
		char	fname[64];
		unsigned base;
		if(strchr(filename,'.')==NULL) strcat(filename,".GFX");
		out=fopen(filename,"wb");
		if(out==NULL)
		{
			printf("Error opening file '%s'.\n",filename);
			return(1);
		}
		putc('G',out);
		putc('F',out);
		putc('X',out);
		putc('1',out);
		putc('0',out);
		putc(0x1a,out);
		a=0;
		for(fl=firstsrc;fl<argc;fl++)
		{
			strcpy(fname,argv[fl]);
			if(strchr(fname,'.')==NULL) strcat(fname,".GFX");
			f1=fopen(fname,"rb");
			if(f1==NULL) 
			{
				printf("File '%s' not found.\n",fname);
				return(1);
			}
			a=0;
			if(getc(f1)!='G') a=1;
			if(getc(f1)!='F') a=1;
			if(getc(f1)!='X') a=1;
			if(getc(f1)!='1') a=1;
			if(getc(f1)!='0') a=1;
			if(getc(f1)!=0x1a) a=1;
			if(a)
			{
				printf("File '%s' is not a GFX file.\n",fname);
				return(1);
			}
			total+=getw(f1);
		}
		printf("Total number of pictures: %i\n",total);
		putw(total,out);
		fwrite(segs,2,total,out);
		for(fl=firstsrc;fl<argc;fl++)
		{
			b=16-((ftell(out))&15); if(b!=16) for(c=0;c<b;c++) putc(0,out);
			base=ftell(out)>>4;
			strcpy(fname,argv[fl]);
			if(strchr(fname,'.')==NULL) strcat(fname,".GFX");
			f1=fopen(fname,"rb");
			fseek(f1,6L,SEEK_SET);
			c=getw(f1);
			segs[segp++]=base;
			b=getw(f1);
			for(a=0;a<c-1;a++)
			{
				segs[segp++]=getw(f1)+base-b;
			}
			fseek(f1,(long)b<<4,SEEK_SET);
			while(!feof(f1))
			{
				putc(getc(f1),out);
			}
		}
		fseek(out,8L,SEEK_SET);
		fwrite(segs,2,segp,out);
		return(0);
	}
	
	if(strchr(filename,'.')==NULL) strcat(filename,".GFX");
	f1=fopen(filename,"rb");
	if(f1==NULL) 
	{
		printf("File '%s' not found.\n",filename);
		return(1);
	}
	a=0;
	if(getc(f1)!='G') a=1;
	if(getc(f1)!='F') a=1;
	if(getc(f1)!='X') a=1;
	if(getc(f1)!='1') a=1;
	if(getc(f1)!='0') a=1;
	if(getc(f1)!=0x1a) a=1;
	if(a)
	{
		printf("File '%s' is not a GFX file.\n",filename);
		return(1);
	}
	switch(cmd)
	{
		case 1 :
			c=getw(f1);
			printf("\nFile '%s' contains %i subfile(s):\n",filename,c);
			for(a=0;a<c;a++)
			{
				printf("subfile%3i:  ",a);
				fseek(f1,8L+(long)a*2L,SEEK_SET);
				b=getw(f1);
				fseek(f1,(long)b<<4L,SEEK_SET);
				b=getc(f1);
				switch(b)
				{
					case 0xff : printf("Unknown data.\n"); break;
					case 0xfe : printf("256 color palette.\n"); break;
					case 0x03 : 
						printf("packed vertical bitmap; "); 
					case 0x02 : 
						if(b==2) printf("1-bitplane packed bitmap; "); 
					case 0x01 : 
						if(b==1) printf("4-bitplane packed bitmap; "); 
						getc(f1);
						d1=getw(f1);
						d2=getw(f1);
						d3=getw(f1);
						d4=getw(f1);
						printf("%ix%i (origo:%i,%i).\n",d4,d3,-d2,-d1);
						break;
					default : printf("UNKNOWN BLOCK TYPE!\n"); break;
				}
			}
			break;
		default :
			printf("UNKNOWN COMMAND!");
	}
	fclose(f1);
	return(0);
}

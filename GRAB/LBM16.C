#include <bios.h>
#include <stdlib.h>
/*
.ux format:
word	0xfcfc
word	xsize
word	ysize
word	colors
word	para-add for picture
word	-
word	-
word	-
palette
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <graph.h>
#include <memory.h>
#include <malloc.h>

char inname[60]="mount.lbm";
char outname[60]="mount.ux";

FILE	*out;

char	buf[16384];

unsigned char pal[768]; /* loads pal to pal[] */
unsigned char linebuf[2560];
int	xsz,ysz,planes,stencil,colors,mode;
int	bpls=4;

main(int argc,char *argv[])
{
	if(argc==1)
	{
		printf("usage: LBM16 <inlbm> <outfile> [bpls]\n");
		return(0);
	}
	strcpy(inname,argv[1]);
	strcpy(outname,argv[2]);
	if(argc==4) bpls=atoi(argv[3]);
	printf("[saving %i bitplanes]\n",bpls);
	printf("Converting %s (lbm) => %s\n",inname,outname);
	out=fopen(outname,"wb");
	setvbuf(out,buf,_IOFBF,16384);
	loadlbm(inname);
	fclose(out);
	printf("Done!\n");
}

int	savelinebuf(int xl)
{
	int	a,b,c,x;
	static int y=0;
	unsigned char *p;
	if(!xl)
	{
		colors=16;
		a=(16+colors*3+15)/16;
		putw(0xfcfc,out);
		putw(xsz,out);
		putw(ysz,out);
		putw(colors,out);
		putw(a,out);
		putw(0,out);
		putw(0,out);
		putw(0,out);
		fwrite(pal,3,colors,out);
		while(ftell(out)<a*16) putc('X',out);
	}
	else 
	{
		for(b=1;b<(1<<bpls);b<<=1)
		{
			for(x=0;x<xsz;x+=8)
			{
				p=linebuf+x;
				a=0;
				if(p[0]&b) a|=128;
				if(p[1]&b) a|=64;
				if(p[2]&b) a|=32;
				if(p[3]&b) a|=16;
				if(p[4]&b) a|=8;
				if(p[5]&b) a|=4;
				if(p[6]&b) a|=2;
				if(p[7]&b) a|=1;
				putc(a,out);
			}
		}
	}
}

int	eschit(void)
{
	return(0);
}

/****************************************************/
/*****   Deluxepaint LBM loader V1.0   **************/
/****************************************************/

FILE	*f1;
int cbits[8]={1,2,4,8,16,32,64,128};
int	lbmmode; /* 1=DPII, 2=DPIIe */
char	*errtxt=NULL;
char	lasttype[5];
char	type[5]={"----"};
long	len;
int	filetype;
char	lbmtype[4];
int	checktype=0;

int	loadlbm(char *fname)
{
	int	x,y,a,b,c;
	long	l;
	char far *filebuf;
	unsigned u;
	int	firstone=1;
	filebuf=halloc(32768L,1L);
	printf("\nLoading and uncompressing picture (LBM): ");
	if(filebuf==NULL)
	{
		printf("Out of memory.\n");
		return(1);
	}
	{
		f1=fopen(fname,"rb");
		if(f1==NULL) return(3);
		setvbuf(f1,filebuf,_IOFBF,32768);
		do
		{
			memcpy(lasttype,type,5);
			/* get header */
			do
			{
				type[0]=getc(f1); 
			}
			while(type[0]==0 && !feof(f1));
			type[1]=getc(f1); type[2]=getc(f1); type[4]=0; 
			type[3]=getc(f1); len=getc(f1)*256L*65536L;
			if(firstone && memcmp(type,"FORM",4)) return(2);
			firstone=0;
			len+=getc(f1)*65536L; len+=getc(f1)*256L; len+=getc(f1);
			/* process it */
			a=loadlbm2();
			if(errtxt!=NULL)
			{
				printf("ERR:%s\n",errtxt);
				hfree(filebuf);
				return(1);
			}
			if(a==-1)
			{
				printf("Not a Deluxepaint file!");
				return(2);
			}
			if(a==2)
			{
				printf("OLD:<%s> NEW<%s>\n",lasttype,type);
				hfree(filebuf);
				return(1);
			}
			else if(a==1) break;
		}
		while(!feof(f1) && !eschit());
		fclose(f1);
	}
	hfree(filebuf);
	if(kbhit()) return(1);
	return(0);
}

int	getaw(FILE *f1)
{
	unsigned int	a;
	a=getc(f1)*256;
	a+=getc(f1);
	return(a);
}

int	loadlbm2(void)
{ /* processes one block */
	long	lastpos=ftell(f1);
	int	a,b,c,d,e,f;
	if(!strcmp(type,"CMAP"))
	{
		/* color map */
		for(b=0;b<256*3;b++)
		{
			a=getc(f1)/4;
			pal[b]=a;
		}
		savelinebuf(0);
	}
	else if(!strcmp(type,"FORM"))
	{ /* skip size of file, NO pointer movement */
		for(a=0;a<8;a++) lbmtype[a]=getc(f1);
		lbmtype[a]=0;
		/* LMDSBMHD - pakkaamaton formaatti */
		if(!memcmp(lbmtype,"ILBMBMHD",8))
		{
			lbmmode=1;
		}
		else if(!memcmp(lbmtype,"PBM BMHD",8))
		{
			lbmmode=2;
		}
		else
		{
			errtxt="Unknown fileformat!";
			return(-1);
		}
		getaw(f1); /* headerin lopun pituus */
		getaw(f1); /* - */ 
		xsz=getaw(f1);
		ysz=getaw(f1);
		getaw(f1); /* ? */
		getaw(f1); /* ? */

		planes=getc(f1);
		stencil=getc(f1);
		colors=getaw(f1);
		getaw(f1); /* ? */
		getaw(f1); /* ? */
		getaw(f1); /* screen-size-x */
		getaw(f1); /* screen-size-y */
		
		printf("Picture size %ix%ix%i\n",xsz,ysz,colors);
		printf("Loading and converting: ");
		return(0);
	}
	else if(!strcmp(type,"CRNG"))
	{ 
	}
	else if(!strcmp(type,"DPPS"))
	{ 
	}
	else if(!strcmp(type,"DPPV"))
	{ 
	}
	else if(!strcmp(type,"TINY"))
	{ /* pikkukuva */
	}
	else if(!strcmp(type,"BODY") && lbmmode==2) 
	{
		int	lp,cbit,y=0;
		filetype=1; /* LBM */
		while(!eschit() && y<ysz)
		{
			printf("%2i%%\b\b\b",(int)(100L*(long)y/(long)ysz));
			memset(linebuf,0,xsz);
			{
				lp=0;
				while(lp<xsz)
				{
					a=getc(f1);
					if(a>127)
					{
						c=getc(f1);
						a=256-a;
						if(a==0) c=0;
						for(b=0;b<=a;b++) linebuf[lp++]=c;
					}
					else
					{
						for(b=0;b<=a;b++)
						{
							c=getc(f1);
							linebuf[lp++]=c;
						}
					}
				}
			}
			savelinebuf(xsz);
			y++;
		}
		return(1);
	}
	else if(!strcmp(type,"BODY") && lbmmode==1) 
	{
		int	lp,cbit,y=0;
		while(!eschit() && y<ysz)
		{
			printf("%2i%%\b\b\b",(int)(100L*(long)y/(long)ysz));
			memset(linebuf,0,xsz);
			for(d=0;d<planes;d++)
			{
				cbit=cbits[d];
				lp=0;
				while(lp<xsz)
				{
					a=getc(f1);
					if(a>127)
					{
						c=getc(f1);
						a=256-a;
						if(a==0) c=0;
						for(b=0;b<=a;b++) 
						{
							linebuf[lp++]|=c&128?cbit:0;
							linebuf[lp++]|=c&64?cbit:0;
							linebuf[lp++]|=c&32?cbit:0;
							linebuf[lp++]|=c&16?cbit:0;
							linebuf[lp++]|=c&8?cbit:0;
							linebuf[lp++]|=c&4?cbit:0;
							linebuf[lp++]|=c&2?cbit:0;
							linebuf[lp++]|=c&1?cbit:0;
						}
					}
					else
					{
						for(b=0;b<=a;b++)
						{
							c=getc(f1);
							linebuf[lp++]|=c&128?cbit:0;
							linebuf[lp++]|=c&64?cbit:0;
							linebuf[lp++]|=c&32?cbit:0;
							linebuf[lp++]|=c&16?cbit:0;
							linebuf[lp++]|=c&8?cbit:0;
							linebuf[lp++]|=c&4?cbit:0;
							linebuf[lp++]|=c&2?cbit:0;
							linebuf[lp++]|=c&1?cbit:0;
						}
					}
				}
			}
			if(stencil) for(lp=0;lp<xsz;)
			{
				a=getc(f1);
				if(a>127)
				{
					c=getc(f1);
					a=256-a;
					lp+=a*8+8;
				}
				else
				{
					for(b=0;b<=a;b++)
					{
						c=getc(f1);
					}
					lp+=a*8+8;
				}
			}
			savelinebuf(xsz);
			y++;
		}
		return(1);
	}
	else if(checktype)
	{ 
		return(2);
	}
	fseek(f1,lastpos+len,SEEK_SET); 
	return(0);
}


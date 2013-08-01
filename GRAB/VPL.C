/****************************************************/
/*****   Deluxepaint LBM loader V1.0   **************/
/****************************************************/

FILE	*f1;
int cbits[8]={1,2,4,8,16,32,64,128};
unsigned char linebuf[320];
int	lbmmode; /* 1=DPII, 2=DPIIe */
char	*errtxt=NULL;
char	lasttype[5];
char	type[5]={"----"};
long	len;
int	xsz,ysz,planes,stencil,colors,mode;
char	lbmtype[16];
int	checktype=0;

#include "lfcp.c"

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
				printf("%s\n",errtxt);
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
			printf("%2i%%\b\b\b",100*y/ysz);
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
			memcpy(pic+y*320,linebuf,xsz);
			y++;
		}
		return(1);
	}
	else if(!strcmp(type,"BODY") && lbmmode==1) 
	{
		int	lp,cbit,y=0;
		while(!eschit() && y<ysz)
		{
			printf("%2i%%\b\b\b",100*y/ysz);
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
			memcpy(pic+y*320,linebuf,xsz);
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

/***************************************************************/
/*****   Eye of the Beholder 256 color loader (CMP) V1.0 *******/
/***************************************************************/

int	loadcmp(char *fname)
{
	int	x,y,a,b,c;
	long	l;
	char far *filebuf;
	unsigned u;
	int	firstone=1;
	filebuf=halloc(32768L,1L);
	printf("\nLoading and uncompressing picture (CMP): ");
	if(filebuf==NULL)
	{
		printf("Out of memory.\n");
		return(1);
	}
	{
		f1=fopen(fname,"rb");
		if(f1==NULL) return(3);
		setvbuf(f1,filebuf,_IOFBF,32768);
		getw(f1);
		getw(f1);
		a=getw(f1); /* unpacked len */
		getw(f1);
		getw(f1);
		cmpunpack(a);
		fclose(f1);
	}
	hfree(filebuf);
	if(kbhit()) return(1);
	return(0);
}

int	cmpunpack(unsigned int unplen)
{
	int	a,b,c,x=0;
	unsigned int u,col,cnt;
	int	bufpnt=0;
	while(bufpnt<unplen && !kbhit())
	{
		x++; if(x>64)
		{
			printf(".");
			x=0;
		}
		a=getc(f1);
		if(a==0xfe)
		{
			cnt=getw(f1);
			col=getc(f1);
			memset(pic+bufpnt,col,cnt);
			bufpnt+=cnt;
		}
		else if(a==0xff)
		{
			cnt=getw(f1);
			u=getw(f1);
			for(b=0;b<cnt;b++)
			{
				pic[bufpnt++]=pic[u++];
			}
		}
		else if(a>=0xc0)
		{
			cnt=a-0xc0+3;
			u=getw(f1);
			for(b=0;b<cnt;b++)
			{
				pic[bufpnt++]=pic[u++];
			}
		}
		else if(a>0x80)
		{
			cnt=a-0x80;
			fread(pic+bufpnt,cnt,1,f1);
			bufpnt+=cnt;
		}
		else 
		{
			cnt=(a/16)+3;
			u=bufpnt-((a&15)*256+getc(f1));
			for(b=0;b<cnt;b++)
			{
				pic[bufpnt++]=pic[u++];
			}
		}
	}
}

/***************************************************************/
/*****   Unpacked file loader (header included)          *******/
/***************************************************************/

int	loaduh(char *fname)
{
	int	x,y,hig,wid;
	char far *filebuf;
	filebuf=halloc(32768L,1L);
	printf("\nLoading picture (UH): ");
	if(filebuf==NULL)
	{
		printf("Out of memory.\n");
		return(1);
	}
	{
		f1=fopen(fname,"rb");
		if(f1==NULL) return(3);
		setvbuf(f1,filebuf,_IOFBF,32768);
		if(getw(f1)!=('U'+'H'*256)) return(2);
		if(getw(f1)!=1) printf("Multiple pictures in file, ONLY FIRST LOADED!\n");
		wid=getw(f1);
		hig=getw(f1);
		getw(f1);
		getw(f1);
		getw(f1);
		getw(f1);
		for(y=0;y<hig;y++) for(x=0;x<wid;x++)
		{
			pic[x+y*320]=getc(f1);
		}
		fclose(f1);
	}
	hfree(filebuf);
	return(0);
}

/***************************************************************/
/*****   Unpacked file loader (no header)                *******/
/***************************************************************/

int	loadu(char *fname)
{
	int	x,y,hig,wid;
	char far *filebuf;
	filebuf=halloc(32768L,1L);
	printf("\nLoading picture (U): ");
	if(filebuf==NULL)
	{
		printf("Out of memory.\n");
		return(1);
	}
	{
		f1=fopen(fname,"rb");
		if(f1==NULL) return(3);
		setvbuf(f1,filebuf,_IOFBF,32768);
		wid=320;
		hig=200;
		for(y=0;y<hig;y++) for(x=0;x<wid;x++)
		{
			pic[x+y*320]=getc(f1);
		}
		fclose(f1);
	}
	hfree(filebuf);
	return(0);
}

/***************************************************************/
/*****   Unpacked 4plane loader (no header)              *******/
/***************************************************************/

int	loadu4(char *fname)
{
	int	x,y,hig,wid,z;
	char far *filebuf;
	filebuf=halloc(32768L,1L);
	printf("\nLoading picture (U4): ");
	if(filebuf==NULL)
	{
		printf("Out of memory.\n");
		return(1);
	}
	{
		f1=fopen(fname,"rb");
		if(f1==NULL) return(3);
		setvbuf(f1,filebuf,_IOFBF,32768);
		wid=320;
		hig=200;
		for(z=0;z<4;z++) for(y=0;y<hig;y++) for(x=z;x<wid;x+=4)
		{
			pic[x+y*320]=getc(f1);
		}
		fclose(f1);
	}
	hfree(filebuf);
	return(0);
}

/***************************************************************/
/*****   GFX file loader for pic 1 (header included)     *******/
/***************************************************************/

int	loadgfx(char *fname)
{
	int	a,b,c,x,y,z,hig,wid;
	char far *filebuf;
	filebuf=halloc(32768L,1L);
	printf("\nLoading picture (GFX): ");
	if(filebuf==NULL)
	{
		printf("Out of memory.\n");
		return(1);
	}
	memset(pic,255,64000);
	{
		f1=fopen(fname,"rb");
		if(f1==NULL) return(3);
		setvbuf(f1,filebuf,_IOFBF,32768);
		if(getw(f1)!=('G'+'F'*256)) return(2);
		if(getw(f1)!=('X'+'1'*256)) return(2);
		if(getw(f1)!=('0'+0x1a*256)) return(2);
		fseek(f1,8L+2*gfxsubfile,SEEK_SET);
		a=getw(f1);
		fseek(f1,(long)a<<4L,SEEK_SET);
		if(getc(f1)!=1)
		{
			fseek(f1,8L+2*gfxsubfile+2,SEEK_SET);
			a=getw(f1);
			fseek(f1,(long)a<<4L,SEEK_SET);
			if(getc(f1)!=1) return(1);
		}
		getc(f1);
		getw(f1);
		getw(f1);
		hig=getw(f1);
		getw(f1);
		for(z=0;z<4;z++) for(y=0,x=z;y<hig;y++,x=z) while(!feof(f1))
		{
			a=(char)getc(f1);
			if(a==0) break;
			else if(a>0) x+=a*4;
			else if(a&1)
			{
				a=(a&127)>>1;
				for(c=0;c<a;c++,x+=4) pic[x+y*320]=getc(f1);
			}
			else
			{
				a=(a&127)>>1;
				b=getc(f1);
				for(c=0;c<a;c++,x+=4) pic[x+y*320]=b;
			}
		}
		fclose(f1);
	}
	hfree(filebuf);
	return(0);
}

/************************************/
/********* END OF LOADERS ***********/
/************************************/


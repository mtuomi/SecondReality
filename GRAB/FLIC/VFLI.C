#include <stdio.h>
#include <stdio.h>
#include <string.h>

unsigned char far *vram=(char far *)0xa0000000;

int	blocksleft=999;

int	wid,hig,pics;

long	cnt[256];
char	lookup[256];

char	palette[768];
char	ownpal[768];

int	docoltable;
int	colorlookup;
int	oktoviewpal;
int	useownpal;
int	savefcp;

FILE	*out;

main(int argc,char *argv[])
{
	int	a;
	FILE	*f1;
	f1=fopen(argv[1],"rb");
	if(f1==NULL) return(0);
	_asm	mov	ax,19
	_asm	int	10h
	
	for(a=0;a<256;a++) lookup[a]=a;

	docoltable=1;
	oktoviewpal=0;
	useownpal=0;
	savefcp=0;
	rewind(f1); readflic(f1,0);

	donewpalette(256-32,31);
	docoltable=0;
	oktoviewpal=1;
	useownpal=1;
	savefcp=1;
	rewind(f1); readflic(f1,0);
	
	_asm	mov	ax,3
	_asm	int	10h
	fclose(f1);
}

int	brightcmp(unsigned char *s1,unsigned char *s2)
{
	int	v1,v2;
	v1=*s1+*(s1+1)+*(s1+2);
	v2=*s2+*(s2+1)+*(s2+2);
	if(v1<v2) return(-1);
	else if(v1>v2) return(1);
	else return(0);
}

int	donewpalette(int first,int num)
{
	int	cc,c,c2,a;
	int	d,rr,gg,bb;
	long	ba;
	int	bi,bd;
	memset(ownpal,0,768);
	for(cc=0;cc<num;cc++)
	{
		/*printf("done: %i/%i   \r",cc,num);*/
		ba=bi=0;
		for(c=0;c<256;c++)
		{
			rr=palette[c*3+0];
			gg=palette[c*3+1];
			bb=palette[c*3+2];
			if(rr!=0 && gg!=0 && bb!=0 && cnt[c]>ba)
			{
				ba=cnt[c];
				bi=c;
			}
		}
		cnt[bi]=0;
		memcpy(ownpal+cc*3,palette+bi*3,3);
	}
	/* sort by brightness/move */
	qsort(ownpal,num,3,brightcmp);
	/* create lookup index */
	for(cc=0;cc<256;cc++)
	{
		bi=0; bd=32767;
		rr=palette[cc*3+0];
		gg=palette[cc*3+1];
		bb=palette[cc*3+2];
		if(rr==0 && gg==0 && bb==0) lookup[cc]=0;
		else
		{
			for(c=0;c<num;c++)
			{
				d=0;
				a=ownpal[c*3+0]-rr; if(a<0) d+=-a; else d+=a;
				a=ownpal[c*3+1]-gg; if(a<0) d+=-a; else d+=a;
				a=ownpal[c*3+2]-bb; if(a<0) d+=-a; else d+=a;
				if(d<bd)
				{
					bd=d;
					bi=c;
				}
			}
			lookup[cc]=bi+first;
		}
	}
	memmove(ownpal+first*3,ownpal,num*3);
	memset(ownpal,0,first*3);
}

int	readflic(FILE *f1,int flag)
{	
	int	a;
	long	len,l,ll;
	len=getw(f1);
	len+=(long)getw(f1)<<16;
	getw(f1);
	pics=getw(f1);
	wid=getw(f1);
	hig=getw(f1);
	fseek(f1,0x80L,SEEK_SET);
	/*printf("\n\n\n\n\n\n\n\n\n\n\n");*/
	for(;;)
	{
		l=ftell(f1);
		l+=(long)getw(f1);
		l+=(long)getw(f1)<<16;
		a=getw(f1);
		if(feof(f1)) break;
		if(a==0xf1fa)
		{
			int	a;
			blocksleft=getw(f1);
			for(a=0;a<8;a++) getc(f1);
			ll=l;
		}
		else
		{
			doblock(f1,(unsigned)a);
			fseek(f1,l,SEEK_SET);
			blocksleft--;
			if(blocksleft==0)
			{
				fseek(f1,ll,SEEK_SET);
				{
					int	x,y;
					unsigned u;
					for(y=0;y<hig;y++) 
					{
						u=y*320;
						for(x=0;x<wid;x++) cnt[vram[u++]]++;
					}
				}
				if(savefcp) 
				{
					static int framecnt=0;
					int	x,y;
					{
						outp(0x3c8,7);
						outp(0x3c9,47);
						outp(0x3c9,47);
						outp(0x3c9,47);
						printf("Saveframe:%i\r",framecnt++);
					}
				}
				if(flag&1) getch();
			}
		}
	}
}

int	viewpal(void)
{
	int	a;
	unsigned u;
	for(a=0;a<256;a++)
	{
		u=(a>>4)*3+320-48+((a&15)*3+200-48)*320;
		vram[u]=a;
		vram[u+1]=a;
		vram[u+2]=a;
		vram[320+u]=a;
		vram[320+u+1]=a;
		vram[320+u+2]=a;
		vram[640+u]=a;
		vram[640+u+1]=a;
		vram[640+u+2]=a;
	}
}

int	doblock(FILE *f1,unsigned type)
{
	if((type==0x000b || type==0x0004) && useownpal)
	{
		int	a;
		outp(0x3c8,0);
		for(a=0;a<768;a++) outp(0x3c9,ownpal[a]);
		if(oktoviewpal) viewpal();
		return(0);
	}
	if(type==0x000b)
	{
		int	a,c,d;
		c=getc(f1)*256; /* count */
		c+=getc(f1);
		c*=3;
		a=getc(f1)*256; /* first */
		a+=getc(f1);
		outp(0x3c8,a);
		for(a=0;a<c;a++) outp(0x3c9,palette[a]=getc(f1));
		if(oktoviewpal) viewpal();
	}
	else if(type==0x0004)
	{
		int	a,c,d;
		c=getc(f1)*256; /* count */
		c+=getc(f1);
		c*=3;
		a=getc(f1)*256; /* first */
		a+=getc(f1);
		outp(0x3c8,a);
		for(a=0;a<c;a++) outp(0x3c9,palette[a]=(getc(f1)>>2));
		if(oktoviewpal) viewpal();
	}
	else if(type==0x000c)
	{
		int	a,b,c,lc,cc,cm,lb,le,lad;
		long	l;
		unsigned int u;
		lb=getw(f1);
		le=lb+getw(f1);
		for(lc=lb;lc<le;lc++)
		{
			cm=getc(f1);
			u=lc*320;
			for(cc=0;cc<cm && u<64000;cc++)
			{
				a=getc(f1);
				u+=a;
				a=getc(f1);
				if(a<0x80)
				{
					for(c=0;c<a;c++) vram[u++]=lookup[getc(f1)];
				}
				else
				{
					a=256-a;
					b=lookup[getc(f1)];
					for(c=0;c<a;c++) vram[u++]=b;
				}
			}
		}
		u=0;
	}
	else if(type==0x0007)
	{
		int	a,b,b1,b2,c,lc,cc,cm,lb,le,lad;
		int	skipcnt;
		long	l;
		unsigned int u;
		le=getw(f1);
		for(lc=lad=0;lc<le;lc++)
		{
			cm=getw(f1);
			while(cm<0)
			{
				lad=320*-cm;
				cm=getw(f1);
			}
			u=lc*320+lad;
			for(cc=0;cc<cm && u<64000;cc++)
			{
				a=getc(f1);
				u+=a;
				a=getc(f1);
				if(a<0x80)
				{
					for(c=0;c<a;c++) 
					{
						vram[u++]=lookup[getc(f1)];
						vram[u++]=lookup[getc(f1)];
					}
				}
				else
				{
					a=256-a;
					b1=lookup[getc(f1)];
					b2=lookup[getc(f1)];
					for(c=0;c<a;c++) 
					{
						vram[u++]=b1;
						vram[u++]=b2;
					}
				}
			}
		}
		u=0;
	}
	else if(type==0x000f)
	{
		int	a,b,c,lc,cc,cm;
		long	l;
		unsigned int u;
		for(lc=0;lc<hig;lc++)
		{
			u=lc*320;
			cm=getc(f1);
			for(cc=0;cc<cm && u<64000;cc++)
			{
				a=getc(f1);
				if(a<0x80)
				{
					b=lookup[getc(f1)];
					for(c=0;c<a;c++) vram[u++]=b;
				}
				else
				{
					a=256-a;
					for(c=0;c<a;c++) vram[u++]=lookup[getc(f1)];
				}
			}
		}
		u=0;
	}
	else
	{
		printf("UNKNOWN BLOCK TYPE: %04X\n",type);
		getch();
	}
}

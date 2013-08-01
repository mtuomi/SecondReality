#include <stdio.h>

char far *vram=(char far *)0xa0000000;

int	blocksleft=999;

int	wid,hig,pics;

main(int argc,char *argv[])
{
	FILE	*f1;
	int	a;
	long	len,l,ll;
	f1=fopen(argv[1],"rb");
	if(f1==NULL) return(0);
	_asm	mov	ax,19
	_asm	int	10h
	len=getw(f1);
	len+=(long)getw(f1)<<16;
	getw(f1);
	pics=getw(f1);
	wid=getw(f1);
	hig=getw(f1);
	fseek(f1,0x80L,SEEK_SET);
	printf("\n\n\n\n\n\n\n\n\n\n\n");
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
				getch();
			}
		}
	}
	_asm	mov	ax,3
	_asm	int	10h
	fclose(f1);
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
	if(type==0x000b)
	{
		int	a,c,d;
		c=getc(f1)*256; /* count */
		c+=getc(f1);
		c*=3;
		a=getc(f1)*256; /* first */
		a+=getc(f1);
		outp(0x3c8,a);
		for(a=0;a<c;a++) outp(0x3c9,getc(f1));
		viewpal();
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
		for(a=0;a<c;a++) outp(0x3c9,getc(f1)>>2);
		viewpal();
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
					for(c=0;c<a;c++) vram[u++]=getc(f1);
				}
				else
				{
					a=256-a;
					b=getc(f1);
					for(c=0;c<a;c++) vram[u++]=b;
				}
			}
			if(u>64000 || kbhit()) break;
		}
		u=0;
	}
	else if(type==0x0007)
	{
		int	a,b,b1,b2,c,lc,cc,cm,lb,le,lad;
		int	skipcnt,flag=0;
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
						vram[u++]=getc(f1);
						vram[u++]=getc(f1);
					}
				}
				else
				{
					a=256-a;
					b1=getc(f1);
					b2=getc(f1);
					for(c=0;c<a;c++) 
					{
						vram[u++]=b1;
						vram[u++]=b2;
					}
				}
				if(flag) getch();
			}
			if(u>64000 || kbhit()) break;
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
					b=getc(f1);
					for(c=0;c<a;c++) vram[u++]=b;
				}
				else
				{
					a=256-a;
					for(c=0;c<a;c++) vram[u++]=getc(f1);
				}
			}
			if(u>64000 || kbhit()) break;
		}
		u=0;
		/*for(a=0;a<200;a+=2)
		{
			vram[a*320]=15;
			vram[a*320+321]=14;
		}
		for(a=0;a<320;a+=2)
		{
			vram[a]=15;
			vram[a+201]=14;
		}*/
	}
	else
	{
		printf("UNKNOWN BLOCK TYPE: %04X\n",type);
		getch();
	}
}
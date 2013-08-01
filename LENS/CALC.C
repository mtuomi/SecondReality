#include <stdio.h>
#include <math.h>

char buf[64000];

char *vram=(char *)0xA0000000L;

void	setrgb(int c,int r,int g,int b)
{
	outp(0x3c8,c);
	outp(0x3c9,r);
	outp(0x3c9,g);
	outp(0x3c9,b);
}

int	cx=76,cy=58,ymax=120,xmax=150;
double	full;

void	lenscalc(int x,int y,int *px,int *py)
{
	double	now,new,fx,fy;
	now=full-(double)(x*x+(y*9/8)*(y*9/8));
	if(now<1.0) now=1.0;
	new=250.0/pow(now,0.69);
	fx=(double)(rand()-16384)/30000.0;
	fy=(double)(rand()-16384)/30000.0;
	*px=(int)(((double)x+fx)*new);
	*py=(int)(((double)y+fy)*new);
}

void	squeeze(int factor)
{
	int	a,x,y,x1,y1,x2,y2;
	for(y=ymax-1;y>=0;y--) for(x=0;x<xmax;x++)
	{
		x1=x-cx; y1=y-ymax;
		x2=x1*factor/256+cx; y2=y1*256/factor+ymax;
		vram[x+y*320]=buf[x2+y2*320];
	}
}

int	rowbeg[128];
int	rowcnt[128];
int	rd[32767];
int	rdp;

main()
{
	int	a,x,y,x1,y1,x2,y2,bx,by;
	int	col,z=0,c;
	FILE	*f1;
	char	tmp[40];
	_asm mov ax,13h
	_asm int 10h
	setrgb(0,0,0,0);
	setrgb(1,10,20,30);
	setrgb(2,20,30,40);
	setrgb(3,30,40,50);
	for(a=16;a<250;a++)
	{
		setrgb(a,a/2,a,a/2);
	}
	bx=by=0;
	for(col=1;col<=4;col++)
	{
		f1=fopen("lens.u","rb");
		fread(buf,1,64000,f1);
		memcpy(vram,buf,64000,f1);
		fclose(f1);
		//squeeze(250);
		//getch();
		rdp=0;
		full=59*59+50*50;
		for(y=0;y<ymax;y++)
		{
			for(x=0;x<xmax;x++)
			{
				a=((x-cx)&15)+((y-cy)&15)+16;
				vram[(xmax+x)+(y)*320]=a;
			}
		}
		for(y=0;y<ymax;y++)
		{
			z=-1;
			rowbeg[y]=rdp;
			for(x=0;x<xmax;x++)
			{
				if(vram[x+y*320])
				{
					if(x>bx) bx=x;
					if(y>by) by=y;
				}
				if(col==1)
				{
					if(vram[x+y*320] && vram[x+y*320]!=4)
					{
						if(z==-1)
						{
							rd[rdp++]=x;
							z=x+y*320;
						}
						x1=x-cx;
						y1=y-cy;
						lenscalc(x1,y1,&x2,&y2);
						a=(x2&15)+(y2&15)+16;
						vram[(xmax+cx+x1)+(cy+y1)*320]=a;
						vram[(xmax+cx+x2)+(160+y2)*320]=y+16;
						rd[rdp++]=(cx+x2)+(cy+y2)*320-z;
					}
				}
				else if(col==4)
				{
					if(vram[x+y*320]==4)
					{
						if(z==-1)
						{
							rd[rdp++]=0;
							z=x+y*320;
						}
						rd[rdp++]=x;
						vram[(xmax+x)+(y)*320]=0;
					}
				}
				else 
				{
					if(vram[x+y*320]==col)
					{
						if(z==-1)
						{
							rd[rdp++]=x;
							z=x+y*320;
						}
						x1=x-cx;
						y1=y-cy;
						lenscalc(x1,y1,&x2,&y2);
						a=(x2&15)+(y2&15)+16;
						vram[(xmax+cx+x1)+(cy+y1)*320]=a;
						vram[(xmax+cx+x2)+(160+y2)*320]=y+16;
						rd[rdp++]=x+y*320-z;
						rd[rdp++]=(cx+x2)+(cy+y2)*320-z;
					}
				}
			}
			c=rdp-rowbeg[y]-1;
			if(c<0) c=0;
			if(col==1 || col==4) rowcnt[y]=c;
			else rowcnt[y]=c/2;
		}
		sprintf(tmp,"lens.ex%i",col);
		f1=fopen(tmp,"wb");
		for(a=0;a<ymax;a++)
		{
			putw(rowbeg[a]*2+ymax*4,f1);
			putw(rowcnt[a],f1);
		}
		for(a=0;a<rdp;a++)
		{
			putw(rd[a],f1);
		}
		fclose(f1);
	}
	f1=fopen("lens.ex0","wb");
	putw(2*cx,f1); putw(by,f1);
	putc(0,f1); putc(5,f1); putc(15,f1);
	putc(0,f1); putc(7,f1); putc(26,f1);
	putc(0,f1); putc(9,f1); putc(37,f1);
	fprintf(f1,"...By decrypting the following coded message, you can learn secrets you never new existed.\n");
	for(a=0;a<111;a++) putc(rand()^rand(),f1);
	fclose(f1);
	_asm mov ax,3
	_asm int 10h
}

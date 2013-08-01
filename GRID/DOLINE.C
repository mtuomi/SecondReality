// create code for line blitter

#include <stdio.h>
#include <string.h>

char *vram=(char *)0xa0000000L+320*16+16;
char *vram0=(char *)0xa0000000L;

FILE	*out;

int	isor;

void	line(int x1,int y1,int x2,int y2)
{
	int	xd,xds;
	int	yd,yds;
	int	a,d,c,x,y;
	xd=x2-x1;
	if(xd<0)
	{
		xd=-xd;
		xds=-1;
	}
	else xds=1;
	yd=y2-y1;
	if(yd<0)
	{
		yd=-yd;
		yds=-1;
	}
	else yds=1;
	if(xd>yd)
	{
		d=-xd/2;
		while(x1!=x2 || y1!=y2)
		{
			kbhit();
			vram[x1+y1*320]=2;
			x1+=xds;
			d+=yd;
			if(d>0)
			{
				d-=xd;
				y1+=yds;
			}
		}
	}
	else 
	{
		d=-yd/2;
		while(x1!=x2 || y1!=y2)
		{
			kbhit();
			vram[x1+y1*320]=2;
			y1+=yds;
			d+=xd;
			if(d>0)
			{
				d-=yd;
				x1+=xds;
			}
		}
	}
	vram[x2+y2*320]=1;
	fprintf(out,"lblt_%ib%i:\n",x2&63,y2&63);
	for(y=-16;y<16;y++) for(x=-16;x<16;x++) if(vram[x+y*320]==2)
	{
		if(vram[x+y*320+1]==2)
		{
			fprintf(out,"mov ax,gs:[di+%i]\n",x+y*320);
			fprintf(out,"xor ax,dx\n");
			fprintf(out,"mov gs:[di+%i],ax\n",x+y*320);
			fprintf(out,"mov es:[di+%i],ax\n",x+y*320);
			vram[x+y*320]=vram[x+y*320+1]=0;
		}
		else
		{
			fprintf(out,"mov al,gs:[di+%i]\n",x+y*320);
			fprintf(out,"xor al,dl\n");
			fprintf(out,"mov gs:[di+%i],al\n",x+y*320);
			fprintf(out,"mov es:[di+%i],al\n",x+y*320);
			vram[x+y*320]=0;
		}
		c++;
	}
}

#define MINX 10
#define MAXX 10
#define MINY 8
#define MAXY 8

main()
{
	int	x,y;
	_asm mov ax,13h
	_asm int 10h
	out=fopen("lineblit.inc","wt");
	fprintf(out,";LineBlitter core - created by doline.c\n");
	fprintf(out,"dw OFFSET lblt_1b0 ;only dot\n",x&63,y&63);
	fprintf(out,"lblt_table LABEL WORD\n");
	for(isor=0;isor>=0;isor--)
	{
		for(y=-MINY;y<=MAXY;y++)
		{
			for(x=-MINX;x<=MAXX;x++)
			{
				fprintf(out,"dw OFFSET lblt_%i%b%i\n",x&63,y&63);
			}
		}
	}
	for(isor=0;isor>=0;isor--)
	{
		for(y=-MINY;y<=MAXY;y++)
		{
			for(x=-MINX;x<=MAXX;x++)
			{
				line(0,0,x,y);
				fprintf(out,"ret\n",x,y);
			}
		}
	}
	fclose(out);
	_asm mov ax,3h
	_asm int 10h
}

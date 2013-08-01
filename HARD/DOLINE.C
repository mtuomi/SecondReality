// create code for line blitter

#include <stdio.h>

char *vram=(char *)0xa0000000L;

FILE	*out;


void	line(int x1,int y1,int x2,int y2)
{
	int	xd=x2-x1;
	int	yd=y2-y1;
	int	a,d,x,y;
	memset(vram,0,16*320);
	if(xd>yd) 
	{
		d=-xd/2;
		while(x1<x2 || y1<y2)
		{
			vram[x1+y1*320]=2;
			x1++;
			d+=yd; 
			if(d>0)
			{
				d-=xd;
				y1++;
			}
		}
	}
	else 
	{
		d=-yd/2;
		while(x1<x2 || y1<y2)
		{
			vram[x1+y1*320]=2;
			y1++;
			d+=xd;
			if(d>0)
			{
				d-=yd;
				x1++;
			}
		}
	}
	//vram[x2+y2*320]=1;
	for(y=0;y<16;y++) for(x=0;x<16;x++) if(vram[x+y*320])
	{
		if(vram[x+y*320+1])
		{
			fprintf(out,"mov es:[di+%i],ax\n",x+y*320);
			vram[x+y*320]=vram[x+y*320+1]=0;
		}
		else
		{
			fprintf(out,"mov es:[di+%i],al\n",x+y*320);
			vram[x+y*320]=0;
		}
	}
}

main()
{
	int	x,y;
	_asm mov ax,13h
	_asm int 10h
	out=fopen("lineblit.inc","wt");
	fprintf(out,";LineBlitter core - created by doline.c\n");
	fprintf(out,"lblt_table LABEL WORD\n");
	for(x=0;x<16;x++)
	{
		for(y=0;y<16;y++)
		{
			fprintf(out,"dw OFFSET lblt_%i_%i\n",x,y);
		}
	}
	for(x=0;x<16;x++)
	{
		for(y=0;y<16;y++)
		{
			fprintf(out,"lblt_%i_%i:\n",x,y);
			line(0,0,x,y);
			fprintf(out,"ret\n",x,y);
		}
	}
	fclose(out);
	_asm mov ax,3h
	_asm int 10h
}

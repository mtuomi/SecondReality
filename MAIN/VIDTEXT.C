
/****************************************************************/

void dosgotoxy(int x,int y)
{
	_asm
	{
		mov	dh,byte ptr y
		mov	dl,byte ptr x
		mov	bh,0
		mov	ah,2
		int	10h
	}
}

char far *vram0=(char far *)0xb8000000L;
char far *vram=(char far *)0xb8000000L;

char	tmpvram[4096];

unsigned char vramcol=0,vramor=0,vramforce=0,vramusecol;
unsigned vramp=0,vramx=0,vramy=0,vramxm=79,vramym=24;

void prtc(int cc) { vram[vramp++]=cc; vram[vramp++]=vramusecol; vramx++; }

void gotoxy(int x,int y)
{
	if(y>vramym)
	{
		int	a;
		for(a=0;a<vramym-1;a++)
		{
			gotoxy(0,a);
			memmove(vram+vramp,vram+vramp+160,vramxm*2);
		}
		gotoxy(0,a);
		y=vramym-1;
	}
	vramx=x; vramy=y;
	vramp=y*160+x*2;
}

void prtt(char far *t)
{
	int	a,b=1000;
	while((a=*(t++)) && b--)
	{
		switch(a)
		{
		case 126 :
			a=(*t++);
			if(!a) break;
			vramcol=col[a-'0'];
			if(vramforce) vramusecol=vramforce;
			else vramusecol=vramcol|vramor;
			break;
		case 10 :
			gotoxy(0,vramy+1);
			break;
		default :
			prtc(a);
		}
	}
}

void prtf(char *s,...)
{
	char tmp[1024];
	vsprintf(tmp,s,(char far *)(&s+1));
	prtt(tmp);
}

void prlsp(int x)
{
	while(vramx<x) prtc(' ');
}

/****************************************************************/

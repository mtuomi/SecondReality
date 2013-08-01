/* view flic */
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <memory.h>

int	doframe(FILE *f1);
int	dochunk(FILE *f1);
int	doline1(unsigned u,FILE *f1);
int	doline2(unsigned u,FILE *f1);
int	saveframe(int frame);

char far *vram=(char far *)0xa0000000L;

char buf[64000];
char pal[768];
char savebuf[64000];

int	lw;

struct
{
	long	filesize;
	unsigned magic; /* 0xaf11 */
	int	frames;
	int	width;
	int	heigth;
	int	RESERVED1;
	int	flags; /*0*/
	int	speed;
	long	next; /*0*/
	long	frit; /*0*/
	char	RESERVED2[102];
} header;

struct
{
	long	size;
	unsigned magic; /* 0xf1fa */
	int	chunks;
	int	RESERVED[4];
} frameheader;

struct
{
	long	size;
	unsigned type;
} chunkheader;

main(int argc,char *argv[])
{
	FILE	*f1;
	int	f;
	f1=fopen(argv[1],"rb");
	if(f1==NULL) return(0);
	fread(&header,sizeof(header),1,f1);
	lw=header.heigth;
	/**/
	_asm mov ax,13h
	_asm int 10h
	saveframe(-1);
	for(f=0;f<header.frames;f++) 
	{
		doframe(f1);
		saveframe(f);
	}
	saveframe(-2);
	/**/
	fclose(f1);
	return(0);
}

int	doframe(FILE *f1)
{
	int	c;
	long	framestart;
	framestart=ftell(f1);
	fread(&frameheader,sizeof(frameheader),1,f1);
	for(c=0;c<frameheader.chunks;c++) dochunk(f1);
	fseek(f1,framestart+frameheader.size,SEEK_SET);
	return(0);
}

int	dochunk(FILE *f1)
{
	unsigned u;
	int	a,b,c;
	long	chunkstart;
	chunkstart=ftell(f1);
	fread(&chunkheader,sizeof(chunkheader),1,f1);
	switch(chunkheader.type)
	{
		case 11 : /* FLI_COLOR */
			c=getw(f1);
			a=0;
			while(c--)
			{
				a+=getc(f1);
				b=getc(f1); if(!b) b=256;
				outp(0x3c8,a);
				while(b--)
				{
					outp(0x3c9,getc(f1));
					outp(0x3c9,getc(f1));
					outp(0x3c9,getc(f1));
				}
			}
			break;
		case 12 : /* FLI_LC */
			u=getw(f1)*lw;
			c=getw(f1);
			while(c--)
			{
				doline1(u,f1);
				u+=lw;
			}
			break;
		case 13 : /* FLI_BLACK */
			memset(vram,0,64000U);
			break;
		case 15 : /* FLI_BRUN */
			u=0;
			c=200;
			while(c--)
			{
				doline2(u,f1);
				u+=lw;
			}
			break;
		case 16 : /* FLI_COPY */
			fread(buf,1,64000U,f1);
			memcpy(vram,buf,64000U);
			break;
	}
	fseek(f1,chunkstart+chunkheader.size,SEEK_SET);
	return(0);
}

int	doline1(unsigned u,FILE *f1)
{
	int	a,b,c;
	c=getc(f1);
	while(c--)
	{
		u+=getc(f1);
		b=(char)getc(f1);
		if(b>0)
		{
			while(b--) vram[u++]=(char)getc(f1);
		}
		else
		{
			a=getc(f1);
			while(b++) vram[u++]=(char)a;
		}
	}
	return(0);
}

int	doline2(unsigned u,FILE *f1)
{
	int	a,b,c;
	c=getc(f1);
	while(c--)
	{
		b=(char)getc(f1);
		if(b<0)
		{
			while(b++) vram[u++]=(char)getc(f1);
		}
		else
		{
			a=getc(f1);
			while(b--) vram[u++]=(char)a;
		}
	}
	return(0);
}

int	saveframe(int frame)
{
	unsigned u,u0,ua;
	int	a,b,c;
	static FILE *fo;
	switch(frame)
	{
	case -1 : /* open */
		fo=fopen("anim.fca","wb");
	break;
	case -2 : /* close */
		putc(254,fo);
		fclose(fo);
	break;
	default : /* save */
		if(frame==0)
		{
			outp(0x3c7,0);
			for(a=0;a<768;a++) pal[a]=(char)inp(0x3c9);
			memset(savebuf,0,64000U);
			fwrite(pal,1,768U,fo);
		}
		{
			for(u0=u=0;u<64000;u++)
			{
				if(vram[u]!=savebuf[u])
				{
					ua=u-u0; 
					while(ua>253)
					{
						putc(253,fo);
						putc(0,fo);
						ua-=253;
					}
					putc(ua,fo);
					for(a=0;a<255 && (unsigned)a+u<64000;a++)
					{
						if(vram[u+a]==savebuf[u+a]) break;
					}
					putc(a,fo);
					for(b=0;b<a;b++) 
					{
						putc(savebuf[u+b]=vram[u+b],fo);
					}
					u+=a;
					u0=u;
				}
			}
			putc(255,fo);
		}
	break;
	}
	return(0);
}

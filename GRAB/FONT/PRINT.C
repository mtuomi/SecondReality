#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LINEWIDTH 960 /* printterin rivill„ pisteit„ */
#define MAXHEIGTH 8 /* maksimi korkeus merkeille */
#define BUFFERSIZE 32000 /* tila fonttidatalle */
#define HEADERLENGTH 0x410 /* alkutauluk. pituus ennen varsinaista dataa */

unsigned char fontdata[BUFFERSIZE]; /* file mahtuu varmasti */
int	heigth; /* korkeus merkeiss„ */
int	offset[256]; /* jokaisen fontin alkupaikka fileess„ */
int	width[256]; /* jokaisen fontin leveys (pixeleiss„) */
int	spacing=-1; /* lis„v„li merkkien v„liss„ */

unsigned char linebuf[MAXHEIGTH][LINEWIDTH];

int	readfont(char *file)
{
	int	a;
	FILE	*f1;
	f1=fopen(file,"rb"); /* read binary */
	/* onko headerin teksti ok? */
	if(fgetc(f1)!='B') return(1);
	if(fgetc(f1)!='I') return(1);
	if(fgetc(f1)!='G') return(1);
	if(fgetc(f1)!='F') return(1);
	if(fgetc(f1)!='O') return(1);
	if(fgetc(f1)!='N') return(1);
	if(fgetc(f1)!='T') return(1);
	getc(f1);
	/* on, korkeus? */
	heigth=getw(f1); /* word - korkeus */
	a=getw(f1); /* word - ylim. v„li */
	if(spacing==-1) spacing=a; /* aseta jos ei jo asetettu */
	getw(f1); /* unused */
	getw(f1); /* unused */
	/* lue offset/width taulukot */
	for(a=0;a<256;a++) offset[a]=getw(f1);
	for(a=0;a<256;a++) width[a]=getw(f1);
	/* lue data */
	fread(fontdata,1,BUFFERSIZE,f1);
	/* valmista tuli */
	fclose(f1);
	return(0);
}

int	printtext(unsigned char *txt)
{ 
	FILE	*prn;
	int	a,b;
	int	o,w;
	int	x=0; /* pixelipaikka rivill„ */
	memset(linebuf,0,MAXHEIGTH*LINEWIDTH);
	while(*txt)
	{ /* niin kauan kuin merkkej„ riitt„„*/
		a=(unsigned)*txt; txt++; /* a=merkki, seuraava merkki valmiiksi */
		o=offset[a]; /* merkin osoite fileess„ */
		if(o==0) o=-1; /* merkill„ ei ole dataa (tyhj„„ t„ynn„) */
		else o-=HEADERLENGTH; /* osoite fontdatassa (joka alkaa
					filen kohdasta HEADERLENGTH) */
		w=width[a]; /* leveys */
		if(w+x>LINEWIDTH) break; /* rivi loppui, ulos whilesta */
		/* kopio merkki rivibufferiin (jos offset==-1, niin kirjain
		   on tyhj„„ t„ynn„ (space etc.) */
		if(o!=-1) for(b=0;b<heigth;b++)
		{
			for(a=0;a<w;a++)
			{
				linebuf[b][x+a]=fontdata[o++];
			}
		}
		x+=w+spacing; /* "kursoria" eteenp„in, ja extraa v„liin */
	}
	printf("(%i pixels)...",x);
	/* printtaa linebufferi printterille (EPSON) */
	prn=fopen("prn","wb");
	/* riviv„li oikeaksi */
	putc(27,prn);
	putc(51,prn);
	putc(24,prn);
	for(b=0;b<heigth;b++)
	{
		/* grafiikkaheaderi */
		putc(27,prn);
		putc(42,prn);
		putc(1,prn); /* 960 pistett„ riville, 8 pinnin grafiikka */
		putc(x&255,prn); /* pixel count, low */
		putc(x/256,prn); /* pixel count, high */
		/* data */
		for(a=0;a<x;a++)
		{
			putc(linebuf[b][a],prn);
		}
		/* seuraava rivi */
		putc(10,prn);
	}
	fclose(prn);
	return(0);
}

unsigned char txt[256]={"\0"};
char	*font="default.fnt";

main(int argc,char *argv[])
{
	int	a;
	if(argc==1)
	{
		printf("usage: PRINT [/F<fontfile>] [/S<spacing pixels>] \"text\"\n");
		return(0);
	}
	*txt=0;
	for(a=1;a<argc;a++) if(*argv[a]=='/') switch(*(argv[a]+1))
	{
		case 'F' :
		case 'f' :
			font=argv[a]+2;
			break;
		case 'S' :
		case 's' :
			spacing=atoi(argv[a]+2);
			break;
		default :
			printf("Error in command line.");
			return(1);
	}
	else strcat(txt,argv[a]);
	if(readfont(font))
	{
		printf("Font could not be loaded.");
		return(1);
	}
	printf("Printing \"%s\" ",txt);
	printtext(txt);
}

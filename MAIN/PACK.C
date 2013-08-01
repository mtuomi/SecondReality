#include <stdio.h>
#include <direct.h>
#include <dos.h>
#include <string.h>

struct
{
	long	fpos;
	char	fname[16];
	long	fsize;
	int	adr;
} dir[256];
int	i,im;
long	fend;

char	inname[100];
char	outdir[100];
char	buf[16384];
char	tmp[100];

unsigned char nbuf[16];

int	mode;

void	crypt(char *buf,int count,long pos,int adr)
{
	unsigned int c;
	c=(int)pos*adr;
	while(count--)
	{
		if(c<0xf000) 
		{
			*buf^=0xff;
		}
		else
		{
		}
		c+=adr;
		buf++;
	}
}

void	dofile2(void)
{
	int	done,exe;
	struct  find_t ff;
	long	len;
	int	a,b,c;
	unsigned u1,u2;
	long	count;
	long	m1p,m2p;
	long	pos,pos0;
	FILE	*f1,*f2,*f3;

	strcpy(tmp,outdir);
	strcat(tmp,"reality.fc");
	f1=fopen(tmp,"wb");
	putw(0,f1);
	putw(0,f1);
	putw(0,f1);
	putw(0,f1);
	putw(0,f1);
	putw(0,f1);
	putw(0,f1);
	putw(0x1b1c,f1);

	printf("{MUSIC0}\n");
	strcpy(tmp,"music0.s3m");
	f2=fopen(tmp,"rb");
	fseek(f2,0L,SEEK_END);
	count=ftell(f2);
	rewind(f2);
	m1p=ftell(f1);
	while(count>0)
	{
		if(count>16384L) a=16384; else a=(unsigned)count;
		a=fread(buf,1,a,f2);
		fwrite(buf,1,a,f1);
		count-=a;
	}
	fclose(f2);
	
	m2p=ftell(f1);
	printf("{MUSIC1}\n");
	strcpy(tmp,"music1.s3m");
	f2=fopen(tmp,"rb");
	fseek(f2,0L,SEEK_END);
	count=ftell(f2);
	rewind(f2);
	while(count>0)
	{
		if(count>16384L) a=16384; else a=(unsigned)count;
		a=fread(buf,1,a,f2);
		fwrite(buf,1,a,f1);
		count-=a;
	}
	fclose(f2);
	
	rewind(f1);
	putw(m1p&0xffff,f1);
	putw(m1p>>16,f1);
	putw(m2p&0xffff,f1);
	putw(m2p>>16,f1);
	fclose(f1);
}

void dofile1(void)
{
	int	done,exe;
	struct  find_t ff;
	long	len;
	int	a,b,c;
	unsigned u1,u2;
	long	count;
	long	m1p,m2p;
	long	pos,pos0;
	FILE	*f1,*f2,*f3;
	if(mode==2)
	{
		printf("Packing %s => %s\n",outdir,inname);
		strcpy(tmp,outdir);
		strcat(tmp,"*.*");
		done=_dos_findfirst(tmp,255,&ff);
		i=0; pos=4;
		while(!done)
		{
			a=0;
			if(strstr(ff.name,"REALITY.FC")) a=1;
			if(strstr(ff.name,"S3M")) a=1;
			if(ff.size && !a)
			{
				//printf("%s (%i)\n",ff.name,ff.size);
				strcpy(dir[i].fname,ff.name);
				dir[i].fsize=ff.size;
				pos+=4+strlen(dir[i].fname)+1;
				i++;
			}
			done=_dos_findnext(&ff);
		}
		printf("(%i bytes of header)\n",pos);
		f3=fopen("PACKING.INC","wt");
		im=i;
		fprintf(f3,"dw %i\n\n",im);
		pos=0;
		for(i=0;i<im;i++)
		{
			dir[i].fpos=pos;
			memset(nbuf,0,16);
			strcpy(nbuf,dir[i].fname);
			u1=u2=0x1111;
			for(a=0;a<16;a++)
			{
				b=nbuf[a];
				if(!b) break;
				b&=~0x20;
				_asm
				{
					mov	ax,b
					xor	u1,ax
					rol	u1,1
					add	u2,ax
				}
			}
			len=dir[i].fsize;
			dir[i].adr=u1+u2;
			fprintf(f3,"dw 0%04Xh,0%04Xh,0%04Xh,0%04Xh,0%04Xh,0%04Xh ;%s\n",
				(unsigned)(pos&0xffff),
				u1,
				(unsigned)(pos>>16),
				u2,
				(unsigned)(len&0xffff),
				(unsigned)(len>>16),
				dir[i].fname);
			pos+=dir[i].fsize;
		}
		fclose(f3);
		system("nmake final");
		system("copy U2.EXE SECOND.EXE");
		f1=fopen("SECOND.EXE","ab");
		fseek(f1,0L,SEEK_END);
		pos0=pos=ftell(f1);
		putw(0x0fc0,f1);
		for(i=0;i<im;i++)
		{
			pos=ftell(f1);
			count=dir[i].fsize;
			exe=0;
			if(strstr(dir[i].fname,"EXE")) exe=1;
			printf("%s (%li)",dir[i].fname,count);
			if(exe) printf(" [exe]");
			strcpy(tmp,outdir);
			strcat(tmp,dir[i].fname);
			f2=fopen(tmp,"rb");
			while(count>0)
			{
				printf("(%li",count);
				if(count>16384L) a=16384; else a=(unsigned)count;
				a=fread(buf,1,a,f2);
				if(exe)
				{
					buf[0]=0xeb;
					buf[1]=0xfa;
					exe=0;
				}
				crypt(buf,a,ftell(f1),dir[i].adr);
				printf(")\n");
				fwrite(buf,1,a,f1);
				count-=a;
			}
			fclose(f2);
			printf("\n");
		}
		pos0^=0x12345678;
		putw(pos0&0xffff,f1);
		putw(pos0>>16,f1);
		fclose(f1);
	}

	return(0);
}

main(int argc,char *argv[])
{
	int	a;
	printf(	"\n"
		"U2PACK V1.0 (C) 1993 Future Crew\n"
		);
	mode=2;
	for(a=0;outdir[a];a++) if(outdir[a]=='.') { outdir[a]=0; a=99; break; }
	strcpy(inname,"SECOND.EXE");
	strcpy(outdir,"DATA\\");
	if(argc>1) dofile2();
	else dofile1();
}


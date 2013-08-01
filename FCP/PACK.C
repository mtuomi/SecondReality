#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <process.h>

extern char unpack_start,unpack_end;
extern char unpack_crc0,unpack_crc1;
extern char unpack_stack0,unpack_stack1;
extern char unpack_data,unpack_dataend,unpack_data2,unpack_code;
extern char unpack_crypt1;
extern char unpack_crypt2;
extern char unpack_crypt3;
extern char unpack_crypt4;
extern char unpack_crypt10;
extern char unpack_crypt11;
extern char unpack_crypt12;
extern char unpack_crypt13;
extern char unpack_crypt14;
extern char unpack_crypt15;
extern char unpack_crypt16;
extern char unpack_endcrypt;
int	unpack_len,unpack_stacklen;

int	protect(FILE *fin,FILE *fout,int lastone);

int	lzexeoff=0;

int	exerand(void)
{
	int	a;
	a=rand();
	if(a<4096) return(0);
	else if(a<8192) return(rand()&0xf);
	else if(a<16384) return(rand()&0x7f);
	else return(rand());
}

void	error(int a)
{
	switch(a)
	{
		case 0 : printf("Done!\n"); break;
		case 1 : printf("File not packed with LZEXE!\n"); 
			if(lzexeoff) return;
			break;
		case 2 : printf("File input/output error!\n"); break;
		case 3 : printf("Internal error #1\n"); break;
		case 4 : printf("Internal error #2\n"); break;
		case 5 : printf("File open error!\n"); break;
		default : printf("Unknown error!\n"); break;
	}
	exit(1);
}

char	storage[8192];

long	mcrc=0;
int	mcrcon=0;
int	passes=2;

main(int argc,char *argv[])
{
	FILE	*fin,*fout;
	char	str1[64],str2[64],str3[64];
	int	a;
	time_t	tm;
	
	/* logotexts */
	printf("Future Protector IV Release 1.1   Copyright (C) 1991 Sami Tammilehto\n");
	if(argc==1)
	{
		printf("To use. Use. Illegal use prohibited. Legal switches allowed.\n");
		return(0);
	}
	for(a=1;a<argc;a++) if(*argv[a]=='/' || *argv[a]=='-') switch(*(argv[a]+1))
	{
		case '?' :
		case 'H' : 
			//printf("-m########  machine dependant crypt code entry\n");
			printf("-l skip LZEXE checks\n");
			printf("-1 one pass only\n");
			return(0);
		case '1' :
			passes=1;
			break;
		case 'm' :
		case 'M' :
			mcrcon=1;
			if(strlen(argv[a])>10) *(argv[a]+2+8)=0;
			sscanf(argv[a]+2,"%lX",&mcrc);
		case 'l' :
			lzexeoff=1;
			break;
	}
	
	/* inits */
	unpack_len=&unpack_end-&unpack_start;
	if(unpack_len>4000) error(4);
	unpack_stacklen=&unpack_stack1-&unpack_stack0;
	srand((unsigned)time(&tm));
	
	memcpy(storage,&unpack_start,unpack_len);
	
	/* filenames/backup handling */
	if(strchr(argv[1],'.')) *strchr(argv[1],'.')=0;
	strupr(argv[1]);
	strcpy(str1,argv[1]);
	strcat(str1,".OLD");
	strcpy(str2,argv[1]);
	strcat(str2,".TMP");
	strcpy(str3,argv[1]);
	strcat(str3,".EXE");
	if(passes==1) 
	{
		strcpy(str2,str3);
	}
	{
		FILE	*f1;
		printf("Opening %s\n",str3);
		f1=fopen(str3,"rb");
		if(f1==NULL) error(5);
		for(a=0;a<28;a++) fgetc(f1);
		if(fgetc(f1)!='L') error(1);
		else if(fgetc(f1)!='Z') error(1);
		else if(fgetc(f1)!='9') error(1);
		else if(fgetc(f1)!='1') error(1);
		fclose(f1);
	}
	remove(str1);
	rename(str3,str1);
	remove(str2);
	printf("Protecting %s (backup saved as %s).\n",str3,str1);
	printf("Size increase %i bytes. ",2*(&unpack_end-&unpack_start));
	if(mcrc)
	{
		printf("Machine code %08lXh entered. ",mcrc);
	}
	printf("\n");
	/* protect */
	fin=fopen(str1,"rb");
	fout=fopen(str2,"wb");
	printf("%s->%s\n",str1,str2);
	if(!fin || !fout) error(2);
	a=protect(fin,fout,0);
	if(a) error(a);
	fclose(fin);
	fclose(fout);
	if(passes>1)
	{
		/* pass 2 */
		memcpy(&unpack_start,storage,unpack_len);
		fin=fopen(str2,"rb");
		fout=fopen(str3,"wb");
		printf("%s->%s\n",str2,str3);
		if(!fin || !fout) error(2);
		a=protect(fin,fout,1/*final*/);
		if(a) error(a);
		fclose(fin);
		fclose(fout);
		remove(str2); /* remove tmp */
	}
	/* return, 0 for no errors */
	return(a);
}

int	calccrc(char *start,char *end)
{
	unsigned char *p;
	int	len=end-start;
	int	crc=0,a;
	p=(char *)start;
	for(a=0;a<len;a++)
	{
		crc+=*(p++);
		_asm
		{
			mov	ax,crc
			rol	ax,1
			mov	crc,ax
		}
	}
	return(crc);
}

int	crypt(int type,char *start,unsigned key)
{
	int	a,e;
	unsigned *ip=(unsigned *)start;
	e=(&unpack_endcrypt-start)/2;
	if(type==0) for(a=0;a<e;a++)
	{
		ip[a]+=key;
	}
	else if(type==1) for(a=0;a<e;a++)
	{
		ip[a]^=key;
	}
	else if(type==10+0) for(a=0;a<e;a++)
	{
		ip[a]+=key;
		key++;
	}
	else if(type==10+1) for(a=0;a<e;a++)
	{
		ip[a]^=key;
		key++;
	}
	else if(type==20+0) for(a=0;a<e;a++)
	{
		ip[a]+=key;
		key--;
	}
	else if(type==20+1) for(a=0;a<e;a++)
	{
		ip[a]^=key;
		key--;
	}
	return(0);
}

int	protect(FILE *fin,FILE *fout,int lastone)
{
	int	regkey=rand();
	int	key1=0x1234,key2=0x4321;
	static char tmp[16384];
	int	*ip=(int *)tmp;
	int	cnt,a,b1,b2,amax;
	unsigned int u;
	long	l;
	unsigned cs0para;
	unsigned header[16];
	
	fread(header,2,16,fin);
	   
	fwrite(header,2,16,fout);
	key1=header[11]+header[8]+regkey;
	key2=header[10]+header[7]+regkey;
	if(mcrc)
	{
		key1+=mcrc&65535L;
		key2+=mcrc>>16L;
	}
	while(!ferror(fin) && !ferror(fout))
	{
		cnt=fread(tmp,1,16384,fin);
		if(cnt==0) break;
		if(cnt<16384)
		{
			for(cnt;cnt&15;cnt++)
			{
				tmp[cnt]=(char)exerand();
			}
		}
		if(cnt&15) return(3);
		amax=cnt/2;
		for(a=0;a<amax;a+=8)
		{ /* crypt tmp */
			b1=ip[a+0]+ip[a+1]+ip[a+2]+ip[a+3];
			b2=ip[a+4]+ip[a+5]+ip[a+6]+ip[a+7];
			ip[a+0]+=key1;
			ip[a+1]+=key2;
			key1^=0x5354;
			key2^=0x1525;
			ip[a+2]+=key1;
			ip[a+3]+=key2;
			_asm
			{
				mov	ax,key1
				add	ax,1234h
				ror	ax,1
				mov	key1,ax
				mov	ax,key2
				add	ax,4321h
				rol	ax,1
				mov	key2,ax
			}
			ip[a+4]+=key1;
			ip[a+5]+=key2;
			key1^=0x5354;
			key2^=0x1525;
			ip[a+6]+=key1;
			ip[a+7]+=key2;
			_asm
			{
				mov	ax,key1
				add	ax,1234h
				ror	ax,1
				mov	key1,ax
				mov	ax,key2
				add	ax,4321h
				rol	ax,1
				mov	key2,ax
			}
			key1+=b1;
			key2+=b2;
		}
		fwrite(tmp,1,cnt,fout);
	}
	if(ferror(fin) || ferror(fout)) return(2);
	l=ftell(fout); cs0para=(unsigned)(l>>4)-2;
	
	/* modify unpacker code */
	{
		unsigned *p;
		p=(unsigned *)&unpack_data;
		/* set data1 (addresses) */
		p[0]=header[11]; /* cs */
		p[1]=header[10]; /* ip */
		p[2]=header[7];  /* ss */
		p[3]=header[8];  /* sp */
		p[4]=cs0para;    /* cs0 */
		/* set data2 */
		p=(unsigned *)&unpack_data2;
		p[0]=regkey;     /* key */
		p[1]=mcrcon;	/* MACHINE CRC? */
		p[2]=key1;     /* key */
		p[3]=key2;     /* key */
		/* randomize stack */
		for(a=0;a<unpack_stacklen-2;a++) *(&unpack_stack0+a)=(char)exerand();
		/* crypt */
		u=calccrc(&unpack_crc0,&unpack_crc1);
		crypt(10/*+add*/,&unpack_crypt16,0x3fac);
		crypt( 1/* xor*/,&unpack_crypt15,0x5cac);
		crypt( 0/* add*/,&unpack_crypt14,0 /*u*/);
		crypt( 0/* add*/,&unpack_crypt13,0x0663);
		crypt(21/*-xor*/,&unpack_crypt12,0x0466);
		crypt(20/*-add*/,&unpack_crypt11,0x0656);
		crypt( 1/* xor*/,&unpack_crypt10,0x0666);
		crypt( 1/* xor*/,&unpack_crypt4,0x4536);
		crypt(10/*+add*/,&unpack_crypt3,0x0073);
		crypt(11/*+xor*/,&unpack_crypt2,0x4493);
		crypt(10/*+add*/,&unpack_crypt1,0x5f27);
	}
	/* save unpacker */
	fwrite(&unpack_start,1,unpack_len,fout);
	if(ferror(fout)) return(2);

	/* modify header */
	l=ftell(fout)-1;
	header[2]=(unsigned)(l/512)+1;
	header[1]=(unsigned)l&511;
	header[11]=header[7]=cs0para; /* cs/ss */
	header[10]=&unpack_code-&unpack_start; /* ip */
	header[8]=&unpack_stack1-&unpack_start; /* sp */
	if(lastone)
	{
		header[14]=0;
		header[15]=0;
	}
	/* save header */   
	fseek(fout,0L,SEEK_SET);
	fwrite(header,2,16,fout);
	if(ferror(fout)) return(2);
	
	return(0);
}


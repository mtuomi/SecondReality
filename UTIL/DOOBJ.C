#include <stdio.h>
#include <string.h>

char	buf[0x400];

char	pubname[80];
unsigned long seglen=0x5;
char	segname[80];

char	blk1[]={0x80,3,0, 1, ':', 0};
char	blk2[]={0x96,2,0, 0x00, 0};
char	blk3[]={0x98,7,0, 0x68,0x0a,0x00,0x02,0x03,0x01, 0};
char	blk5[]={0x8a,2,0, 0x00, 0};

char	blkt[256];

#define objput(blk) fwrite(blk,1,blk[1]+3,f1);

main(int argc,char *argv[])
{
	long int u,v,w;
	int	a;
	FILE	*f1,*f2;
	if(argc==1)
	{
		printf("doobj <source file> <public label name> <destination object file>\n");
		return(0);
	}
	f2=fopen(argv[1],"rb");
	if(!f2)
	{
		printf("%s not found.\n",argv[1]);
		return(1);
	}
	f1=fopen(argv[3],"wb");
	
	printf("Creating %s: ",argv[3]);
	
	strcpy(pubname,argv[2]);
	strcpy(segname,"DATA_");
	strcat(segname,pubname);
	fseek(f2,0L,SEEK_END);
	seglen=ftell(f2);
	rewind(f2);
	
	objput(blk1);
	objput(blk2);
	blkt[0]=0x96;
	blkt[1]=strlen(segname)+1+7+1+1;
	blkt[2]=0;
	blkt[3]=strlen(segname);
	memcpy(blkt+4,segname,strlen(segname));
	a=4+strlen(blkt+4);
	blkt[a]=7;
	memcpy(blkt+a+1,"FARDATA",7);
	a=blkt+a+1+strlen(blkt+a+1);
	blkt[a]=0;
	objput(blkt);
	blk3[4]=seglen&0xff;
	blk3[5]=seglen>>8;
	objput(blk3);
	memset(blkt,0,256);
	blkt[0]=0x90;
	blkt[1]=7+strlen(pubname);
	blkt[2]=0;
	blkt[3]=0;
	blkt[4]=1;
	blkt[5]=strlen(pubname);
	strcpy(blkt+6,pubname);
	a=6+strlen(blkt+6);
	blkt[a]=0;
	blkt[a+1]=0;
	objput(blkt);
	for(u=0;u<seglen;u+=0x400)
	{
		w=u+0x400; if(w>seglen) w=seglen;
		putc(0xa0,f1);
		putw(w-u+4,f1);
		putc(1,f1);
		putw(u,f1);
		fread(buf,1,0x400,f2);
		fwrite(buf,1,w-u,f1);
		{
			a=100*u/seglen; if(a>99) a=99;
			printf("%02i%%\b\b\b",a);
		}
		putc(0,f1);
	}
	objput(blk5);
	
	printf("Done!\n");
	
	fclose(f1);
	fclose(f2);
}

#include <stdio.h>
#include <conio.h>
#include <dos.h>

char    buf[16384];

main()  {
	int a,b;
	FILE    *f1,*f2,*f3;

	if(_argc!=2)
		{
		printf("usage: %s <file.exe>\n");
		exit(1);
		}

	f1=fopen(_argv[1],"rb");
	if(f1==0) exit(1);
	f2=fopen("exebody.dat","wb");
	if(f2==0) exit(1);
	f3=fopen("exehead.dat","wb");
	if(f3==0) exit(1);

	fread(buf,1,0x1c,f1);
	fwrite(buf,1,0x1c,f3);

	fseek(f1,0x08,SEEK_SET);
	fseek(f1,a=getw(f1)*16,SEEK_SET);

	while(!feof(f1))
		{
		a=fread(buf,1,16384,f1);
		if(a==0) break;
		fwrite(buf,1,a,f2);
		}
	}

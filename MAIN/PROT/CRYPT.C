#include <io.h>
#include <fcntl.h>
#include <dos.h>
#include <stdio.h>
#include <string.h>

extern int far arand();
extern char far randtau[1024];
char	far buf[64000];

main()  {
	int	p1, p2, p3, p4;
	long	l,sl,w;
	int	a,b,r;
	int	f1;

	if(_argc!=2) return(1);

	f1=open(_argv[1],O_RDWR|O_BINARY);
	if(f1==-1) return(1);
	l=read(f1,buf,64000);
	lseek(f1,0,SEEK_SET);

	p1=0;
	do do do while(buf[p1++]!=0x12); while(buf[p1++]!=0x34); while(buf[p1++]!=0x56); while(buf[p1++]!=0x78);
	p2=p1;			// p2=cstart

	do do do while(buf[p1++]!=0x12); while(buf[p1++]!=0x34); while(buf[p1++]!=0x56); while(buf[p1++]!=0x78);
	for(a=p1-4;a<p2+768;a++) buf[a]=rand();
	p3=p1-4;		// p3=randtau

	p1=p2+768;
	do do do while(buf[p1++]!=0x12); while(buf[p1++]!=0x34); while(buf[p1++]!=0x56); while(buf[p1++]!=0x78);
	p4=p1-4;	   	// randtau code end
	for(a=p4;a<p3+1024;a++) buf[a]=rand();

	do do do while(buf[p1++]!=0x12); while(buf[p1++]!=0x34); while(buf[p1++]!=0x56); while(buf[p1++]!=0x78);
	sl=p1-4;		// p1=crypt_start

	asm  {
		mov	ax, p1
		add	ax, 29787d
		mov	cx, 15553d
		mul	cx
		and	ax, 511d
		mov	a, ax
		}

	for(;p1<l;p1++,a=(a+15553U)&511)
		buf[a+p2+256]=buf[p1]^0x77;

	memcpy(&randtau[0],&buf[p3],1024);
	for(a=0;a<p2-4;a++) buf[a]=buf[a]^arand();

//---------------------------------------------------------------------------
	for(a=0,r=0;a<256*3;a++)
		{
		if(a<256) { r+=0x7a33; r*=0x2345; }
		else if(a<256*2) { r+=0x455c; r*=0x825d; }
		else { r+=0xaa4b; } // r*=0xde88; }
		buf[p2+(a&255)]^=r;
		}

	for(a=0,r=0;a<256*2;a++)
		{
		if(a<256) { r+=0x455c; r*=0x825d; }
		else { r+=0xaa4b; } // r*=0xde88; }
		buf[p2+(a&255)]^=r;
		}

	for(a=0,r=0;a<256;a++)
		{
		r+=0xaa4b; // r*=0xde88;
		buf[p2+(a&255)]^=r;
		}

//------------------------------------------------------------------------

	write(f1,buf,sl);
	chsize(f1,sl);
	close(f1);
	return(0);
	}
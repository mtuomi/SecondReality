/* stmik ems loader (the feared one) */

int	loadmode=2;
// 0=all low
// 1=<8K low, >8K EMS
// 2=all EMS
// -1=load no samples
int	noems=0;
int	gusmode=0;
int	nosurround=0;

//#include <stdio.h>
//#include <bios.h> // REGS struct 
//#include <malloc.h>
//#include <sys\types.h>
//#include <sys\stat.h>
//#include <fcntl.h>
//#include <io.h>
//#include "stmik.h"

#pragma pack(1)
typedef unsigned char uchar;

int	emsenabled;
unsigned emspageframe,emsnullhandle;
void *emsframepnt;

struct	ds_smp /* instrument block, sample */
{
	char type; /* 1 for sample */
	char filename[12];
	unsigned char memseg2;
	unsigned memseg;
	long length;
	long lbeg;
	long lend;
	char vol;
	char disk;
	char pack; /* 0=normal(disk), 1=amiga(module), 2=ADPCM */
	char flags; /* +1=loop */
	long c2spd;
	char _reserved2[4];
	unsigned guspos;
	unsigned lend512;
	long lastused; /* in ilib, used as time/datestamp */
	char name[28];
	char _magic[4]; /* "SCRS" */
};

struct	ds_fileheader
{
	char	name[28];
	char	_magic_pinit; /* 1ah (or 0h in mem when initialized) */
	char	type;	/* 16=module, 17=song */
	char	_reserved1[2];
	int	ordnum;
	int	insnum;
	int	patnum;
	unsigned flags;
	int	cwtv; /* created with tracker version */
	int	ffv; /* file format version */
	char	_magic_signature[4]; /* SCRM */
	uchar	mastervol;
	uchar	initspeed;
	uchar	inittempo;
	char	_reserved[13];
	uchar	channel[32]; /* channel types */
};

struct ds_smp *moduleins;

static int	freemem(char far *block)
{
	int	a;
	if(block) 
	{
		hfree(block);
	}
	return(0);
}

static char far *getmem(long size)
{
	unsigned int	a;
	unsigned int paras;
	char far *p;
	paras=((size+31L)>>4L)+1L;
	if(_dos_allocmem(paras,&a)) 
	{
		_asm mov ax,7
		_asm int 10h
		_asm
		{
			mov dx,3c8h
			mov al,0
			out dx,al
			mov al,16
			out dx,al
			out dx,al
			out dx,al
		}
		printf("OUT OF MEMORY RESERVING %li BYTE BLOCK! (%i)\n",size,a);
		exit(3);
	}
	p=(char far *)(((long)a)<<16L);
	return(p);
}

static char far *insseg2fp(unsigned int seg)
{
	if(seg<0xf000) return((char far *)((long)seg<<16));
	else
	{
		int	x;
		union REGS reg;
		seg&=0xfff;
		for(x=0;x<4;x++)
		{
			reg.h.ah=0x44;
			reg.h.al=x;
			reg.x.bx=x;
			reg.x.dx=seg;
			int86(0x67,&reg,&reg);
		}
		return(emsframepnt);
	}
}

void	checkins(struct ds_smp *ins)
{
	char far *p;
	if(ins->type==1) 
	{
		memcpy(ins->_magic,"SCRS",4);
		if(ins->length==0)
		{

			ins->flags&=0xfffe;
			return;
		}
		if(ins->vol>64) ins->vol=64;
		if(ins->c2spd>65535) ins->c2spd=65535;
		/* following three will be >999999 */
		if(ins->length>64000) ins->length=64000;
		if(ins->lend<=ins->lbeg) ins->flags&=0xfffe; /* disable loop */
		if(ins->lbeg>ins->length) ins->lbeg=ins->length;
		if(ins->lend>ins->length) ins->lend=ins->length;
		/* do sample continuing for fast looping */
		if(ins->flags&1)
		{
			unsigned u,v,i;
			p=insseg2fp(ins->memseg);
			if(ins->lend512)
			{
				for(i=ins->lend512;i<ins->length;i++) p[i]=p[i+512];
			}
			u=(unsigned)ins->lend;
			v=(unsigned)ins->lbeg;
			for(i=ins->length-1;i>=ins->lend;i--) p[i+512]=p[i];
			for(i=0;i<512;i++) p[u+i]=p[v+i];
			ins->lend512=ins->lend;
		}
		else
		{
			unsigned u,v,i;
			p=insseg2fp(ins->memseg);
			if(ins->lend512)
			{
				for(i=ins->lend512;i<ins->length;i++) p[i]=p[i+512];
				ins->lend512=0;
			}
			memset(p+ins->length,128,512);
		}
	}
	else if(ins->type>=2 && ins->type<=7) memcpy(ins->_magic,"SCRI",4);
	else memset(ins->_magic,0,4);
}

static unsigned getinsmem(long length)
{
	union REGS reg;
	unsigned int eh;
	char far *p;
	length+=512;
	if(loadmode && (emsenabled && (length>3000L || loadmode==2)))
	{ /* try to get space in ems for the sample */
		union REGS reg;
		reg.h.ah=0x43;
		reg.x.bx=(length+16383L)/16384L;
		int86(0x67,&reg,&reg);
		eh=reg.x.dx;
		if(reg.h.ah==0)
		{ /* ok to continue, got mem */
			eh|=0xf000;
			//printf("{elloc:%04X}\n",eh);
			return(eh);
		}
	}
	/* no ems/short sample, try conventional memory */
	p=getmem(length);
	if(!p) return(0);
	eh=(unsigned)((long)p>>16);
	//printf("{alloc:%04X}\n",eh);
	return(eh);
}

static unsigned xloadins(int handle,long length)
{
	char far *p;
	unsigned eh;
	eh=getinsmem(length);
	if(!eh) return(0);
	p=insseg2fp(eh);
	read(handle,p,(unsigned)length);
	return(eh);
}

static int	resetems(void)
{
	union REGS reg;
	if(zemspageframe) emsenabled=1;
	if(emsenabled)
	{
		reg.h.ah=0x45;
		reg.x.dx=zemspagehandle;
		int86(0x67,&reg,&reg);
	}
	return(0);
}

static int	initems(void)
{
	static int emsinitialized=0;
	int	a;
	union REGS reg;
	if(noems) return(0);
	zemspageframe=0;
	emsinitialized=1;
	{ /* check for ems */
		FILE	*f1;
		f1=fopen("*EMMXXXX0","rb");
		if(f1!=NULL)
		{
			emsenabled=1;
			fclose(f1);
		}
	}
	if(emsenabled)
	{
		reg.h.ah=0x41;
		int86(0x67,&reg,&reg);
		zemspageframe=emspageframe=reg.x.bx;
		emsframepnt=(char far *)((long)emspageframe*65536L);
		if(reg.h.ah!=0 || emspageframe<0x1000) return(-1);
		reg.h.ah=0x43;
		reg.x.bx=1;
		int86(0x67,&reg,&reg);
		zemspagehandle=emsnullhandle=reg.x.dx;
	}
	return(0);
}

static void	freeinsmem(struct ds_smp *ins,char far *module)
{
	union REGS reg;
	unsigned u;
	if(ins->type==1 && (u=ins->memseg))
	{
		u+=((long)module>>16);
		//printf("{free:%04X}\n",u);
		if(u<0xf000)
		{
			freemem((char far *)((long)u<<16));
		}
		else
		{
			reg.h.ah=0x45;
			reg.x.dx=(u&0xfff);
			int86(0x67,&reg,&reg);
		}
	}
	ins->memseg=0;
}

static char far *loads3mems(unsigned int h) /* dos file handle */
{
	long	filebase=0L;
	int	orders,patterns,instruments;
	int	moduleseg;
	int	insseg;
	int	*parains;
	int	*parapat;
	char far *module;
	struct ds_fileheader tmpheader;
	struct ds_fileheader *header;
	struct ds_smp *ins;
	unsigned u;
	char far *p;
	long	l;
	int	a,b;
	
	if(gusmode)
	{
		_asm
		{
			mov	dx,a
			mov	ax,102h ;cleargusmem
			mov	bx,4
			int	0fch
		}
	}
	
	if(loadmode!=0 && loadmode!=-1) initems();
	
	filebase=tell(h);
	read(h,&tmpheader,sizeof(tmpheader));

	l=sizeof(struct ds_fileheader)+tmpheader.ordnum+
		tmpheader.patnum*2+tmpheader.insnum*2;
	module=getmem(l+tmpheader.insnum*0x50+0x10);
	moduleins=ins=(struct ds_smp *)((long)module+(((l+0x10)>>4)<<16));
	moduleseg=(long)module>>16;
	insseg=(long)ins>>16;
	
	lseek(h,filebase,SEEK_SET);
	read(h,module,(size_t)l);

	header=(struct ds_fileheader *)module;
	header->type=16;
	orders=header->ordnum;
	instruments=header->insnum;
	patterns=header->patnum;
	parains=(int *)(module+sizeof(struct ds_fileheader)+header->ordnum);
	parapat=parains+header->insnum;
	
	for(a=0;a<instruments;a++)
	{
		lseek(h,filebase+((long)parains[a]<<4L),SEEK_SET);
		read(h,ins+a,0x50);
		parains[a]=a*5+insseg-moduleseg;
	}
	
	for(a=0;a<patterns;a++)
	{
		b=parapat[a];
		if(!b) continue;
		lseek(h,filebase+((long)b<<4L),SEEK_SET);
		read(h,&u,2);
		p=getmem(u);
		*(int *)p=u;
		read(h,p+2,u-2);
		parapat[a]=((long)p>>16)-moduleseg;
	}
	
	if(gusmode)
	{
		char	*p=module;
		_asm
		{
			mov	bx,4
			mov	ax,101h
			mov	dx,word ptr p[2]
			int	0fch // initmodule
		}
	}

	for(a=0;a<instruments;a++) if((ins+a)->type==1 && (ins+a)->memseg)
	{
		long	off;
		int	seg;
		if(nosurround && (ins+a)->filename[strlen((ins+a)->filename)-1]=='$')
		{
			(ins+a)->type=0;
			(ins+a)->memseg=0;
			(ins+a)->guspos=0;
			continue;
		}
		if(loadmode==-1)
		{
			(ins+a)->memseg=0;
			(ins+a)->guspos=0;
		}
		else
		{
			off=(long)(ins+a)->memseg<<4L;
			off+=(long)(ins+a)->memseg2<<20L;
			lseek(h,filebase+off,SEEK_SET);
			seg=xloadins(h,(ins+a)->length);
			(ins+a)->memseg=seg;
			checkins(ins+a);
			(ins+a)->memseg-=moduleseg;
			(ins+a)->guspos=0;
		}
		if(gusmode)
		{
			if((ins+a)->memseg)
			{
				_asm
				{
					mov	dx,a
					mov	ax,100h
					mov	bx,4
					int	0fch
				}
				freeinsmem(ins+a,module);
			}
		}
	}
	return(module);
}

static int	frees3mems(char far *module)
{
	struct ds_fileheader *header;
	struct ds_smp *ins;
	int	*parains;
	int	*parapat;
	unsigned int u;
	long	l;
	int	a;
	header=(struct ds_fileheader *)module;
	l=sizeof(struct ds_fileheader)+header->ordnum+
		header->patnum*2+header->insnum*2;
	ins=(struct ds_smp *)((long)module+(((l+0x10)>>4)<<16));
	parains=(int *)(module+sizeof(struct ds_fileheader)+header->ordnum);
	parapat=parains+header->insnum;
	for(a=0;a<header->insnum;a++) if((ins+a)->type==1 && (ins+a)->memseg)
	{
		freeinsmem(ins+a,module);
	}
	for(a=0;a<header->patnum;a++) 
	{
		u=parapat[a];
		if(u)
		{
			u+=((long)module>>16);
			freemem((char far *)((long)u<<16));
		}
	}
	freemem(module);
	if(loadmode!=0 && loadmode!=-1) resetems();
	return(0);
}

static int	frees3msamples(char far *module)
{
	struct ds_fileheader *header;
	struct ds_smp *ins;
	int	*parains;
	int	*parapat;
	long	l;
	int	a;
	header=(struct ds_fileheader *)module;
	l=sizeof(struct ds_fileheader)+header->ordnum+
		header->patnum*2+header->insnum*2;
	ins=(struct ds_smp *)((long)module+(((l+0x10)>>4)<<16));
	parains=(int *)(module+sizeof(struct ds_fileheader)+header->ordnum);
	parapat=parains+header->insnum;
	for(a=0;a<header->insnum;a++) if((ins+a)->type==1 && (ins+a)->memseg)
	{
		freeinsmem(ins+a,module);
	}
	return(0);
}

static int	readys3mems(char far *module)
{
	struct ds_fileheader *header;
	struct ds_smp *ins;
	int	*parains;
	int	*parapat;
	long	l;
	int	a;
	return(0);
	_asm
	{
		mov	dx,a
		mov	ax,102h ;cleargusmem
		mov	bx,4
		int	0fch
	}
	header=(struct ds_fileheader *)module;
	l=sizeof(struct ds_fileheader)+header->ordnum+
		header->patnum*2+header->insnum*2;
	ins=(struct ds_smp *)((long)module+(((l+0x10)>>4)<<16));
	parains=(int *)(module+sizeof(struct ds_fileheader)+header->ordnum);
	parapat=parains+header->insnum;
	/*
	for(a=0;a<header->insnum;a++) if((ins+a)->type==1 && (ins+a)->memseg)
	{
		_asm
		{
			mov	dx,a
			mov	ax,100h
			mov	bx,4
			int	0fch
		}
		freeinsmem(ins+a,module);
	}
	*/
	return(0);
}

/* external interface */

char *	stmik_emsready(char *p) // moves instruments to ems/gravis
{
	readys3mems(p);
}

char *	stmik_emsload(unsigned int h)
{
	char	*p;
	p=loads3mems(h);
	return(p);
}

void	stmik_emsfree(char *p)
{
	frees3mems(p);
}

void	stmik_emsfreesamples(char *p)
{
	frees3msamples(p);
}

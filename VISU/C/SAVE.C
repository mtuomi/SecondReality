

long	blockstack[16];
int	blockstackp=0;

void	beginblock(char *label)
{
	blockstack[blockstackp]=ftell(outb);
	blockstackp++;
	putc(label[0],outb);
	putc(label[1],outb);
	putc(label[2],outb);
	putc(label[3],outb);
	putc(0xff,outb);
	putc(0xff,outb);
	putc(0xff,outb);
	putc(0xff,outb);
}

void	endblock(void)
{
	long	op,l;
	op=ftell(outb);
	while(op&3)
	{
		op++;
		putc(0,outb);
	}
	blockstackp--;
	l=blockstack[blockstackp];
	fseek(outb,l+4,SEEK_SET);
	l=op-l-8;
	putc((int)l&255,outb); l>>=8;
	putc((int)l&255,outb); l>>=8;
	putc((int)l&255,outb); l>>=8;
	putc((int)l&255,outb);
	fseek(outb,op,SEEK_SET);
}

void saveobject(struct s_cobject *co)
{
	object	*o;
	char	fname[32];
	long	l;
	int	a,b;
	
	o=co->o;
	sprintf(fname,"%s.%03i",scenename,co->index);
	outb=fopen(fname,"wb");
	strcpy(co->fname,fname);
	
	beginblock("VERS");
	putw(0x0100,outb);
	endblock();

	beginblock("NAME");
	fprintf(outb,co->name);
	putc(0,outb);
	endblock();
	
	beginblock("VERT");
	putw(o->vnum,outb);
	putw(0,outb);
	fwrite(o->v0,sizeof(vlist),o->vnum,outb);
	endblock();
	
	beginblock("NORM");
	putw(o->nnum,outb);
	putw(o->nnum1,outb);
	fwrite(o->n0,sizeof(nlist),o->nnum,outb);
	endblock();
	
	beginblock("POLY");
	fwrite(o->pd,1,o->pdlen,outb);
	endblock();
	
	for(b=0;b<o->plnum;b++)
	{
		if(!b) beginblock("ORD0");
		else beginblock("ORDE");
		fwrite(o->pl[b],2,o->pl[b][0],outb);
		endblock();
	}

	beginblock("END ");
	endblock();

	co->size=ftell(outb);
	fclose(outb);
}

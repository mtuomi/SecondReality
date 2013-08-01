
void savevgastate(unsigned char *vbp)
{
	int	a,b;
	// read / store regs
	*vbp++=inp(0x3cc);
	for(a=0;a<=0x4;a++)
	{
		outp(0x3c4,a);
		b=inp(0x3c5);
		*vbp++=b;
	}
	for(a=0;a<=0x18;a++)
	{
		outp(0x3d4,a);
		b=inp(0x3d5);
		*vbp++=b;
	}
	for(a=0;a<=0x8;a++)
	{
		outp(0x3ce,a);
		b=inp(0x3cf);
		*vbp++=b;
	}
	for(a=0;a<=0x14;a++)
	{
		inp(0x3d4+6);
		outp(0x3c0,a);
		b=inp(0x3c1);
		*vbp++=b;
		outp(0x3c0,b);
		outp(0x3c0,32);
	}
	*vbp++=0;
}

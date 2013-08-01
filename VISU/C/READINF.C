
void readinf(void)
{
	char	tmp[256];
	int	a,b,c,d;
	object	*o;
	char	*p,*p0,*t;
	matnum=0;
	while(!feof(in))
	{
		p0=p=getline();
		if(*p==';') continue;
		t=gettoken(&p);
		IFT("scene")
		{
			t=gettoken(&p);
			a=atoi(t);
			in2=in;
			resetscene();
			doscene(a);
			in=in2;
		}
		else IFT("hide")
		{
			t=gettoken(&p);
			o=findobject(t,-9);
			o->flags&=~(F_VISIBLE);
		}
		else IFT("show")
		{
			t=gettoken(&p);
			o=findobject(t,-9);
			o->flags|=(F_VISIBLE);
		}
		else IFT("fov")
		{
			t=gettoken(&p);
			a=atoi(t);
			usefov=a;
		}
	}
	printf("\n\n");
}

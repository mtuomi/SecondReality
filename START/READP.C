struct st_readp
{
	int	magic;
	int	wid;
	int	hig;
	int	cols;
	int	add;
};

void	readp(char *dest,int row,char *src)
{
	int	bytes,a,b;
	struct st_readp *hdr;
	hdr=(struct st_readp *)src;
	if(row==-1)
	{
		memcpy(dest,src+16,hdr->cols*3);
		return;
	}
	if(row>=hdr->hig) return;
	src+=hdr->add*16;
	while(row)
	{
		src+=*(int *)src;
		src+=2;
		row--;
	}
	bytes=*(int *)src;
	src+=2;
	while(bytes--)
	{
		a=*src++;
		if(a&0x80)
		{
			b=*src++;
			bytes--;
			a&=0x7f;
			while(a--) *dest++=b;
		}
		else *dest++=a;
	}
}

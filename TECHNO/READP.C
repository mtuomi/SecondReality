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
		_asm
		{
			cmp	word ptr src[0],4000h
			jb	k1
			sub	word ptr src[0],4000h
			add	word ptr src[2],400h
		k1:
		}
	}
	bytes=*(int *)src;
	src+=2;
	_asm
	{
		push	si
		push	ds
		push	di
		push	es
		mov	cx,bytes
		lds	si,src
		add	cx,si
		les	di,dest
	l1:	mov	al,ds:[si]
		inc	si
		or	al,al
		jns	l2
		mov	ah,al
		and	ah,7fh	
		mov	al,ds:[si]
		inc	si
	l4:	mov	es:[di],al
		inc	di
		dec	ah
		jnz	l4
		cmp	si,cx
		jb	l1
		jmp	l3
	l2:	mov	es:[di],al
		inc	di
		cmp	si,cx
		jb	l1
	l3:	pop	es
		pop	di
		pop	ds
		pop	si
	}
}

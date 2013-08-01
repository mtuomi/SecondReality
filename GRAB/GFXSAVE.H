/*****************************************************************
	Saves gfxdata to file
	 
	variables required from outside:
	unsigned char pic[];    ;array to 320x200 1BM
	int backcol;            ;transparent color
	int fcporigo;           ;1 if origo in the center of area
	int fcporigox,fcporigoy ;if ^=2 then origoxy
*****************************************************************/

int	writefcporigo(FILE *f1,int x1,int y1,int x2,int y2)
{
	int	xl=x2-x1+1,yl=y2-y1+1;
	int	x,y,a,b,c;
	if(fcporigo==3)
	{
		for(x=x1;x<=x2;x++) for(y=y1;y<=y2;y++)
		{
			if(vram[x+y*320]==1)
			{
				putw(-y,f1);
				putw(-x,f1);
				pic[x+y*320]=vram[x+y*320]=vram[1+x+y*320];
			}
		}
	}
	else if(fcporigo==2)
	{
		putw(-fcporigoy,f1);
		putw(-fcporigox,f1);
	}
	else if(fcporigo)
	{
		putw(-(yl+1)/2,f1);
		putw(-(xl+1)/2,f1);
	}
	else
	{
		putw(0,f1);
		putw(0,f1);
	}
}

int	savegfxdata(FILE *f1,int x1,int y1,int x2,int y2)
{
	int	xl=x2-x1+1,yl=y2-y1+1;
	int	a,x,y,z,c;
	unsigned int u,u2,lastp;
	putc(0x01,f1);
	putc(0,f1);
	if(fcporigo==3)
	{
		for(x=x1;x<=x2;x++) for(y=y1;y<=y2;y++)
		{
			if(vram[x+y*320]==1)
			{
				putw(-y,f1);
				putw(-x,f1);
				pic[x+y*320]=vram[x+y*320]=vram[1+x+y*320];
			}
		}
	}
	else if(fcporigo==2)
	{
		putw(-fcporigoy,f1);
		putw(-fcporigox,f1);
	}
	else if(fcporigo)
	{
		putw(-yl/2,f1);
		putw(-xl/2,f1);
	}
	else
	{
		putw(0,f1);
		putw(0,f1);
	}
	putw(yl,f1);
	putw(xl,f1);
	for(z=0;z<4;z++)
	{
		for(y=y1;y<=y2;y++)
		{
			u =(unsigned)y*320+(unsigned)x1+(unsigned)z;
			u2=(unsigned)y*320+(unsigned)x2;
			lastp=u;
			while(u<=u2)
			{
				if(pic[u]==backcol)
				{
					while(pic[u]==backcol && u<=u2)
					{
						for(c=0;u<=u2 && c<127 && pic[u]==backcol;u+=4,c++);
						if(u<=u2) putc(c,f1);
					}
				}
				if(u>u2) break;
				lastp=u;
				for(c=1;u<=u2 && c<63 && pic[u]==pic[u+4] && pic[u]!=backcol;u+=4,c++);
				if(c>2)
				{
					putc((c<<1)|0x80,f1);
					putc(pic[lastp],f1);
					u+=4;
				}
				else
				{
					u=lastp;
					if((u2-u)+1>2) 
					{
					  for(c=0;u<=u2 && c<63 && 
					    (pic[u]!=pic[u+4] || pic[u]!=pic[u+8]) 
					      && pic[u]!=backcol;u+=4,c++);
					}
					else for(c=0;u<=u2 && c<63 && pic[u]!=backcol;u+=4,c++);
					if(c!=0)
					{
						putc((c<<1)|0x81,f1);
						for(a=0;a<c;a++)
						{
							putc(pic[lastp+a*4],f1);
						}
					}
				}
			}
			putc(0,f1);
			#ifdef VPSOFT
			pxor(x1,y);
			pxor(x2,y);
			#endif
		}
	}
}

int	savegfxdata1(FILE *f1,int x1,int y1,int x2,int y2)
{
	int	xl=x2-x1+1,yl=y2-y1+1;
	int	a,x,y,c;
	unsigned int u,u2,lastp;
	putc(0x02,f1);
	putc(0,f1);
	if(fcporigo==3)
	{
		for(x=x1;x<=x2;x++) for(y=y1;y<=y2;y++)
		{
			if(vram[x+y*320]==1)
			{
				putw(-(y-y1),f1);
				putw(-(x-x1),f1);
				pic[x+y*320]=vram[x+y*320]=vram[1+x+y*320];
				break;
			}
		}
	}
	else if(fcporigo==2)
	{
		putw(-fcporigoy,f1);
		putw(-fcporigox,f1);
	}
	else if(fcporigo)
	{
		putw(-yl/2,f1);
		putw(-xl/2,f1);
	}
	else
	{
		putw(0,f1);
		putw(0,f1);
	}
	putw(yl,f1);
	putw(xl,f1);
	{
		for(y=y1;y<=y2;y++)
		{
			u =(unsigned)y*320+(unsigned)x1;
			u2=(unsigned)y*320+(unsigned)x2;
			while(u<=u2)
			{
				if(pic[u]==backcol)
				{
					while(pic[u]==backcol && u<=u2)
					{
						for(c=0;u<=u2 && c<127 && pic[u]==backcol;u++,c++);
						if(u<=u2) putc(c,f1);
					}
				}
				if(u>u2) break;
				lastp=u;
				for(c=1;u<u2 && c<63 && pic[u]==pic[u+1] && pic[u]!=backcol;u++,c++);
				if(c>2)
				{
					putc((c<<1)|0x80,f1);
					putc(pic[lastp],f1);
					u++;
				}
				else
				{
					u=lastp;
					if((u2-u)+1>2) 
					{
					  for(c=0;u<=u2 && c<63 && 
					    (pic[u]!=pic[u+1] || pic[u]!=pic[u+2]) 
					      && pic[u]!=backcol;u++,c++);
					}
					else for(c=0;u<=u2 && c<63 && pic[u]!=backcol;u++,c++);
					if(c!=0)
					{
						putc((c<<1)|0x81,f1);
						for(a=0;a<c;a++)
						{
							putc(pic[lastp+a*1],f1);
						}
					}
				}
			}
			putc(0,f1);
			#ifdef VPSOFT
			pxor(x1,y);
			pxor(x2,y);
			#endif
		}
	}
}

int	savegfxdata3(FILE *f1,int x1,int y1,int x2,int y2)
{
	int	xl=x2-x1+1,yl=y2-y1+1;
	int	a,x,y,c;
	int	skip;
	unsigned int u,u2,lastp;
	y1--;
	putw(0x0003,f1);
	writefcporigo(f1,x1,y1,x2,y2);
	putw(yl+1,f1);
	putw(xl,f1);
	{
		for(x=x1;x<=x2;x++)
		{
			skip=1;
			for(y=y1+1;y<=y2;y++)
			{
				u=x+y*320;
				if(pic[u]==backcol) 
				{
					skip++;
					continue;
				}
				for(c=y;y<=y2+1;y++)
				{
					u=x+y*320;
					if(pic[u]==backcol) break;
				}
				putc(y-c,f1);
				putc(skip,f1); skip=0;
				for(a=c;a<y;a++) putc(pic[x+a*320],f1);
				y--;
			}
			putw(0,f1);
			#ifdef VPSOFT
			pxor(x1,y);
			pxor(x2,y);
			#endif
		}
	}
}

int	savegfxdatax(FILE *f1,int x1,int y1,int x2,int y2)
{ /* codewise optimized sprite */
	int	xl=x2-x1+1,yl=y2-y1+1;
	int	a,x,y,z,c,be,b;
	int	skip;
	unsigned char buf[400],wbuf[16];
	unsigned int u,u2,lastp;
	putw(0x0004,f1);
	writefcporigo(f1,x1,y1,x2,y2);
	putw(yl,f1);
	putw(xl,f1);
	for(z=0;z<4;z++)
	{
		for(y=y1;y<=y2;y++)
		{
			memset(buf,backcol,400);
			for(be=0,x=x1+z;x<=x2;x+=4)
			{
				buf[be++]=pic[x+y*320];
			}
			for(c=b=0;b<be;b+=8)
			{
				wbuf[0]=0;
				for(a=0;a<8;a++)
				{
					if(buf[a+b]!=backcol)
					{
						wbuf[a+1]=buf[a+b];
						wbuf[0]|=(1<<a);
					}
				}
				if(wbuf[0])
				{ // some data!
					if(c>0)
					{
						a=c*8;
						while(a>240)
						{
							putc(0,f1);
							putc(240,f1);
							a-=240;
						}
						if(a)
						{
							putc(0,f1);
							putc(a,f1);
						}
						c=0;
					}
					putc(wbuf[0],f1);
					for(a=0;a<8;a++)
					{
						if(wbuf[0]&(1<<a))
						{
							putc(wbuf[a+1],f1);
						}
					}
				}
				else c++;
			}
			putc(0,f1);
			putc(0,f1);
			#ifdef VPSOFT
			pxor(x1,y);
			pxor(x2,y);
			#endif
		}
	}
}


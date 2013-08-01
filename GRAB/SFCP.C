 
char	packbuf[16384];
int	packbufpnt,packbufcnt;

#define stosw(xx) { packbuf[packbufpnt++]=xx&255; packbuf[packbufpnt++]=xx>>8; }
#define stosb(xx) packbuf[packbufpnt++]=xx

int	savepack(FILE *f1)
{
	int	a,b,c;
	if(f1!=NULL) 
	{
		b=packbuf[2];
		a=packbuf[3];
		if((a==0 && b<128) || (a==-1 && b>-128)) c=1; else c=0;
		packbuf[0]=packbufcnt&255; packbuf[1]=packbufcnt>>8;
		if(packbufcnt==1 && c==1)
		{
			packbuf[3]=128;
			fwrite(packbuf+2,1,packbufpnt-2,f1);
		}
		else fwrite(packbuf,1,packbufpnt,f1);
	}
	packbufpnt=2;
	packbufcnt=0;
}

int	savefcpdata(FILE *f1,int x1,int y1,int x2,int y2)
{ /* single */
	int	style;
	int	tx1=x1,ty1=y1,tx2=x2,ty2=y2;
	int	xl=x2-x1+1,yl=y2-y1+1;
	int	xo,yo;
	int	x,y,z,zm=1;
	int	a,b,c,d,w,wc;
	unsigned u;
	long	tell4,telltbl;
	long	pic1pos;
	unsigned planepos[4];
	savepack(NULL);
	pic1pos=ftell(f1);
	planepos[0]=planepos[1]=planepos[2]=planepos[3]=0x1234;
	putc(0x10,f1); /* pic ver */
	switch(fcpstyle)
	{
		case 15 : a=15; /* fastwing */
			break;
	}
	style=a;
	if(fcp4plane) a|=16;
	if(fcptransp) a|=32;
	if(fcpsavepal) a|=64;
	putc(a,f1);
	if(fcporigo && fcpstyle==15)
	{
		xo=xl/2;
		yo=yl/2;
	}
	else xo=yo=0;
	putw(xo,f1); /* minx */
	putw(yo,f1); /* miny */
	putw(xl-xo-1,f1); /* maxx */
	putw(yl-yo-1,f1); /* maxy */
	if(fcp4plane) 
	{
		memcpy(pic2,pic,64000);
		x1=x1/4;
		x2=(x2+3)/4;
		xl=x2-x1+1;
		a|=16;
		tell4=ftell(f1);
		zm=4;
	}
	else xo+=x1; yo+=y1;
    for(z=0;z<zm;z++) 
    {
	if(fcp4plane) 
	{
		x1=0;
		x2=xl-1;
		memset(pic,backcol,64000);
		b=xl; if(xl*4+z>(tx2-tx1+1)) b--;
		for(y=y1;y<=y2;y++) for(a=0,x=tx1+z;a<b;x+=4,a++)
		{
			pic[a+y*320]=pic2[x+y*320];
		}
		planepos[z]=ftell(f1);
	}
	switch(style)
	{
		case 15 : /* fastwing */
			for(y=y1;y<=y2;y++) 
			{
				for(x=x1;x<=x2;x++)
				{
					u=x+y*320; 
					if(pic[u]!=backcol || !fcptransp)
					{
						putw(y-yo,f1);
						putw(x-xo,f1);
						b=x2-x+1;
						d=pic[u];
						w=0; wc=0;
						for(a=0;a<b;)
						{
							for(c=0;a<b;a++)
							{
								if(pic[u+a]==d) c++;
								else break;
							}
							if(c>4)
							{
								if(wc>0)
								{
									int	a;
									packbufcnt++;
									stosw(wc);
									for(a=0;a<wc;a++)
									{
										stosb(pic[u+w+a]);
									}
									w+=wc;
								}
								packbufcnt++;
								stosw(-c);
								stosb(d);
								w=a; wc=0;
								if(pic[u+a]==backcol && fcptransp) break;
								d=pic[u+a];
							}
							else
							{
								wc=a-w;
								d=pic[u+a];
								if(pic[u+a]==backcol && fcptransp) break;
							}
						}
						if(wc>0)
						{
							int	a;
							packbufcnt++;
							stosw(wc);
							for(a=0;a<wc;a++) 
							{
								stosb(pic[u+w+a]);
							}
						}
						x+=a-1;
						savepack(f1);
					}
				}
				pxor(tx1,ty1+y-y1);
				pxor(tx2,ty1+y-y1);
			}
			putw(0x8000,f1);
			break;
	}
    }
    if(fcp4plane) 
    {
	memcpy(pic,pic2,64000);
    }
    fseek(f1,0L,SEEK_END);
}

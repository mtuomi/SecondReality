/****************************************************/
/*****   FuturePicture loader (FCP) V1.0 ************/
/****************************************************/

int	loadfcp(char *fname)
{
	int	multiple=0;
	int	pz;
	int	format;
	int	x,y,a,b,c,d;
	int	cnt,cntm;
	int	minx,miny,maxx,maxy,xo,yo,cols;
	int	infocnt=0;
	int	style;
	int	z,zm=1;
	int	plane4;
	long	l;
	char far *filebuf;
	unsigned u;
	filebuf=halloc(32768L,1L);
	#ifndef AOLOAD
	printf("\nLoading and uncompressing picture (FCP): ");
	#endif
	if(filebuf==NULL)
	{
		printf("Out of memory.\n");
		return(1);
	}
	if(setback255) memset(pic2,255,64000);
	{
		/* header */
		f1=fopen(fname,"rb");
		if(f1==NULL) return(3);
		setvbuf(f1,filebuf,_IOFBF,32768);
		a=getc(f1);
		if(a!='F') return(2);
		a=getc(f1);
		if(a!='C') return(2);
		a=getc(f1);
		if(a!='P') return(2);
		a=getc(f1);
		if(a!=0x1a) return(2);
		a=getc(f1);
		if(a!=0x10) return(2);
		b=getc(f1);
		pics=getw(f1);
		fread(picpos,4,pics,f1);
		if(b&1)
		{
			fread(pal,256*3,1,f1);
		}
		if(pics>1)
		{
			multiple=1;
			#ifndef AOLOAD
			printf("\n\nFCP file contains multiple pictures.\n"
				"Press Space to view next with animation\n"
				"      Enter to view next with flipping\n"
				"      'S' to select picture\n\n"
				"Press any key to start.");
				getch();
				reg.x.ax=0x13;
				int86(0x10,&reg,&reg);
			#endif
			viewpal(0);
		}
		/* pic */
	for(pz=0;pz<pics;pz++)
	{
		fseek(f1,(long)picpos[pz],SEEK_SET);
		a=getc(f1);
		if(a!=0x10) return(2);
		format=getc(f1); style=format&15;
		minx=getw(f1);
		miny=getw(f1);
		maxx=getw(f1);
		maxy=getw(f1);
		xo=-minx;
		yo=-miny;
		#ifdef AOLOAD
		bx1=by1=0;
		if(maxx-minx>bx2) bx2=maxx-minx;
		if(maxy-miny>by2) by2=maxy-miny;
		#endif
		if(format&16)
		{
			plane4=4;
			zm=4;
		}
		else plane4=1;
		for(z=0;z<zm;z++)
		{
			if(style==15)
			{ /* fastwing */
				while(!eschit())
				{
					infocnt++; infocnt&=63;
					if(infocnt==0 && pics==1 && !multiple) printf(".");
					y=getw(f1);
					if(y==0x8000) break;
					y+=yo;
					x=getw(f1)+xo;
					cntm=getw(f1);
					if(cntm<0)
					{
						a=(int)((char)cntm);
						cntm=1;
						d=0;
					}
					else d=1;
					for(cnt=0;cnt<cntm;cnt++)
					{
						if(d) a=getw(f1);
						else d=1;
						if(a<0)
						{
							b=getc(f1);
							a=-a;
							for(c=0;c<a;c++,x++) pic[x*plane4+z+y*320]=b;
						}
						else
						{
							for(c=0;c<a;c++,x++) pic[x*plane4+z+y*320]=getc(f1);
						}
					}
				}
			}
		}
		if(pics!=1) 
		{
			#ifndef AOLOAD
			viewpic(0);
			{
				a=getch();
				if(a==27 || a=='s' || a=='S') break;
				if(a==13 && pz!=pics-1) memset(pic,setback255?255:0,64000);
			}
			#else
			onepicdone();
			#endif
		}
	}
		fclose(f1);
	}
	return(0);
}


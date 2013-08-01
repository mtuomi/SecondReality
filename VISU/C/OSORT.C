// object sorting

//#define SORTDIRS 6
#define SORTDIRS 8

long	osx,osy,osz; // sorting eye
long	aosx,aosy,aosz; // sorting eye
long	bosx,bosy,bosz; // sorting eye
static long osd=0;

void	osetup(object *o,int flag)
{ // sets up camera distance etc for good viewing
	long cx,cy,cz;
	long ox,oy,oz;
	long d=osd;
	int a,c;
	vlist *v;
	for(c=0;c<64;c++)
	{
		ox=osx*d/256L;
		oy=osy*d/256L;
		oz=osz*d/256L;
		v=o->v0+o->pl[0][1];
		cx=v->x;
		cy=v->y;
		cz=v->z;
		calc_setrmatrix_camera(o->r,ox+cx,oy+cy,oz+cz,cx,cy,cz,0);
		calc_nrotate(o->nnum1,o->n,o->n0,o->r);
		calc_rotate(o->vnum,o->v,o->v0,o->r);
		calc_project(o->vnum,o->pv,o->v);
		if(!flag) break;
		for(a=0;a<o->vnum;a++) if(o->pv[a].vf) break;
		if(a==o->vnum) break;
		d=d*4/3;
	}
	if(osd<d) osd=d;
}

void	sortdir(int d,long *osx,long *osy,long *osz)
{
	switch(d)
	{
	/*
	case 0 : *osx=-256; *osy=0; *osz=0; break;
	case 1 : *osy=-256; *osz=0; *osx=0; break;
	case 2 : *osz=-256; *osx=30; *osy=30; break;
	case 3 : *osx=256; *osy=0; *osz=0; break;
	case 4 : *osy=256; *osz=0; *osx=0; break;
	case 5 : *osz=256; *osx=30; *osy=30; break;
	*/
	case 0 : *osx=-256; *osy=-256; *osz=-256; break;
	case 1 : *osx=256; *osy=-256; *osz=-256; break;
	case 2 : *osx=256; *osy=256; *osz=-256; break;
	case 3 : *osx=-256; *osy=256; *osz=-256; break;
	case 4 : *osx=-256; *osy=-256; *osz=256; break;
	case 5 : *osx=256; *osy=-256; *osz=256; break;
	case 6 : *osx=256; *osy=256; *osz=256; break;
	case 7 : *osx=-256; *osy=256; *osz=256; break;
	case 8 : *osx=-366; *osy=0; *osz=0; break;
	/*
	case 0 : *osx=-256; *osy=-256; *osz=-256; break;
	case 1 : *osx=256; *osy=-256; *osz=-256; break;
	case 2 : *osx=256; *osy=256; *osz=-256; break;
	case 3 : *osx=-256; *osy=256; *osz=-256; break;
	case 4 : *osx=-256; *osy=-256; *osz=256; break;
	case 5 : *osx=0; *osy=366; *osz=0; break;
	case 6 : *osx=256; *osy=-256; *osz=256; break;
	case 7 : *osx=0; *osy=-366; *osz=0; break;
	case 8 : *osx=256; *osy=256; *osz=256; break;
	case 9 : *osx=-366; *osy=0; *osz=0; break;
	case 10 : *osx=-256; *osy=256; *osz=256; break;
	case 11 : *osx=366; *osy=0; *osz=0; break;
	case 12 : *osx=30; *osy=30; *osz=-366; break;
	case 13 : *osx=30; *osy=30; *osz=366; break;
	*/
	default : *osx=*osy=*osz=0; break;
	}
}

struct s_osp
{
	long	dis; // distance maximum
	long	dismin; // distance minimum
	long	a,b,c,d; // plane equation
	int	o; // offset
	int	swapped;
} osp[MAXFC];
struct s_osp osptmp;

int	qcmp(struct s_osp *a,struct s_osp *b)
{
	if(a->dis>b->dis) return(-1);
	if(a->dis<b->dis) return(1);
	return(0);
}

// -1=j farther than i (swap)
// 0==indetermined 
// 1==i farther than j (ok)
// 2==i farther than j.. (next)}
int	planecompare(struct s_osp *i,struct s_osp *j,object *o,int flag)
{
	int	*ip,*pp;
	long	d1=0,d2=0,dc=0,d,d1a,d2a,dd;
	int	d1c=0,d2c=0;
	int	a,c,r;

	if(i->dismin>j->dis) return(2);

	pp=(int *)(j->o+(char *)(o->pd));
	c=*pp++; // sides
	c&=0xff;
	pp++; // skip color
	pp++; // skip normal
	//fprintf(fdeb,"compare i=f%i(%i) with j=f%i(%i)\n",i-osp,i->o,j-osp,j->o);
	while(c--)
	{
		a=*pp++;
		d=(long)(((double)o->v[a].x*(double)i->a+(double)o->v[a].y*(double)i->b+(double)o->v[a].z*(double)i->c+(double)i->d)/1000.0);
		
		if(debcount<1000)
		{
			//fprintf(fdeb,"v%i:%li ",a,d);
			debcount++;
		}

		if(d<0) { d1+=d; d1c++; }
		else { d2+=d; d2c++; }
	}
	if(d1c) d1/=d1c;
	if(d2c) d2/=d2c;
	// camera at zero, so dc=i->d;
	dc=i->d;
	if(d1<0) d1a=-d1; else d1a=d1;
	if(d2<0) d2a=-d2; else d2a=d2;
	d=d1a-d2a; if(d<0) d=-d;
	dd=(d1a+d2a)/8;
	if(d<dd) return(0); // comaparator face cuts this one in middle
	//fprintf(fdeb,"\ncamera at %li, d1=%li, d1c=%i, d2=%li, d2c=%i, d1a=%li d2a=%li\n",dc,d1,d1c,d2,d2c,d1a,d2a);
	if(d1a>d2a)
	{
		if(d1>0 && dc>0) r=1;
		else if(d1<0 && dc<0) r=1;
		else r=-1;
	}
	else
	{
		if(d2>0 && dc>0) r=1;
		else if(d2<0 && dc<0) r=1;
		else r=-1;
	}
	if(flag) 
	{
		a=planecompare(j,i,o,0);
		if(a==r) r=0;
		//fprintf(fdeb,"returns %i\n\n",r);
	}
	return(r);
}

void	objectsort(object *o)
{
	static int skipw=0;
	long	z,z1,z2;
	long	px,py,pz,pa,pb,pc,pd,td;
	int	fbak,i,j,k;
	int	d,a,c,b,n,onum;
	int	changed;
	int	*ip,*pp;
	osd=256;
	for(d=0;d<SORTDIRS;d++)
	{
		sortdir(d,&osx,&osy,&osz);
		osetup(o,1);
	}
	for(d=0;d<SORTDIRS;d++)
	{
		aosx=osx;
		aosy=osy;
		aosz=osz;
		sortdir(d,&osx,&osy,&osz);
		bosx=osx;
		bosy=osy;
		bosz=osz;
		if(d && !skipw)
		{
			for(a=0;a<=64;a+=2)
			{
				b=64-a;
				osx=(aosx*(long)b+bosx*(long)a)>>6;
				osy=(aosy*(long)b+bosy*(long)a)>>6;
				osz=(aosz*(long)b+bosz*(long)a)>>6;
				osetup(o,0);
				fbak=o->flags;
				o->flags&=~(F_GOURAUD);
				vNext();
				vis_drawobject(o);
				o->flags=fbak;
			}
		}
		osx=bosx;
		osy=bosy;
		osz=bosz;
		osetup(o,0);
		ip=o->pl[0]+2;
		onum=0;
		while(*ip)
		{
			a=osp[onum].o=*ip++;
			pp=(int *)(osp[onum].o+(char *)o->pd);
			c=*pp++; // sides
			c&=0xff;
			if(a!=68 && a!=56) 
			{
			//	*pp=-1;
			}
			pp++; // skip color
			n=*pp++; // skip normal
			z=0; z2=0x7fffffff;
			//fprintf(fdeb,"face %i: ",osp[onum].o);
			while(c--)
			{
				a=*pp++;
				//fprintf(fdeb,"%i ",a);
				if(c==1)
				{
					px=o->v[a].x;
					py=o->v[a].y;
					pz=o->v[a].z;
				}
				z1=o->v[a].z;
				if(z1>z) z=z1;
				if(z1<z2) z2=z1;
			}
			//fprintf(fdeb,"\n");
			osp[onum].a=pa=o->n[n].x/4;
			osp[onum].b=pb=o->n[n].y/4;
			osp[onum].c=pc=o->n[n].z/4;
			osp[onum].d=pd=-px*pa-py*pb-pz*pc;
			osp[onum].dis=z;
			osp[onum].dismin=z2;

			pp=(int *)(osp[onum].o+(char *)o->pd);
			c=*pp++; // sides
			c&=0xff;
			pp++; // skip color
			pp++; // skip normal
			while(c--)
			{
				a=*pp++;
				td=o->v[a].x*pa+o->v[a].y*pb+o->v[a].z*pc+pd;
			}

			onum++;
		}
		qsort(osp,onum,sizeof(osp[0]),qcmp);
		//resort by planes
#if 0
		for(i=0;i<onum;i++)
		{
			for(k=i+1;k<onum;k++)
			{
				for(j=k;j<onum;j++)
				{
					for(a=0;a<8;a++)
					{
						//fprintf(fdeb,"%i%c ",osp[a].o,osp[a].swapped?'*':'.');
					}
					//fprintf(fdeb,"\n");
					a=planecompare(osp+i,osp+j,o,1);
					// -1=j farther than i (swap)
					// 0==indetermined
					// 1==i farther than j (ok)
					// 2==i farther than j.. (next)
					/*
					if(a==2) 
					{
						j=onum;
						break;
					}
					else */
					if(a==-1)
					{	
						break;
					}
				}
				if(j==onum) break; //all faces after i were before it
				memcpy(&osptmp,osp+k,sizeof(osptmp));
				memcpy(osp+k,osp+i,sizeof(osptmp));
				memcpy(osp+i,&osptmp,sizeof(osptmp));
			}
		}
#endif		
		o->plnum=d+2;
		ip=o->pl[d+1];
		*ip++=onum+3;
		ip++; // skip center vertex id
		for(a=0;a<onum;a++)
		{
			*ip++=osp[a].o;
		}
		*ip++=0;
		
		fbak=o->flags;
		o->flags&=~(F_GOURAUD);
		vNext();
		vis_drawobject(o);
		if(!skipw)
		{
			vNext();
			vis_drawobject(o);
			vNext();
			vis_drawobject(o);
			vNext();
			vis_drawobject(o);
			a=getch();
			if(a==27) break;
			if(a==13) skipw=1;
		}
		o->flags=fbak;
	}
}

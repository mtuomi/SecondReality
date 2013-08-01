 
// temporary structs for loading
struct s_tmpvx
{
	long	x;
	long	y;
	long	z;
	int	power; // for center
}; 
struct s_tmpnr
{
	long	x;
	long	y;
	long	z;
}; 
struct s_tmpgnr // gouraud normal
{
	long	x;
	long	y;
	long	z;
	int	power;
}; 
struct s_tmpfc
{
	int	v[3]; // vertex number
	char	vv[3]; // vertex (edge) visible
	char	used;
	char	material;
};

// buffers for reading objects from asc
char oname[64];
int vxleft,fcleft;
int vxmap[MAXVX];
struct s_tmpvx vxdata[MAXVX];
struct s_tmpnr nrdata[MAXNR];
struct s_tmpgnr gvxdata[MAXVX];
struct s_tmpfc fcdata[MAXFC];
struct s_tmpvx *vx=vxdata;
struct s_tmpnr *nr=nrdata;
struct s_tmpfc *fc=fcdata;
struct s_tmpgnr *gvx=gvxdata;
int vxnum,fcnum,nrnum,nrnum1;
int vxcount,fccount; // original in 3DS

int	facedata[16384],facedatalen; // len=words
int	faceoff[MAXFC],facenum;

#include "opt.c"

int	addvx(long x,long y,long z)
{
	int	i;
	for(i=0;i<vxnum;i++)
	{
		if(vx[i].x==x && vx[i].y==y && vx[i].z==z) 
		{
			vx[i].power++;
			return(i);
		}
	}
	vx[vxnum].power=1;
	gvx[vxnum].power=0;
	vx[vxnum].x=x;
	vx[vxnum].y=y;
	vx[vxnum].z=z;
	return(vxnum++);
}

int	addnr(long x,long y,long z)
{
	int	i;
	for(i=0;i<nrnum;i++)
	{
		if(nr[i].x==x && nr[i].y==y && nr[i].z==z) 
		{
			return(i);
		}
	}
	nr[nrnum].x=x;
	nr[nrnum].y=y;
	nr[nrnum].z=z;
	return(nrnum++);
}

void	resetscene(void)
{
	struct s_cobject *co1;
	int	i,a;
	co1=co;
	for(i=0;i<conum;i++)
	{
		co1->last_on=0;
		co1->last_x=0;
		co1->last_y=0;
		co1->last_z=0;
		for(a=0;a<9;a++) co1->last_m[a]=0;
		co1++;
	}
}

int	duplicate(struct s_cobject *co0)
{
	int	a;
	struct s_cobject *co1;
	co1=co+conum;
	co1->index=co0->index;
	strcpy(co1->name,co0->name);
	co1->last_on=0;
	co1->last_x=0;
	co1->last_y=0;
	co1->last_z=0;
	for(a=0;a<9;a++) co1->last_m[a]=0;
	co1->lastused=-1;
	co1->duplicate=1;
	co1->original=co0-co;
	co1->size=co0->size;
	strcpy(co1->fname,co0->fname);
	co1->o=getmem(sizeof(object));
	memcpy(co1->o,co0->o,sizeof(object));
	// new object needs new matrices
	co1->o->r=getmem(sizeof(rmatrix));
	co1->o->r0=getmem(sizeof(rmatrix));
	memset(co1->o->r,0,sizeof(rmatrix));
	memset(co1->o->r0,0,sizeof(rmatrix));
	return(conum++);
}

struct s_cobject *create(void)
{
	int	a;
	object	*o;
	
	co[conum].o=o=getmem(sizeof(object));
	memcpy(co[conum].name,oname,17);
	co[conum].name[17]=0;
	co[conum].index=conum;
	co[conum].last_on=0;
	co[conum].last_x=0;
	co[conum].last_y=0;
	co[conum].last_z=0;
	for(a=0;a<9;a++) co[conum].last_m[a]=0;
	o->flags=F_DEFAULT;
	o->r=getmem(sizeof(rmatrix));
	o->r0=getmem(sizeof(rmatrix));
	calc_setrmatrix_ident(o->r0);
	o->r0->x=o->r0->y=o->r0->z=0;
	o->pd=getmem(facedatalen*2);
	o->pdlen=facedatalen*2;
	o->plnum=1;
	for(a=0;a<SORTDIRS+1;a++)
	{
		o->pl[a]=getmem((3+facenum)*2);
	}
	o->nnum=nrnum;
	o->nnum1=nrnum1;
	o->vnum=vxnum;
	o->v0=getmem(vxnum*sizeof(vlist));
	o->n0=getmem(nrnum*sizeof(nlist));
	o->v=getmem(vxnum*sizeof(vlist));
	o->n=getmem(nrnum*sizeof(nlist));
	o->pv=getmem(vxnum*sizeof(pvlist));
	o->vf=0;
	// copy vertices etc to just allocated object
	for(a=0;a<vxnum;a++)
	{
		o->v0[a].x=vx[a].x;
		o->v0[a].y=vx[a].y;
		o->v0[a].z=vx[a].z;
		o->v0[a].normal=vx[a].power; // power=normal
		o->v0[a].RESERVED=0xfc;
	}
	for(a=0;a<nrnum;a++)
	{
		o->n0[a].x=(int)nr[a].x;
		o->n0[a].y=(int)nr[a].y;
		o->n0[a].z=(int)nr[a].z;
		o->n0[a].RESERVED=0xfc;
	}
	memcpy(o->pd,facedata,facedatalen*2);
	memcpy(o->pl[0]+2,faceoff,2*facenum);
	o->pl[0][0]=facenum+3; // words in list
	o->pl[0][1]=0; // closest=center in list 0 (set later)
	o->pl[0][facenum+2]=0; // end
	return(co+(conum++));
}

int	calccenter(void)
{
	long	x,y,z,p;
	int	a;
	for(x=y=z=0,a=1;a<vxnum;a++)
	{
		p=vx[a].power;
		x+=vx[a].x*p; y+=vx[a].y*p; z+=vx[a].z*p;
	}
	if(vxnum>1)
	{
		x/=vxnum-1; y/=vxnum-1; z/=vxnum-1;
	}
	print("Centerxyz: %li,%li,%li\n",x,y,z);
	return(addvx(x,y,z));
}

void unify(long *x,long *y,long *z)
{
	double	d;
	d=sqrt((double)*x*(double)*x+(double)*y*(double)*y+(double)*z*(double)*z);
	if(!d) return;
	d=1/d;
	*x=(long)(NORMALSIZE*(double)*x*d);
	*y=(long)(NORMALSIZE*(double)*y*d);
	*z=(long)(NORMALSIZE*(double)*z*d);
}

void objectdone(void)
{
	long	x,y,z;
	long	cx,cy,cz;
	int	svx[SORTDIRS+1];
	int	a,center;
	struct s_cobject *co;
	if(vxleft || fcleft) error("Not all vertices (%i missing) or faces (%i missing) found!",vxleft,fcleft);
	optimize(); // combines triangles to larger faces and creates polydata
	center=calccenter(); // calculate center
	cx=vx[center].x;
	cy=vx[center].y;
	cz=vx[center].z;
	for(a=0;a<SORTDIRS;a++) // calculate box for Z sort
	{
		sortdir(a,&x,&y,&z);
		svx[a]=addvx(x+cx,y+cy,z+cz);
	}
	nrnum1=nrnum;
	// add gouraud normals
	for(a=0;a<vxnum;a++) if(gvx[a].power)
	{
		x=gvx[a].x/gvx[a].power;
		y=gvx[a].y/gvx[a].power;
		z=gvx[a].z/gvx[a].power;
		unify(&x,&y,&z);
		vx[a].power=addnr(x,y,z);
	}
	else vx[a].power=-1;
	// all vertices & normals should be in tmp structs now!
	co=create(); // allocate memory for object and copy data to it
	co->duplicate=0;
	co->original=conum-1;
	co->lastused=-1;
	co->o->pl[0][1]=center; // sortlist0's closest vertex should be center
	for(a=0;a<SORTDIRS;a++) co->o->pl[a+1][1]=svx[a];
	print("   Faces: %i => %i\n",fccount,facenum);
	print(" Normals: %i => %i (%i)\n",fccount,nrnum1,nrnum);
	print("Vertices: %i => %i\n",vxcount,vxnum);
	co->f1=fccount; co->f2=facenum;
	co->n1=fccount; co->n2=nrnum1; co->ng=nrnum;
	co->v1=vxcount; co->v2=vxnum;
	*oname=0;
}

void readasc(void)
{
	char	tmp[256];
	int	a,b,i1,i2,i3;
	int	i1v,i2v,i3v;
	long	x,y,z;
	float	f,fx,fy,fz;
	char	*p,*p0,*t;
	print("\n");
	print(">>>>>>>>>>>>>>>> ASC file <<<<<<<<<<<<<<<<\n");
	print("\n");
	while(!feof(in))
	{
		p0=p=getline();
		if(emptyline(p)) continue;
		// process line
		t=getfirsttoken(&p);
		IFT("Named")
		{
			if(*oname) 
			{
				objectdone();
				print("}\n");
				print("\n");
			}
			gettoken(&p); // 'object:'
			t=gettoken(&p);
			strcpy(tmp,t);	
			do
			{
				p0=p=getline();
			}
			while(emptyline(p));
			IFS(p,"Tri-mesh")
			{
				print("TRIMESH %s\n",tmp);
				strcpy(oname,tmp);
				p=p0; 
				getseek(&p,"Vertices:");
				i1=getint(&p);
				p=p0; 
				getseek(&p,"Faces:");
				i2=getint(&p);
				print("vertices:%i faces:%i\n",i1,i2);
				print("{\n");
				//
				vxnum=0;
				nrnum=0;
				fcnum=0;
				vxcount=vxleft=i1;
				fccount=fcleft=i2;
				//
			}
			else 
			{
				print("SKIPPING OBJECT %s (%s)\n",tmp,p);
			}
			continue; 
		}
		if(!*oname) continue; // no object being processed!
		IFT("Vertex")
		{
			t=gettoken(&p);
			IFT("list:") continue;
			i1=atoi(t);
			//if(i1!=vxnum) error("Vertex indices out of order");
			getxxyyzz(&p,&x,&y,&z);
			vxmap[i1]=addvx(x,y,z);
			print("vertex %i (%li,%li,%li)\n",i1,x,y,z);
			vxleft--;
		}
		else IFT("Smoothing:")
		{
			// skip smoothing groups for now
		}
		else IFT("Material:")
		{
			t=p0+10;
			for(a=0;a<strlen(t);a++) if(t[a]=='\"')
			{
				t[a]=0;
				break;
			}
			b=strlen(t);
			for(a=0;a<matnum;a++) 
			{
				if(!memcmp(t,mat[a].name,b))
				{
					fc[fcnum-1].material=a;
					break;
				}
			}
		}
		else IFT("Face")
		{
			t=gettoken(&p);
			IFT("list:") continue;
			a=atoi(t);
			if(a!=fcnum) error("Face indices out of order");
			t=gettoken(&p);
			i1=atoi(t+2);
			t=gettoken(&p);
			i2=atoi(t+2);
			t=gettoken(&p);
			i3=atoi(t+2);
			t=gettoken(&p);
			i1v=t[3]-'0';
			t=gettoken(&p);
			i2v=t[3]-'0';
			t=gettoken(&p);
			i3v=t[3]-'0';
			fc[fcnum].v[0]=vxmap[i1];
			fc[fcnum].v[1]=vxmap[i2];
			fc[fcnum].v[2]=vxmap[i3];
			fc[fcnum].vv[0]=(char)i1v;
			fc[fcnum].vv[1]=(char)i2v;
			fc[fcnum].vv[2]=(char)i3v;
			fc[fcnum].used=0;
			fc[fcnum].material=0;
			print("face %i (%c%i,%c%i,%c%i)\n",a,
				i1v?'+':'-',i1,
				i2v?'+':'-',i2,
				i3v?'+':'-',i3);
			fcnum++;
			fcleft--;
		}
		else print("Unknown: %s\n",t);
		if(debug) print(" (%s)\n",p0);
	}
	if(*oname) 
	{
		objectdone();
		print("}\n");
		print("\n");
	}
}

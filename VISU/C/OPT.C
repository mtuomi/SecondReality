/* ASC optimization from:
	char oname[64];
	struct s_tmpvx *vx;
	struct s_tmpfc *fc;
	int vxnum,fcnum,vxleft,fcleft;
to:
	int	facedata[32767],facedatalen;
	int	faceoff[MAXFC],facenum;
*/

int	currentface;
int	connects[256],connectp;
int	pvnum;
int	pv[MAXFACE];

void	padd(int v) // add vertex to pv
{
	if(pvnum && pv[pvnum-1]==v) return;
	pv[pvnum++]=v;
	if(pvnum>=MAXFACE)
	{
		int	a;
		printf("FATAL ERROR: FACE OVERFLOW (SEE REPORT)");
		print("FATAL: FACE %i OVERFLOWS (max %i sides) [",currentface,MAXFACE);
		for(a=0;a<connectp;a++) print("%i ",connects[a]);
		print("]");
		exit(3);
	}
}

void	pexpand(struct s_tmpfc *f,int i,int cntn) // add face to pv
{
	int	k,l,v1,v2,cnt;
	if(f->used) return;
	f->used=1;
	for(cnt=0;cnt<cntn;cnt++)
	{
		if(f->vv[i]) padd(f->v[i]);
		else
		{ // find the polygon on side and recurse
			l=-1; // nothing found
			v1=f->v[(i==2)?0:i+1];
			v2=f->v[i];
			for(k=0;k<fcnum;k++) if(!fc[k].used)
			{
				if(fc[k].v[0]==v1 && fc[k].v[1]==v2)
				{
					l=1; break;
				}
				if(fc[k].v[1]==v1 && fc[k].v[2]==v2)
				{
					l=2; break;
				}
				if(fc[k].v[2]==v1 && fc[k].v[0]==v2)
				{
					l=0; break;
				}
			}
			if(l==-1)
			{ // failed to find
				padd(f->v[i]);
			}
			else
			{
				connects[connectp++]=k;	
				pexpand(fc+k,l,2);
			}
		}
		i=(i==2)?0:i+1;
	}
}

void	optimize(void)
{
	int	nrm,i,j,flags,a;
	facedatalen=facenum=0;
	facedata[facedatalen++]=0; // 'null' face
	for(j=0;j<vxnum;j++) 
	{
		gvx[j].x=gvx[j].y=gvx[j].z=gvx[j].power=0;
	}
	for(i=0;i<fcnum;i++)
	{
		currentface=i;
		connectp=0;
		pvnum=0;
		connects[connectp++]=i;	
		pexpand(fc+i,0,3);
		if(pvnum)
		{
			long	x=0,y=0,z=0;
			// calc normal
			{
				struct s_tmpvx *v;
				struct s_tmpvx *w;
				int	a,b;
				double	dl;
				for(a=0;a<pvnum;a++)
				{
					v=vx+pv[a]; 
					w=vx+pv[a?(a-1):(pvnum-1)]; 
					x+=(v->y-w->y)*(v->z+w->z);
					y+=(v->z-w->z)*(v->x+w->x);
					z+=(v->x-w->x)*(v->y+w->y);
				}
				dl=sqrt((double)x*(double)x+(double)y*(double)y+(double)z*(double)z);
				if(dl<-1 || dl>1)
				{
					x=-(long)((double)x*NORMALSIZE/dl);
					y=-(long)((double)y*NORMALSIZE/dl);
					z=-(long)((double)z*NORMALSIZE/dl);
				}
				nrm=addnr(x,y,z);
			}
			// write stuff
			flags=mat[fc[i].material].flags;
			faceoff[facenum++]=facedatalen*2; // char offset
			facedata[facedatalen++]=pvnum|flags; // sides | flags
			facedata[facedatalen++]=mat[fc[i].material].color; // color
			facedata[facedatalen++]=nrm; // normal
			for(j=0;j<pvnum;j++) 
			{
				a=pv[j];
				gvx[a].x+=x;
				gvx[a].y+=y;
				gvx[a].z+=z;
				gvx[a].power++;
				facedata[facedatalen++]=a;
			}
		}
	}
}

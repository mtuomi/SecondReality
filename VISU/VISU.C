#include <stdio.h>
#include <string.h>
#include "c.h"

#define GCHAR *(((char *)d)++)
#define GINT *(((int *)d)++)
#define GLONG *(((long *)d)++)

object * vis_loadobject(char *fname)
{
	int	error;
	int	a,b,c;
	object	*o;
	long	l;
	char	*d,*d0;
	d=readfile(fname);
	o=getmem(sizeof(object));
	
	o->flags=F_DEFAULT;
	o->r=getmem(sizeof(rmatrix));
	o->r0=getmem(sizeof(rmatrix));
	memset(o->r,0,sizeof(rmatrix));
	memset(o->r0,0,sizeof(rmatrix));
	o->vnum=0;
	o->nnum=0;
	o->v0=NULL;
	o->n0=NULL;
	o->v=NULL;
	o->n=NULL;
	o->pv=NULL;
	o->plnum=1;
	
	for(;;)
	{
		error=0;
		d0=d;
		d+=8;
		if(!memcmp(d0,"END ",4)) break;
		else if(!memcmp(d0,"VERS",4)) 
		{
			a=GINT;
			if(a!=0x100)
			{
				printf("Version not 1.00\n");
			}
		}
		else if(!memcmp(d0,"NAME",4)) 
		{
			o->name=(char *)d;
		}
		else if(!memcmp(d0,"VERT",4)) 
		{
			o->vnum=GINT;
			GINT;
			o->v0=(vlist *)d;
			o->v=getmem(sizeof(vlist)*o->vnum);
			o->pv=getmem(sizeof(pvlist)*o->vnum);
		}
		else if(!memcmp(d0,"NORM",4))
		{
			o->nnum=GINT;
			o->nnum1=GINT;
			o->n0=(nlist *)d;
			o->n=getmem(sizeof(vlist)*o->nnum);
		}
		else if(!memcmp(d0,"POLY",4)) 
		{
			o->pd=(polydata *)d;
		}
		else if(!memcmp(d0,"ORD",3)) 
		{
			a=d0[3];
			if(a=='0') b=0;
			else if(a=='E') b=o->plnum++;
			else error=1;
			if(!error)
			{
				o->pl[b]=(polylist *)d;
			}
		}
		else error=1;
		if(error)
		{
			printf("Unknown block: %c%c%c%c\n",d0[0],d0[1],d0[2],d0[3]);
		}
		l=*(long *)(d0+4);
		d=d0+l+8;
	}

	#if 0
	printf("Sortlists: ");
	for(a=0;a<9;a++) printf("%i ",o->plv[a]);
	printf("Vertices: %i (%Fp=>%Fp=>%Fp)\n",o->vnum,o->v0,o->v,o->pv);
	printf("Normals: %i (%Fp=>%Fp)\n",o->nnum,o->n0,o->n);
	getch();
	#endif
	
	return(o);
}

void	vis_drawobject(object *o)
{
	int	a,b,c;
	long	al,bl;
	nlist	*n;
	int	*ijp;
	if(!(o->flags&F_VISIBLE)) return;
	calc_rotate(o->vnum,o->v,o->v0,o->r);
	if(o->flags&F_GOURAUD) calc_nrotate(o->nnum,o->n,o->n0,o->r);
	else calc_nrotate(o->nnum1,o->n,o->n0,o->r);
	o->vf=calc_project(o->vnum,o->pv,o->v);
	if(o->vf) return; // object was completely out of screen
	a=0; al=0x7fffffffL;
	for(b=1;b<o->plnum;b++)
	{
		c=o->pl[b][1];
		bl=(o->v+c)->z;
		if(bl<al)
		{
			al=bl;
			a=b;
		}
	}
	draw_polylist(o->pl[a],o->pd,o->v,o->pv,o->n,o->flags);
}

#define determ(c1,c2,r1,r2) (m[c1+r1*3]*m[c2+r2*3]-m[c1+r2*3]*m[c2+r1*3])

int	calc_invrmatrix(rmatrix *r)
{
	double	m0[9];
	double	m[9];
	double	d[9];
	double	det;
	int	a,x,y;
	for(a=0;a<9;a++) m[a]=m0[a]=((double)r->m[a])/UNIT;
	d[0]= determ(1,2,1,2);
	d[1]=-determ(0,2,1,2);
	d[2]= determ(0,1,1,2);
	d[3]=-determ(1,2,0,2);
	d[4]= determ(0,2,0,2);
	d[5]=-determ(0,1,0,2);
	d[6]= determ(1,2,0,1);
	d[7]=-determ(0,2,0,1);
	d[8]= determ(0,1,0,1);
	det=m[0]*d[0]+m[1]*d[1]+m[2]*d[2];
	if(det>-0.0001 && det<0.0001) return(0); // could not invert
	for(x=0;x<3;x++) for(y=0;y<3;y++)
	{
		m[y+x*3]=d[x+y*3]/det;
	}
	for(a=0;a<9;a++) 
	{
		r->m[a]=(long)(m[a]*UNIT);
	}

	#if 0
	printf("\ndet:%10lf\n",det);
	for(y=0;y<3;y++) 
	{
		printf("d[] ");
		for(x=0;x<3;x++) 
		{
			printf("%6.3lf ",d[x+y*3]);
		}
		printf("\n");
	}
	for(y=0;y<3;y++) 
	{
		for(x=0;x<3;x++) 
		{
			printf("%6.3lf ",m0[x+y*3]);
		}
		printf("* ");
		for(x=0;x<3;x++) 
		{
			printf("%6.3lf ",m[x+y*3]);
		}
		printf("= ");
		for(x=0;x<3;x++) 
		{
			det=m[x+0*3]*m0[0+y*3]+m[x+1*3]*m0[1+y*3]+m[x+2*3]*m0[2+y*3];
			printf("%6.3lf ",det);
		}
		printf("\n");
	}
	printf("\n");
	#endif
	return((int)(UNIT*det));
}


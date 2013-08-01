
int	findobject_co;
object *findobject(char *t,int frame) 
{
	int	i,l=strlen(t);
	int	found1=-1;
	for(i=1;i<conum;i++)
	{
		if(!memcmp(co[i].name,t,l))
		{
			if(found1==-1) found1=i;
			if(co[i].lastused!=frame) 
			{
				findobject_co=i;
				co[i].lastused=frame;
				return(co[i].o);
			}
		}
	}
	if(found1!=-1)
	{
		print("^ Object %s duplicated.\n",t);
		i=duplicate(co+found1);
		co[i].lastused=frame;
		findobject_co=i;
		return(co[i].o);
	}
	print("ERROR: object %s not found!\n",t);
	return(co[0].o);
}

int lsize(long l)
{
	if(!l) return(0);
	if(l>=-127 && l<=127) return(1);
	if(l>=-32767 && l<=32767) return(2);
	return(3);
}

void	putl(long l,FILE *f)
{
	putw(l&0xffff,f);
	putw(l>>16,f);
}

void	lwrite(int f,long l)
{
	switch(f&3)
	{
	case 0 : break;
	case 1 : putc(l,outb); break;
	case 2 : putw(l,outb); break;
	case 3 : putl(l,outb); break;
	}
}

void readvue(void)
{	
	static int lastobject_co=0;
	int	a,ix,iy,visfield;
	int	frame;
	long	x,y,z,x2,y2,z2,ll;
	float	f,fx,fy,fz;
	rmatrix	tmpm;
	rmatrix	tmpm2;
	char	*p,*p0,*t;
	object	*o;
	print("\n");
	print(">>>>>>>>>>>>>>>> VUE file <<<<<<<<<<<<<<<<\n");
	print("\n");
	while(!feof(in))
	{
		p0=p=getline();
		if(emptyline(p)) continue;
		// process line
		t=gettoken(&p);
		IFT("frame")
		{
			t=gettoken(&p);
			frame=a=atoi(t);
			if(!drawpass)
			{ // init stuff for script
				lastobject_co=0;
			}
			print("FRAME %i\n",a);
			if(kbhit()) break;
		}
		else IFT("transform")
		{
			t=gettoken(&p);
			print("TRANSFORM %s",t);
			o=findobject(t,frame);
			o->r0->m[0]=(long)(UNIT*getfloat(&p));
			o->r0->m[3]=(long)(UNIT*getfloat(&p));
			o->r0->m[6]=(long)(UNIT*getfloat(&p));
			o->r0->m[1]=(long)(UNIT*getfloat(&p));
			o->r0->m[4]=(long)(UNIT*getfloat(&p));
			o->r0->m[7]=(long)(UNIT*getfloat(&p));
			o->r0->m[2]=(long)(UNIT*getfloat(&p));
			o->r0->m[5]=(long)(UNIT*getfloat(&p));
			o->r0->m[8]=(long)(UNIT*getfloat(&p));
			getxyz(&p,&x,&y,&z);
			o->r0->x=x;
			o->r0->y=y;
			o->r0->z=z;
			print(" (xyz: %li %li %li)\n",x,y,z);
			o->flags&=~(F_GOURAUD);
		}
		else IFT("camera")
		{
			getxyz(&p,&x2,&y2,&z2);
			print("CAMERA (%li,%li,%li)->",x2,y2,z2);
			getxyz(&p,&x,&y,&z);
			print("(%li,%li,%li) ",x,y,z);
			f=getfloat(&p); // roll
			print("Roll:%f ",f);
			a=(angle)f*65536L/360L;
			calc_setrmatrix_camera(&camera,x2,y2,z2,x,y,z,a);
			f=getfloat(&p); // field of vision
			f=(float)usefov;
			print("FOV:%f \n",f);
			f=f*65536.0/360.0;
			visfield=(angle)f;
			visfield&=0xff00;
			vid_cameraangle(visfield);
			
			vDraw();

  			if(!drawpass)
			{
			   int coi,coilast=0;
			   unsigned int u;
			   struct s_cobject *cop;
			   object *o;
			   for(coi=1;coi<conum;coi++)
			   {
			   	co[coi].on=0;
			   }
			   // scan fill_object for truely visible objects
			   for(u=0;u<64000;u++)
			   {
			   	a=fill_object[u];
				if(a) co[a].on=1;
			   }
			   // camera always on!
			   co[0].on=1;
			   co[0].last_on=1;
			   fprintf(fdeb,"%i\n",conum);
			   for(coi=0;coi<conum;coi++)
			   {
				int	flag,a,b,pflaglen;
				long	pflag,mbytes;
				long	wx,wy,wz;
				long	l;

				flag=pflag=pflaglen=0;
				cop=co+coi;
				o=cop->o;
				
			   	fprintf(fdeb,"%s:%i\n",cop->name,cop->on);
			   
				if(!cop->on)
				{ // object hidden
					if(cop->last_on)
					{
						flag=0x40;
						cop->last_on=0;
					}
				}
				else
				{ // visible
				
					if(!cop->last_on)
					{
						flag=0x80;
						cop->last_on=1;
					}
					
					l=o->r0->z - cop->last_z;
					a=lsize(l); wz=l;
					pflag<<=2; pflag|=a;
					
					l=o->r0->y - cop->last_y;
					a=lsize(l); wy=l;
					pflag<<=2; pflag|=a; 
					
					l=o->r0->x - cop->last_x;
					a=lsize(l); wx=l;
					pflag<<=2; pflag|=a;
					
					mbytes=1;
					print("[pfl:%06lX]",pflag);
					for(a=0;a<9;a++) 
					{
						b=o->r0->m[a] - cop->last_m[a];
						if(b<-127 || b>127) mbytes=0;
						if(b) pflag|=0x80L<<a;
					}
					print("[pfl:%06lX]",pflag);
					if(!mbytes) pflag|=0x40;
					
					if(!pflag) pflaglen=0;
					else if(!(pflag&0xffffff00L)) pflaglen=1;
					else if(!(pflag&0xffff0000L)) pflaglen=2;
					else pflaglen=3;
					flag|=0x10*pflaglen;
				}
					
				if(flag)
				{
					if( (coi&0xff0)!=(coilast&0xff0) )
					{
						putc(0xC0|(coi>>4),outb);
						putc((coi&0x0f)|flag,outb);
					}
					else
					{
						putc((coi&0x0f)|flag,outb);
					}
					coilast=coi;
					if(pflaglen)
					{
						l=pflag;
						while(pflaglen--) 
						{
							putc(l,outb);
							l>>=8;
						}
						print("[pfl:%06lX wxyz:%li,%li,%li]\n",pflag,wx,wy,wz);
						lwrite(pflag,wx);
						lwrite(pflag>>2,wy);
						lwrite(pflag>>4,wz);
						for(a=0;a<9;a++) if(pflag&(0x80<<a))
						{
							b=o->r0->m[a] - cop->last_m[a];
							if(mbytes) putc(b,outb);
							else putw(b,outb);
						}
					}
					for(a=0;a<9;a++) cop->last_m[a]=o->r0->m[a];
					cop->last_x=o->r0->x;
					cop->last_y=o->r0->y;
					cop->last_z=o->r0->z;
				}
			    }
			    putc(0xff,outb);
			    putc(visfield>>8,outb);
			}
		}
		else print("Unknown: %s\n",t);
		if(debug) print(" (%s)\n",p0);
	}
	putc(0xff,outb);
	putc(0xff,outb);
}

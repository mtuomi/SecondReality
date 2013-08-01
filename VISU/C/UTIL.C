
void	print(char *s,...)
{
	static int skip=0;
	static int lines=-1;
	int a;
	char out[256];
	char *p;
	va_list	argp;
	va_start(argp,s);

	if(!reporton) return;

	vsprintf(out,s,argp);
	fprintf(freport,out);

	if(!printon) return;

	if(lines==-1) 
	{
		lines=0;
		printf("-------------------------------------------------------------------------------\n");
	}

	p=out;
	while(*p) if(*(p++)=='\n') lines++;
	fputs(out,stdout);
	if(lines>23 && !skip) 
	{
		lines=-1;
		printf(  "--------[space for next page, enter to disable paging, esc to abort]---------");
		a=getch();
		printf("\r");
		if(a==10) 
		{
			printon=0;
			skip=1;
		}
		if(a==13) skip=1;
		if(a==27) 
		{
			printf("\n\n");
			exit(1);
		}
	}
	va_end(argp);
}

void	error(char *s,...)
{
	int a;
	char out[256];
	char *p;
	va_list	argp;
	va_start(argp,s);
	vsprintf(out,s,argp);
	print("ERROR: %s\n",out);
	va_end(argp);
}

void	*getmem(long size)
{
	void	*p;
	if(size>65000L)
	{
		printf("GETMEM: attempting to reserved >64K (%li byte block)\n",size);
		exit(3);
	}
	p=malloc((size_t)size);
	if(!p)
	{
		printf("GETMEM: out of memory (%li byte block)\n",size);
		exit(3);
	}
	return(p);
}

void	freemem(void *p)
{
	free(p);
}

char	*getline(void)
{
	static char linebuf[1024];
	char	*p;
	linebuf[2]=0;
	fgets(linebuf+2,1024-2,in);
	*(linebuf+2+strlen(linebuf+2)-1)=0;
	return(linebuf+2);
}

int	emptyline(char *p) // line start == space => empty
{ 
	if(!*p || isspace(*p)) return(1);
	return(0);
}

char	*getseek(char **p,char *s)
{
	char	*t;
	t=strstr(*p,s);
	if(!t) return(NULL);
	*p=t+strlen(s);
	return(t);
}

char	*gettoken(char **p)
{
	static char tokenbuf[256];
	char *t=tokenbuf;
	while(isspace(**p)) (*p)++;
	while(!isspace(**p) && **p) 
		*t++=*(*p)++;
	*t=0;
	while(isspace(**p)) (*p)++;
	return(tokenbuf);
}

char	*getfirsttoken(char **p)
{
	static char tokenbuf[256];
	char *t=tokenbuf;
	while(isspace(**p)) (*p)++;
	while(!isspace(**p) && **p && **p!='"') 
		*t++=*(*p)++;
	*t=0;
	while(isspace(**p)) (*p)++;
	return(tokenbuf);
}

int	getint(char **p)
{
	char	*t;
	t=gettoken(p);
	return(atoi(t));
}

float	getfloat(char **p)
{
	char	*t;
	float	f;
	t=gettoken(p);
	sscanf(t,"%f",&f);
	return(f);
}

void	getxzy(char **p,long *x,long *y,long *z)
{
	char	*t;
	float	f;
	t=gettoken(p);
	sscanf(t,"%f",&f);
	*x=(long)(f*fxmul+fxadd);
	t=gettoken(p);
	sscanf(t,"%f",&f);
	*z=(long)(f*fymul+fyadd);
	t=gettoken(p);
	sscanf(t,"%f",&f);
	*y=(long)(f*fzmul+fzadd);
}

void	getxyz(char **p,long *x,long *y,long *z)
{
	char	*t;
	float	f;
	t=gettoken(p);
	sscanf(t,"%f",&f);
	*x=(long)(f*fxmul+fxadd);
	t=gettoken(p);
	sscanf(t,"%f",&f);
	*y=(long)(f*fymul+fyadd);
	t=gettoken(p);
	sscanf(t,"%f",&f);
	*z=(long)(f*fzmul+fzadd);
}

void	getxxyyzz(char **p,long *x,long *y,long *z)
{ /* skips X: Y: and Z: */
	char	*t;
	float	f;
	t=gettoken(p); // X:
	if(!memcmp(t,"X:",3)) t=gettoken(p);
	else t+=2;
	sscanf(t,"%f",&f);
	*x=(long)(f*fxmul+fxadd);
	t=gettoken(p); // Y:
	if(!memcmp(t,"Y:",3)) t=gettoken(p);
	else t+=2;
	sscanf(t,"%f",&f);
	*y=(long)(f*fymul+fyadd);
	t=gettoken(p); // Z:
	if(!memcmp(t,"Z:",3)) t=gettoken(p);
	else t+=2;
	sscanf(t,"%f",&f);
	*z=(long)(f*fzmul+fzadd);
}

#define IFT(zz) if(!stricmp(zz,t))
#define IFS(z1,z2) if(!memicmp(z1,z2,sizeof(z2)-1))

char	*readfile(char *name)
{
	return(NULL);
}


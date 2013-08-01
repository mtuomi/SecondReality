
void readmat(void)
{
	char	tmp[256];
	int	a,b,c,d;
	char	*p,*p0,*t;
	matnum=0;
	printf("Materials:\n");
	while(!feof(in))
	{
		p0=p=getline();
		if(*p==';') continue;
		if(*p)
		{
			t=gettoken(&p);
			for(a=0;a<strlen(t);a++) if(t[a]=='#') t[a]=' ';
			strcpy(mat[matnum].name,t);
			mat[matnum].color=0;
			mat[matnum].flags=0;
			mat[matnum].colorlen=1;
			while(*p)
			{
				t=gettoken(&p);
				a=toupper(*t);
				switch(a)
				{
				case 'X' :
					mat[matnum].flags|=F_2SIDE;
					break;
				case 'L' :
					mat[matnum].colorlen=atoi(t+1);
					break;
				case 'G' :
					mat[matnum].flags|=F_GOURAUD;
					break;
				default :
					if(a>='0' && a<='9')
					{
						mat[matnum].color=atoi(t);
					}
					break;
				}
			}
			switch(mat[matnum].colorlen)
			{
			case 32 : mat[matnum].flags|=F_SHADE32; break;
			case 16 : mat[matnum].flags|=F_SHADE16; break;
			case 8 : mat[matnum].flags|=F_SHADE8; break;
			case 1 : break;
			default : printf("ILLEGAL COLOR LENGTH>> "); break;
			}
			sscanf(p,"%s %i %i",mat[matnum].name,&mat[matnum].color,&mat[matnum].colorlen);
			printf("%s colors:%i..%i ",mat[matnum].name,mat[matnum].color,
						mat[matnum].color+mat[matnum].colorlen-1);
			if(mat[matnum].flags&F_GOURAUD) printf("Gouraud ");
			if(mat[matnum].flags&F_TEXTURE) printf("Texture ");
			printf("\n");
			matnum++;
		}
	}
}
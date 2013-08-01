// this file contains seldom needed routines used mainly by the object 
// converter and utilities. The routines here also use math. 

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "c.h"

void	calc_setrmatrix_camera(rmatrix *camera,long x2,long y2,long z2,long x,long y,long z,int roll)
{
	int	a;
	long	ll;
	float	f,fx,fy,fz;
	rmatrix	tmpm;
	rmatrix	tmpm2;
	{
		calc_setrmatrix_ident(camera);
		camera->x=-x2;
		camera->y=-y2;
		camera->z=-z2;
		{
			double	fx,fy,fz,gx,gy,gz,hx,hy,hz;
			double	fd;
			
			/* Z */
			fx=(double)(x-x2); 
			fy=(double)(y-y2); 
			fz=(double)(z-z2);
			fd=1/sqrt(fx*fx+fy*fy+fz*fz);
			fx*=fd; fy*=fd; fz*=fd;
			
			/* X */
			hx=fy;
			hy=-fx;
			hz=0;
			fd=1/sqrt(hx*hx+hy*hy+hz*hz);
			hx*=fd; hy*=fd; hz*=fd;
			
			/* Y */
			gx=fy*hz-hy*fz;
			gy=fz*hx-hz*fx;
			gz=fx*hy-hx*fy;
			fd=1/sqrt(gx*gx+gy*gy+gz*gz);
			gx*=fd;	gy*=fd;	gz*=fd;
			
			tmpm.m[0]=(long)(UNIT*( hx ));
			tmpm.m[1]=(long)(UNIT*( gx ));
			tmpm.m[2]=(long)(UNIT*( fx ));
			tmpm.m[3]=(long)(UNIT*( hy ));
			tmpm.m[4]=(long)(UNIT*( gy ));
			tmpm.m[5]=(long)(UNIT*( fy ));
			tmpm.m[6]=(long)(UNIT*( hz ));
			tmpm.m[7]=(long)(UNIT*( gz ));
			tmpm.m[8]=(long)(UNIT*( fz ));
			tmpm.x=tmpm.y=tmpm.z=0;
		}
		#if 0
		print("ORIGINAL:\n");
		print("%6i %6i %6i\n",tmpm.m[0],tmpm.m[1],tmpm.m[2]);
		print("%6i %6i %6i\n",tmpm.m[3],tmpm.m[4],tmpm.m[5]);
		print("%6i %6i %6i\n",tmpm.m[6],tmpm.m[7],tmpm.m[8]);
		memcpy(&tmpm2,&tmpm,sizeof(rmatrix));
		a=calc_invrmatrix(&tmpm);
		print("INVERSE (det=%i):\n",a);
		print("%6i %6i %6i\n",tmpm.m[0],tmpm.m[1],tmpm.m[2]);
		print("%6i %6i %6i\n",tmpm.m[3],tmpm.m[4],tmpm.m[5]);
		print("%6i %6i %6i\n",tmpm.m[6],tmpm.m[7],tmpm.m[8]);
		calc_mulrmatrix(&tmpm2,&tmpm);
		print("ORIGINAL*INVERSE:\n");
		print("%6i %6i %6i\n",tmpm2.m[0],tmpm2.m[1],tmpm2.m[2]);
		print("%6i %6i %6i\n",tmpm2.m[3],tmpm2.m[4],tmpm2.m[5]);
		print("%6i %6i %6i\n",tmpm2.m[6],tmpm2.m[7],tmpm2.m[8]);
		#else
		calc_invrmatrix(&tmpm);
		#endif
		calc_mulrmatrix(camera,&tmpm);
		calc_setrmatrix_rotxyz(&tmpm,0,-roll,0);
		tmpm.x=0; tmpm.y=0; tmpm.z=0;
		calc_applyrmatrix(camera,&tmpm);
	}
}

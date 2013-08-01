;/****************************************************************************
;** MODULE:	c.h
;** AUTHOR:	Sami Tammilehto / Fennosoftec OY
;** DOCUMENT:	?
;** VERSION:	1.0
;** REFERENCE:	-
;** REVISED BY:	-
;*****************************************************************************
;**
;** C / Include
;** - variable definitions
;** - constant definitions
;**
;****************************************************************************/

#ifndef INCLUDED_C

/*####### Data types ########*/

#ifndef INCLUDED_CD
#include "cd.h"
#endif

/*####### Prototypes for *EXTERNAL* c routines ########*/

extern void *	getmem(long bytes);	// SHOULD BE PROVIDED EXTERNALLY
extern void	freemem(void *);	// SHOULD BE PROVIDED EXTERNALLY
extern char *	readfile(char *name);	// SHOULD BE PROVIDED EXTERNALLY

/*####### Prototypes for c routines ########*/

object * vis_loadobject(char *fname); // visu.c
void	vis_drawobject(object *o); // visu.c

/*####### Prototypes for asm routines ########*/

void	vid_init(int mode);
void	vid_window(long x1,long x2,long y1,long y2,long z1,long z2);
void	vid_cameraangle(angle a);
void	vid_inittimer(void);
void	vid_deinittimer(void);
void	vid_deinit(void);
void	vid_setpal(char *pal);
void	vid_clear(void);
void	vid_clearbg(char *);
void	vid_skyclear(char *sky);
void	vid_switch(void);
void	vid_waitb(void);
void	vid_drawpolylist(polylist *,polydata *,vlist *,pvlist *,nlist *); // OBSOLOTE, USE draw_polylist
void	vid_pic320200(char *,int,int);
void	vid_drawsight(char *);
void	vid_pset(int x,int y,int color);

void	vid_dotdisplay_pvlist(int count,pvlist *list);
void	vid_dotdisplay_zcolor(int count,pvlist *list1,vlist *list2);
void	vid_poly(int color,int sides,...);

int	calc_project(int count,pvlist *dest,vlist *source);
int	calc_project16(int count,pvlist *dest,vlist *source); // for 16 bits
void	calc_setrmatrix_rotyxz(rmatrix *matrix,angle rotx,angle roty,angle rotz);
void	calc_setrmatrix_rotxyz(rmatrix *matrix,angle rotx,angle roty,angle rotz);
void	calc_setrmatrix_rotzyx(rmatrix *matrix,angle rotx,angle roty,angle rotz);
void	calc_setrmatrix_ident(rmatrix *matrix);
void	calc_sftranslate(int count,vlist *dest,long xt,long yt,long zt);
void	calc_rotate(int count,vlist *dest,vlist *source,rmatrix *matrix);
void	calc_rotate16(int count,vlist *dest,vlist *source,rmatrix *matrix);
void	calc_nrotate(int count,nlist *dest,nlist *source,rmatrix *matrix);
void	calc_mulrmatrix(rmatrix *dest,rmatrix *source);
int	calc_invrmatrix(rmatrix *dest);
void	calc_applyrmatrix(rmatrix *dest,rmatrix *apply);
long	calc_singlez(int vertexnum,vlist *vertexlist,rmatrix *matrix);
void	calc_setrmatrix_camera(rmatrix *,long,long,long,long,long,long,int);

void	draw_polylist(polylist *,polydata *,vlist *,pvlist *,nlist *,int);
void	draw_setfillroutine(void (*)(int *));

/*####### Constants ########*/

#define	MAXROWS	480
#define UNIT 16384
#define UNITSHR 14

/*####### External Assembler variables ########*/

extern	long	datanull;
extern	int	rows[MAXROWS];
extern	int	vramseg;
extern	long	projclipx[2];
extern	long	projclipy[2];
extern	long	projclipz[2];
extern	long	projmulx;
extern	long	projmuly;
extern	long	projaddx;
extern	long	projaddy;

#define INCLUDED_CD
#endif

#include <conio.h>
#include <dos.h>
#include "..\dis\dis.h"
#pragma	 inline

extern void tw_opengraph();
extern void tw_waitvr();
extern void tw_setpalette(char far *pal);
extern void tw_setstart(int s);

extern	init_copper();
extern	close_copper();
extern	far int frame_count;
extern  far char *cop_pal;
extern  far int do_pal;

extern	init_uframe(int seg);
extern	ulosta_frame(int start);
extern 	char far pal[];
extern 	far char memblock[];

int	wfade[100]=    {63,32,16,8,4,2,1,0,0,0,
			0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,
			1,2,4,6,9,14,20,28,37,46,
			56,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63};

main()
	{
	int	a,f=0,p=0,frames;
	unsigned  m=65535,b,segp;
	long	len;

	dis_partstart();

	for(a=1;a<64;a++) for(b=0;b<768;b++)
		pal[a*768+b]=(63*a+(64-a)*pal[b])/64;

	while(dis_sync()<10&&!dis_exit());
	tw_waitvr();
	tw_setpalette(&pal[768*63]);
	tw_opengraph();
	init_uframe(FP_SEG(memblock));
	init_copper();
	frame_count=0;
	while(!dis_exit() && f++<45)
		{
		while(frame_count<4); frame_count=0;
		if(f<=40)
			{
			if(p)   {
				p=0;
				ulosta_frame(0x0a400);
				tw_setstart(16384);
				}
			else	{
				p=1;
				ulosta_frame(0x0a000);
				tw_setstart(0U);
				}
			}
		cop_pal=&pal[768*wfade[f]];
		do_pal=1;
		}
	close_copper();
	}


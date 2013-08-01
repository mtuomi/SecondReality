extern void tw_opengraph();
extern void tw_closegraph();
extern void tw_putpixel(int x, int y, int color);
extern int  tw_getpixel(int x, int y);
extern void tw_setpalette(void far *pal);
extern void tw_setpalarea(void far *pal,int start,int cnt);
extern void tw_setrgbpalette(int pal, int r, int g, int b);
extern void tw_setstart(int start);
extern void tw_pictovmem(void far *pic, int to, int len);
extern void tw_crlscr();

extern int far scr_seg;
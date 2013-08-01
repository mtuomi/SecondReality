/* UNREAL 2 - Startup menu
*/
#include <stdio.h>
#include <dos.h>
#include <string.h>
#include <malloc.h>
#include "..\dis\dis.h"

int col[]={
0x00, /* 0=default color */
0x08, /* 1=switch name */
0x09, /* 2=switch value */
0x0a, /* 3=switch description */
0x0b, /* 4=bottom line */
0x0c, /* 5=bottom line key */
0x0f, /* 6=DONOTDISTRIBUTE */
};

#include "vidtext.c"

int	nilli;
int	menuy=0;
int	*menux=&nilli;

#define RGB(r,g,b) { outp(0x3c9,r); outp(0x3c9,g); outp(0x3c9,b); }

int	m_detail=0;
int	m_soundcard=3;
int	m_soundquality=2;
int	m_looping=0;
int	m_exit=0;

void	delay(int f)
{
	while(f--)
	{
		while(!(inp(0x3da)&8));	
		while((inp(0x3da)&8));	
	}
}

menu()
{
	int	y,a;
	
	delay(1);

	outp(0x3c8,0);
	for(a=0;a<768;a++) outp(0x3c9,0);

	delay(5);
	
	dosgotoxy(0,50);
	gotoxy(0,0);
	for(y=0;y<50;y++) prtt("~0                                                                                ");
	gotoxy(0,0);
	prtt("    ~4Ы~5S~4Ы~5E~4Ы~5C~4Ы~5O~4Ы~5N~4Ы~5D~4Ы~5 ~4Ы~5R~4Ы~5E~4Ы~5A~4Ы~5L~4Ы~5I~4Ы~5T~4Ы~5Y~4Ы  "
		"~3(7-OCT-93)~0 ~4 Copyright (C) 1993 ~5Future Crew~4");
	/*
	gotoxy(0,2);
	prtt("  ~6Do not distribute! (just had to say that) This version must not be shown to \n");
	prtt("  ~6anyone outside Future Crew. Remember, officially: we aren't sure we can get \n");
	prtt("  ~6this thing finished for Assembly, but we sure are trying to!");
	*/
	gotoxy(0,24);
	prtt("    ~5\x18~4/~5\x19~4 - change selection  ~5\x1b~4/~5\x1a~4 - change an entry  ~5<ды~4 - continue  ~5ESC~4 - exit ");

	mainmenu();

	delay(20);

	outp(0x3c8,0);
	RGB(0,0,0);
	RGB(16,16,24); // bar
	RGB(13,0,0);
	RGB(43,0,0);
	RGB(23,0,0);
	RGB(53,0,0);
	RGB(33,0,0); // do not use
	RGB(63,0,0);
	outp(0x3c8,56);
	RGB(0,50,63);
	RGB(0,60,63);
	RGB(25,25,30);
	RGB(0,30,60);
	RGB(0,50,60);
	RGB(63,0,0);
	RGB(63,0,0);
	RGB(63,0,0);

	for(;;)	
	{
		mainmenu();
		a=getch();
		if(a==13) break;
		if(a==27) 
		{
			m_exit=1;
			break;
		}
		if(!a) a=1000+getch();
		switch(a)
		{
		case 1072 : menuy--; break;
		case 1080 : menuy++; break;
		case 1075 : (*menux)--; break;
		case 1077 : (*menux)++; break;
		}
	}
	
	gotoxy(0,0);
	for(y=0;y<50;y++) prtt("~0                                                                                ");
	
	if(!m_soundcard)
	{
		gotoxy(0,10);
		prtt(  "~4        You have selected the ~5no sound~4 option. Please note that even\n"
			 "        though the music is not playing, the demo is still syncronized to\n"
			 "        it. This means that there are delays (like in the beginning) that\n"
			 "        feel unnecessarily long. There is no way to skip parts while you\n"
			 "        watch the demo, so you can only wait.\n");
		prtt("~5\n        Press any key to continue...\n");
		getch();
		gotoxy(0,0);
		for(y=0;y<50;y++) prtt("~0                                                                                ");
	}
}

int	menucolorize(int index,int *x)
{
	int	a;
	static char *v;
	if(menuy==index-1)
	{
		v[4*2]=175;
		//v[4*2+1]=7;
		v[80*2-4*2]=174;
		//v[80*2-4*2+1]=7;
	}
	if(menuy==index)
	{
		menux=x;
		col[1]|=0x10;
		col[2]|=0x10;
	}
	else
	{
		col[1]&=~0x10;
		col[2]&=~0x10;
	}
	v=vram+vramp;
}

int	mainmenu(void)
{
	int	a;
	
	if(menuy<1) menuy=1;
	if(menuy>3) menuy=3;
	
	if(m_detail<0) m_detail=1;
	if(m_detail>1) m_detail=0;
	if(m_soundcard<0) m_soundcard=3;
	if(m_soundcard>3) m_soundcard=0;
	if(m_soundquality<0) m_soundquality=2;
	if(m_soundquality>2) m_soundquality=0;
	if(m_looping<0) m_looping=1;
	if(m_looping>1) m_looping=0;
	gotoxy(0,2);
	
/*	
	menucolorize(0,&m_detail);
	switch(m_detail)
	{
	case 0:	prtf(
"~0    ~1       Detail:~2 Full (486 or higher)                                      ~0  \n"
		); break;
	case 1:	prtf(
"~0    ~1       Detail:~2 Reduced (386)                                             ~0  \n"
		); break;
	} 
*/
	col[1]&=~0x10; col[2]&=~0x10; prtf(
"~0        ~3 This demonstration has been designed for fast machines, which         ~0  \n"
"~0        ~3 in this case means 33Mhz 486 computers. The demo also runs on         ~0  \n"
"~0        ~3 slower 386 computers, but some parts will naturally slow down.        ~0  \n"
"~0        ~3 In case you encounter difficulties running this demo, try             ~0  \n"
"~0        ~3 rebooting you computer with a clean boot (no TSRs etc.)               ~0  \n"
	"\n");
	menucolorize(1,&m_soundcard);
	switch(m_soundcard)
	{
	case 0:	prtf(
"~0    ~1       Soundcard:~2 No sound                                               ~0  \n"
		); break;
	case 1:	prtf(
"~0    ~1       Soundcard:~2 SoundBlaster (mono)                                    ~0  \n"
		); break;
	case 2:	prtf(
"~0    ~1       Soundcard:~2 SoundBlaster Pro (stereo)                              ~0  \n"
		); break;
	case 3:	prtf(
"~0    ~1       Soundcard:~2 Gravis Ultrasound, 512K of memory (stereo)             ~0  \n"
		); break;
	}
	col[1]&=~0x10; col[2]&=~0x10; prtf(
"~0        ~3 With Gravis Ultrasound or no music, the demo requires only            ~0  \n"
"~0        ~3 570,000 bytes of conventional memory. With SoundBlaster, an           ~0  \n"
"~0        ~3 additional 1MB of expanded memory (EMS) is required.                  ~0  \n"
	"\n");
	menucolorize(2,&m_soundquality);
	switch(m_soundquality)
	{
	case 0:	prtf(
"~0    ~1       Sound quality:~2 Poor                                               ~0  \n"
		); break;
	case 1:	prtf(
"~0    ~1       Sound quality:~2 Standard                                           ~0  \n"
		); break;
	case 2:	prtf(
"~0    ~1       Sound quality:~2 High                                               ~0  \n"
		); break;
	}
	col[1]&=~0x10; col[2]&=~0x10; prtf(
"~0        ~3 For SoundBlaster cards, the quality directly effects the mixing       ~0  \n"
"~0        ~3 rate. For the Gravis Ultrasound, the quality has no effect. If        ~0  \n"
"~0        ~3 the demo runs too slowly, try decreasing the sound quality.           ~0  \n"
	"\n");
	menucolorize(3,&m_looping);
	switch(m_looping)
	{
	case 0:	prtf(
"~0    ~1       Demo looping:~2 Disabled                                            ~0  \n"
		); break;
	case 1:	prtf(
"~0    ~1       Demo looping:~2 Enabled (no end scroller shown)                     ~0  \n"
		); break;
	}
	col[1]&=~0x10; col[2]&=~0x10; prtf(
"~0        ~3 If you wish the demo to run continuously, enable this option.         ~0  \n"
	"\n");
	menucolorize(4,&m_looping);
}

/* Scream Tracker V3.0 Music Interface Kit V1.1á
** Copyright (C) 1991,1992,1993 Sami Tammilehto
**
** C-header file with label explanations
** Use LARGE memory model.
*/

/**** ROUTINES (C-CALLING CONVENTION) ****/

extern int zinit(void);
/* after _zoutput* has been set, call this routine to initialize the
   soundcard and the player routines */
   
extern int zinitmodule(char far *module);
/* this routine calculates some internal pointers for the module to
   be played. It must be called before zplaysong(). Note that only
   one module at a time can be in initialized state. All the zplaysong()
   etc routines use the module initialized with this routine */

extern int zinitanothermodule(char far *module);

extern int zloadinstruments(void);
extern int zloadinstrument(int);

extern int zsoundon(void);
/* this routine turns on the interrupts etc and engages the mixing
   engine. Call this before zplaysong(). Well, the following might
   sound weird :-) but with some configurations (SBpro with fast
   machine at least sometimes) you need to initialize like this:
   zsoundon(); zsoundoff(); zsoundon(); 
   This is of course a big, but I haven't had time to debug it yet :-) 
   Anyhow, with the previous three call sequence, all seems to work! */
   
extern int zsoundoff(void);
/* this routine turns off the interrupts and stops mixing. Call this
   when the playing is finished to clean up interrupts taken by 
   zsoundon() */
   
extern int zshutup(void);
/* this routine shuts up all the sounds playing without deinitializing
   the player (like zsoundoff() does) */
   
extern int zplaysong(int order);
/* this routine starts playing the module at order [order] */

extern int zgotosong(int order,int row);
/* this routine seeks to the selected [order] and [row] in the module */

extern int zstopsong(void); 
/* this routine stops the module being played, but keeps the mixing engine
   on for zplaynote() etc */
   
extern int zplaynote(int channel,int note,int ins,int vol,int cmd,int info);
/* this command plays a [note] (hi nibble=octave, lo nibble=note) on [channel]
   with volume [vol], command [cmd] and infobyte [info] */
   
extern void zpollme(void);
/* this routine must be called about 30..100 times a second when the zpollmix
   variable is set. See the zpollmix description for more info */
   
/**** VARIABLES ****/

/**** INITIALIZATION VARIABLES (SET BEFORE ZINIT) ****/

/* the next 5 variables give the current soundcard settings */
extern int far		zoutputmode;
/* for zoutputmode:
   0=none :-)
   1=SoundBlaster
   2=PC-Speaker (this works VERY badly, if at all)
   3=SoundMaster II
   4=Covox Speech Thing (at address zoutputio, this works VERY badly too)
   5=SoundBlaster Pro (stereo if the song's stereo bit set) */
extern int far		zoutputio;
extern int far		zoutputirq;
extern int far		zoutputdma; /* currently this has no effect :-( */
extern int far		zoutputladlib; /* address of *left* adlib */
extern int far		zoutputradlib; /* address of *right* adlib */

extern unsigned far	zmixspeed;
/* this is the speed at which the data is mixed. Keep it above 4Khz to
   avoid SBC from crashing and below 22Khz for normal SB, 44Khz for pro.
   In stereo mode, the actual mixing rate is HALF this rate, since
   every actual sample takes two bytes! */

extern unsigned far	zbufsize;
/* this is the size of the DMA buffer. The smaller the size, the more 
   'real time' are the np_* variables. The optimum is about twice
   the amount of bytes the mixer needs to mix each time it is called.
   Generally, if there is weird cracking etc in the sound, try a bigger
   DMA buffer or a lower mixing speed. Note that the value in this register
   *must* be one of the following:
   512	Suggested for speeds <=16Khz (when zpollme called at 70Hz)
   1024 Suggested for speeds <=44Khz (when zpollme called at 70Hz)
   2048 Use these longer buffers if the player is not called very
   4096 often or you encounter cracks in the sound. */
   
/**** INFORMATION VARIABLES ****/

/* all np_ contain the 'now playing' status of the player. They all refer
   to the state of the playing engine, which is a bit ahead of the actual
   music. So depending on the DMA buffer size, things like np_row could
   tell the situation after a few frames */
 
extern int far		np_masterflags;
/* some flags:
   +8=vol 0 optimizations enabled
   +16=full amiga compatibility mode enabled */
   
extern int far		np_mastervol;
/* current master volume */

extern int far		np_mastermul;
/* 7 lower bits: current master multipler (pretty much like master volume)
   bit 8: stereo on/off
   
extern int far		np_speed;
/* current speed, set with command Axx */

extern int far		np_tempo;
/* current tempo, set with command Txx */

/* the next four variables give the current position of the song that is
   being played (mixed) */
extern int far		np_loop;
extern int far		np_ord;
extern int far		np_pat;
extern int far		np_row;
extern int far		np_zinfo;
extern int far		np_zplus;
extern int far		np_zframe;

/* the following two variables are used for EMS sample support, don't
   mess with them unless you know what you are doing :-) */
extern unsigned far	zemspageframe;
extern unsigned far	zemspagehandle;

/**** SPECIAL VARIABLES ****/

extern char far		znoborders;
/* if 0, then the player changes border color when it's active. This
   helps to see how much time the player takes. Normally set to 1 to
   disable borders. The following colors are used in the borders:
   5=DMA/IRQ interrupt
   4=player mixing
   3=player processing notes/effects
   1=player doing special effects/scope/etc misc
   */

extern char far		zpollmix;
/* if 0, the STMIK uses timer interrupt (at 70hz) to mix sound. This
   is the easiest way, but might cause problems with demos etc that
   don't want interrupts at inconvenient times, like when the VGA
   is in the retrace. if 1, then the program using the stmik must
   call the zpollme() routine often enought to keep the music 
   going. In practice calling it once per frame is optimal. The 
   bigger the dma buffer, the longer time the time between calls
   may be. The following procedure in using zpollmix is adviced:
   1. Start playing first normally with zpollmix=0. (zsoundon(),zplaysong())
   2. Set zpollmix to 1
   3. Start calling the zpollme() regularly
   4. Capture the timer interrupt (if necessary)
   5. ... actual program running with zpollme() being called ...
   6. Restore timer interrupt
   7. Call zsoundoff() to restore interrupts to the state before 1. 
   In general about interrupts:
   STMIK captures the DMA interrupt of the soundcard and timer.
   The old timer is called at the normal 18 ticks / sec to keep
   dos clock etc going. If you set zpollmix on, the old timer
   routine still gets called through the STMIK zpollme().
   If you want to disable the dos timer, capture it BEFORE
   initializing stmik and set it point to an iret. Also, in your
   own timer routine, you don't need to call the stmik timer
   interrupt.
   */
   
extern char far		zmemory;
/* special gravis memory control
   */

extern char far		zirqsync;
/* The soundblaster/soundmaster generates an IRQ at regular intervals
   in the process of the DMA transfer. The processing of this interrupt
   takes only a scan line or so and happens about once per second, so
   normally this shouldn't cause any problems to the programs using
   STMIK. However, in some special cases, like some low level VGA demo
   effects, *no* interrupts should occur at bad times, not even a
   DMA interrupt. In these cases, this variable can be set to 1.
   In irqsync mode, the STMIK automatically syncronizes the DMA blocks
   so that the DMA IRQ interrupt will occur *on top* of the actual
   mixing routine. In this mode, *no* music related interrupts *should*
   occur outside the zpollme() routine. IrqSync does lower the sound
   quality a bit, but under normal circumstances it is not noticeable.
   Also, for IrqSync to work properly, the zpollme should be called
   at regular intervals (that is, at the beginning of each frame etc).
   Also, note that it takes a second or so for the zirqsync mode to
   syncronize the DMA interrupt (that is, for a second or so the
   DMA interrupt 'travels' towards the player routine in the frame
   until it locks on place). It is generally recommended, that this
   option is not used unless specially required. Zirqsync generally
   fails at mixing speeds <8000Hz */

/* The zchn struct contains equ/scope/etc info for each channel. No
   exact documentation here... sorry */
extern struct st_zchn
{
	unsigned char	aenabled;
	unsigned char	achannelused;
	unsigned char	aequ;
	unsigned char	channelnum;
	unsigned long	RESERVED1;	//unsigned long	aadr;
	unsigned long	RESERVED2;	//unsigned long	amax;
	unsigned long	RESERVED3;	//unsigned long	ares;
	unsigned int	RESERVED4;	//unsigned	aseg;
	unsigned int	aspd;
	unsigned long	RESERVED5;	//unsigned long	addlh; /* l/h */
	unsigned int	RESERVED6;	//unsigned	acnt;
	unsigned char	avol;
	unsigned char	amixtype;
	unsigned long 	addherz; /* lo/hi */
	unsigned int	asldspd;
	unsigned int  	ac2spd;
	unsigned int	aorgspd;
	unsigned int	avibcnt;
	unsigned int	avib;
	unsigned char	aorgvol;
	unsigned char	alasteff1,alasteff2;
	unsigned char	atreon;
	unsigned char	atrigcnt;
	unsigned char	atremor;
	unsigned char	avslide;
	unsigned char	a0volcut;
	unsigned char	note;
	unsigned char	ins;
	unsigned char	vol;
	unsigned char	cmd;
	unsigned char	info;
	unsigned char	lastins;
	unsigned char	lastnote;
	unsigned char	alastnfo;
	unsigned char	lastadlins;
	unsigned char	addherzretrig;
	unsigned char	addherzretrigvol;
	unsigned char	vibtretype;
	unsigned char	a0clearcnt;
	unsigned char	UNUSED3[1];
	unsigned char	m_none;
	unsigned char	m_vol;
	unsigned int	m_poslow;
	unsigned long	m_pos;
	unsigned long	m_end;
	unsigned long	m_loop;
	unsigned long	m_speed;
	unsigned int	m_safe;
	unsigned int	m_lsafe;
	unsigned long	m_base;
	unsigned char	UNUSED4[4];
} zchn[];

/* The zspeedadjust & zspeedadjust2 are for Gravis Ultrasound and zpollme.
   Don't ask how to use them. Just use them - correctly. */
extern int zspeedadjust;
extern int zspeedadjust2;

/* The following are the EMS interface (link stmikems.300) */
char *	stmik_emsload(unsigned int h);
void	stmik_emsfree(char *p);
void	stmik_emsfreesamples(char *p);


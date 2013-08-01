/* Demo Int Server (DIS) V1.0 - Header & documentation file */

extern int dis_version(void);
/* Initialize dis. IMPORTANT. This routine must be called at start of each
** demo part, it will clear the exitkey flag and do other stuff as well...
** returns version number
** 0x100=V1.0
** 0=dis not installed!
*/

extern int dis_indemo(void);
/* Returns status if the actual demo is running.
** 0=part run from dos
** 1=part run from demo (no mode switches etc)
*/

extern int dis_waitb(void);
/* waits for border, retuns number of frames since last call.
** (currently returns always 1)
*/

extern int dis_exit(void);
/* returns 1 if part should exit, 0 if not.
** Currently any key press sets dis_exit return status to 1.
*/

extern void dis_partstart(void);
/* initializes dis (calls dis_version), if dis is not detected
** exits to dos with an error msg.
*/

extern void * dis_msgarea(int areanumber);
/* returns a pointer to a 64 byte interpart communications area.
** areanumber is 0..3
*/

extern int dis_muscode(int);
/* returns a music syncronization code. As a parameter, give the
** code you are waiting, so a skip can be easily done by DIS.
*/

extern int dis_musplus(void);
/* returns a music syncronization code. As a parameter, give the
** code you are waiting, so a skip can be easily done by DIS.
*/

extern int dis_musrow(int);
/* returns a music syncronization code. As a parameter, give the
** code you are waiting, so a skip can be easily done by DIS.
*/

extern void dis_setcopper(int routine_number,void (*routine)(void));
/* routine=1(top of screen)/2(bottom of screen)/3(retrace)
** routine=pointer to routine
*/

void _dis_setmframe(int frame);
int _dis_getmframe(void);
int _dis_sync(void);

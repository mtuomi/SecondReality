extern	int	 	initloader(int num,...); /* initializes the names of the packfiles where datafiles are to be found by default */
extern	int	 	initloaderpath(int num,...); /* initializes a path to find datafiles from if they are not in the 'packfile' */
extern	char far *	getmem(long size); /* allocates memory for [size] bytes */
extern	int	 	freemem(char far *block); /* frees a memory block */
extern	int		exists(char *fname,long *size);
extern	char far *	readfile(char *fname); /* reserves memory for file and loads it and returns a pointer to it */
extern	int		readfileto2(char *buf,char *fname,long pos,long count); /* reads a file to a pre-reserved buffer */
extern	int		psopen(char *fname,long *size);

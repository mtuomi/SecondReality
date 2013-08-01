#include "..\dis\dis.h"

extern plz();
extern vect();

main()  {
	dis_partstart();
	init_copper();
	initvect();
	plz();
	vect();
	close_copper();
	}
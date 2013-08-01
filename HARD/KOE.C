main()
{
	int	a=0;
	_asm mov ax,13h
	_asm int 10h
	while(!kbhit())
	{
		printf(".");
		while(!(inp(0x3da)&8));
		printf(".");
		while((inp(0x3da)&8));
		printf("%i\r",a++);
	}
}

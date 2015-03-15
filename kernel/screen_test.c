//A simple function that tests out all character attributes to be in character mode
void screen_test()
{
	char *vidmem = (char*)0xb8000;
	unsigned short it = 0;
	while(it < 256)
	{
		vidmem[it*2] = 'K';
		vidmem[it*2+1] = it;
		it += 1;
	}
}

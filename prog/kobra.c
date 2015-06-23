//KOBRA INTERPRETER SHELL

#include "programs.h"
#include "screen.h"

int kobra(int argc, char **argv)
{
	while(1)
	{
		((char*)VIDEO_ADDRESS)[18] = '#';
          ((char*)VIDEO_ADDRESS)[19] = 0x03;	
	}
	
	return 0;				
}

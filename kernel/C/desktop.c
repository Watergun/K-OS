//K-TOP DESKTOP ENVIRONMENT
#include "globalstatus.h"


int desktop (int, char**);


int desktop(int argc, char **argv)
{
	//Check if already a desktop started
	if(global_videomode != VID_TEXT)
		return 0;
	
	//VGA Init
	vga_init();
	global_videomode = VID_VGA;

	//Background color
	
}	


void desktop_pass_char(char c)
{
	//
	
}

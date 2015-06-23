//List of programs that can possibly get the keyboard focus
//
//1. Terminal
//2. Desktop

#include "globalstatus.h"

void pass_character(char c)
{
	//Find active program
	if(global_videomode == VID_TEXT)
		terminal_pass_char(c);
	else if(global_videomode == VID_VGA)
		desktop_pass_char(c);
}

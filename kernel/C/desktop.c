
//K-TOP DESKTOP ENVIRONMENT // - - - K-PANELS - - -
#include "globalstatus.h"
#include "vga.h"
#include "multitask.h"
#include "memory.h"
#include "stdheader.h"
#include "desktop.h"
#include "process.h"
#include "util.h"
#include "allocation.h"
#include "string.h"
#include "fonts.h"

/*
320 * 200 px = 64000 px
64000 * 

*/

//Desktop inbox
int desktop_status;

//Global (System) desktop colors
char desktop_backgroundcolor;
char desktop_panelcolor = VGA_COLOR(3, 2, 1);

char* desktop_videobuffer;

int desktop(int argc, char **argv)
{
	//Check if already a desktop started
	if(global_videomode != VID_TEXT)
		return 0;
	
	//Std process command set
	system_name_process(__process__, "K-Top Environment");
	system_set_process_inbox(__process__, (int)&desktop_status);

	// - - - VGA Init - - -
	vga_init();
	global_videomode = VID_VGA;

	// - - - KTOP-Panel Layout Table (PLT) - - -
	global_desktop_plt 		= (int*) system_reserve_unit();
	flash_memory((char*)global_desktop_plt, UNITSIZE);

	global_desktop_plt[0] 	= 0;			//Memory base
	global_desktop_plt[1]	= UNITSIZE; 	//Memory size
	global_desktop_plt[3]	= 0x35; 		//PLT ID
	memory_copy("K-OS Panel Layout Table", (char*) ((int)global_desktop_plt)+16, 23);

	//Reserve video buffer
	desktop_videobuffer = (char*) memory_allocate(64000);
	flash_memory(desktop_videobuffer, 64000);

	// - - - Start K-TOP user screen - - -
	int login_panel = desktop_create_panel("Login", 0, 0, 0, 0);
	
	desktop_backgroundcolor = VGA_COLOR(1, 2, 2);
	
	//Fill with content
	char *login_panel_raw = (char*) (login_panel+64);
	fonts_write_text("Aa", 10, 10, 0, VGA_COLOR(0, 0, 5), login_panel_raw);
//	flash_memory(login_panel_raw, 64000);

//int u = 0;
//for(; u < 64000; u++)
//{
//	desktop_videobuffer[u] = VGA_COLOR(5, 2, 0);
//}

//u = 0;
//char *vidmem = (char*)VGA_VIDEO_MEMORY;
//for(; u < 64000; u++)
//{
//	vidmem[u] = desktop_videobuffer[u];
//}
	// - - - K-TOP Mainloop - - -
	int running = 1;
	while(running)
	{
		//Handle incoming messages 
		if(desktop_status & EXIT_SIGNAL)
		{
			running = 0;
			desktop_status ^= EXIT_SIGNAL;
		}

		//Draw text
		

		//Render
		desktop_render();

		//Status marker [DEBUG]
		static int marker = 0;

		((char*)VGA_VIDEO_MEMORY)[marker] = VGA_COLOR(5, 0, 0);
		((char*)VGA_VIDEO_MEMORY)[marker-1] = desktop_backgroundcolor;
		if(marker >= VGA_X_DIMENSION-1)
			marker = 0;
		else marker += 1;
	}
}	


//KTOP Rendering function
void desktop_render()
{
	//Look through the PLT and draw all panels
	if(global_desktop_plt == 0 || desktop_videobuffer == 0)
	{
		return;
	}

	//Paint background	
	int i = 0;
/*	for(; i < VGA_TOTAL_PIXELS; i++)
	{
		desktop_videobuffer[i] = desktop_backgroundcolor;
	}
*/
	//Paint panels
	int plt_index = 0;
	while(plt_index < 504)
	{
		int plt_entry = global_desktop_plt[16 + plt_index*2];
		if(plt_entry != 0)
		{
			//Retrieve panel
			int *panel = (int*)global_desktop_plt[16 + plt_index*2 + 1];
			char *raw_panel_data = (char*)(((int)panel)+64);
			
			/* SPECIFIC PANEL BLOCKHEADER:
			ORIGINAL BLOCK	[4B]		0
			BLOCK SIZE	[4B]		1
			NEXT BLOCK	[4B]		2
			ID			[4B]		3
			TITLE 		[32B]    (4-11)
			PANELPOS		[4B]		12
			PANELSIZE		[4B]		13
			PANELFRAME	[4B]		14
			-------------------
						[60B] -> [64B]
			*/

			//Retrieve metadata
			ushort x = INT16_HIGH(panel[12]);
			ushort y = INT16_LOW(panel[12]);

			ushort width = INT16_HIGH(panel[13]);
			ushort height = INT16_LOW(panel[13]);
		
			ushort frame_th_up = INT16_HIGH(panel[14]) >> 8;
			ushort frame_th_rest = (INT16_HIGH(panel[14]) << 8) >> 8;
			char frame_color = (char) INT16_LOW(panel[14]);

			//Correct metadata
			if(width > (VGA_X_DIMENSION - frame_th_rest*2))
				width = VGA_X_DIMENSION - frame_th_rest*2;

			if(height > (VGA_Y_DIMENSION - (frame_th_rest + frame_th_up)))
				height = VGA_Y_DIMENSION - (frame_th_rest + frame_th_up);
		
			if(x < frame_th_rest)
				x = frame_th_rest;
			
			if(y < frame_th_up)
				y = frame_th_up;
				
			//Draw frame
			ushort i = x - frame_th_rest;
			ushort j = y - frame_th_up;

			ushort full_width = frame_th_rest*2 + width;
			ushort full_height = frame_th_up + frame_th_rest + height;
/*
			for(; i < (width + frame_th_rest*2); i++)
			{
				for(; j < (height + frame_th_rest + frame_th_up); j++)
				{
					if(i < x || i > (width+x) || j < y || j > (height+y))
						desktop_videobuffer[i*j + j] = frame_color;
				}
			}
*/
			//Paint panel
			i = 0;

			for(; i < VGA_TOTAL_PIXELS; i++)
			{
				char pixel = raw_panel_data[i];
//				if(pixel < 216)
				desktop_videobuffer[i] = pixel;
			}

			//Increment plt index
			plt_index++;
		}
		else
			break;
	}

	//Swap buffer to screen
	i = 0;
	char *vid_mem = (char*) VGA_VIDEO_MEMORY;
	for(; i < VGA_TOTAL_PIXELS; i++)
	{
		vid_mem[i] = desktop_videobuffer[i];
	}
//	desktop_debug(190);
}

//Create new panel and add to PLT
int desktop_create_panel(char *title, uint width, uint height, uint x, uint y)
{
	//If width or height equals zero, automatically select fullscreen (x & y args are ignored)
	if(width == 0 || height == 0)
	{
		width = VGA_X_DIMENSION;
		height = VGA_Y_DIMENSION;
		x = 0;
		y = 0;
	}

	//Create panel --> with USABLE_UNITSIZE == 4032 -> units <= 16
	int total_size = VGA_X_DIMENSION*VGA_Y_DIMENSION;
	int units = total_size / USABLE_UNITSIZE;
	if(units % USABLE_UNITSIZE) units++;

	//Save startingunit
	int *ptr = (int*) system_reserve_unit();
	flash_memory((char*) ptr, UNITSIZE);

	//Other units
	int it = 1;
	for(; it < units; it++)
	{
		int u = system_reserve_unit();
		flash_memory((char*) u, UNITSIZE);
	}

	static int WID = 0x10000;

	ptr[0] = 0;			//Original unit
	ptr[1] = total_size;	//Full screen size as block size
	ptr[2] = 0;			//no next unit
	ptr[3] = 0x35 | (WID++);
	memory_copy(title, (char*) ((int)ptr)+16, strlen(title));
	ptr[12] = INT32(x, y);
	ptr[13] = INT32(width, height);
	ptr[14] = 0;

	//Create PLT entry
	int plt_index = 0;
	while(plt_index < 504)
	{
		if(global_desktop_plt[16 + plt_index*2] == 0)
		{
			global_desktop_plt[16 + plt_index*2] = WID-1;
			global_desktop_plt[16 + plt_index*2 + 1] = (int) ptr;
			break;
		}
		else
			plt_index += 1;
	}

	//Draw title
	//fonts_write_text("Aa", 10, 10, 0, 200, (char*) &ptr[16]);

	return (int) ptr;
}

//Changes the panel frame scheme
void desktop_set_panel_frame(int panel_id, uint thickness_up, uint thickness_rest, char color)
{

}

//Automatically being called in dektop_create_panel and dektop_close_panel
void desktop_plt_service_routine()
{
	//
}

//Called by the keyboard interrupt
void desktop_pass_char(char c)
{
	//Switch to focused process
}

//Free panel memory
void desktop_close_panel(int panel_id)
{

}

//DEBUG Function
void desktop_debug(char color)
{
	((char*)VGA_VIDEO_MEMORY)[52000] = color;
}


// # # # # K-TOP PANEL WIDGETS # # # #
void desktop_create_widget_label()
{

}

void desktop_create_widget_button()
{


}

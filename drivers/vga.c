#include "stdheader.h"
#include "vga.h"

/*
TODO:

Disable Output
Unlock CRTC		-> Unlock Registers

Load Registers
Clear Screen
Load Fonts

Lock CRTC			-> Lock Registers
Enable Display
*/

/*
Ports:
0x3C0:	WRITE: I+D 0x3C0  READ: I: 0x3C0 D:0x3C1
0x3C2:	WRITE (Misc. Output Reg.) [READ -> 0x3CC]
0x3C4:
0x3CE:
0x3D4:	WRITE/READ: I: 0x3XX D: 0x3XX + 1
0x3D6:	(DAC Mask Reg.) R/W
0x3C7:	READ DAC Color Register Index
0x3C8: 	WRITE DAC Color Register Index
0x3C9: 	WRITE DAC Color Register Data (R/G/B) x3



/*

REGISTER NAME		| PORT | INDEX	| mode 13h value 

Mode Control:		0x3C0	0x10		0x41
Overscan:			0x3C0	0x11		0x00
ColorPlaneEnable:	0x3C0	0x12		0x0F
Horiz. Panning:	0x3C0	0x13		0x00
Color Sect:		0x3C0	0x14		0x00
Misc. Output:		0x3C2	N/A		0x63
Clock Mode:		0x3C4	0x01		0x01
Character select:	0x3C4	0x03		0x00
Memory Mode:		0x3C4	0x04		0x0E
Mode:			0x3CE	0x05		0x40
Misc.:			0x3CE	0x06		0x05
Horiz. Total:		0x3D4	0x00		0x5F
Horiz. Disp.En. End:0x3D4	0x01		0x4F
Horiz. Blank S:	0x3D4	0x02		0x50
Horiz. Blank E:	0x3D4	0x03		0x82
Horiz. Retrace S:	0x3D4	0x04		0x54
Horiz. Retrace E:	0x3D4	0x05		0x80
Vertic. Total:		0x3D4	0x06		0xBF
Overflow:			0x3D4	0x07		0x1F
Preset row scan:	0x3D4	0x08		0x00
Max. Scan Line:	0x3D4	0x09		0x41
Vertic. Retrace S:	0x3D4	0x10		0x9C
Vertic. Retrace E:	0x3D4	0x11		0x8E
Vertic. Disp.En.End:0x3D4	0x12		0x8F
Logical Width:		0x3D4	0x13		0x28	
Underline Location:	0x3D4	0x14		0x40
Vertic. Blank S:	0x3D4	0x15		0x96
Vertic. Blank E	0x3D4	0x16		0xB9
Mode Control:		0x3D4	0x17		0xA3
*/

void vga_pixel(uchar, uint,uint);
void vga_clear_screen();

//Initialises the VGA Mode of the graphics output
void vga_init()
{
	/*
		Initialises 13h Mode
	*/

	//Attribute Controller Registers
	port_byte_in(0x3DA);
	uchar temp = port_byte_in(0x3C0);

	port_byte_out(0x3C0, 0x10);
	port_byte_out(0x3C0, 0x41);
	port_byte_out(0x3C0, 0x11);
	port_byte_out(0x3C0, 0x00);
	port_byte_out(0x3C0, 0x12);
	port_byte_out(0x3C0, 0x0F);
	port_byte_out(0x3C0, 0x13);
	port_byte_out(0x3C0, 0x00);
	port_byte_out(0x3C0, 0x14);
	port_byte_out(0x3C0, 0x00);

	port_byte_out(0x3C0, temp);	//Important!

	//(optional)
	port_byte_in(0x3DA);	

	//General Registers
	
	/*Individual I/O Port*/
	port_byte_out(0x3C2, 0x63);
	
	//Sequencer Registers
	temp = port_byte_in(0x3C4);

	port_byte_out(0x3C4, 0x01);
	port_byte_out(0x3C5, 0x01);
	port_byte_out(0x3C4, 0x02);
	port_byte_out(0x3C5, 0x0F);
	port_byte_out(0x3C4, 0x03);
	port_byte_out(0x3C5, 0x00);
	port_byte_out(0x3C4, 0x04);
	port_byte_out(0x3C5, 0x0E);

	port_byte_out(0x3C4, temp);

	//Graphics Controller Registers
	temp = port_byte_in(0x3CE);
	
	port_byte_out(0x3CE, 0x05);
	port_byte_out(0x3CF, 0x40);
	port_byte_out(0x3CE, 0x06);
	port_byte_out(0x3CF, 0x05);

	port_byte_out(0x3CE, temp);
	
	//CRTC Registers
	temp = port_byte_in(0x3D4);

	port_byte_out(0x3D4, 0x00);
	port_byte_out(0x3D5, 0x5F);
	port_byte_out(0x3D4, 0x01);
	port_byte_out(0x3D5, 0x4F);
	port_byte_out(0x3D4, 0x02);
	port_byte_out(0x3D5, 0x50);
	port_byte_out(0x3D4, 0x03);
	port_byte_out(0x3D5, 0x82);
	port_byte_out(0x3D4, 0x04);
	port_byte_out(0x3D5, 0x54);
	port_byte_out(0x3D4, 0x05);
	port_byte_out(0x3D5, 0x80);
	port_byte_out(0x3D4, 0x06);
	port_byte_out(0x3D5, 0xBF);
	port_byte_out(0x3D4, 0x07);
	port_byte_out(0x3D5, 0x1F);
	port_byte_out(0x3D4, 0x08);
	port_byte_out(0x3D5, 0x00);
	port_byte_out(0x3D4, 0x09);
	port_byte_out(0x3D5, 0x41);
	port_byte_out(0x3D4, 0x0F);
	port_byte_out(0x3D5, 0x31);
	port_byte_out(0x3D4, 0x10);
	port_byte_out(0x3D5, 0x9C);
	port_byte_out(0x3D4, 0x11);
	port_byte_out(0x3D5, 0x8E);
	port_byte_out(0x3D4, 0x12);
	port_byte_out(0x3D5, 0x8F);
	port_byte_out(0x3D4, 0x13);
	port_byte_out(0x3D5, 0x28);
	port_byte_out(0x3D4, 0x14);
	port_byte_out(0x3D5, 0x40);
	port_byte_out(0x3D4, 0x15);
	port_byte_out(0x3D5, 0x96);
	port_byte_out(0x3D4, 0x16);
	port_byte_out(0x3D5, 0xB9);
	port_byte_out(0x3D4, 0x17);
	port_byte_out(0x3D5, 0xA3);

	port_byte_out(0x3D4, temp);

/*
	//DAC Registers (Color Palette Setting)
	int color_begin = 0x40;
	port_byte_out(0x3C8, color_begin);

	for(; color_begin < 256; color_begin++)
	{	
		//Red
		port_byte_out(0x3C9, 0);	
		//Green	
		port_byte_out(0x3C9, 0);
		//Blue
		port_byte_out(0x3C9, 0);
	}
*/

	//Finish!
	
	vga_clear_screen();

	char *video_memory = (char*)VGA_VIDEO_MEMORY;
		

	int i = 0;	
	for(; i < 200; i++)
  	{
		int j = 0;
		for(; j < 256; j++)
		{
			vga_pixel(j, j, i);
			
		}
	}	

}

//Zeros all bytes
void vga_clear_screen()
{
	int i = 0;
	int x = VGA_X_DIMENSION * VGA_Y_DIMENSION;
	char *vid_mem = (char*) VGA_VIDEO_MEMORY;
	for(; i < 64000; i++)
	{
		vid_mem[i] = 0;
	}
}

//Puts a given char to the specified position
void vga_pixel(uchar color, uint x, uint y) 
{
   char *vid_mem = (char*) VGA_VIDEO_MEMORY;
   uint offset     = y*VGA_X_DIMENSION + x;

   vid_mem[offset] = color;
}



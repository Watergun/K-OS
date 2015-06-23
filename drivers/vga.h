#ifndef VGA_H
#define VGA_H

#define VGA_VIDEO_MEMORY 0xA0000

//COLOR PALETTE SETTING
#define VGA_CP_ASCRGB
//#define VGA_CP_DEFAULT 


#define VGA_Y_DIMENSION	200
#define VGA_X_DIMENSION	320

#define VGA_COLOR(R,G,B) (R*36 + G*6 + B)

//VGA Initialisation
extern void vga_init();

#endif

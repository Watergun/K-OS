#ifndef VGA_H
#define VGA_H

#define VGA_VIDEO_MEMORY 0xA0000

//COLOR PALETTE SETTING
#define VGA_CP_ASCRGB
//#define VGA_CP_DEFAULT 


#define VGA_Y_DIMENSION	200
#define VGA_X_DIMENSION	320
#define VGA_TOTAL_PIXELS 64000

//Create VGA COLOR from R, G, B channels
#define VGA_COLOR(R,G,B) (R*36 + G*6 + B)
//Extract R, G, B channels from VGA COLOR
#define VGA_RGB_R(C) (C%36)
#define VGA_RGB_G(C) (((int)C/6)%6)
#define VGA_RGB_B(C) (C%6)

//VGA Functions
extern void vga_init();
extern void vga_pixel(char, unsigned int, unsigned int);
extern void vga_clear_screen();

#endif

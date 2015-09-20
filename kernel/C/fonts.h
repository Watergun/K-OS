#ifndef FONTS_H
#define FONTS_H

#define SYSTEM_FONT_WIDTH 		6
#define SYSTEM_FONT_HEIGHT 		12
#define SYSTEM_FONT_TOTAL_PIXELS	72
#define SYSTEM_FONT_SIZE			9

#include "stdheader.h"

//All implemented functions: Descriptions in .c file
int fonts_get_font_data(int font_id, char c, char *letter);

void fonts_build_font(char *letter, char, char, char, char, char, char, char, char, char);
void fonts_write_text(char *text, uint x, uint y, int font_id, char font_color, char *videobuffer);

#endif

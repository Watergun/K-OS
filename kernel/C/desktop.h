#ifndef DESKTOP_H
#define DESKTOP_H

#include "stdheader.h"

//All implemented functions -> Read .c file for further details
int desktop(int argc, char **argv);
int desktop_create_panel(char *name, uint x, uint y, uint width, uint height);


void desktop_render();
void desktop_set_panel_frame(int panel, uint tu, uint tr, char color);
void desktop_plt_service_routine();
void desktop_pass_char(char c);
void desktop_close_panel(int panel);

void desktop_debug(char color);

//The global K-OS K-Panel Layout Table
int *global_desktop_plt;

#endif

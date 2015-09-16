#ifndef IO_PORTS_H
#define IO_PORTS_H

#include "stdheader.h"


//All implemented functions -> See .c for details
uchar port_byte_in(ushort port);
ushort port_word_in(ushort port);
uint port_dword_in(ushort port);

void port_byte_out(ushort port, uchar data);
void port_word_out(ushort port, ushort data);
void port_dword_out(ushort port, uint data);
#endif

#ifndef UTIL_H
#define UTIL_H

#include "stdheader.h"

//List all implemented functions -> Descriptions can be found in .c file

void memory_copy(char *src, char *dest, size_t size);
void memory_copy_rev(char *src, char *dest, size_t size);
void flash_memory(char *addr, size_t size);
void itoa(int i, char *buf);

char digit_to_char(short d);
char hex_to_char(short x);

void tm_print_hex(int i);
void tm_print_byte_hex(char b);
void tm_print_memory(char *addr, size_t count);
void tm_print_dword_raw(int val);

#endif

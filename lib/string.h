#ifndef STRING_H
#define STRING_H

#include "stdheader.h"

//All implemented string functions -> Look for the .c file to get further details

size_t strlen(char *string);

int strcmp(char *str1, char *str2, int maxlen);
int strcount_char(char *text, char c);
int strcount_words(char *text);
int atoi(char *str);
short char_to_digit(char c);

#endif

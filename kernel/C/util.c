#include "screen.h"

void memory_copy(char*,char*,int);
void itoa(int,char*);
char digit_to_char(short);
void TM_print_hex(int);
char hex_to_char(short);
void TM_print_memory(char *, int);
void TM_print_byte_hex(char);

/*Copy bytes from one place to another		*/
void memory_copy(char *source, char *destination, int size)
{
	unsigned int i;
	for(i = 0; i < size; i++)
		destination[i] = source[i];
}

void itoa(int number, char* buffer)
{	
	short i = 0;
	int number_cpy = number;
	while(number_cpy > 0)
	{
		number_cpy /= 10;	
		i++;
	}

	buffer[i] = 0;
	
	for(;i > 0; i--)
	{
		buffer[i-1] = digit_to_char(number % 10);
		number /= 10;
	}
}

//Converts a digit to its ASCII-character
char digit_to_char(short digit)
{
	return (char) (digit + 0x30);
}

//Prints a hexadecimal value (32bit -> 8 hex characters)
void TM_print_hex(int value)
{
	int i;
	for(i = 7; i >= 0; i--)
	{
		int filter = 0xF;
		short digit = (value & (filter << (i*4))) >> (i*4);
		char hex = hex_to_char(digit);
		print_char(hex,0xF0);
	}
}

char hex_to_char(short digit)
{
	digit &= 0xF;
	if(digit >= 0xA)
		return (digit-0xA)+0x41;	
	else
		return digit+0x30;
}

//Print Memory
void TM_print_memory(char* address, int bytes)
{
	int i;
	for(i = 0;i < bytes; i++)
	{
		char byte = address[i];
		TM_print_byte_hex(byte);
	}
}

void TM_print_byte_hex(char val)
{
	char hex1 = hex_to_char((val & 0xF0) >> 4);
     char hex2 = hex_to_char(val & 0x0F);

     print_char(hex1, TM_COLORS(TM_RED, TM_GREEN));
     print_char(hex2, TM_COLORS(TM_RED, TM_GREEN));
     print_char(' ', TM_COLORS(0, TM_GREEN));
}

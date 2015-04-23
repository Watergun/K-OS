#include "screen.h"

void memory_copy(char*,char*,int);
void itoa(int,char*);
char digit_to_char(short);
void tm_print_hex(int);
char hex_to_char(short);
void tm_print_memory(char *, int);
void tm_print_byte_hex(char);
void tm_print_dword(int);

/*Copy bytes from one place to another		*/
void memory_copy(char *source, char *destination, int size)
{
	unsigned int i;
	for(i = 0; i < size; i++)
	{
		destination[i] = source[i];
	}
}

/*Copy bytes from one place to another (in reverse)	*/
void memory_copy_rev(char *source, char *destination, int size)
{
	unsigned int i;
	for(i = 1; i <= size; i++)
	{
		destination[size-i] = source[size-i];
	}
}

//overwrites the given memory with zeros
void flash_memory(char *address, int bytes)
{
	int i = 0;
	for(i; i < bytes; i++)
		address[i] = 0;
}

//Converts a digit to its ASCII character
char digit_to_char(short digit)
{
     return (char) (digit + 0x30);
}

//Converts a number to its ASCII string representation
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

//Prints a hexadecimal value (32bit -> 8 hex characters)
void tm_print_hex(int value)
{
	int i;
	for(i = 7; i >= 0; i--)
	{
		int filter = 0xF;
		short digit = (value & (filter << (i*4))) >> (i*4);
		char hex = hex_to_char(digit);
		tm_print_char(hex,0xF0);
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
void tm_print_memory(char* address, int bytes)
{
	int i;
	for(i = 0;i < bytes; i++)
	{
		char byte = address[i];
		tm_print_byte_hex(byte);
	}
}

void tm_print_byte_hex(char val)
{
	char hex1 = hex_to_char((val & 0xF0) >> 4);
     char hex2 = hex_to_char(val & 0x0F);

     tm_print_char(hex1, TM_COLORS(TM_WHITE, TM_BLUE));
     tm_print_char(hex2, TM_COLORS(TM_WHITE, TM_BLUE));
     tm_print_char(' ', TM_COLORS(0, TM_BLUE));
}

void tm_print_dword(int val)
{
	tm_print_char(val, TM_DEFAULT_STYLE);
	tm_print_char(val >> 8, TM_DEFAULT_STYLE);
	tm_print_char(val >> 16, TM_DEFAULT_STYLE);
	tm_print_char(val >> 24, TM_DEFAULT_STYLE);
}

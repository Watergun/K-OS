//time programs

#include "allocation.h"
#include "terminal.h"
#include "util.h"

int time(int argc, char **argv)
{
	int *timeptr = (int*) 0x500;

	char *buffer = (char*) memory_allocate(4);
	itoa(*timeptr, buffer);

	terminal_print_string(buffer);

	memory_free((int)buffer);

	return 0;
}

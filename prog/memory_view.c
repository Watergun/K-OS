//memory_view: prints the memory at the specified address charwise

#include "programs.h"
#include "string.h"
#include "util.h"

//memory_view [address] [bytes]
int memory_view(int argc, char **argv)
{
	if(argc < 3)
		return 1;
	if(!argv[1])
		return 1;	

	char *address = (char*) atoi(argv[1]);
//tm_print_hex(address);
	int bytes = (int) atoi(argv[2]);

	unsigned int index = 0;
	for(index; index < bytes; index++)
	{
		tm_print_byte_hex(address[index]);
	}

	return 0;
}

int memory_view_func(char *address, unsigned int bytes)
{	
	if(!address || !bytes)
		return 1;
	unsigned int index = 0;
	for(; index < bytes; index++)
		tm_print_byte_hex(address[index]);

	return 0;
}

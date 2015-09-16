//io_ports.c:
//I/O Ports for bytes and words
//[C Wrappers with Inline Assembly Code]

unsigned char port_byte_in(unsigned short port) 
{
	//A C wrapper function that read a byte from the specified port
	//"=a" (result) means: put Al register in variable RESULT when finished
	//"d" (port) means: load EDX with port
	unsigned char result;
	__asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
	return result;
}

void port_byte_out(unsigned short port, unsigned char data)
{
	//"a" (data) means: load EAX with data
	//"d" (port) means: load EDX with port
	__asm__("out %%al, %%dx"  : : "a" (data), "d" (port));
}

unsigned short port_word_in(unsigned short port)
{
	unsigned short result;
	__asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
	return result;
}

void port_word_out(unsigned short port, unsigned short data)
{
	__asm__("out %%ax, %%dx" : : "d" (port), "a" (data));
}


unsigned int port_dword_in(unsigned short port)
{
	unsigned int result;
	__asm__("in %%dx, %%eax" : "=a" (result) : "d"(port));
	return result;
}

void port_dword_out(unsigned short port, unsigned int data)
{
	__asm__("out %%eax, %%dx" : : "d" (port), "a" (data));
}

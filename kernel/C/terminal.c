//K-OS Terminal
#include "stdheader.h"
#include "screen.h"
#include "kernelprograms.h"
#include "programs.h"

//a little pointer for terminal variables
char *tm_video_buffer;

//List of all implemented functions
int terminal(int, char**);
void terminal_set_statement();
void terminal_print_string(char*);
void terminal_exec_line(int);
void terminal_pass_char(char);
void terminal_swap_buffers();
char **split_exec_line(char *buffer, int *pargc);

int terminal(int argc, char **argv)
{
	//Give this terminal process a name	
	name_process("K-OS Root Terminal");

	//Swap buffer for the terminal (TM)
	tm_video_buffer = (char*)malloc(4000);

	terminal_set_statement();

	return 0;
}

void terminal_set_statement()
{
	tm_print_char('\n', TM_DEFAULT_STYLE);
	tm_print_char('<', TM_DEFAULT_STYLE);
	tm_print_char('?', TM_DEFAULT_STYLE);
	tm_print_char('>', TM_DEFAULT_STYLE);
}

void terminal_print_string(char *str)
{
	tm_print_char('\n', TM_DEFAULT_STYLE);
	tm_print(str);
}

void terminal_exec_line(int length)
{
	// READ COMMAND
	char *buffer = (char*) malloc(length);	

	int pos = tm_get_cursor();		//get cursor position

//DEBUG
//tm_print_hex(buffer);

	int i = 0;
	for(; i < length; i++)			//copy the command into the buffer
	{
		buffer[i] = tm_read_char(pos-(length-i)*2);	
	}

	//null-terminate the buffer
	buffer[length] = 0;	

	//EXECUTE COMMAND
	
	//Either switch here directly or have a big list over all commands
	//Choosing method 1 at first

	int argc = 0;
	char **argv = (char**) split_exec_line(buffer, &argc);

	if(!argv)
	{
		terminal_print_string("Terminal Error");
		return;	
	}
	
	size_t len = strlen(argv[0]);
	int result;

	if(!strcmp("hostname", argv[0], len))
		result = hostname(argc, argv);
	else if(!strcmp("osname", argv[0], len))
		result = osname(argc, argv);
	else if(!strcmp("memoryview", argv[0], len))
		result = memory_view(argc, argv);
	else if(!strcmp("time", argv[0], len))
		result = time(argc, argv);
	else if(!strcmp("pci", argv[0], len))
		result = pciforce(argc, argv);
	else if(!strcmp("vga", argv[0], len))
		result = vgatest(argc, argv);

//
	else
	{
		terminal_print_string("Command not found!");
		result = 0;
	}

	//handle result...

	terminal_set_statement();	
}

void terminal_pass_char(char c)
{
	static int mark_distance = 0;

	if(c == 0x0D)		//Carriage Return
	{
		if(mark_distance == 0)
		{	
			terminal_set_statement();
			return;
		}
		terminal_exec_line(mark_distance);
		mark_distance = 0;
	}
	else if(c == 0x08)	//Backspace
	{	
		if(mark_distance > 0)
		{
			tm_delete_last();
			mark_distance--;
		}
	}
	else if(c == 0x09)	//Tab
	{
		//Tabsize: 5
		tm_print("     ");	
	}
	else				//Normal character
	{
		mark_distance++;
		tm_print_char(c, TM_DEFAULT_STYLE);
	}
}

char **split_exec_line(char *buffer, int *pargc)
{
	//receive informations about the buffer
	int ptrindex = 0;
	int strlength = strlen(buffer);
				
	//There can be just as much arguments as effective spaces in the buffer
	char **argv = (char**) malloc(word_count(buffer)*4);

//DEBUG
//tm_print_hex(word_count(buffer));
//tm_print_hex(argv);
	
	int i = 0, j = 0, c = 0;

	for(; i <= strlength; i++)
	{
		if(buffer[i] != ' ' && buffer[i] != 0)
		{
			if(!c)
				j = i;
			c++;
		}
		else	if(c != 0)	//Create new argument
		{
			char *arg = (char*) malloc(c);
//DEBUG
//tm_print_hex(arg);
			
			memory_copy(buffer + j , arg, c);
			arg[c] = 0;
			argv[ptrindex] = arg;
			ptrindex++;
			c = 0;
		}	
	}

//DEBUG
//memory_view_func(argv, 64);

	*pargc = ptrindex;
	return argv;
}

void terminal_swap_buffers()
{
	int i = 0;
	for(i; i < 25*80; i+=2)
	{
		char c = tm_read_char(i);
		*((char*)0xb8000+i) = *((char*)tm_video_buffer+i);
		*((char*)tm_video_buffer+i) = c;				
	}
}

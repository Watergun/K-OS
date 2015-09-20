//K-OS Terminal
#include "terminal.h"
#include "stdheader.h"
#include "screen.h"
#include "programs.h"
#include "multitask.h"
#include "string.h"
#include "allocation.h"
#include "process.h"
#include "util.h"
#include "desktop.h"

//DEBUG
#include "fonts.h"

//a little pointer for terminal variables
char *tm_video_buffer;

//Terminal status 
int terminal_status;
int mark_distance;

//Infomrations about the terminal-started process
int hosted_process;
int process_inbox;

//Terminal process
int terminal(int argc, char **argv)
{
	//Give this terminal process a name
	system_name_process(__process__, "K-OS Root Terminal");
	system_set_process_inbox(__process__, (int)&terminal_status);

	//Swap buffer for the terminal (TM)
	tm_video_buffer = (char*)memory_allocate(4000);

	mark_distance 		= 0;
	terminal_status 	= 0;
	hosted_process 	= 0;
	process_inbox 		= 0;
	terminal_set_statement();

	int running = 1;
	while(running)
	{
		if(terminal_status & EXIT_SIGNAL)
		{
			running = 0;
			terminal_status ^= EXIT_SIGNAL;
		}
		if(terminal_status & HOSTED_PROCESS_TERMINATED)
		{
			terminal_set_statement();
			terminal_status ^= HOSTED_PROCESS_TERMINATED;
		}
		if(terminal_status & EXECUTE_LINE)
		{
			terminal_exec_line();
			terminal_status ^= EXECUTE_LINE;
		}

          ((char*)VIDEO_ADDRESS)[18] = 'T';
          ((char*)VIDEO_ADDRESS)[19] = 0x06;
//DEBUG
//int a = 5;
//tm_print_hex(&a);
	}
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

void terminal_exec_line()
{
	int length = mark_distance;

	// READ COMMAND
	char *buffer = (char*) memory_allocate(length);	

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
	char **argv = (char**) terminal_split_exec_line(buffer, &argc);

	if(!argv)
	{
		terminal_print_string("Terminal Error");
		return;	
	}
	
	size_t len = strlen(argv[0]) + 1;
	int result;
	
	
	if(!strcmp("osname", argv[0], len))
		terminal_start_process((int)osname, argc, argv);
	else if(!strcmp("memoryview", argv[0], len))
		terminal_start_process((int)memory_view, argc, argv);
	else if(!strcmp("time", argv[0], len))
		terminal_start_process((int)time, argc, argv);
	else if(!strcmp("pci", argv[0], len))
		terminal_start_process((int)pciforce, argc, argv);
	else	if(!strcmp("ktop", argv[0], len))
		terminal_start_process((int)desktop, argc, argv);
	else if(!strcmp("help", argv[0], len))
		terminal_start_process((int)terminal_help, argc, argv);
	else if(!strcmp("kobra", argv[0], len))
		terminal_start_process((int)kobra, argc, argv);
//
	else
	{
		terminal_print_string("Command not found!");
		terminal_set_statement();
		result = 0;
	}

	//handle result...

	//free allocated memory
	memory_free((int)buffer);
	for(; argc > 0; argc--)
		memory_free((int)argv[argc-1]);
	memory_free((int)argv);

	mark_distance = 0;
}

void terminal_pass_char(char c)
{
	if(c == 0x0D)		//Carriage Return
	{
		if(mark_distance == 0)
		{
			terminal_set_statement();
			return;
		}
		
		terminal_status |= EXECUTE_LINE;
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

char **terminal_split_exec_line(char *buffer, int *pargc)
{
	//receive informations about the buffer
	int ptrindex = 0;
	int strlength = strlen(buffer);
				
	//There can be just as much arguments as effective spaces in the buffer
	char **argv = (char**) memory_allocate(strcount_words(buffer)*4);

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
			char *arg = (char*) memory_allocate(c);
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

//Swaps the character buffer and the videomemory
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

//Launches a process inside the terminal
void terminal_start_process(int code, int argc, char**argv)
{
	//
	hosted_process = system_new_process(code, 0, 4096);
	
	//Launch
	system_start_process(hosted_process, argc, argv);
}




//The terminal-help program (own process)
int terminal_help(int argc, char **argv)
{
	//This program prints out all available commands
	terminal_print_string("osname, kobra, ktop, memoryview, pci, time");

	return 0;
}

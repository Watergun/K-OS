//K-OS Terminal
#include "screen.h"

void terminal_set_statement()
{
	tm_print_char('\n', TM_DEFAULT_STYLE);
	tm_print_char('<', TM_DEFAULT_STYLE);
	tm_print_char('?', TM_DEFAULT_STYLE);
	tm_print_char('>', TM_DEFAULT_STYLE);
}

void terminal_print_string(char *str)
{
	tm_print_char('\n');
	tm_print(str);
	terminal_set_statement();
}

void terminal_exec_line()
{
	int pos = tm_get_cursor();
	int i = 0;
	//Find Command
	//Execute Command
	terminal_set_statement();
}

void terminal_pass_char(char c)
{
	static int mark_distance = 0;

	if(c == 0x0D)		//Carriage Return
	{
		terminal_exec_line();
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


//KOBRA INTERPRETER SHELL
//
//Kobra is a small calculator
//(not) a cheap imitation of Python

#include "programs.h"
#include "screen.h"
#include "multitask.h"
#include "stdheader.h"
#include "process.h"

//kobra functions
void kobra_execute();

//Main kobra routine
int kobra(int argc, char **argv)
{
	//standard process initialisation
	int running = 1;
	int messagebox = 0;

	system_name_process(__process__, "Kobra Shell");
	system_set_process_inbox(__process__, (int) &messagebox);

	//Message-loop
	while(running)
	{
		((char*)VIDEO_ADDRESS)[18] = '#';
          ((char*)VIDEO_ADDRESS)[19] = 0x03;

		if(messagebox & EXIT_SIGNAL)
			system_remove_process_signal(__process__, EXIT_SIGNAL);
			running = 0;

		if(messagebox & EXECUTE_LINE)
			system_remove_process_signal(__process__, EXECUTE_LINE);
			kobra_execute();
	}

	return 0;
}

//Executes the current input line
void kobra_execute()
{


}

//Eventmanagement
#include "screen.h"
#include "kernel.h"

int event;
int event_argument;

//Unnecessary?
int kernel_init_events()
{
	return 0;
}


void kernel_register_event(int event_callee, int arg)
{
	event = event_callee;
	event_argument = arg;
}

int kernel_start_event()
{
	if(!event)
		return 0;

	int (*ev)(int) = (int(*)(int)) event;
	event = 0;

//DEBUG
//tm_print_char('#', TM_COLORS(TM_RED, TM_BLACK));
	return ev(event_argument);
}

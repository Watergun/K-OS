//Eventmanagement
#include "screen.h"

int event;

int init_events()
{
	return 0;
}

void register_kernelevent(int event_callee)
{
	event = event_callee;
}

int start_event()
{
	if(!event)
		return 0;

	int (*ev)(void) = (int(*)()) event; 
	event = 0;

//DEBUG
//tm_print_char('#', TM_COLORS(TM_RED, TM_BLACK));

	return ev();
}

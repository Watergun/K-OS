//Contains essential functions

#include "multitask.h"
#include "screen.h"
#include "stdheader.h"
#include "globalstatus.h"
#include "process.h"

/*
[!]
When the Interrupt ends, "iret" pops EIP, CS and EFLAGS
[!]
*/

//This program gets called by every finished process
//(the return result is still in EAX)
// ~ gets called "simulated return address" at some points ~
void ph_process_end_loop()
{
	int result = 0;

	//handle return result
	__asm__("mov %%eax, %%ebx" : "=b"(result) :);

	switch(result)
	{
		case 0:	//Everything is fine
			break;
		case -1:	//Fatal error
			break;
		case 1: 	//Error
			break;
		default:
			break;
	}

	//delete current process in the PIT

	//WARNING:
	global_critical_section = 1;

//DEBUG
//tm_print_char('D', TM_COLORS(TM_BLACK, TM_WHITE));
//tm_print_hex(result);

/*	[LET THAT BE DONE BY THE PIT_SERVICE_ROUTINE]
	int pindex = global_pit[11];
	global_pit[16 + pindex*2] = 0;		//Zero the entry out
	global_pit[16 + pindex*2 + 1] = 0;
*/

	int pindex = global_system_pit[11];
	global_system_pit[16 + pindex*2] = 0x10;	//Mark for deletion
	//((int*)__process__)[3] = 0x10;	// (?)

	//Message the hosting process about the termination of this one
     int host = ((int*)__process__)[15];
     system_send_process_signal(host, HOSTED_PROCESS_TERMINATED);

//tm_print_char('A', TM_COLORS(TM_BLACK, TM_BLUE));

	//Clean up the PIT
	system_pit_service_routine();

//tm_print_char('O', TM_COLORS(TM_BLACK, TM_BLUE));

//DEBUG
//tm_print_hex(host);

/*
CRITICAL WARNING: If the host-process terminates before and the empty slot in the PIT
				gets filled by a new process, the wrong process may be messaged by that
		[We assume this won't happen!]
*/

	global_critical_section = 0;

	//Wait for switch
	while(1)
	{}
}

//Creates a PIT entry and prepares the initialising stackframe
void ph_add_process(int* address)
{
	if(!global_system_pit)
		return;

	//Save to restore after this function
//	int gcs = global_critical_section;
//	global_critical_section = 1;

	//Create a PIT Pointer starting at the data section (Offset 64)
	int *pitptr = (int*) (((int)global_system_pit)+64);

	int index = 0;	//Starting right after the memory block header

	//Searching empty PIT space
	while(pitptr[index] != 0 && index < 1008)
	{
		index += 2;
	}

	//Empty PIT entry found
	if(pitptr[index] == 0)
	{
		pitptr[index] = address[3];		//ID
		pitptr[index+1] = (int) address;	//Address

//DEBUG
//tm_print_char('#', 0x90);
//

// EDI ESI EBP ESP EBX EDX ECX EAX EIP CS EFLAGS	RETADDR	ARGC ARGV
// 0	  4	 8	12  16  20  24	 28	32  36 40		44		48	52
// 0	  1   2   3   4   5   6   7   8   9  10		11		12	13

		//Prepare the initialising Stackframe
  		int *stackmark =(int*)((int*)(address[14]))[14];	//The stack's "stack" marks the top of the Initframe
		stackmark[0] = 0;						//clean stream destination pointer
		stackmark[1] = 0;						//clean stream source pointer
		stackmark[2] = ((int)stackmark) + 44;		//basepointer at simulated return address
		stackmark[3] = ((int)stackmark) + 32;		//stackpointer at real return address
		stackmark[4] = 0;						//clean ebx
		stackmark[5] = 0;						//clean edx
		stackmark[6] = 0;						//clean ecx
		stackmark[7] = 0;						//clean eax
		stackmark[8] = address[11];				//instruction pointer at process code
		stackmark[9] = 0x08;					//segment pointer at code segment
		int eflags = 0;
		__asm__("pushfl");
		__asm__("mov (%%esp), %%eax" : "=a"(eflags) :);
		__asm__("popfl");
		stackmark[10] = eflags;					//standard process flags
		stackmark[11] = (int)ph_process_end_loop;		//simulated return address is the result handling routine
//DEBUG
//memory_view_func(stackmark, 56);		

		//ARGC and ARGV are already filled out
	}

//	global_critical_section = gcs;
}

//Rountine to switch to the next process in line
int ph_switch_process(int previous_stackmark)
{
//DEBUG
//tm_print_hex(previous_stackmark);

	//no switch, if critical
	if(global_critical_section || (global_bootstatus == 1))
		return previous_stackmark;

//KNOWN ERROR: SYSTEMS STACKMARK IS SET TO THE CURRENT ESP RIGHT HERE -> THEREFORE NO PROCESS CHANGE 
//REASON: CHANGE FROM "SYSTEM" TO "SYSTEM"
 
	//save this stackmark in the interrupted process
	if(__process__ != 0 && global_bootstatus > 2)
		((int*)((int*)__process__)[14])[14] = previous_stackmark;

	system_pit_service_routine();

	//process_activation:
	//__asm__("nop");

	//increment PIT index
	int pit_index = global_system_pit[11] + 1;

	//reached the end of the PIT
	if(global_system_pit[16 + pit_index*2] == 0)
		pit_index = 0;

	//Update PIT index
	global_system_pit[11] = pit_index;

	int *process = (int*) global_system_pit[16 + pit_index*2 + 1];
	int  new_stackmark = ((int*)process[14])[14];
	//Active process
	__process__ = (int) process;

//DEBUG
//tm_print_char('M', 0x04);
//tm_print_hex(new_stackmark);
	
	//Return the new stackmark
	return new_stackmark;
}

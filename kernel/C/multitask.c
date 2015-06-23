//Contains multitasking tables and functions

#include "multitask.h"

int high_priority_process = 0;
/*
Memory unit header: byte 52 - 56: register pointer
points to:
PROCESS ID
EIP
EAX
ECX
EDX
EBX
ESP
EBP
ESI
EDI
EFLAGS
= 10 Registers a 4 byte to be saved (per process) + PID
= 44 Byte

When the Interrupt ends, "iret" pops EIP, CS and EFLAGS 
*/

//Creates a PIT entry and allocates 44 byte for its registers
void ph_add_process(int address)
{
	if(!global_pit)
		return;

	int *pitptr = (int*) (((int)global_pit)+64);

	int index = 16;	//Starting right after the memory block header
	while(global_pit[index] != 0 && index < 1008)
	{	
		index += 2;
	}

	//Empty PIT entry found
	if(global_pit[index] == 0)
	{
		global_pit[index] = ((int*)address)[3];		//ID
		global_pit[index+1] = address;

		int it = 0;
		//Get a pointer to the PSS
		int *pss = (int*) (((int*)global_pit)[12] + 64);
		while(pss[it] != 0 && it < 1008)
		{
			it += 11;
		}

		//Empty PSS entry found
		if(pss[it] == 0)
		{
			//Prepare (ID, EIP, ESP, EBP, EFLAGS)
			pss[it] = ((int*)address)[3];		//Process ID
			pss[it+1] = ((int*)address)[11];	//Process code
			pss[it+6] = ((int*)(((int*)address)[14]))[14];	//Process stack (better: the stack's stack)
			pss[it+7] = pss[it+6];			//Process stack
			int eflags = 0;
			__asm__("pushfl");
			__asm__("mov (%%esp), %%eax" : "=a" (eflags) :);
			__asm__("popfl");
			pss[it+10] = eflags;			//Process flags

			//Save the address of this save state
			((int*)address)[13] = ((int)pss) + it*4;
		}
	}
}

//Rountine to switch to the next process in line
int* ph_switch_process()
{
	//no switch, if critical
	if(global_critical_section)
		return 0;	
/* NOT WORKING
	if(high_priority_process)
	{
		__process__ = high_priority_process;
		tm_print_char('#', 0xF0);
		tm_print_hex(__process__);
		return ((int*)__process__)[13];
	}
*/
	//increment PIT index
	int pit_index = ((int*)global_pit[12])[11] + 1;

	/* XXX pit_service_routine();	*/

	//Reached the end of the PIT
	if(global_pit[16 + pit_index*2] == 0)
		pit_index = 0;
	((int*)global_pit[12])[11] = pit_index;

	int *process = (int*) global_pit[16 + pit_index*2 + 1];
	int *pss = (int*) process[13];

	//
//DEBUG
	__process__ = process;

	//Just to be sure:
	if(pss[0] != global_pit[16 + pit_index*2])
	{
		//Fatal error: Wrong process to register configuration
		return 0;
	}

	//Return the register save address
	return pss;
}

/* NOT WORKING
//This function allows to take influence on the process switcher
void ph_force_switch(int process)
{
tm_print_char('!', 0x0F);

	high_priority_process = process;
	//call the switching interrupt
	__asm__("int $0x20");
tm_print_char('!', 0x0F);
tm_print_hex(__process__);

	high_priority_process = 0;
}
*/

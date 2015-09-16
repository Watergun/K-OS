//Defines the process data structure and functions

#include "process.h"
#include "screen.h"
#include "multitask.h"
#include "memory.h"
#include "globalstatus.h"
#include "stdheader.h"
#include "util.h"
#include "string.h"
#include "kernel.h"

//If more than one consecutive unit is needed, the unitsize is expandable to a multiple of 4096

//Size: 52 -> 64
/*
typedef struct
{
     int *memory_base;        			//where the (program-)memory begins				0
     int memory_size;         			//how large the memory unit is [4096]				1
     int *next_unit;          			//where the next memory unit of this program is		2
     int id;                  			//id number									3
     char label[24];          			//name										4-9
     char security_status;    			//which privilege   (0,1,2,3)						10
     char active;             			//avtive/inactive   (1,0)
	char flags[2];						//Additional flags
	int (*code)(int argc, char**argv);		//The process code								11
	int apt_address;					//allocation pointer table address					12
	int inmsg;						//Message space (just incoming)					13
	int stack_ptr;						//Process-own stack								14
	int hosting_process;				//Host-process									15
} process;

#######
ID list
#######

APT:		0xA
Stack:	0xB
PIT:		0xF
Deletion:	0x10
WLT:		0x35

1st Process: 0x55

*/
//the process identitiy table (PIT) has space for 4032/4/2 = 504 process entries
//every process must be registered in this table

//Creates a new process (Original unit, APT, ID->PIT, Stack)
int system_new_process(int process_code, char security_status, int unitsize)
{
	int gcs = global_critical_section;
	global_critical_section = 1;

	static int ID = 0x55;

	int units = unitsize / USABLE_UNITSIZE;
	if(unitsize % USABLE_UNITSIZE) units++;

	//Save the startingunit
	int *ptr = (int*) system_reserve_unit();
	flash_memory((char*)ptr, UNITSIZE);

	int iterator = 1;
	for(; iterator < units; iterator++)
	{
		int u = system_reserve_unit();
		flash_memory((char*)u, UNITSIZE);
	}


	//reserve this processe's allocation pointer table and stack
	int apt = system_reserve_unit();
	flash_memory((char*)apt, UNITSIZE);

	int stack = system_reserve_unit();
	flash_memory((char*)stack, UNITSIZE);

	ptr[0] = 0;				//mem. base = 0 -> process origin
	ptr[1] = units*UNITSIZE;		//std. unit size
	ptr[2] = 0;				//no next unit
	ptr[3] = ID++;				//ID
	ptr[4] = 0;				//label
	ptr[10] = security_status;	//security + active + other flags will follow
	ptr[11] = process_code;		//process code
	ptr[12] = apt;				//apt
	ptr[14] = stack;			//stack

	int *aptptr = (int*)apt;
	aptptr[0] = (int) ptr;
	aptptr[1] = UNITSIZE;
	aptptr[2] = 0;
	aptptr[3] = 0xA;		//Every APT has 0xA

	int *stackptr = (int*)stack;
	stackptr[0] = (int) ptr;
	stackptr[1] = UNITSIZE;
	stackptr[2] = 0;
	stackptr[3] = 0xB;		//Every stack has 0xB
	stackptr[14] = stack + 4096;	//The stack's stack points at the top of the stack

	global_critical_section = gcs;

	return (int) ptr;
}

//This function just reserves a new unit with the specified size
//and connects it to the original unit
int system_expand_process_memory(int *memory_base, int unitsize)
{
	//Calculate the amount of units in a row
	int units = unitsize / USABLE_UNITSIZE;
	if(unitsize % USABLE_UNITSIZE)
		units++;

	//Reserve a fresh unit (first), because just the first unit needs a header
	int *new_unit = (int*)system_reserve_unit();
	flash_memory((char*)new_unit, UNITSIZE);

	int iterator = 1;
	for(; iterator < units; iterator++)
	{
		int u = system_reserve_unit();
		flash_memory((char*)u, UNITSIZE);
	}

//DEBUG
//tm_print_char('(', TM_COLORS(TM_BLACK, TM_RED));
//tm_print_hex(units*UNITSIZE);
//tm_print_char(')', TM_COLORS(TM_BLACK, TM_RED));

	//Connect to original unit
	memory_base[2] = (int)new_unit;

	new_unit[0] 	= (int)memory_base;		//previous unit as memory base
	new_unit[1] 	= units*UNITSIZE;		//size
	new_unit[2]	 = 0;				//no next unit
	new_unit[3] 	= memory_base[3];		//copy ID
	memory_copy((char*)((int)memory_base)+16,(char*)((int)new_unit)+16, 24);
	new_unit[10] = memory_base[10];		//same security level
	new_unit[11] = 0;					//no process code, since it's just a mem. expansion
	new_unit[12] = 0;					//no apt, ...

	return (int)new_unit;
}

//The process manager has a table of process IDs and their base addresses
void system_process_manager_init()
{
	global_system_pit 	= (int*) system_reserve_unit();
	global_system_pit[0]	= 0; 			//memory base
	global_system_pit[1]	= UNITSIZE;		//unit size
	global_system_pit[3]	= 0xF;			//ID = 0xF = PIT
	memory_copy("K-OS Process ID Table", (char*) ((int)global_system_pit)+16, 21);

	global_system_pit[11] = 0;				//PIT index for multitasking

	//Set to null
	__process__ = 0;
	global_critical_section = 1;
}

//Simply gives a process a name
int system_name_process(int process, char* name)
{
	memory_copy(name, (char*) (process+16), strlen(name));
}



void system_flash_memory_(int arg)
{
	//This little function just flashes the specified memory block
	//it gets called by a kernel event
	
	//The given argument is the memory address
	flash_memory((char*) arg, UNITSIZE);
}

//Loops though the PIT and looks for empty table entries
//in such case, it fills them up again
//If a pro

void system_pit_service_routine()
{
	if(!global_system_pit)
		return;

	__DEBUG_LEVEL__ = 3;

	//Deactivate MT
	global_critical_section = 1;
//	system_use_system_ressources();
	
	//remember null entries (to fill out)
	int null_entry = 0;

	//The Service routine loops through the whole PIT (even through null entries)
	int pindex = 0;	//process index (index stepsize: 1)
	while(pindex < 504)
	{
		if((global_system_pit[16 + 2*pindex] == 0) && (null_entry == 0))
		{
//DEBUG
//tm_print_char('N', TM_COLORS(TM_GREEN, TM_BLACK));

			null_entry = pindex;
		}

		if(global_system_pit[16 + 2*pindex] == 0x10) //Process marked for deletion
		{
//DEBUG
//tm_print_char('X', TM_COLORS(TM_BLACK, TM_RED));

			//Delete all process memory blocks, APT block and stack block
			int *process_address = (int*) global_system_pit[16 + 2*pindex + 1];

			int *memory_block = process_address;

			//Delete all memory extensions
			while(memory_block[2] != 0)
			{
				//Deletion is done by flashing the memory block
				int *next_memory_block = (int*) memory_block[2];
				flash_memory((char*) memory_block, UNITSIZE);

				//TODO: Somehow save this block for future block allocations

				memory_block = next_memory_block;
			}

//DEBUG
//tm_print_char('Y', TM_COLORS(TM_BLACK, TM_RED));
//flash_memory(0, 1);

			//Zero the APT and Stack
			int apt_block = process_address[12];
			flash_memory((char*) apt_block, UNITSIZE);

//DEBUG
//tm_print_char('Y', TM_COLORS(TM_BLACK, TM_RED));

			int stack_block = process_address[14];

//Don't get our hands dirty by flashing our current stack
//			flash_memory(stack_block, UNITSIZE);
			kernel_register_event((int)system_flash_memory_, stack_block);

			//Delete the process block
			flash_memory((char*) process_address, UNITSIZE);

			//Delete the PIT entry
			global_system_pit[16 + pindex*2] = 0;
			global_system_pit[16 + pindex*2] = 0;

//DEBUG
//tm_print_char('E', TM_COLORS(TM_BLACK, TM_GREEN));

			null_entry = pindex;
		}//if(ID == 0x10) [Deletion]

		//check for null entries to fill out
		else if((global_system_pit[16 + pindex*2] != 0) && (null_entry != 0))
		{
//DEBUG
//tm_print_char('F', TM_COLORS(TM_BLACK, TM_GREEN));

			if(null_entry > pindex)
			{
				return;
			}

			//fill
			global_system_pit[16 + null_entry*2] = global_system_pit[16 + pindex*2];			//ID
			global_system_pit[16 + null_entry*2 + 1] = global_system_pit[16 + pindex*2 + 1];	//Address

			//zero the other one out
			global_system_pit[16 + pindex*2] = 0;
			global_system_pit[16 + pindex*2 + 1] = 0;

			//continue at the first null entry occurence
			pindex = null_entry;
			null_entry = 0;
		}

		pindex++;
	}//while(pindex < 504)

	//Activate MT
	global_critical_section = 0;
}

//Starts a given process by address
int system_start_process(int process_address, int argc, char **argv)
{
	//Deactivate Multitasking
	global_critical_section = 1;
	
	//TODO: Check if address is valid

	//Prepare the new process to be called by the ISR
     int *stack = (int*) ((int*)process_address)[14];

	//Fill in the hosting process
	((int*)process_address)[15] = __process__;

//DEBUG
//#ifdef DEBUG_LEVEL_ALL
//tm_print_char('P', TM_COLORS(TM_GREEN, TM_BLACK));
//tm_print_hex(stack);
//#endif
//
	int esp = 0;
	if(stack[3] == 0xB)
		esp = stack[14];
	else
		esp = (int) stack;
		
	//at this point, reserve the initialising stackframe
	esp -= 56;	//12(argc + argv + ret) + 44 (Registers) = 56

	/*
	--- STACKFRAME END
	ARGV			(PROGRAM ARGS)
	ARGC			
	RETURN ADDRESS (PROCESS END ROUTINE)
	EFLAGS		(PROCESSOR FLAGS)
	CS			(CODESEGMENT)
	EIP			(PROCESS CODE)
	EAX			zero
	ECX			zero
	EDX			zero
	EBX			zero
	ESP			(STACKMARK)
	EBP			
	ESI			zero
	EDI			zero
	--- STACKFRAME BEGIN   -> ['esp' VARIABLE]
	14 * INT32 = 56 Byte
	*/
	//fill in the arguments
	((int*)esp)[12] = argc;
	((int*)esp)[13] = (int)argv;

	stack[14] = esp;

	//Prepare PIT and Stackframe (Registers)
	ph_add_process((int*)process_address);

//DEBUG
//tm_print_char('S', 0x48);
//memory_view_func(esp, 56);

	//Reactivate Multitasking
	global_critical_section = 0;

	return 0;
}

//Inter-process-communication realized via Message system (like in Windows)
//the given "messagebox" argument has to be a 4-byte integer variable's address

void system_set_process_inbox(int process_address, int messagebox_address)
{
	((int*)process_address)[13] = messagebox_address;
}

//Simple function to give a signal
void system_send_process_signal(int process_address, int signal)
{
	int *messagebox = (int*) (((int*)process_address)[13]);
	*messagebox |= signal;
}

//Simple function to remove signal
void system_remove_process_signal(int process_address, int signal)
{
	int *messagebox = (int*) (((int*)process_address)[13]);
	*messagebox ^= signal;
}

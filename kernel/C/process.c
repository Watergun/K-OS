//Defines the process data structure and functions

#include "screen.h"
#include "multitask.h"
#include "memory.h"

//If more than one consecutive unit is needed, the unitsize is expandable to a multiple of 4096

//Size: 52 -> 64
/*
typedef struct
{
     int *memory_base;        			//where the (program-)memory begins
     int memory_size;         			//how large the memory unit is [4096]
     int *next_unit;          			//where the next memory unit of this program is
     int id;                  			//id number
     char label[24];          			//name
     char security_status;    			//which privilege   (0,1,2,3)
     char active;             			//avtive/inactive   (1,0)
	char flags[2];						//Additional flags
	int (*code)(int argc, char**argv);		//The process code
	int apt_address;					//allocation pointer table address
	int reg_ptr;						//[reserved memory]	-> multitask.c
	int stack_ptr;						//Process-own stack
} process;
*/

//the process identitiy table (PIT) has space for 4032/4/2 = 504 process entries
//every process must be registered in this table

//Creates a new process (Original unit, APT, ID->PIT, Stack)
int new_process(int process_code, char security_status, int unitsize)
{
	global_critical_section = 1;

	static int ID = 0x55;

	int units = unitsize / USABLE_UNITSIZE;
	if(unitsize % USABLE_UNITSIZE) units++;

	//Save the startingunit
	int *ptr = (int*) reserve_unit();
	flash_memory((char*)ptr, UNITSIZE);

	int iterator = 1;
	for(; iterator < units; iterator++)
	{
		int u = reserve_unit();
		flash_memory((char*)u, UNITSIZE);
	}


	//reserve this processe's allocation pointer table and stack
	int apt = reserve_unit();
	flash_memory((char*)apt, UNITSIZE);

	int stack = reserve_unit();
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

	global_critical_section = 0;
	
	return (int) ptr;
}

//This function just reserves a new unit with the specified size
//and connects it to the original unit
int expand_memory(int *memory_base, int unitsize)
{
	//Calculate the amount of units in a row
	int units = unitsize / USABLE_UNITSIZE;
	if(unitsize % USABLE_UNITSIZE) 
		units++;

	//Reserve a fresh unit (first), because just the first unit needs a header
	int *new_unit = (int*)reserve_unit();
	flash_memory((char*)new_unit, UNITSIZE);	

	int iterator = 1;
	for(; iterator < units; iterator++)
	{
		int u = reserve_unit();
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
	memory_copy(((int)memory_base)+16,((int)new_unit)+16, 24);	
	new_unit[10] = memory_base[10];		//same security level
	new_unit[11] = 0;					//no process code, since it's just a mem. expansion
	new_unit[12] = 0;					//no apt, ...

	return (int)new_unit;
}

//The memory manager itself has a table of process IDs and their base addresses
void memory_manager_init()
{
	global_pit 	= (int*) reserve_unit();
	global_pit[0]	= 0; 			//memory base
	global_pit[1]	= UNITSIZE;		//unit size
	global_pit[3]	= 0xF;			//ID = 0xF = PIT
	memory_copy("K-OS Process ID Table", ((int)global_pit)+16, 21);

	//Reserve memory for the Process Save State (PSS) -> multitask.c
	int* pss 	= (int*) reserve_unit();
	pss[0] = global_pit;
	pss[1] = UNITSIZE;
	pss[3] = 0x10;
	pss[11] = 0;		//This is just for remarks (the current PIT/PSS switch index)

	//in the APT slot of the PIT
	global_pit[12] = pss;

	//The PSS saves all registers of running processes, so that when a process is switched, every value is saved
	//Furthermore, it saves the index of the running process
	pss[11] = 0;	//Using the "Process Code Address" 4-byte slot

	__process__ = 0;
	//Set to null
	global_critical_section = 0;
}

//Simply gives a process a name
int name_process(int process, char* name)
{
	memory_copy(name, process+16, strlen(name));
}

//Loops though the PIT and looks for empty table entries
//in such case, it fills them up again
void pit_service_routine()
{
	//TODO

}


//Starts a given process by address
int start_process(int process_address, int argc, char **argv)
{
	//Deactivate Multitasking
	global_critical_section = 1;

	//TODO: Check if address is valid

	//Prepare the new process to be called by the ISR
     int *stack = ((int*)process_address)[14];
	int esp = stack[14];
	esp -= 8;
	((int*)esp)[0] = argc;
	((int*)esp)[1] = argv;
	stack[14] = esp;

	//Prepare PIT and PSS
	ph_add_process(process_address);

	//Function pointer
//	int (*pr)(int, char**);

	//Take over the process code address
//	pr = (int(*)(int,char**)) ((int*)process_address)[11];

	//Reactivate Multitasking
	global_critical_section = 0;

	//Execute code
	//pr(argc, argv);
	return 0;
}

//Defines the process data structure and functions

#define UNITSIZE 4096
#define USABLE_UNITSIZE 4032
#define UNITHEADERSIZE 64

#include "multitask.h"
#include "screen.h"

//If more than one consecutive unit is needed, the unitsize is expandable to a multiple of 4096

//Size: 52 -> 64
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
} process;

//Static variables to handle the PIT
static int *pit;
static int pit_offset;

//the process identitiy table (PIT) has space for 4032/4/2 = 504 process entries
//every process must be registered in this table

//Creates a new process (Original unit, APT, ID->PIT )
int new_process(int process_code, char security_status, int unitsize)
{	
	static int ID = 0x55;

	int units = unitsize / USABLE_UNITSIZE;
	if(unitsize % USABLE_UNITSIZE) units++;

	//Save the startingunit
	int *ptr = (int*)reserve_unit();
	flash_memory((char*)ptr, UNITSIZE);
	
	int iterator = 1;
	for(; iterator < units; iterator++)
	{
		int u = reserve_unit();
		flash_memory((char*)u, UNITSIZE);
	}
		
	//make new entry in process identity table (PIT) (ID/PTR TABLE)
	((int*)pit_offset)[0] = ID;
	((int*)pit_offset)[1] = (int)ptr;
	pit_offset += 8;

	//reserve this processe's allocation pointer table
	int apt = reserve_unit();
	flash_memory((char*)apt, UNITSIZE);
	
	ptr[0] = 0;				//mem. base = 0 -> process origin
	ptr[1] = units*UNITSIZE;		//std. unit size
	ptr[2] = 0;				//no next unit
	ptr[3] = ID++;				//ID
	ptr[4] = 0;				//label
	ptr[10] = security_status;	//security + active + other flags will follow
	ptr[11] = process_code;		//process code
	ptr[12] = apt;				//apt

/*
	//write the apt header
	int *aptptr = (int*)apt;
	*(aptptr) = (int)ptr;		//an apt's mem base points to its process
	*(aptptr+4) = UNITSIZE;		//
	*(aptptr+8) = 0;			//
	*(aptptr+12) = 0xA;			//ID = 0xA = APT
	*(aptptr+16) = 0;			//no label needed
	*(aptptr+40) = 0;			//no sec status needed
	*(aptptr+41) = 0;			//no flags needed
	*(aptptr+44) = 0;			//no process code
	*(aptptr+48) = 0;			//no apt
*/
	int *aptptr = (int*)apt;
	aptptr[0] = (int) ptr;
	aptptr[1] = UNITSIZE;
	aptptr[2] = 0;
	aptptr[3] = 0xA;
		
	return (int) ptr;
}

//This function just reserves a new unit with the specified size
//and connects it with the original unit
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
	memory_copy(memory_base+16, new_unit+16, 24);	
	new_unit[10] = *(memory_base+40);		//same security level
	new_unit[11] = 0;					//no process code, since it's just a mem. expansion
	new_unit[12] = 0;					//no apt, ...

	return (int)new_unit;
}

//The memory manager itself has a table of process IDs and their base addresses
void init_memory_manager()
{
	pit = (int*) reserve_unit();
	flash_memory(pit, UNITSIZE);	

	pit[0]	 = 0;			//memory base
	pit[1]	 = UNITSIZE;		//unit size
	pit[3]	 = 0xF;			//ID = 0xF = PIT
	memory_copy("K-OS Process ID Table", (pit+16), 21);

	pit_offset = 0;
}

//Simply gives a process a name
int name_process(char* name)
{
	memory_copy(name, active_process+16, strlen(name));
}

//Loops though the PIT and looks for empty table entries
//in such case, it fills them up again
void pit_service_routine()
{
	//TODO

}

//Starts a given process by address
void start_process(int process_address, int argc, char **argv)
{		
	//TODO: Check if address is valid
	
	//Function pointer
	int (*pr)(int, char**);
	
	//Take over the process code address
	pr = (int(*)(int,char**)) ((int*)process_address)[11];

	//
	add_process(process_address);	
	
	//Execute code
	pr(argc, argv);
}

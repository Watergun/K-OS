//Allocation function (takes datasize and returns address)
//it looks through the APT and finds a suitable place for new data
//
//the APT is a simple pointer-datasize table (4 B/4 B) and has therefore space
//for 504 pointer entries
// 
//note that all pointers in the APT must (!) be translated from monounit to multiunit addressing
//APT Pointers are just offset values starting from memory_base + 64 (so the 1st ptr/offset is 0) 

//Support for APT-Extension (TODO!) isn't implemented yet

#include "allocation.h"
#include "multitask.h"
#include "screen.h"
#include "memory.h"
#include "process.h"
#include "kernel.h"
#include "util.h"

int memory_allocate_data(int process_address, unsigned int size)
{
//DEBUG
//tm_print_char('A', TM_COLORS(TM_BLACK, TM_GREEN));
//tm_print_hex(process_address);

	//Make datasize to a multiple of 4
	size = 4 * (size/4 + 1);
	//Get allocation pointer table
	int *apt = (int*)((int*)process_address)[12];
	apt = (int*) (((int)apt)+64);

	int pointer = 0;	
	
	//APT indices
	int index = 0;
	int found = 0;
	int lastptr = 0;
	int insert = 0;
	
	while(index < 1008)
	{
		//1. [get pointer]
		int ptr = apt[index];
//int ptrsize = apt[index+1];		
//DEBUG	
//tm_print_char('(', TM_DEFAULT_STYLE);	
//tm_print_hex(ptr);
//tm_print_char('|', TM_DEFAULT_STYLE);
//tm_print_hex(ptrsize);
//tm_print_char(')', TM_DEFAULT_STYLE);

		//compare to next pointer
		int nextptr = apt[index+2];
		
		//add the pointer's datasize to the pointer
		ptr += apt[index+1];		
//DEBUG
//tm_print_hex(ptr);

		//2. [check space]

		//if this is the last pointer in the list
          if(!ptr)
          {
//DEBUG
//tm_print_char('L', TM_COLORS(TM_BLACK, TM_YELLOW));
               pointer = lastptr;
               found = 1;
          }

		//or if there is place between two pointers
		else if((nextptr-ptr) >= size)
		{
			pointer = ptr;
			insert = 1;		
			found = 1;
		}
	
		//3. [next step investigation: unit break]
		if(found)
		{	
//DEBUG
//tm_print_char('?', TM_DEFAULT_STYLE);
//tm_print_hex(pointer);

			//Check if unitrange break
		     int databegin = alloc_translate_apt_pointer(process_address, pointer);
     		int dataend = alloc_translate_apt_pointer(process_address, pointer+size);

//DEBUG
//tm_print_hex(databegin);
//tm_print_hex(dataend);					
			
			if((dataend - databegin) != size || !databegin || !dataend)
		     {
//DEBUG
//tm_print_char('X', TM_COLORS(TM_RED, TM_BLACK));

          		//The current unit doesn't have enough space for the data
          		//So look for the next one

				int next_unit = 0;
				int *current_unit = (int*) alloc_get_unit_by_address(process_address, databegin);

				//Reserve one additional unit, if there is no next one
                    //important: to expand memory, the last unit in the unitchain has to be passed to the function
				if(current_unit[2] == 0)
				{	

//DEBUG
//tm_print_char('N', TM_COLORS(TM_VIOLET, TM_BLACK));

					//Done !
					next_unit = (int) system_expand_process_memory((int*)alloc_get_last_unit(process_address), size);
					pointer = alloc_translate_address(process_address, next_unit + 64);
					break;
				}
				else		//more investigations are incoming now
					next_unit = current_unit[2];
				
//DEBUG
//tm_print_char('I', TM_COLORS(TM_BLUE, TM_BLACK));
//tm_print_hex(next_unit);

				//next question: is there enough space for our data on the next unit?
				
				//<problem possibility 1> collision with next pointer
				databegin = next_unit + 64;

//DEBUG
//tm_print_char('{', TM_DEFAULT_STYLE);
				int databegin_ptr = alloc_translate_address(process_address, databegin);
//tm_print_char('}', TM_DEFAULT_STYLE);


//tm_print_char('D', TM_COLORS(TM_CYAN, TM_BLACK));
//tm_print_hex(databegin);
//tm_print_hex(databegin_ptr);

				//if this is the last ptr in the APT
				if(!ptr)
				{
//DEBUG
//tm_print_char('L', TM_COLORS(TM_BLACK, TM_YELLOW));

					//<problem possibility 2> next unit is too small
					int *memory_base = (int*) next_unit;

					//look for units which can hold the size
					while(memory_base != 0 && (memory_base[1]-64) < size)
						memory_base = (int*) memory_base[2];
					
					//no unit can hold the size
					if(!memory_base)
						next_unit = system_expand_process_memory(memory_base, size);

					//Done !
					pointer = alloc_translate_address(process_address, next_unit);
					break;
				}
				
				//if there is another pointer somewhere, but space to it is big enough
				if((nextptr - databegin_ptr) >= size)
				{
					//<problem possibility 2> unit(s) between pointers is(are) too small for the data
					
					//this is the unit where the next pointer points to			
					int nextptr_unit = alloc_get_unit_by_address(process_address, alloc_translate_apt_pointer(process_address, nextptr));
	
					int *memory_base = (int*) next_unit;
					while(memory_base != 0 && (memory_base[1]-64) < size)
					{
						int unit = alloc_get_unit_by_address(process_address, (int)memory_base);
						
						//nextptr passed: give up
						if(unit > nextptr_unit)
							goto next;

						memory_base = (int*) memory_base[2];
					}	 

					//critical memory error (no unit left, but still a nextptr in apt)
					if(!memory_base)
						return 0;
 			
					next_unit = (int) memory_base;					

					//is the gap still big enough?
					databegin_ptr = alloc_translate_address(process_address, next_unit + 64);
					if((nextptr - databegin_ptr) >= size)
					{
						//Done !
						pointer = databegin_ptr;
						break;
					}

					//nice try, but a fail in the end
					else
						goto next;

				}//if(nextptr - databegin_ptr >= size)
				
				//not big enough
				else			
					goto next;
			}//unitbreak check
			
			else
				break;

		}//if(found)

		next:

		//check next APT pointer
		found = 0;
		insert = 0;
//DEBUG
//tm_print_char('F', TM_DEFAULT_STYLE);
//tm_print_hex(ptr);
		lastptr = ptr;
		index += 2;
	}

	//insertion activated
	if(insert)
	{
		index += 2;
		//move all entries one index further
		int offset = index*4;
		memory_copy_rev((char*) ((int)apt)+offset, (char*) ((int)apt) + offset + 8, (4032-offset)-8);
	}

//DEBUG
//tm_print_char('$', TM_DEFAULT_STYLE);
//tm_print_hex(pointer);
//tm_print_hex(index);

	//fill in apt entry data
	apt[index] 	= pointer;
	apt[index+1] 	= size;

//DEBUG
//memory_view_func(apt, 64);
//	
	//translate pointer to real physical address
	int address = alloc_translate_apt_pointer(process_address, pointer);

	if(address == 0)
		return 0;

//DEBUG
//tm_print_hex(address);
	
	return address;
}

//Translates an APT Pointer to a physical address
int alloc_translate_apt_pointer(int process_address, int ptr)
{
	int memory_base = process_address;
	int address = 0;

	if(ptr < 0 || process_address <= 0)
		return -1;
	
	while(ptr >= 0)
	{
		int unitstart = memory_base + 64;
		int unitsize = ((int*)memory_base)[1];
		unitsize -= 64;
//DEBUG
//tm_print_hex(unitstart);
//tm_print_hex(unitsize);

		//The pointer reaches inside the unit
		if((ptr - unitsize) < 0)
		{ 
//DEBUG
//tm_print_char('X', 0x0f);
			address = unitstart+ptr;
			break;
		}

		//The pointer reaches out of the unit
		ptr -= unitsize;
		memory_base = ((int*)memory_base)[2];
		
		//If there is no next unit
		if(memory_base == 0)
			return 0;
	}

	return address;
}

int alloc_translate_address(int process_address, int address)
{
	int memory_base = process_address;
	int offset = 0;

	while(1)
	{
		int unitstart = memory_base+64;
		int unitsize = ((int*)memory_base)[1] - 64;
		int unitend = unitstart + unitsize;
		
		if(address <= memory_base)
			return -1;
		if(address > memory_base && address < unitstart)
			return -1;
		if(address > memory_base && address < unitend)
		{
			offset += address - unitstart;

			break;
		}
		if(address > unitend)
		{
			memory_base = ((int*)memory_base)[2];
			if(memory_base == 0)
				return -1;

			offset += unitsize;	
		}
	}

	return offset;
}

//Returns the last unit in a unit chain
int alloc_get_last_unit(int process_address)
{
	int *address = (int*) process_address;
	while(address)
	{
		int nextunit = address[2];
		if(nextunit == 0)
			return (int) address;
		address = (int*) nextunit;
	}
	return 0;
}

//Returns the unit, in which the given address lies
int alloc_get_unit_by_address(int process_address, int address)
{
	int *memory_base = (int*) process_address;	

	while(memory_base)
	{
		//If address points to somewhere else
		if((int)memory_base > address)
			return 0;

		//If address lies in unitrange
		int unitsize = memory_base[1];
		if((int)memory_base <= address && (((int)memory_base)+unitsize) > address)
			return (int) memory_base;
		
		//If address reaches out of the unit
		else
			memory_base = (int*) memory_base[2];
	}

	return 0;
}


//Service routine to look for empty APT entries and fix them
//also: reordering its entries
void apt_service_routine(int process)
{
	//This APT Service checks the active APT
	int index = 0;
	int *apt = (int*) ((int*)process)[12];
	int lastptr = 0;

	while(index < 1008)
	{
		int ptr = apt[index];		

		//Hole in APT found 
		if(ptr == 0xFFFF)
		{	
//DEBUG
//tm_print_char('O', TM_COLORS(TM_BLUE, TM_RED));
//memory_view_func(&apt[index], 16);
//tm_print_char('|', TM_COLORS(TM_BLUE, TM_RED));
			//Simply move everything one APT entry further to the front
			memory_copy((char*) &apt[index+2],(char*) &apt[index], 4032 - (index*4));
			flash_memory((char*) ((int)apt) + 4024, 8);

//memory_view_func(&apt[index], 16);
		}

		//Deletion needed (however this came)
		else if(ptr == lastptr && ptr != 0)
		{
			//delete APT entry
			apt[index] = 0;
			apt[index+1] = 0;

			//let the next loop fix that
			index -= 2;
		}

		index += 2;
	}
}


//Real memory allocation (using active process)
int memory_allocate(unsigned int size)
{
	return memory_allocate_data(__process__, size);
	//return 0x00100000;
}

//Frees the specified memory (in active process)
void memory_free(int address)
{
//DEBUG
//tm_print_char('(', TM_DEFAULT_STYLE);
//tm_print_hex(address);
//tm_print_char(')', TM_DEFAULT_STYLE);

	int *apt = (int*) ((int*)__process__)[12];
	apt = (int*) (((int)apt)+64);
	
	//some error
	if(!address || !apt)
		return;

	//Find the given address in the apt
	int index = 0;
	while(index < 1008)
	{
		int ptr = apt[index];
//DEBUG
//tm_print_hex(ptr);
//tm_print_char(':', TM_DEFAULT_STYLE);	

		//APT ended before address could be found (maybe apt_service_routine doesn't work?)
		if(index != 0 && ptr == 0)
			return;

		int address_it = alloc_translate_apt_pointer(__process__, ptr);

//DEBUG		
//tm_print_hex(address_it);

		//Could not find address
		if(address_it == 0)
			return;
	
		//the addresses match
		if(address == address_it)
		{
//DEBUG
//tm_print_char('F', TM_DEFAULT_STYLE);
//tm_print_hex(ptr);
//tm_print_hex(address);

			//Free memory (mark free APT entry with 0xffff
			apt[index] = 0xFFFF;
			apt[index+1] = 0;

			//Immediatly call the service routine after this
			kernel_register_event((int)apt_service_routine, __process__);

			break;
		}
		else
			index += 2;
	}

	//At this point, the apt is full and no matching pointer was found
	return;
}

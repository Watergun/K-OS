//Allocation function (takes datasize and returns address)
//it looks through the APT and finds a suitable place for new data
//
//the APT is a simple pointer-datasize table (4 B/4 B) and has therefore space
//for 504 pointer entries
// 
//note that all pointers in the APT must (!) be translated from monounit to multiunit addressing
//APT Pointers are just offset values starting from memory_base + 64 (so the 1st ptr/offset is 0) 

//Support for APT-Extension (TODO!) isn't implemented yet

#include "multitask.h"
#include "screen.h"

int allocate_data(int process_address, int size)
{
//DEBUG
//tm_print_char('A', TM_COLORS(TM_BLACK, TM_GREEN));

	//Make datasize to a multiple of 4
	size = 4 * (size/4 + 1);
	//Get allocation pointer table
	int *apt = (int*)((int*)process_address)[12];
	apt = (int*) (((int)apt)+64);

	int pointer = 0;	
	
	//APT index
	int index = 0;
	int found = 0;
	int lastptr = 0;
	int insert = 0;
	
	while(index < 504)
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
		     int databegin = translate_apt_pointer(process_address, pointer);
     		int dataend = translate_apt_pointer(process_address, pointer+size);

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
				int *current_unit = (int*) get_unit_by_address(process_address, databegin);

				//Reserve one additional unit, if there is no next one
                    //important: to expand memory, the last unit in the unitchain has to be passed to the function
				if(current_unit[2] == 0)
				{	

//DEBUG
//tm_print_char('N', TM_COLORS(TM_VIOLET, TM_BLACK));

					//Done !
					next_unit = (int) expand_memory(get_last_unit(process_address), size);
					pointer = translate_address(process_address, next_unit + 64);
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
				int databegin_ptr = translate_address(process_address, databegin);
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
						next_unit = expand_memory(memory_base, size);

					//Done !
					pointer = translate_address(process_address, next_unit);
					break;
				}
				
				//if there is another pointer somewhere, but space to it is big enough
				if((nextptr - databegin_ptr) >= size)
				{
					//<problem possibility 2> unit(s) between pointers is(are) too small for the data
					
					//this is the unit where the next pointer points to			
					int nextptr_unit = get_unit_by_address(process_address, translate_apt_pointer(process_address, nextptr));
	
					int *memory_base = (int*) next_unit;					
					while(memory_base != 0 && (memory_base[1]-64) < size)
					{
						int unit = get_unit_by_address(process_address, memory_base);
						
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
					databegin_ptr = translate_address(process_address, next_unit + 64);
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
		memory_copy_rev(((int)apt)+offset, ((int)apt) + offset + 8, (4032-offset)-8);
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
	int address = translate_apt_pointer(process_address, pointer);

	if(address == 0)
		return 0;

//DEBUG
//tm_print_hex(address);
	
	return address;
}

//Translates an APT Pointer to a physical address
int translate_apt_pointer(int process_address, int ptr)
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

int translate_address(int process_address, int address)
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
int get_last_unit(int process_address)
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
int get_unit_by_address(int process_address, int address)
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
//(also: reordering its entries
void apt_service_routine()
{
	//TODO: write code
}


//Real memory allocation (using active process)
//Will be substituted by a better function, when multicore using is activated
char* malloc(int size)
{
	return (char*) allocate_data(active_process, size);
	//return 0x00100000;
}

//Frees the speicified memory 
void mfree(int address)
{
	//1. l

}

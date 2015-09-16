#ifndef ALLOCATION_H
#define ALLOCATION_H

//All implemented functions -> For descriptions, seek the .c file

int memory_allocate_data(int process, unsigned int bytes); 

int alloc_translate_apt_pointer(int process, int ptr);
int alloc_translate_address(int process, int addr);
int alloc_get_unit_by_address(int process, int addr);
int alloc_get_last_unit(int process);

void apt_service_routine(int process);

int memory_allocate(unsigned int bytes);
void memory_free(int ptr);

#endif

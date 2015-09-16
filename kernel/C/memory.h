#ifndef MEMORY_H
#define MEMORY_H
//Some defines for memory usage

#define UNITSIZE 4096
#define USABLE_UNITSIZE 4032
#define UNITHEADERSIZE 64

//All implemented memory functions (Comment: Except the allocation functions, which are in another file)

//Implemented in Assembler: reserves a memoryblock of size 0x1000 [UNITSIZE] = 4096 Bytes 
int system_reserve_unit();

#endif

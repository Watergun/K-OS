#ifndef MEMORY_H
#define MEMORY_H
//Some defines for memory usage

#define UNITSIZE 4096
#define USABLE_UNITSIZE 4032
#define UNITHEADERSIZE 64

//Functions
char *malloc(int datasize);
void mfree(int address);

#endif

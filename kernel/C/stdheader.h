#ifndef STDHEADER_H
#define STDHEADER_H

typedef unsigned int size_t;

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

#define NULL	0

#define INT32(H, L)	((H << 16) | ((L << 16) >> 16))
#define INT16_HIGH(I)			(I >> 16)
#define INT16_LOW(I)			((I << 16) >> 16) 
#define BIT_FROM_BYTE(Byte, Index)	((Byte << (7-Index)) >> 7)

/*
INTER - PROCESS - COMMUNICATION MESSAGES
*/

//four-byte values, but the first byte must be null

//Exit current program
#define EXIT_SIGNAL 			0x00000001

//Terminal-specific, but can be used for other purposes too
#define EXECUTE_LINE               0x00000010

//
#define HOSTED_PROCESS_TERMINATED	0x00010000


#endif

;K-OS INTERRUPT DESCRIPTOR TABLE
;Table of Interrupt Routine Descriptors

;8byte Structure:
;Offset (bit 0-15)		2 byte
;codesegment selector	2 byte
;zero				1 byte
;type and attributes	1 byte
;Offset (16-31)		2 byte
;
;Type attribute:
;7 | Present | Descriptor Privilege | Storage Segment | Gate Type	| 0
;		1				2			1				4
;32 bit Interrupt Gate:	1 00 0 1110b => 0x8e

;Whole IDT must equal 256*8 = 2048 bytes

global IDT_START
global IDT_END

IDT_START:
;	#1	
;	dw	0x0000	;Offset 1
;	dw	0x0008	;Code segment
;	db	0x00		;zero
;	db	0x8e		;32 bit interrupt gate
;	dw	0x0000	;offset 2

times 48 dq 0x00008e0000080000

IDT_END: 

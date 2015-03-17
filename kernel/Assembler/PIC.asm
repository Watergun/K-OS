;This PIC_remap function reinitialises the PICs
;to map IRQ 0-7 to 0x20-0x27

global PIC_remap
global PIC_EOI

MASTER_PIC equ 0x20
SLAVE_PIC equ 0xA0
MASTER_PIC_DATA equ 0x21
SLAVE_PIC_DATA equ 0xA1

ICW1_ICW4	equ 0x01
ICW1_INIT equ 0x10

PIC_ICW 	db 0x00

PIC_remap:
	;To begin, save the data that is already loaded
	pusha
	in ax, MASTER_PIC_DATA
	push ax
	in ax, SLAVE_PIC_DATA
	push ax

	;Create ICW1 -> Starts Initialisation
	mov al, [PIC_ICW]
	or al, ICW1_ICW4
	or al, ICW1_INIT
	out MASTER_PIC, al		;Port ICW1 out to Master PIC
	out SLAVE_PIC, al		;Port ICW1 out to Slave PIC
	
	;ICW2 contains the offset of the Interrupt Table -> Remapping
	mov al, 0x20			;INT 08h - 15h -> INT 20h - 27h
	out MASTER_PIC_DATA, al
	mov al, 0x28			;INT 70h - 77h -> INT 28h - 2Fh
	out SLAVE_PIC_DATA, al
	
	;ICW3 
	mov al, 0x4			;Tell MASTER_PIC to connect to SLAVE_PIC via IRQ2 -> bit 3 (0100b)
	out MASTER_PIC_DATA, al
	mov al, 0x2			;Tell SLAVE_PIC it's connected on MASTER_PICs 2nd IRQ -> 2 (0010b)
	out SLAVE_PIC_DATA, al
	
	;ICW4
	mov al, 0x1			;0x1 enables 80x86 mode
	out MASTER_PIC_DATA, al
	out SLAVE_PIC_DATA, al

	;Restore data from before
	pop ax
	out SLAVE_PIC_DATA, ax
	pop ax
	out MASTER_PIC_DATA, ax
	popa
	ret

;This PIC_EOI function sends the End Of Interrupt signal
;which tells the PIC to continue sending Interrupts to the CPU
;IMPORTANT: This function has a parameter (int) IRQ_number

global PIC_EOI

EOI_SIGNAL equ 0x20

PIC_EOI:
	push eax
	mov eax, dword [esp+8]
	;If IRQ_number is >= 8, both PICs must be signaled
	cmp eax, 8
	jl .master_pic
.slave_pic:
	mov al, EOI_SIGNAL
	mov dx, SLAVE_PIC
	out dx, al
.master_pic:
	mov al, EOI_SIGNAL
	mov dx, MASTER_PIC
	out dx, al
	pop eax
	ret

;K-OS Boot sector 
[org 0x7c00]
[bits 16]

KERNEL equ 0x1000			;This is where the kernel will be loaded to

boot_start:

mov [DRIVE_NO], dl			;BIOS stores our boot drive in DL (0x80 for primary harddrive)
mov bp, 0x9000				;Setup the stack
mov sp, bp

call load_kernel			;Load the kernel

call switch_to_pm				;Switch to 32 bit protected mode
jmp $

;Some 16bit functions
%include "print/print_string.asm"
%include "disk/disk_load.asm"
%include "print/print_hex.asm"
%include "print/print_char.asm"

load_kernel:
mov bx, STR_LOAD_KERNEL		;Print a booting kernel loading message
call print_string

mov bx, KERNEL				;Setup paramters for disk_load function
mov dh, 0x15				;15 sectors (will mostly load fewer sectors)
mov dl, [DRIVE_NO]
call disk_load
ret

[bits 32]					;32 bit code

%include "print_string.asm"
%include "gdt.asm"
%include "switch_to_pm.asm"

;This is where we arrive after switching to protected mode
BEGIN_PM:

mov ebx, STR_PROTECTED_MODE	;Print message 
call print_string_32bit

call KERNEL				;Jump to the loaded kernel

jmp $

;Global variables
DRIVE_NO			db 0
STR_PROTECTED_MODE	db "BOOTING OS... ", 0
STR_LOAD_KERNEL 	db "LOADING KERNEL... ", 0

;Bootsector padding
times 510-($-$$) db 0
dw 0xaa55

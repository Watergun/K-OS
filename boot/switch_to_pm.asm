[bits 16]

;Switch to protected mode
switch_to_pm:

cli		;We must switch off interrupts until we have set-up the protected mode
		;interrupt vector otherwise interrupts won't be handled correctly anymore

lgdt [gdt_descriptor]	;Load our global descriptor table, which defines the protected mode
					;segments (e.g. for code and data)

mov eax, cr0		;To make the switch to protected mode, we set the first bit of CR0, a control register
or eax, 0x1
mov cr0, eax

jmp CODE_SEG:init_pm	;Make a far jump (i.e. to a new segment) to our 32-bit code.
					;This forces the CPU to flush its cache of pre-fetched and real-mode decoded
					;instructions, which can cause problems

[bits 32]
;Initialise registers and the stack in PM
init_pm:

mov ax, DATA_SEG	;Now in PM, our old segments are meaningless, so we point our segment registers
mov ds, ax		;to the data selector (segment) we defined in our GDT
mov ss, ax
mov es, ax
mov fs, ax
mov gs, ax

mov ebp, 0x8FF00	;Give the stack enough free memory to use
mov esp, ebp

call BEGIN_PM		;Jump back

;K-OS Interrupt Service Routines INT 0 - 48

[extern PIC_EOI]
[extern Keyboard_Code]
[extern TM_print_byte_hex]
[extern print_char]

;global ISR_0h
;global ISR_1h
;global ISR_2h
global ISR_20h
global ISR_21h
global ISR_23h
global ISR_24h
global ISR_25h
global ISR_26h
global ISR_27h
global ISR_28h
global ISR_29h
global ISR_2Ah
global ISR_2Bh
global ISR_2Ch
global ISR_2Dh
global ISR_2Eh
global ISR_2Fh
global ISR_30h

ISR_20h:		;Hardware Interrupt [Ring 0] (Timer)
	call PIC_EOI
	iret
	
ISR_21h:		;Hardware Interrupt [Ring 0] (Keyboard)
	pusha	
	mov dx, 0x64
	in al, dx
	and al, 0x01	
	jz .end

	mov dx, 0x60
	in al, dx

	;push ax
	;call Keyboard_Code
	;pop bx

	push 0x00F0
	push ax
	call print_char
	pop ax
	pop ax

	.end:
	call PIC_EOI
	popa
	iret

;
;ISR_22h not implemented (!) because:
;There is no INT 22h since we connected IRQ 2 of the master PIC to the slave PIC 
;

ISR_23h:		;Hardware Interrupt [Ring 0] (COM2) Serial Port 2 & 4
	call PIC_EOI
	iret

ISR_24h:		;Hardware Interupt [Ring 0] (COM1) Serial Port 1 & 3
	mov byte [0xb8002], 'M'
	mov byte [0xb8003], 0x45
	call PIC_EOI
	iret		
	
ISR_25h:		;Hardware Interrupt [Ring 0] (Sound Card)
	call PIC_EOI
	iret

ISR_26h:		;Hardware Interrupt [Ring 0] (Floppy Disk Controller)
	call PIC_EOI
	iret

ISR_27h:		;Hardware Interrupt [Ring 0] (Parallel Port) (Mostly Fake IRQ)
	call PIC_EOI
	iret

ISR_28h:		;Hardware Interrupt [Ring 0] (Real Time Clock)
	push eax
     mov byte al, byte [0xb8004]
     inc al
     mov byte [0xb8004], al
     mov byte [0xb8005], 0x47
     pop eax

	call PIC_EOI
	iret

ISR_29h:		;Hardware Interrupt [Ring 0] (Open Interrupt)
	mov byte [0xb8002], 'M'
     mov byte [0xb8003], 0x45
     call PIC_EOI
     iret


	call	PIC_EOI
	iret

ISR_2Ah:		;Hardware Interrupt [Ring 0] (Open Interrupt)
	call PIC_EOI
	iret

ISR_2Bh:		;Hardware Interrupt [Ring 0] (Open Interrupt)
	call PIC_EOI
	iret

ISR_2Ch:       ;Hardware Interrupt [Ring 0] (PS/2 Mouse)
	mov byte [0xb8002], 'M'
     mov byte [0xb8003], 0x45
     call PIC_EOI
     iret

ISR_2Dh:       ;Hardware Interrupt [Ring 0] (Floating Point Unit / Coprocessor)
	call PIC_EOI
	iret

ISR_2Eh:       ;Hardware Interrupt [Ring 0] (Primary ATA Hard Disk)
	call PIC_EOI
	iret

ISR_2Fh:       ;Hardware Interrupt [Ring 0] (Secondary ATA Hard Disk)
	call PIC_EOI
	iret

ISR_30h:		;Software Interrupt [Ring 3] (Kernel Call)
	call PIC_EOI
	iret

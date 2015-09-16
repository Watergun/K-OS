;K-OS Interrupt Service Routines INT 0 - 48

;Here, all the interrupt routines are defined
;Every interrupt routine (IR) has to send the End Of Interrupt signal 
;to the PIC before returning with the 'iret' command 

[extern PIC_EOI]
[extern keyboard_code]
[extern system_pass_character]
[extern tm_print_char]
[extern tm_print_hex]
[extern ph_switch_process]
[extern memory_view_func]
[extern __process__]
[extern __DEBUG_LEVEL__]
[extern global_bootstatus]
[extern system_halt_system]

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
	;This interrupt takes the role of the process switcher
	;Everytime this timer ticks, the processhandler switches to the next process in the chain

	;Increment tickcounter
	push eax
	push edx

	mov eax, dword [0x500]
	inc eax
	mov dword [0x500], eax

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;DEBUG: Reduce switching speed
;	mov edx, 0
;	push ebx
;	mov ebx, 0xA0
;	div ebx
;	pop ebx
;	cmp edx, 1
;
;	jz .continue
;
;	pop edx
;	pop eax
;	jmp .skip
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	.continue:
	pop edx	;Obligatory anyway
	pop eax
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	;If no process exists, skip MT
;	cmp dword [__process__], 0
;	je .test

	;The bootstatus is 1 if this routine gets called for the first time
	cmp dword [global_bootstatus], 1
	je .skip

	;Save Registers on the stack
	pushad 	;Now we've got all the registers on the stack (EDI,ESI,EBP,ESP,EBX,EDX,ECX,EAX,EIP,CS,EFLAGS)
			;	offsets:							  0	 4	8   12  16  20  24  28  32  36 40

	.switch:	;Activate the new process (e.g. load the new stackmark)

;DEBUG
;call tm_print_hex ;Print ESP

	;the new switching concept just saves the stackmark / ESP
	push esp

	call ph_switch_process	;Returns the stackmark of the new process
	add esp, 4

	mov [RESULT], eax

;cli
;push 0x0E
;push 'I'
;call tm_print_char
;add esp, 8

;push dword 56
;push dword [RESULT]
;call tm_print_hex
;call memory_view_func
;add esp, 8
;jmp system_halt_system

	;Check for zero (e.g. switch didn't work / something went definitely wrong)
	cmp dword [RESULT], 0
	jz system_halt_system

;push dword 0x45
;push dword 'F'
;call tm_print_char
;add esp, 8

	mov esp, [RESULT]			;activate the new stackmark

;call system_halt_system

	.load:
	popad			;Now (almost) all registers are restored

	.skip:
	call PIC_EOI

	;The next instruction returns to the stack-saved CS:EIP position with EFLAGS
	iret

	RESULT dd 0x0

ISR_21h:		;Hardware Interrupt [Ring 0] (Keyboard)

	;This interrupt routine handles the keyboard
	;First check if a pressed key caused the interrupt

	pushad
	mov dx, 0x64
	in al, dx
	and al, 0x01
	jz .end

	;Get the keyboard scan code on port 0x60
	mov dx, 0x60
	in al, dx

	;Convert the keyboard scan code to an ascii character
	push eax
	call keyboard_code
	add esp, 4

	;Unusable/Unsupported key
	cmp al, 0
	je .end

	;Pass the character to the active program
	push eax
	call system_pass_character
	pop eax
;
;DEBUG
;Print everything about the current process

cmp dword [__DEBUG_LEVEL__], 5		;Debug level 5: All !
jnz .DEBUG_END

push dword [__process__]
call tm_print_hex

push dword [esp]
call tm_print_hex

push esp
call tm_print_hex

add esp, 16

.DEBUG_END:
;
;
	;Return from interrupt routine
	.end:
	call PIC_EOI
	popad
	iret

;
;ISR_22h not implemented (!) because:
;There is no INT 22h since we connected IRQ 2 of the master PIC to the slave PIC 
;

ISR_23h:		;Hardware Interrupt [Ring 0] (COM2) Serial Port 2 & 4
     push eax
     push 0x4F
     push 'A' 
     call tm_print_char
     pop eax 
     pop eax
     pop eax

	call PIC_EOI
	iret

ISR_24h:		;Hardware Interupt [Ring 0] (COM1) Serial Port 1 & 3
	push eax
     push 0x4F
     push 'B' 
     call tm_print_char
     pop eax 
     pop eax
     pop eax
	call PIC_EOI
	iret		
	
ISR_25h:		;Hardware Interrupt [Ring 0] (Sound Card)
	push eax
     push 0x4F
     push 'C'
     call tm_print_char
     pop eax
     pop eax
     pop eax

	call PIC_EOI
	iret

ISR_26h:		;Hardware Interrupt [Ring 0] (Floppy Disk Controller)
	push eax
     push 0x4F
     push 'D'
     call tm_print_char
     pop eax
     pop eax
     pop eax


	call PIC_EOI
	iret

ISR_27h:		;Hardware Interrupt [Ring 0] (Parallel Port) (Mostly Fake IRQ)
	push eax
     push 0x4F
     push 'E'
     call tm_print_char
     pop eax
     pop eax
     pop eax

	call PIC_EOI
	iret

ISR_28h:		;Hardware Interrupt [Ring 0] (Real Time Clock)
	push eax
     push 0x4F
     push 'F'
     call tm_print_char
     pop eax
     pop eax
     pop eax

	call PIC_EOI
	iret

ISR_29h:		;Hardware Interrupt [Ring 0] (Open Interrupt)
	push eax
     push 0x4F
     push 'G'
     call tm_print_char
     pop eax
     pop eax
     pop eax

     call PIC_EOI
     iret


	call	PIC_EOI
	iret

ISR_2Ah:		;Hardware Interrupt [Ring 0] (Open Interrupt)
	push eax
     push 0x4F
     push 'H'
     call tm_print_char
     pop eax
     pop eax
     pop eax


	call PIC_EOI
	iret

ISR_2Bh:		;Hardware Interrupt [Ring 0] (Open Interrupt)
	push eax
     push 0x4F
     push 'I'
     call tm_print_char
     pop eax
     pop eax
     pop eax

	call PIC_EOI
	iret

ISR_2Ch:       ;Hardware Interrupt [Ring 0] (PS/2 Mouse)
	push eax
     push 0x4F
     push 'J'
     call tm_print_char
     pop eax
     pop eax
     pop eax

     call PIC_EOI
     iret

ISR_2Dh:       ;Hardware Interrupt [Ring 0] (Floating Point Unit / Coprocessor)
	call PIC_EOI
	iret

ISR_2Eh:       ;Hardware Interrupt [Ring 0] (Primary ATA Hard Disk)
	push eax
     push 0x4F
     push 'K'
     call tm_print_char
     pop eax
     pop eax
     pop eax

	call PIC_EOI
	iret

ISR_2Fh:       ;Hardware Interrupt [Ring 0] (Secondary ATA Hard Disk)
	push eax
     push 0x4F
     push 'L'
     call tm_print_char
     pop eax
     pop eax
     pop eax

	call PIC_EOI
	iret

ISR_30h:		;Software Interrupt [Ring 3] (Kernel Call)
	call PIC_EOI
	iret

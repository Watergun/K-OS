;K-OS Interrupt Service Routines INT 0 - 48

;Here, all the interrupt routines are defined
;Every interrupt routine (IR) has to send the End Of Interrupt signal 
;to the PIC before returning with the 'iret' command 

[extern PIC_EOI]
[extern keyboard_code]
[extern pass_character]
[extern tm_print_char]
[extern tm_print_hex]
[extern ph_switch_process]
[extern memory_view_func]
[extern __process__]

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
;	mov ebx, 0x08
;	div ebx
;	pop ebx
;	cmp edx, 1
;
;	jz .continue
;
;	pop edx
;	pop eax
;	jmp .skip
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.continue:
	pop edx
	pop eax

	;Save Registers on the stack
	pushad 	;Now we've got all the registers on the stack (EDI,ESI,EBP,ESP,EBX,EDX,ECX,EAX,EIP,CS,EFLAGS)
			;	offsets:							  0	 4	8   12  16  20  24  28  32  36 40

	;Move them to their PSS
	mov eax, [__process__]

	cmp eax, 0			;Happens, when the kernel is called for the first time
	jz .switch

	add eax, 13 * 4		;Get to the PSS address (e.g. pss = process[13])
	mov eax, dword [eax]
	mov ebx, dword [esp+32]
	mov [eax+4], ebx		;EIP
	mov ebx, dword [esp+28]
	mov [eax+8], ebx		;EAX
	mov ebx, dword [esp+24]
	mov [eax+12], ebx		;ECX
	mov ebx, dword [esp+20]
     mov [eax+16], ebx        ;EDX
 	mov ebx, dword [esp+16]
     mov [eax+20], ebx        ;EBX
 	mov ebx, dword [esp+12]
     mov [eax+24], ebx        ;ESP
 	mov ebx, dword [esp+8]
     mov [eax+28], ebx        ;EBP
 	mov ebx, dword [esp+4]
     mov [eax+32], ebx        ;ESI
 	mov ebx, dword [esp]
     mov [eax+36], ebx        ;EDI
 	mov ebx, dword [esp+40]
     mov [eax+40], ebx        ;EFLAGS

.switch:	;Activate the new process

	call ph_switch_process	;Returns the pss of the new process
	cmp eax, 0
	jz .end

	add eax, 40			;Get to the EFLAGS
	add esp, 44			;Now ESP points right behind EFLAGS
	push dword [eax]		;EFLAGS
	push dword 0x08		;CS
	sub eax, 36
	push dword [eax]		;EIP
	add eax, 4
	push dword [eax]		;EAX
	add eax, 4
	push dword [eax]		;ECX
	add eax, 4
	push dword [eax]		;EDX
	add eax, 4
	push dword [eax]		;EBX
	add eax, 4
	push dword [eax]		;ESP
	add eax, 4
	push dword [eax]		;EBP
	add eax, 4
	push dword [eax]		;ESI
	add eax, 4
	push dword [eax]		;EDI


.end:
	popad			;Now all registers are restored
.skip:
	call PIC_EOI

	;The next instruction returns to the stack-saved CS:EIP position with EFLAGS
	iret

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
	pop ebx

	;Unusable/Unsupported key
	cmp al, 0
	je .end

	;Pass the character to the active program
	push eax
	call pass_character
	pop eax

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

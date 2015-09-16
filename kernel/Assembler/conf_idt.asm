;load_idt: Configures and loads the Interrupt Descriptor List,
;conf_idt: Moves the Interrupt Routine labels (ISR_Xh) to the corresponding Gates (IDT Entries)

global load_IDT

[extern IDT_START]
[extern IDT_END]
[extern ISR_20h]
[extern ISR_21h]
[extern ISR_23h]
[extern ISR_24h]
[extern ISR_25h]
[extern ISR_26h]
[extern ISR_27h]
[extern ISR_28h]
[extern ISR_29h]
[extern ISR_2Ah]
[extern ISR_2Bh]
[extern ISR_29h]
[extern ISR_2Ah]
[extern ISR_2Bh]
[extern ISR_2Ch]
[extern ISR_2Dh]
[extern ISR_2Eh]
[extern ISR_2Fh]
[extern ISR_30h]
[extern TM_print_memory]

idt_descriptor:
	dw 48*8-1
	dd IDT_START

load_IDT:
	call conf_idt
	lidt [idt_descriptor]
	ret

conf_idt:
	pusha
	mov eax, ISR_20h
	mov ebx, 0x20
	call store_routine
	
	mov eax, ISR_21h
	mov ebx, 0x21
	call store_routine

	mov eax, ISR_23h
	mov ebx, 0x23
	call store_routine

	mov eax, ISR_24h
     mov ebx, 0x24
     call store_routine

	mov eax, ISR_25h
     mov ebx, 0x25
     call store_routine	

	mov eax, ISR_26h
     mov ebx, 0x26
     call store_routine

	mov eax, ISR_27h
     mov ebx, 0x27
     call store_routine

	mov eax, ISR_28h
     mov ebx, 0x28
     call store_routine

	mov eax, ISR_29h
     mov ebx, 0x29
     call store_routine

	mov eax, ISR_2Ah
     mov ebx, 0x2A
     call store_routine

	mov eax, ISR_2Bh
     mov ebx, 0x2B
     call store_routine

	mov eax, ISR_2Ch
     mov ebx, 0x2C
     call store_routine

	mov eax, ISR_2Dh
     mov ebx, 0x2D
     call store_routine

	mov eax, ISR_2Eh
     mov ebx, 0x2E
     call store_routine

	mov eax, ISR_2Fh
     mov ebx, 0x2F
     call store_routine

	mov eax, ISR_30h
	mov ebx, 0x30
	call store_routine

;DEBUG
;	push 8
;	lea eax, [IDT_START+0x21*8]
;	push eax
;	call TM_print_memory
;	pop eax
;	pop eax
;

	popa
	ret	

store_routine:				;ISR is stored in EAX, INT Nr. is stored in BL
	imul bx, 0x0008
	add ebx, IDT_START
	mov word [ebx], ax
	shr eax, 16
	mov word [ebx+6], ax
	ret

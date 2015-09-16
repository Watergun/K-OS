; system.asm: System tools and procedures

[global system_use_system_ressources]
[global system_use_process_ressources]
[global system_halt_system]

[extern __SYSTEM_PROCESS__]
[extern global_critical_section]
[extern __process__]
[extern tm_print_hex]
[extern memory_view_func]

;
;system_use_system_ressources: Used by system functions, that get called by other processes
;						So that they temporarily use the stack and memory of the system


PROCESS_ADDRESS	dd 0x0
PROCESS_STACKMARK 	dd 0x0

;Good to know: can only be used when Multitasking is switched off!
;Reason: too chaotic, if else
system_use_system_ressources:
	;When there is no system process		(Critical matter)
	cmp dword [__SYSTEM_PROCESS__], 0
	je .failure

	;When the system process is already active (Doesn't matter)
	;mov eax, [__SYSTEM_PROCESS__]
	;cmp eax, [__process__]
	;je

	;When there is no current process		(Doesn't matter)
	;cmp dword [__process__], 0

	;When the last system ressouce use wasn't finished  (Critical matter)
	cmp dword [PROCESS_ADDRESS], 0
	jne .failure

	;When multitasking is active (must be deactivated) (Critical matter)
	cmp dword [global_critical_section], 0
	je .failure


	;Everything okay
	mov eax, [__process__]
	mov [PROCESS_ADDRESS], eax
	mov [PROCESS_STACKMARK], esp

	;Ready to switch ressources
	mov eax, [__SYSTEM_PROCESS__]
	mov [__process__], eax
	add eax, 4*14			;stackaddress
	mov ebx, [eax]
	add ebx, 4*14			;stackmark
	mov eax, [ebx]
	mov esp, eax
;	mov ebp, eax

	;Copy return address
	mov eax, [PROCESS_STACKMARK]
	push dword [eax]

;DEBUG
;call tm_print_hex
;jmp system_halt_system

	jmp .end

	.failure:
;DEBUG
push dword 0xAFAF
call tm_print_hex
add esp, 4
jmp .end
;

	.end:
	ret


;The reverse function of above: returns memory and stack to the old values
system_use_process_ressources:
	;When there was no process before / got randomly called without the first use --> ending
	cmp dword [PROCESS_ADDRESS], 0
	je .failure

	cmp dword [PROCESS_STACKMARK], 0
	je .failure

	;When multiasking is suddenly active -> fatal -> deactivate multitasking + system halt
	cmp dword [global_critical_section], 0
	je .mt_active

	;Everything okay
;	mov eax, [PROCESS_ADDRESS]
;	mov [__process__], eax

	;Copy return address
	mov eax, [esp]
	mov esp, [PROCESS_STACKMARK]
	add esp, 4
	push eax

	mov dword [PROCESS_ADDRESS], 0
	mov dword [PROCESS_STACKMARK], 0

	jmp .end

	.failure:
;DEBUG
push dword 0xFAFA
call tm_print_hex
add esp, 4
jmp .end
;
	.mt_active:
	mov dword [global_critical_section], 1
	jmp system_halt_system

	.end:
	ret


;Halting function (for fatal errors or aimed debugging)
system_halt_system:
	;If not already done, disable interrupts
	;cli [STILL ALLOW: Y/N?]

	;If not already done, disable MT
	mov dword [global_critical_section], 1

	;Print HALT hex code
	push dword 0x80808080

	call tm_print_hex

	;Do nothing
	.end_loop:
	jmp .end_loop


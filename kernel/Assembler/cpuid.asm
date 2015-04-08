;Contains the CPU ID check

global cpuid

[extern tm_print_dword]

;To check cpuid support, a processor flag will be changed
;if it could be changed, the processor does support the cpuid instruction

cpuid_instr_support:
	;If the cpuid 
	pushfd
	pushfd
	xor dword [esp], 0x00200000
	popfd
	pushfd
	pop eax
	xor eax, [esp]
	popfd
	and eax, 0x00200000
	ret

cpuid:
	pusha
	call cpuid_instr_support
	cmp eax, 0
	je .end
	;the cpuid instruction is supported by the cpu

	;CPU Vendor ID
	mov eax, 0x0	
	cpuid
	push ebx
	call tm_print_dword
	pop ebx
	push ecx
	call tm_print_dword
	pop ecx
	push edx
	call tm_print_dword
	pop edx

.end:
	popa
	ret

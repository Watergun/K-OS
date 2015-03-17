global datatest

data dq 0x00008e0000080001
	
[extern TM_print_memory]

datatest:
	pusha
	;mov ax, 0x0001
	;mov [data], ax
	;mov word [data+2], 0x0008
	;mov word [data+4], 0x8e00
	;mov word [data+6], 0x0000
	mov eax, 0x8
	push eax
	mov eax, data
	push eax
	call TM_print_memory
	pop eax
	pop eax
	popa
	ret
	

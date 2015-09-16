;prototype memory manager that uses the free space in the lower Mbyte (see memory_map.txt)
;to reserve memory

;It splits the memory in units with 4 kB each

;0x500 - 0x1000 [HALF UNIT], used for system variables (fixed)
;0x500: time (16 byte)

[global system_reserve_unit]
[global space_pointer]

SPACE_POINTER	dd 0x00010000			;begins after the OS

;TODO: Write a good memory manager, not a simple incrementing function like this

system_reserve_unit:
	mov eax, dword [SPACE_POINTER]	;load space pointer
	add eax, 0x1000				;add unitsize to pointer
	cmp eax, 0x00080000				;watch out for Stack
	jl .end
	mov eax, 0x00100000				;1 MB Line
.end:
	mov dword [SPACE_POINTER], eax	;save new space pointer
	ret

space_pointer:						;just returns the space pointer
	mov eax, dword [SPACE_POINTER]
	ret


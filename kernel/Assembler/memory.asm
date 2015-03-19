;First memory manager that uses the free space in the lower Mbyte (see memory_map.txt)
;to reserve memory

;0x500 - 0x7bff
;

[global reserve_dword]
[global reserve_memory]

SPACE_POINTER	dd 0x00000500

reserve_dword:
.stage1:
	mov eax, dword [SPACE_POINTER]
	mov ebx, eax
	add ebx, 0x04
	mov dword [SPACE_POINTER], ebx
	cmp dword [SPACE_POINTER], 0x00007bff
	jl .end
	cmp dword [SPACE_POINTER], 0x00007e00
	jl .stage2
	cmp dword [SPACE_POINTER], 0x0007ffff
	jl .end
	cmp dword [SPACE_POINTER], 0x00100000
	jl .stage3
.end:
	ret

.stage2:
	mov dword [SPACE_POINTER], 0x00007c00
	jmp .end

.stage3:
	mov dword [SPACE_POINTER], 0x00100000
	jmp .end


reserve_memory:

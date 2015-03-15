[bits 32]
;Constants
VIDEO_MEMORY equ 0xb8000
WB_FORMAT	equ 0xf0

;prints a null-terminated string pointed to by EBX
print_string_32bit:
pusha					;Push all registers on the stack
mov edx, VIDEO_MEMORY		;Set edx to the start of video memory

.loop:
mov al, [ebx]				;Store the char at EBX in AL
mov ah, WB_FORMAT			;Store the attributes in AH

cmp al, 0					;If 0 encountered, the end of string is reached
je done

mov [edx], ax				;Store char and attributes at current character cell
add ebx, 1				;Next Character in string
add edx, 2				;Next Character Cell (2Bytes) in video memory

jmp .loop					;loop

done:
popa						;Restore all registers
ret

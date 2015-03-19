;Contains the current keyboard status

global keyboard_get_status
global keyboard_xor_status

;MODIFIER_STATUS:
;bit 0: STRG
;bit 1: SHIFT
;bit 2: CAPS LOCK
;bit 3: ALT
;bit 4: ALT GR

MODIFIER_STATUS db 0

keyboard_get_status:
	mov eax, [MODIFIER_STATUS]
	ret

keyboard_xor_status:		;Sets or unsets modifier status flags using the XOR operation
	push ebx
	mov eax, [esp+8]
	mov bl, byte [MODIFIER_STATUS]
	xor al, bl
	mov byte [MODIFIER_STATUS], al
	
	pop ebx
	ret

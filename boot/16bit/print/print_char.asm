;Prints the value that is stored in BL

print_char:
	push ax		;Save AX to stack
	mov ah, 0x0e	;Tele-type mode
	mov al, bl

	int 0x10

	pop ax		;Restore AX from stack
	ret

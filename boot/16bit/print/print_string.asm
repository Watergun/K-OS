;A function that simply prints a string, whose address is stored in BX

print_string:
	push ax		;Save AX to stack
	mov ah, 0x0e	;BIOS teletype mode
	push cx		;Save CX to stack, because we need it during procedure
.loop:
	mov al, [bx]	;take a character from the address that is stored in BX
	int 0x10	;BIOS routine
	add bx, 1	;iterate the string address
	mov cl, [bx]	;see if the next character is a null
	cmp cl, 0
	je .end	;then exit the loop
	jmp .loop	;else loop again
.end:
	pop cx		;Restore CX
	pop ax		;Restore AX
	ret

;Prints the value stored in DX as a hexadecimal value

print_hex:
	push bx			;Save BX because we need it during procedure
	push cx 		;Save CX because we need it during procedure

shift1:				;The 16-bit hexadecimal value is splitted into 4-bit pieces, converted and printed
				;using the print_char function
	mov cx, dx
	and cx, 0xf000
	shr cx, 12
	call convert
	mov bl, cl
	call print_char

	mov cx, dx
	and cx, 0x0f00
	shr cx, 8
	call convert
	mov bl, cl
	call print_char

	mov cx, dx
	and cx, 0x00f0
	shr cx, 4
	call convert
	mov bl, cl
	call print_char

	mov cx, dx
	and cx, 0x000f
	call convert
	mov bl, cl
	call print_char

	pop cx			;Restore CX
	pop bx			;Restore BX
	ret

;Converts a hex value to its corresponding ASCII_letter
convert:
	cmp cl, 0x09		;If it's greater than 0x9 (i.e. A,B,C,D,E or F)
	jle number
	sub cl, 0x0a		;Conversion	for A,B,C,D,E,F
	add cl, 0x41
	jmp c_end
number:
	add cl, 0x30		;Conversion	for 0,1,2,3,4,5,6,7,8,9
c_end:
	ret

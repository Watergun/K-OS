;this is just a test file
global test

test:
	pusha
	mov ebx, 0xb8000
	mov al, 'F'
	mov ah, 0xf0
	mov [ebx], al
	mov [ebx+1], ah
	popa
	ret 

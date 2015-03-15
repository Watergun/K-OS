;Ensures that we jump straight into the kernels entry function
[bits 32]		;We're in protected mode now, so use 32bit instructions
[extern main]	;Declare that we will be referencing the external symbol
			;'main', so the linker can substitute the final address
call main		;invoke main() in our C kernel

jmp $


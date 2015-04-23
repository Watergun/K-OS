;load DH sectors to EX:BX from drive DL, (Assume Partition Table is at 0x7be)
disk_load:
push dx		;store DX on stack so later we can recall how many sectory were
			;requested to be read

call print_hex	;DEBUG reasons	

mov ah, 0x02	;BIOS read sector function
mov al, dh	;Read DH Sectors

;mov dh, byte [0x7bf] ;Head
mov dh, 0x0
;mov ch, byte [0x7c1] ;Cylinder
mov ch, 0x0
;mov cl, byte [0x7c0] ;Sector 
mov cl, 0x02
;inc cl
;and cl, 0x3f	;Just 6 bits are for sectors

int 0x13		;BIOS interrupt

jc disk_error	;Jump if error (carry flag set)
			;this will likely happen in emulation because of small "harddisk" size 

.continue:	
pop dx		;Restore DX from the stack
cmp dh, al	;if AL (sectors read) != DH (sectors expected)
jne disk_warn	;Just warn the user (Will happen often in emulation)
ret

disk_warn:	;A Warning won't stop the program
mov bx, DISK_WARNING
call print_string
ret

disk_error:	;Not enough sectors read to memory
mov dh, ah
mov dl, al
call print_hex

cmp al, 0x00	;Register AL contains how many sectors really could be read
jne disk_load.continue	;If AL != 0 continue

mov bx, DISK_ERROR_MSG	;Otherwise 0 sectors read
call print_string
jmp $				;Hang up


;Variables
DISK_WARNING db "Could not read enough sectors ", 0
DISK_ERROR_MSG db "Disk reading error! ", 0


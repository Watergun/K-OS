# $< is the first dependency and $@ is the target file
# $^ is substituted with all of the target's dependency files

KERNEL_ENTRY = kernel/Assembler/kernel_entry.asm

C_SOURCES = $(wildcard kernel/C/*.c drivers/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h clibrary/*.h)
ASM_SOURCES = $(filter-out $(KERNEL_ENTRY), $(wildcard kernel/Assembler/*.asm)) 

OBJ = $(patsubst %.c, %.o, $(C_SOURCES)) $(patsubst %.asm, %.o, $(ASM_SOURCES))

#Default make target
all: os-image

#Run qemu
run: all
	qemu-system-x86_64 os-image -no-reboot

#This is the actual disk image that the computer loads,
#which is the combination of our compiled bootsector and kernel
os-image: boot/boot_sect.bin kernel.bin
	cat $^ > os-image


#Generic compiling
%.o: %.c ${HEADERS}
	gcc -ffreestanding -c $< -o $@

#Putting everything together
kernel.bin: $(patsubst %.asm, %.o, $(KERNEL_ENTRY)) $(OBJ)
	ld $^ -o $@ --oformat binary -Ttext 0x1000

#Assemble kernel
%.o: %.asm
	nasm $< -f elf -o $@

#Assemble boot code
%.bin: %.asm
	nasm $< -f bin -I 'boot/16bit/' -I 'boot/' -o $@

clean:
	rm -fr *.o *.bin os-image
	rm -fr kernel/C/*.o kernel/Assembler/*.o boot/*.bin drivers/*.o drivers/Assembler/*.o

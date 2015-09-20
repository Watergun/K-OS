# $< is the first dependency and $@ is the target file
# $^ is substituted with all of the target's dependency files


GCC_FLAGS = -m32 -Wall -pedantic-errors -Wpedantic -Wextra


KERNEL_ENTRY = kernel/Assembler/kernel_entry.asm

C_SOURCES = $(wildcard kernel/C/*.c drivers/*.c prog/*.c util/*.c lib/*.c)
HEADERS = $(wildcard kernel/C/*.h drivers/*.h prog/*.h util/*.h lib/*.h)
ASM_SOURCES = $(filter-out $(KERNEL_ENTRY), $(wildcard kernel/Assembler/*.asm drivers/*.asm))

OBJ = $(patsubst %.c, %.o, $(C_SOURCES)) $(patsubst %.asm, %.o, $(ASM_SOURCES))

#Default make target
all: os-image

dd: all
	dd if=os-image of=/dev/sdb1

#Run qemu
run: all
	qemu-system-i386 os-image -no-reboot

#This is the actual disk image that the computer loads,
#which is the combination of our compiled bootsector and kernel
os-image: boot/boot_sect.bin kernel.bin
	cat $^ > os-image

#Generic compiling
%.o: %.c ${HEADERS}
	gcc -ffreestanding $< -c -I 'drivers/' -I 'kernel/C/' -I 'prog/' -I 'lib/' -o $@ $(GCC_FLAGS)

#Putting everything together
kernel.bin: $(patsubst %.asm, %.o, $(KERNEL_ENTRY)) $(OBJ)
	ld -m elf_i386 $^ -o $@ --oformat binary -Ttext 0x1000

#Assemble kernel
%.o: %.asm
	nasm $< -f elf -o $@

#Assemble boot code
%.bin: %.asm
	nasm $< -f bin -I 'boot/16bit/' -I 'boot/' -o $@

clean:
	rm -fr *.o *.bin os-image
	rm -fr kernel/C/*.o kernel/Assembler/*.o boot/*.bin drivers/*.o prog/*.o lib/*.o 

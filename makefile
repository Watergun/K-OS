# $< is the first dependency and $@ is the target file
# $^ is substituted with all of the target's dependency files

#Automatically exand to a list of existing files that match the patterns
C_SOURCES = $(wildcard kernel/*.c drivers/*.c clibrary/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h clibrary/*.h)

#Create a list of object files to build, simple by replacing the '.c' extension in C_SOURCES with '.o'
OBJ = ${C_SOURCES:.c=.o}

#Default make target
all: os-image

#Run qemu to simulate booting of our code
run: all
	qemu-system-x86_64 os-image -no-reboot

#This is the actual disk image that the computer loads,
#which is the combination of our compiled bootsector ans kernel
os-image: boot/boot_sect.bin kernel.bin
	cat $^ > os-image

#Generic rule for building 'somefile.o' from 'somefile.c'
%.o: %.c ${HEADERS}
	gcc -ffreestanding -c $< -o $@

#Builds the binary of our kernel from the object files
kernel.bin: kernel/kernel_entry.o ${OBJ}
	ld $^ -o $@ --oformat binary -Ttext 0x1000

#Assemble kernel_entry
%.o: %.asm
	nasm $< -f elf -o $@

#Assemble the boot sector to raw machine code
%.bin: %.asm
	nasm $< -f bin -I 'boot/16bit/' -I 'boot/' -o $@

clean:
	rm -fr *.o *.bin os-image
	rm -fr kernel/*.o boot/*.bin drivers/*.o

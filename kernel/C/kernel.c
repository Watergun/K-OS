#define TIME 0x0500
#include "kernelprograms.h"
#include "programs.h"
#include "globalstatus.h"
#include "memory.h"
#include "multitask.h"
#include "screen.h"

//Prototpe of the kernel process
void kernel_main(int, char**);

//The main entry of the kernel
void main()
{
	//Set Timer to 0
	int *Timer = (int*)TIME;
	*Timer = 0;

	//Initialise textmode
	tm_clear_screen();
	global_videomode = VID_TEXT;

	//Print welcome message
	tm_set_cursor(0);
	tm_print_at("Welcome to K-OS!", 30, 1);

	//Remap Interrupt numbers (mandatory) [PIC = Programmable Interrupt Controller]
  	PIC_remap();

	//Load and configure the Interrupt Descriptor Table (IDT)
	load_IDT();

	//Enable Interrupts
	__asm__("sti");

	//Memory and process manager
	memory_manager_init();

	//Self-representation as kernel process
	int *kernel_process = (int*) reserve_unit();
	kernel_process[0] = 0;
	kernel_process[1] = UNITSIZE;
	kernel_process[2] = 0;
	kernel_process[3] = 1;		//The kernel has ID 1
	name_process(kernel_process, "KERNEL");
	kernel_process[11] = (int) kernel_main;
	kernel_process[14] = 0x90000;		//Reset the kernel's stack

	start_process(kernel_process, 0, 0);

	//Waiting for the ISR to call our kernel
	while(1);
}

//Kernel loop
void kernel_main(int args, char **argv)
{
	//Arguments are zero, but are an obligation

	//Start Terminal
	start_process(new_process(terminal, 0, 4032), 0, 0);

	int c = tm_get_cursor();
	tm_print_at("RUNNING:", 0, 0);
	tm_set_cursor(c);

	//kernel_status_update();
  	while(1)
	{
		((char*)VIDEO_ADDRESS)[18] = 'K';
		((char*)VIDEO_ADDRESS)[19] = 0x05;
	}
}

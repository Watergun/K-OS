#define TIME 0x0500
//#define DEBUG_LEVEL_ALL

#include "system.h"
#include "process.h"
#include "programs.h"
#include "globalstatus.h"
#include "memory.h"
#include "multitask.h"
#include "screen.h"
#include "kernel.h"
#include "terminal.h"

//Prototpe of the kernel process
void kernel_main(int, char**);
extern void PIC_remap();
extern void load_IDT();

//The main entry of the kernel
void main()
{
	//Set Timer to 0
	int *Timer = (int*)TIME;
	*Timer = 0;

	global_bootstatus = 1;

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
	system_process_manager_init();

	//Self-representation as kernel process
	int *kernel_process = (int*) system_reserve_unit();
	kernel_process[0] = 0;
	kernel_process[1] = UNITSIZE;
	kernel_process[2] = 0;
	kernel_process[3] = 1;		//The kernel has ID 1
	system_name_process((int)kernel_process, "KERNEL");
	kernel_process[11] = (int)kernel_main;
	kernel_process[14] = 0x90000;		//Reset the kernel's stack

	//POSSIBLE ERROR: NO APT ! TODO !


	//For simplicity, 0x90000 is a stack block
	*((int*)(0x90000+4*14)) = 0x90000;

	//Set current process
	__process__ = (int)kernel_process;
	__SYSTEM_PROCESS__ = (int)kernel_process;

	system_start_process((int)kernel_process, 0, 0);

	//
	global_bootstatus = 2;

//int addr = ((int*)kernel_process[14])[14];
//tm_print_hex(addr);
//memory_view_func(((int*)kernel_process[14])[14], 0x40);
//tm_print_char('K', TM_COLORS(TM_BLUE, TM_BLACK));
//tm_print_hex(kernel_main);
//tm_print_hex(&addr);

	//Waiting for the ISR to call our kernel process
	while(1);
}

//Kernel loop
void kernel_main(int args, char **argv)
{
	//Arguments are zero, but are an obligation

	__SYSTEM_PROCESS__ = __process__;
	global_bootstatus = 3;

	//Start Terminal
	system_start_process(system_new_process((int)terminal, 0, 4032), 0, 0);

	int c = tm_get_cursor();
	tm_print_at("RUNNING:", 0, 0);
	tm_set_cursor(c);

	//kernel_status_update();
  	while(1)
	{
		//The kernel has no inbox. Better: events, to execute s specific function with an argument
		kernel_start_event();

		//Indicate a running kernel in text-mode
		((char*)VIDEO_ADDRESS)[18] = 'K';
		((char*)VIDEO_ADDRESS)[19] = 0x05;
	}
}

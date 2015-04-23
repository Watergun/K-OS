#define TIME 0x0500
#include "kernelprograms.h"
#include "programs.h"

//The main entry for the kernel
void main()
{	
	//Set Timer to 0
	int *Timer = (int*)TIME;	
	*Timer = 0;
	
	//Initialise Video-output
	char *video_memory = (char*) 0xb8000;
	
	tm_clear_screen();

	//Print welcome message
	tm_set_cursor(0);
	tm_print_at("Welcome to K-OS!", 30, 1);	

	//Remap Interrupt numbers (mandatory) [PIC = Programmable Interrupt Controller]
  	PIC_remap();

	//Load and configure the Interrupt Descriptor Table (IDT)
	load_IDT();

	//Enable Interrupts	
	__asm__("sti");	

	//Start Terminal
	start_process(new_process(terminal, 0, 4032), 0, 0);
//DEBUG
//int ptr = new_process(terminal, 0, 4032);
//tm_print_hex(*((int*)ptr));
//tm_print_hex(*((int*)ptr+4));
//tm_print_hex(*((int*)ptr+8));
//tm_print_hex(*((int*)ptr+12));	


	//VGA Mode
	vga_init();

	//End in the kernel loop
	while(1);
}

//The main entry for the kernel
void main()
{
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

	terminal_set_statement();

	cpuid();

	//End in the kernel loop
	while(1);
}

//The main entry for the kernel
void main()
{
	//Initialise Video-output
	char *video_memory = (char*) 0xb8000;
	
	clear_screen();
	
	//Print welcome message
	char str[] = "Welcome to K-OS!";
	print_at(str, 30, 10);
	set_cursor(0);

	//Remap Interrupt numbers (mandatory)
	PIC_remap();

	//Load and configure the Interrupt Descriptor Table
	load_IDT();

	//Enable Interrupts
	__asm__("sti");	
	
	//datatest();

	//End in the kernel loop
	while(1);
}

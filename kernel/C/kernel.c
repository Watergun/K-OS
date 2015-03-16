extern void test();

//The main entry for the kernel
void main()
{
	//Initialise Video-output
	char *video_memory = (char*) 0xb8000;

	////Color test
	
	clear_screen();
	
	//screen_test();

	//Print welcome message
	char str[] = "Welcome to K-OS!";
	print_at(str, 30, 10);
	set_cursor(0);

	//End in the kernel loop
	while(1);
}

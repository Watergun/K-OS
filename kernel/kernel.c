//The main entry for the kernel
void main()
{
	//Initialise Video-output
	char *video_memory = (char*) 0xb8000;

	////Color test
	//screen_test;
	
	clear_screen();
	
	//Print welcome message
	char str[] = "Welcome to K-OS!";
	print(str);

	//End in the kernel loop
	while(1);
}

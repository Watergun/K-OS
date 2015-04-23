//PROG "hostname"
int hostname(int argc, char **argv)
{
	//This programm requires no argument, so ignore argv
	terminal_print_string("ARCHNET");
	
	return 0;	//No Error
}

int osname(int argc, char **argv)
{
	//No arguments needed
	terminal_print_string("K - Operating System [K-OS]");

	return 0;
}

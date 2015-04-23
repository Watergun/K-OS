//time programs

int time(int argc, char **argv)
{
	int *timeptr = (int*) 0x500;

	char *buffer = (char*) malloc(4);
	itoa(*timeptr, buffer);

	terminal_print_string(buffer);

	//free(buffer);

	return 0;
}

void memory_copy(char*,char*,int);
void itoa(int,char*);
char digit_to_char(short);

/*Copy bytes from one place to another		*/
void memory_copy(char *source, char *destination, int size)
{
	unsigned int i;
	for(i = 0; i < size; i++)
		destination[i] = source[i];
}

void itoa(int number, char* buffer)
{	
	short i = 0;
	int number_cpy = number;
	while(number_cpy > 0)
	{
		number_cpy /= 10;	
		i++;
	}

	buffer[i] = 0;
	
	for(;i > 0; i--)
	{
		buffer[i-1] = digit_to_char(number % 10);
		number /= 10;
	}
}

char digit_to_char(short digit)
{
	return (char) (digit + 0x30);
}


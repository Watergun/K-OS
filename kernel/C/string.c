/*
Standard string working functions
*/

#include "stdheader.h"

//function list
size_t strlen(char*);
int strcmp(char*, char*, int);
size_t charcount(char*, char);
size_t wordcount(char*);
int atoi(char*);
short char_to_digit(char);


//Returns the length of a null-terminated string
size_t strlen(char *str)
{	
	size_t index = 0;
	while(str[index] != 0)
		index++;

	return index;
}

//Compares two strings (null-terminated) and returns if smaller (-1), equal (0) or greater (1)
int strcmp(char *str1, char *str2, int max_length)
{
     //char by char comparison

     int index = 0;
     for(; index < max_length; index++)
     {
          if(str1[index] < str2[index] || (str1[index] == 0 && str2[index] != 0))         //a < b
               return -1;
          else if(str1[index] > str2[index] || (str1[index] != 0 && str2[index] == 0))    //c > b
               return 1;
          //else str1[index] == str2[index]  //b = b
     }

     //at this point, both strings are equal
     return 0;
}

//Counts the number of occurences of a given char in a string
size_t charcount(char *str, char c)
{
	//counter variable
	size_t counter = 0;

	//index
	int i = 0;
	int strlength = strlen(str);
	for(; i < strlength; i++)
	{
		if(str[i] == c)
			counter++;
	}
	
	return counter;
}

//Returns the number of space-seperated words in a string
size_t word_count(char *str)
{
	int ch	 	= 0;
	int i 		= 0;
	size_t len 	= strlen(str);
	size_t num 	= 0;
	
	for(;i < len; i++)
	{
		if(str[i] == ' ')
		{
			ch = 0;
		}
		else
		{
			if(!ch)
			{	
				num++;
				ch = 1;
			}
		}
	}
	
	return num;
}

//String to int conversion
int atoi(char *str)
{
	//TODO: make complete

	//only positive numbers yet
	size_t len = strlen(str);
	size_t i = 0;
	int a = 0;

	for(i;i < len; i++)
	{
		int d = (int) char_to_digit(str[i]);
		int j = 0;
		for(j;j<(len-i-1);j++)	
			d *= 10;
		a += d;
	}	
	
	return a;
}

//Converts a char to a digit

short char_to_digit(char c)
{
	switch(c)
	{
		case '0':
			return 0;
		case '1':
			return 1;
		case '2':
			return 2;
		case '3':
			return 3;
		case '4':
			return 4;
		case '5':
			return 5;
		case '6':
			return 6;
		case '7':
			return 7;
		case '8':
			return 8;
		case '9':
			return 9;
		default:
			return 0;
	}

}

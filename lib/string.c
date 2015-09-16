/*
Standard string working functions
*/

#include "stdheader.h"
#include "string.h"

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
	//length comparison
	size_t str1_len = strlen(str1);
	size_t str2_len = strlen(str2);

	if(str1_len < str2_len)
		return -1;
	else if(str1_len > str2_len)
		return 1;

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
int strcount_char(char *str, char c)
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
int strcount_words(char *str)
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
	//TODO: complete function
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

//Converts number character to its digit
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

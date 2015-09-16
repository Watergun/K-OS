#include "stdheader.h"
#include "desktop.h"
#include "fonts.h"
#include "allocation.h"
#include "util.h"
#include "string.h"

int fonts_get_font_data(int font_id, char ch, char* letter)
{
	int ret = 0;

	//System default
	switch(font_id)
	{	
		case 0:
			switch(ch)
			{	
			//Numbers
				case '0':
					break;
				case '1':
					break;
				case '2':
					break;
				case '3':
					break;
				case '4':
					break;
				case '5':
					break;
				case '6':
					break;
				case '7':
					break;
				case '8':
					break;
				case '9':
					break;

			//Letters
				case 'A':
					fonts_build_font(letter, 0x00, 0xC4, 0x92, 0x79, 0x24, 0x92, 0x00, 0x00, 0x00); 
					break;
				case 'a':
					fonts_build_font(letter, 0x00, 0x00, 0x0C, 0x08, 0xE5, 0x8E, 0x00, 0x00, 0x00);
					break;
				case 'B':
					break;
				case 'b':
					break;
				case 'C':
					break;
				case 'D':
					break;
				case 'd':
					break;
				case 'E':
					break;
				case 'e':
					break;
				case 'F':
					break;
				case 'f':
					break;
				case 'G':
					break;
				case 'g':
					break;
				case 'H':
					break;
				case 'h':
					break;
				case 'I':
					break;
				case 'i':
					break;
				case 'J':
					break;
				case 'j':
					break;
				case 'K':
					break;
				case 'k':
					break;
				case 'L':
					break;
				case 'l':
					break;
				case 'M':
					break;
				case 'm':
					break;
				case 'N':
					break;
				case 'n':
					break;
				case 'O':
					break;
				case 'o':
					break;
				case 'P':
					break;
				case 'p':
					break;
				case 'Q':
					break;
				case 'q':
					break;
				case 'R':
					break;
				case 'r':
					break;
				case 'S':	
					break;
				case 's':
					break;
				case 'T':
					break;
				case 't':
					break;
				case 'U':
					break;
				case 'u':
					break;
				case 'V':
					break;
				case 'v':
					break;
				case 'W':
					break;
				case 'w':
					break;
				case 'X':
					break;
				case 'x':
					break;
				case 'Y':
					break;
				case 'y':
					break;
				case 'Z':
					break;
				case 'z':
					break;

			//Special characters
				case '!':
					break;
				case '\"':
					break;
				case '$':
					break;
				case '%':
					break;
				case '&':
					break;
				case '/':
					break;
				case '(':
					break;
				case ')':
					break;
				case '=':
					break;
				case '?':
					break;
				case '[':
					break;
				case ']':
					break;
				case '{':
					break;
				case '}':
					break;
				case '\\':
					break;
				case '+':
					break;
				case '*':
					break;
				case '#':
					break;
				case '\'':
					break;
				case '-':
					break;
				case '_':
					break;
				case '.':
					break;
				case ':':
					break;
				case ',':
					break;
				case ';':
					break;
				case '<':
					break;
				case '>':
					break;
				case '|':
					break;
			}
			ret = 1;
			break;
		default:
			ret = -1;
			break;
	}
	
	return ret;
}

//Being used in fonts_get_fontdata
void fonts_build_font(char* letter, char c1, char c2, char c3, char c4, char c5, char c6, char c7, char c8, char c9)
{
	letter[0] = c1;
	letter[1] = c2;
	letter[2] = c3;
	letter[3] = c4;
	letter[4] = c5;
	letter[5] = c6;
	letter[6] = c7;
	letter[7] = c8;
	letter[8] = c9;
}


//Public write function
void fonts_write_text(char *text, uint x, uint y, int font_id, char font_color)
{
	//Reserve memory for the letter
	char *letter = (char*)memory_allocate(SYSTEM_FONT_SIZE);
	flash_memory(letter, SYSTEM_FONT_SIZE);	

	char c = 0;
	int i = 0;
	for(i; i < strlen(text); i++)
	{
		c = text[i];
		fonts_get_font_data(font_id, c, letter);
	
		
	}

	memory_free((int)letter);
}


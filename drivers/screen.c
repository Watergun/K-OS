#include "screen.h"

//Overview of all functions
void print_char(char, char);
int get_screen_offset(int,int);
int get_cursor();
void set_cursor(int);
void print_at(char*,int,int);
void print(char*);
void clear_screen();
int handle_scrolling(int);

/* Print a char on the screen at col, row, or at cursor position	 */
void print_char(char character, char attribute_byte)
{
	/*Create a pointer to the start of video memory 	*/
	unsigned char *vidmem = (unsigned char*) VIDEO_ADDRESS;

	/*If attribute byte is zero, assume the default style		*/
	if (!attribute_byte)
		attribute_byte = TM_DEFAULT_STYLE;

	/*Get the video memory offset for the screen location		*/
	int offset = get_cursor();
		
	//If we see a newline character, set offset to the end of current row,
	//so it will be advanced to the first col of the next row
	if (character == '\n')
	{
		int current_row = offset / (2*TM_MAX_COLS);
		offset = get_screen_offset(79, current_row);
	}

	//Otherwise, write the character and its attribute byte to videomemory 
	//at our calculated offset
	else
	{
		vidmem[offset] = character;
		vidmem[offset+1] = attribute_byte;
	}

	//Update the offset to the next character cell, which is two bytes
	//ahead of the current cell
	offset += 2;

	//Make scrolling adjustment, for when we reach the bottom of the screen
	//offset = handle_scrolling(offset);
	
	//Update the cursor  position on the screen device
	set_cursor(offset);
}

int get_screen_offset(int col, int row)
{
	int offset = (row*80 + col) * 2;
	return offset;
}

int get_cursor()
{
	//reg 14: which is the high byte of the cursor's offset
	//reg 15: which is the low byte of the cursor's offset
	//Once the internal register has been selected, we may read or write a byte on the data register
	port_byte_out(REG_SCREEN_CTRL, 14);			//Select high byte
	int offset = port_byte_in(REG_SCREEN_DATA) << 8;	//Get high byte
	port_byte_out(REG_SCREEN_CTRL, 15);			//Select low byte
	offset += port_byte_in(REG_SCREEN_DATA);		//Get low byte

	//Since the cursor offset reported by the VGA hardware is the number
	//of characters, we multiply by two to convert it to 
	//a character cell offset
	return offset*2;
}

void set_cursor(int offset)
{
	offset /= 2;	//Convert from cell offset to char offset

	//This is similar to get_cursor, only now we write bytes to those
	//internal device registers
	port_byte_out(REG_SCREEN_CTRL, 14);
	port_byte_out(REG_SCREEN_DATA, (offset & 0xff00) >> 8);
	port_byte_out(REG_SCREEN_CTRL, 15);
	port_byte_out(REG_SCREEN_DATA, (offset & 0xff));
}

void print_at(char *message, int col, int row)
{
	//Update the cursor if col and row not negative	
	if(col >= 0 && row >= 0)
		set_cursor(get_screen_offset(col, row));
	
	//Loop through each char of the message and print it
	int i = 0;
	while(message[i] != 0)
		print_char(message[i++], TM_DEFAULT_STYLE);	
}

void print(char *message)
{
	print_at(message, -1, -1);
}

void clear_screen()
{
	int row = 0; 
	int col = 0;

	set_cursor(0);

	/* Loop through video memory and write blank characters 	*/
	for(row = 0; row < TM_MAX_ROWS; row++)
		for(col = 0; col < TM_MAX_COLS; col++)
			print_char(' ', TM_DEFAULT_STYLE);
	
	//Move the cursor back to the top left
	set_cursor(get_screen_offset(0,0));
}


/*Advance the text cursor, scrolling the video buffer if necessary      */
int handle_scrolling(int cursor_offset)
{
        //If the cursor is within the screen, return it unmodified
        if (cursor_offset < TM_MAX_ROWS*TM_MAX_COLS*2)
                return cursor_offset;

      /*Shuffle the rows back one    		                         */
        unsigned int i;
        for(i = 0; i < TM_MAX_ROWS; i++)
                memory_copy(get_screen_offset(0, i) + VIDEO_ADDRESS,
                        get_screen_offset(0, i-1) + VIDEO_ADDRESS,
                        TM_MAX_COLS*2);

        /*Blank the last line by setting all bytes to 0         		*/
        char *last_line = (char*) get_screen_offset(0, TM_MAX_ROWS-1) + VIDEO_ADDRESS;
        for(i = 0; i < TM_MAX_COLS*2; i++)
                last_line[i] = 0;

        //Move the offset back one row, such that it is now on the last row,
        //rather than off the edge of the screen
        cursor_offset -= 2*TM_MAX_COLS;

        //Return the updated cursor position
        return cursor_offset;
}



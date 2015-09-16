//Keyboard_Code searches for the appropriate ASCII-Character

//Impemented in Assembler: checks for key-modifiers (e.g.: STRG)
extern char keyboard_get_status();
extern void keyboard_xor_status();

char keyboard_code(unsigned char scancode)
{
	short up_case = 0;
	char status = keyboard_get_status();

	if(status & 0x04)
	{
		if(status & 0x02)
			up_case = 0;
		else
			up_case = 1;
	}
	else if(status & 0x02)
		up_case = 1;

	char ret = 0;
	switch(scancode)
	{
		case 0x01:		//ESC
			ret = 0x1B; //Escape
			break;
		case 0x02:
		case 0x03:		
		case 0x04:
		case 0x05:
		case 0x06:
		case 0x07:
		case 0x08:
		case 0x09:
		case 0x0A:
		case	0x0B:;
			char str1[] = "1234567890";
			if(up_case)
			{	
				str1[0] = '!';
				str1[1] = '\"';
				str1[2] = 23;
				str1[3] = '$';
				str1[4] = '%';
				str1[5] = '&';
				str1[6] = '/';
				str1[7] = '(';
				str1[8] = ')';
				str1[9] = '=';
			}
               ret = str1[scancode-2];
               break;
		case 0x0C:		//ß
			ret = 225;
			if(up_case)
				ret = '?';
			break;
		case 0x0D:		//´
			ret = 96;
			if(up_case)
				ret = '`';
			break;
          case 0x0E:		//Backspace
			ret = 0x08;
			break;
		case 0x0F:		//Tab
			ret = 0x09;	
			break;
		case 0x10:
		case 0x11:
		case 0x12:
		case 0x13:
		case 0x14:
		case 0x15:
		case 0x16:
		case 0x17:
		case 0x18:
		case 0x19:;
			char str2[] = "qwertzuiop";
			if(up_case)
			{
				str2[0] = 'Q';
				str2[1] = 'W';
				str2[2] = 'E';
				str2[3] = 'R';
				str2[4] = 'T';
				str2[5] = 'Z';
				str2[6] = 'U';
				str2[7] = 'I';
				str2[8] = 'O';
				str2[9] = 'P';
			}
               ret = str2[scancode-0x10];
               break;
		case 0x1A:	//ü
			ret = 129;
			if(up_case)
				ret = 154;
			break;
		case 0x1B:	//+ 
			ret = 43;
			if(up_case)
				ret = '*';
			break;
		case 0x1C:	//ENTER
			ret = 0x0D;	//Carriage Return
			break;
		case 0x1D:	//STRG
			keyboard_xor_status(0x01);
			break;
		case 0x1E:
		case 0x1F:
		case 0x20:
		case 0x21:
		case 0x22:
		case 0x23:
		case 0x24:
		case 0x25:
		case 0x26:;
			char str3[] = "asdfghjkl";
			if(up_case)
			{
				str3[0] = 'A';
				str3[1] = 'S';
				str3[2] = 'D';
				str3[3] = 'F';
				str3[4] = 'G';
				str3[5] = 'H';
				str3[6] = 'J';
				str3[7] = 'K';
				str3[8] = 'L';
			}
			ret = str3[scancode-0x1E];
			break;
		case 0x27:	//ö
			ret = 148;
			if(up_case)
				ret = 153;
			break;
		case 0x28:	//ä
			ret = 132;
			if(up_case)
				ret = 142;
			break;
		case 0x29: 	//^
			ret = 0x5E;
			if(up_case)
				ret = 248;
			break;
		case 0x2A:	//SHIFT	-> SHIFT UP = 0xAA
			keyboard_xor_status(0x02);
			break;
		case 0x2B:	
		case 0x2C:
		case 0x2D:
		case 0x2E:
		case 0x2F:
		case 0x30:
		case 0x31:
		case 0x32:
		case 0x33:
		case 0x34:
		case 0x35:;	//Characters are yxcvbnm,.-
			char str4[] = "#yxcvbnm,.-";
			if(up_case)
			{
				str4[0] = '\'';
				str4[1] = 'Y';
				str4[2] = 'X';
				str4[3] = 'C';
				str4[4] = 'V';
				str4[5] = 'B';
				str4[6] = 'N';
				str4[7] = 'M';
				str4[8] = ';';
				str4[9] = ':';
				str4[10] = '_';
			}
			ret = str4[scancode-0x2B];
			break;
		case 0x36:	//SHIFT -> SHIFT UP = 0xBA
			keyboard_xor_status(0x02);			
			break;
		case 0x37:	//FN *
			ret = '*';
			break;
		case 0x38:	//ALT
			keyboard_xor_status(0x08);
			break;
		case 0x39:	//SPACE
			ret = ' ';
			break;
		case 0x3A:	//CAPS LOCK
			keyboard_xor_status(0x04);
			break;
		case 0x3B:
		case 0x3C:
		case 0x3D:
		case 0x3E:
		case 0x3F:
		case 0x40:
		case 0x41:
		case 0x42:
		case 0x43:
		case 0x44:	//F1 - F10
			ret = 0;
			break;
		case 0xAA:	//SHIFT UP means: remove the status flag
		case 0xBA:
			keyboard_xor_status(0x02);
			break;
		default:
			ret = 0;
			break;
	}	
	//Return found value
	return ret;
}


//Keyboard_Code searches for the appropriate ASCII-Character

char Keyboard_Code(char ScanCode)
{
	char ret;
	switch(ScanCode)
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
		case	0x0B:
		case 0x0C:
		case 0x0D:;		//Characters are: 1234567890ß´
			char str1[] = "1234567890ß´";
			ret = str1[ScanCode-2];
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
		case 0x19:
		case 0x1A:
		case 0x1B:;	//Characters are qwertzuiopü+ 
			char str2[] = "qwertzuiopü+";
			ret = str2[ScanCode-0x10];
			break;
		case 0x1C:	//ENTER
			ret = 0x0D;	//Carriage Return
			break;
		case 0x1D:	//STRG
			ret = 0;
			break;
		case 0x1E:
		case 0x1F:
		case 0x20:
		case 0x21:
		case 0x22:
		case 0x23:
		case 0x24:
		case 0x25:
		case 0x26:
		case 0x27:
		case 0x28:
		case 0x29:; 	//Characters are asdfghjklöä^
			char str3[] = "asdfghjklöä^";
			ret = str3[ScanCode-0x1E];
			break;
		case 0x2A:	//SHIFT
			ret = 0x0F;
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
			char str4[] = "yxcvbnm,.-";
			ret = str4[ScanCode-0x2B];
			break;
		case 0x36:	//SHIFT
			ret = 0x0F;
			break;
		case 0x37:	//FN *
			ret = '*';
			break;
		case 0x38:	//ALT
			ret = 0;
			break;
		case 0x39:	//SPACE
			ret = ' ';
			break;
		case 0x3A:	//CAPS LOCK
			ret = 0;
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
		default:
			ret = 0;
			break;
	}	
	//Return found value
	return ret;
}


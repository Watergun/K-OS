#define VIDEO_ADDRESS 0xb8000

#define TM_MAX_ROWS 25
#define TM_MAX_COLS 80

//Some color attributes (Text Mode)

#define TM_DEFAULT_STYLE 0x0E

#define TM_BLACK		0x0
#define TM_BLUE		0x1
#define TM_GREEN		0x2
#define TM_CYAN		0x3
#define TM_RED			0x4
#define TM_VIOLET		0x5
#define TM_BROWN		0x6
#define TM_LIGHT_GREY	0x7
#define TM_GREY		0x8
#define TM_LIGHT_BLUE	0x9
#define TM_LIGHT_GREEN	0xA
#define TM_LIGHT_CYAN	0xB
#define TM_LIGHT_RED	0xC
#define TM_LIGHT_VIOLET	0xD
#define TM_YELLOW		0xE
#define TM_WHITE		0xF

#define TM_COLORS(FONT_COLOR,BACKGND_COLOR) ((BACKGND_COLOR << 4) | FONT_COLOR)

//Screen device I/O ports
#define REG_SCREEN_CTRL	0x3D4
#define REG_SCREEN_DATA 0x3D5

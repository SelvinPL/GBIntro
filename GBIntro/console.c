#include <gb/gb.h>

#define BLINK_CHAR '\a'
#define SPACE_CHAR ' '
#define NEW_LINE_CHAR '\n'
#define END_OF_STRING '\0'

#define USE_BACKGROUND

#ifdef USE_BACKGROUND
#define VRAM_ADDRESS	0x9800
#define set_tile_at_xy set_bkg_tile_xy
#else
#define VRAM_ADDRESS	0x9c00
#define set_tile_at_xy set_win_tile_xy
#endif



#define console_lines	7
#define console_start	8

#define gbdk_stdio_fonts

#ifdef gbdk_stdio_fonts
#define fix_for_gbdk_stdio_fonts	32
#define CURSOR_TILE '\x81'
#else
#define fix_for_gbdk_stdio_fonts	0
#define CURSOR_TILE '\x7f'
#endif 

#define U8CP(cp) ((const uint8_t *)(cp))
#define U8P(cp) ((uint8_t *)(cp))
#define HALF(a)	 ((uint8_t)((a) / 2))

typedef struct
{
	uint8_t x;
	uint8_t y;
	uint8_t* data;
	uint8_t counter;
	uint8_t copy_state;
	uint8_t cursor_blink_state;
	const char* text;
} console_t;

console_t console;

void  consoleSetup(const char* text)
{
	console.y = console_start;
	console.data = U8P(text);
	console.x = 0;
	console.cursor_blink_state = 0;
	console.copy_state = 0;
	console.counter = 0;
	console.text = text;
}


void consoleLoop()
{
	if (!console.counter)
	{
		//console cursor apears on 8 and 4 and dissapear on 6 and 2 ... and stop blinking on 0
		if (console.cursor_blink_state)
		{
			uint8_t blink_state = console.cursor_blink_state;
			if (!(blink_state & 1))
			{
				uint8_t tile = ((blink_state == 4 || blink_state == 8) ? CURSOR_TILE : SPACE_CHAR);
				set_tile_at_xy(console.x, console.y, tile - fix_for_gbdk_stdio_fonts);
			}
			console.cursor_blink_state--;
		}
		else
		{
			uint8_t tile = (*console.data);
			if (tile == NEW_LINE_CHAR)
			{
				set_tile_at_xy(console.x, console.y, SPACE_CHAR - fix_for_gbdk_stdio_fonts);
				console.x = 0;
				console.y++;
				if (console.y == console_start + console_lines)
				{
					console.y = console_start + console_lines - 1;
					//if we are at last line start copying 
					console.copy_state = 1;
				}
			}
			else if (tile == END_OF_STRING)
			{
				//restart
				consoleSetup(console.text);
			}
			else if (tile != BLINK_CHAR)
			{
				set_tile_at_xy(console.x, console.y, tile - fix_for_gbdk_stdio_fonts);
				if (*(console.data + 1) != BLINK_CHAR && console.x < 19)
				{
					set_tile_at_xy(console.x + 1, console.y, CURSOR_TILE - fix_for_gbdk_stdio_fonts);
				}
				console.x++;
			}
			else
			{
				console.cursor_blink_state = 8;
			}
			console.data++;
		}
	}
	else if (console.copy_state == 1)
	{
		console.copy_state++;
		//copy data from bkg from start line + 1 and put it back to start line and ... first half
		vmemcpy((uint8_t*)(VRAM_ADDRESS + 32 * console_start), (uint8_t*)(VRAM_ADDRESS + 32 * (console_start + 1)), 32 * HALF(console_lines - 1));
	}
	else if (console.copy_state == 2)
	{
		console.copy_state++;
		//copy data from bkg from start line + 1 and put it back to start line and ... second half
		vmemcpy((uint8_t*)(VRAM_ADDRESS + 32 * (console_start + HALF(console_lines - 1))),
			(uint8_t*)(VRAM_ADDRESS + 32 * (console_start + HALF(console_lines - 1) + 1)), 32 * (console_lines - 1 - HALF(console_lines - 1)));
	}
	else if (console.copy_state == 3)
	{
		console.copy_state++;
		// ... clear last line vmemset should be better than set_bkg_tiles
		vmemset((void*)(VRAM_ADDRESS + 32 * (console_start + console_lines - 1)), SPACE_CHAR - fix_for_gbdk_stdio_fonts, 20);
	}
	console.counter++;
	if (console.counter == 4)
		console.counter = 0;
}
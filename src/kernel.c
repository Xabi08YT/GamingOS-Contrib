#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include "reboot.c"
#include "print.h"
#include "pic.c"
#include "cursor.c"
#include "../include/screen.h"
#include "../include/types.h"
#include "../include/kb.h"
#include "../include/string.h"
#include "../include/print.h"



#if defined(__linux__)
	#error "This code must be compiled with a cross-compiler"
#elif !defined(__i386__)
	#error "This code must be compiled with an x86-elf compiler"
#endif
 

volatile uint16_t* vga_buffer = (uint16_t*)0xB8000;

const int VGA_COLS = 80;
const int VGA_ROWS = 25;
 

int term_col = 0;
int term_row = 0;
uint8_t term_color = 0x0F; 
 

void term_init()
{
	
	for (int col = 0; col < VGA_COLS; col ++)
	{
		for (int row = 0; row < VGA_ROWS; row ++)
		{
			
			const size_t index = (VGA_COLS * row) + col;
			
			vga_buffer[index] = ((uint16_t)term_color << 8) | ' '; 
		}
	}
}
 

void term_putc(char c)
{
	
	switch (c)
	{
	case '\n': 
		{
			term_col = 0;
			term_row ++;
			break;
		}
 
	default: 
		{
			const size_t index = (VGA_COLS * term_row) + term_col; 
			vga_buffer[index] = ((uint16_t)term_color << 8) | c;
			term_col ++;
			break;
		}
	}
 
	
	if (term_col >= VGA_COLS)
	{
		term_col = 0;
		term_row ++;
	}
 
	
	if (term_row >= VGA_ROWS)
	{
		term_col = 0;
		term_row = 0;
	}
}
 

void term_print(const char* str)
{
	for (size_t i = 0; str[i] != '\0'; i ++) 
		term_putc(str[i]);
}

enum video_type
{
    VIDEO_TYPE_NONE = 0x00,
    VIDEO_TYPE_COLOUR = 0x20,
    VIDEO_TYPE_MONOCHROME = 0x30,
};
 
uint16_t detect_bios_area_hardware(void)
{
    const uint16_t* bda_detected_hardware_ptr = (const uint16_t*) 0x410;
    return *bda_detected_hardware_ptr;
}
 
enum video_type get_bios_area_video_type(void)
{
    return (enum video_type) (detect_bios_area_hardware() & 0x30);
}
 
void delay(int x)
{
    int i = 10000;
    while (--i)
    {
        int j = 10 * x;
        while (--j);
    }
}

void ascii_logo()
{
	print_set_color(PRINT_COLOR_GREEN, PRINT_COLOR_BLACK);
    print_str("                        ######      ###    ##     ## ######## #######   #######   ###### \n");
	print_str("                       ##    ##    ## ##   ###   ### ##       ##    ## ##     ## ##\n");
	print_str("                       ##         ##   ##  #### #### ##       ##    ## ##     ## ##\n");
	print_str("                       ##   #### ##     ## ## ### ## ######   ##    ## ##     ##  ######\n");
	print_str("                       ##    ##  ######### ##     ## ##       ##    ## ##     ##       ##\n");
	print_str("                       ##    ##  ##     ## ##     ## ##       ##    ## ##     ##       ##\n");
	print_str("                        ######   ##     ## ##     ## ######## #######   #######   ######\n");
	print_str("\n");
	print_set_color(PRINT_COLOR_WHITE, PRINT_COLOR_BLACK);
}

void kernel_main()
{	
//	reboot();
	disable_cursor();
	term_init();
	print_clear();
	print_set_color(PRINT_COLOR_WHITE, PRINT_COLOR_BLACK);
	print_str("Loading Pics...");
	init_pics(0x20, 0x28);
	delay(5000);
	print_set_color(PRINT_COLOR_LIGHT_GREEN, PRINT_COLOR_BLACK);
	print_str(" OK\n");
	delay(1000);
	print_set_color(PRINT_COLOR_WHITE, PRINT_COLOR_BLACK);
	print_str("Loading GDT...");
	delay(5000);
	print_set_color(PRINT_COLOR_LIGHT_RED, PRINT_COLOR_BLACK);
	print_str(" GDT not loaded\n");
	delay(1000);
	
	clearScreen();
	
    ascii_logo();
	print_str("\n");
	print_set_color(PRINT_COLOR_RED, PRINT_COLOR_BLACK);
	print_str("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	print_set_color(PRINT_COLOR_WHITE, PRINT_COLOR_BLACK);
	print_str("                                 Loading System...");
	
	delay(15000);
	clearScreen();
	

	while (1)
       {
		
		   		print("\n");
                print("\nGameDOS> ");

                
                string ch = readStr();
                if(strEql(ch,"help"))
                {
						print("\nlogo - Show logo in ascii\n");
                        print("\nreboot - Restart system\n");
						print("\nclear - Clear screen\n");
						print("\nlaunch - Launch other programs\n")
                }
				else if(strEql(ch,"logo"))
                {
						print_str("\n");
                        ascii_logo();
						delay(15000);
						clearScreen();
                }
				else if(strEql(ch,"reboot"))
                {	
                        reboot();
                }
                else if(strEql(ch,"clear"))
                {
                        clearScreen();
                }

				else if(strEql(ch,"launch"))
				{
						launch();
				}
                
                else
                {
                        print("\nBad command!\n");
                }
                
                print("\n");        
       }
}

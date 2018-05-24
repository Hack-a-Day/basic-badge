#include <xc.h>
#include <plib.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include "hw.h"
#include "vt100.h"
#include "post.h"
#include "disp.h"
#include "Z80/hwz.h"

uint16_t i;
const int8_t post_char_table[4*11] = "1234567890=qwertyuiop;/asdfghjkl\n\0zxcvbnm,.\0";
extern uint8_t handle_display;

void io_test_state (uint8_t state);



//B_BDG001
void post (void)
	{
	uint8_t retval,index,line,position,color;
	uint32_t flash_id;
	int8_t temp_string[30];
	handle_display = 0;
	set_led(0,1);
	set_led(1,1);
	set_led(2,1);	
	wait_ms(400);
	tft_fill_area(0,0,320,240,0xFF0000);
	wait_ms(1000);
	tft_fill_area(0,0,320,240,0x00FF00);
	wait_ms(1000);
	tft_fill_area(0,0,320,240,0x0000FF);
	wait_ms(1000);
	handle_display = 1;
	term_init();
	set_led(0,0);
	set_led(1,0);
	set_led(2,0);	

	flash_id = fl_rdid();
	if (flash_id==0x1f4501)
		video_set_color(EGA_WHITE,EGA_GREEN);
	else
		video_set_color(EGA_YELLOW,EGA_RED);
	video_gotoxy(0,15);
	sprintf(temp_string,"FLASH ID is %x\n",flash_id);
	stdio_write(temp_string);
	video_set_color(EGA_WHITE,EGA_BLACK);
	stdio_write("Press LCTRL+ENTER to continue\n");
	wait_ms(200);
	video_set_color(11,1);
	video_gotoxy(1,1);
	stdio_write("u ");
	video_gotoxy(0,2);
	stdio_write("l r ");
	video_gotoxy(22,2);
	stdio_write("b b ");
	video_gotoxy(1,3);
	stdio_write("d ");
	video_gotoxy(22,3);
	stdio_write("- d ");
	video_gotoxy(1,4);
	stdio_write("1 2 3 4 5 6 7 9 8 0 = ");
	video_gotoxy(1,5);
	stdio_write("Q W E R T Y U I O P ; ");
	video_gotoxy(1,6);
	stdio_write("? A S D F G H J K L e ");
	video_gotoxy(1,7);
	stdio_write("s Z X C V B N M < > s ");
	video_gotoxy(11,8);
	stdio_write("sp");
	while (1)
		{
		if (KEY_BRK==0)
			{
			video_gotoxy(22,2);
			video_set_color(11,2);
			stdio_write("  ");						
			}
		if (K_SHIFTR==0)
			{
			video_gotoxy(21,7);
			video_set_color(11,2);
			stdio_write("  ");						
			}					
		if (K_SHIFTL==0)
			{
			video_gotoxy(1,7);
			video_set_color(11,2);
			stdio_write("  ");						
			}	
		if (stdio_get(&retval)!=0)
			{
			if ((retval>=' ')&(retval<0x7F))
				{
				index = 255;
				for (i=0;i<44;i++)
					{
					if (post_char_table[i]==retval)
						index = i;
					}
				if (index<255)
					{
					line = index / 11;
					position = index % 11;
					video_gotoxy (1+(2*position),4+line);
					video_set_color(11,2);
					stdio_write("  ");
					}
				else
					{
					if (retval=='-')
						{
						video_gotoxy(22,3);
						video_set_color(11,2);
						stdio_write("  ");						
						}
					if (retval==' ')
						{
						video_gotoxy(11,8);
						video_set_color(11,2);
						stdio_write("  ");
						}
					}
				}
			else
				{
				if (retval==K_UP) video_gotoxy(1,1);
				if (retval==K_DN) video_gotoxy(1,3);
				if (retval==K_LT) video_gotoxy(0,2);
				if (retval==K_RT) video_gotoxy(2,2);
				if (retval==BACKSPACE) video_gotoxy(24,2);
				if (retval==K_DEL) video_gotoxy(24,3);
				if (retval==NEWLINE) video_gotoxy(21,6);
				if (retval == K_ECR) break;
				video_set_color(11,2);
				stdio_write("  ");						
				}
			}
		}
	video_clrscr();
	term_init();
	color=0;
	while (1)
		{
		video_gotoxy(0,2);
		if (color==3) color = 0;
		if (color==0)
			{
			set_led(0,1);
			set_led(1,0);
			set_led(2,0);
			stdio_write("red LED  \n");
			stdio_write("generator 1, playing F4, ~349Hz  \n");
			wait_ms(600);
			sound_play_notes(65,0,0,200);
			}
		if (color==1)
			{
			set_led(0,0);
			set_led(1,1);
			set_led(2,0);
			stdio_write("green LED  \n");
			stdio_write("generator 2, playing A4, 440Hz  \n");
			wait_ms(600);
			sound_play_notes(0,69,0,200);
			}
		if (color==2)
			{
			set_led(0,0);
			set_led(1,0);
			set_led(2,1);
			stdio_write("blue LED  \n");
			stdio_write("generator 3, playing C5, ~523Hz  \n");
			wait_ms(600);
			sound_play_notes(0,0,72,200);
			}
		video_set_color(EGA_WHITE,EGA_BLACK);
		stdio_write("Press LCTRL+ENTER to continue\n");
		if (stdio_get(&retval)!=0)
			{
			if (retval == K_ECR) break;
			}
		color++;
		}
	set_led(0,0);
	set_led(1,0);
	set_led(2,0);
	video_clrscr();
	term_init();
	U3MODEbits.ON = 0;
	TRISCbits.TRISC13 = 0;
	TRISCbits.TRISC14 = 0;
	TRISBbits.TRISB0 = 0;
	TRISBbits.TRISB1 = 0;
	while (1)
		{
		wait_ms(500);
		video_gotoxy(0,2);
		if (color==6) color = 0;
		io_test_state(color);
		video_gotoxy(1,7);
		video_set_color(EGA_WHITE,EGA_BLACK);
		sprintf(temp_string,"pin # %d\n",color);
		stdio_write(temp_string);
		stdio_write("Press LCTRL+ENTER to continue\n");
		if (stdio_get(&retval)!=0)
			{
			if (retval == K_ECR) break;
			}
		color++;
		}
	video_clrscr();
	term_init();
	video_gotoxy(1,7);
	stdio_write("Press f to format FLASH\n");
	while (1)
		{
		if (stdio_get(&retval)!=0)
			{
			if (retval == 'f') 
				{
				stdio_write("Formatting FLASH...\n");
				if (init_first_x_sects(128,1)==0)
					{
					video_set_color(EGA_WHITE,EGA_GREEN);
					stdio_write("OK, verified\n");
					}
				else
					{
					video_set_color(EGA_YELLOW,EGA_RED);
					stdio_write("Verification error\n");
					}
				break;
				}
			}
		}
	video_set_color(EGA_WHITE,EGA_BLACK);
	stdio_write("End of test, reset badge now\n");
	while(1);
	}


void io_test_state (uint8_t state)
	{
	if (state==0)
		{
		LATCbits.LATC13 = 1;
		LATCbits.LATC14 = 0;
		LATGbits.LATG2 = 0;
		LATGbits.LATG3 = 0;
		LATBbits.LATB0 = 0;
		LATBbits.LATB1 = 0;
		}
	else if (state==1)
		{
		LATCbits.LATC13 = 0;
		LATCbits.LATC14 = 1;
		LATGbits.LATG2 = 0;
		LATGbits.LATG3 = 0;
		LATBbits.LATB0 = 0;
		LATBbits.LATB1 = 0;
		}
	else if (state==2)
		{
		LATCbits.LATC13 = 0;
		LATCbits.LATC14 = 0;
		LATGbits.LATG2 = 1;
		LATGbits.LATG3 = 0;
		LATBbits.LATB0 = 0;
		LATBbits.LATB1 = 0;
		}
	else if (state==3)
		{
		LATCbits.LATC13 = 0;
		LATCbits.LATC14 = 0;
		LATGbits.LATG2 = 0;
		LATGbits.LATG3 = 1;
		LATBbits.LATB0 = 0;
		LATBbits.LATB1 = 0;
		}
	else if (state==4)
		{
		LATCbits.LATC13 = 0;
		LATCbits.LATC14 = 0;
		LATGbits.LATG2 = 0;
		LATGbits.LATG3 = 0;
		LATBbits.LATB0 = 1;
		LATBbits.LATB1 = 0;
		}
	else if (state==5)
		{
		LATCbits.LATC13 = 0;
		LATCbits.LATC14 = 0;
		LATGbits.LATG2 = 0;
		LATGbits.LATG3 = 0;
		LATBbits.LATB0 = 0;
		LATBbits.LATB1 = 1;
		}	
	else
		{
		LATCbits.LATC13 = 0;
		LATCbits.LATC14 = 0;
		LATGbits.LATG2 = 0;
		LATGbits.LATG3 = 0;
		LATBbits.LATB0 = 0;
		LATBbits.LATB1 = 0;
		}	
		
	
	}
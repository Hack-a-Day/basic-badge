#include <xc.h>
#include <plib.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include "hw.h"
#include "vt100.h"
#include "menu.h"
#include "post.h"

uint16_t i;
const int8_t post_char_table[4*11] = "1234567890=qwertyuiop;/asdfghjkl\n\0zxcvbnm,.\0";


void post (void)
	{
	uint8_t retval,index,line,position;
	tft_fill_area(0,0,320,240,0xFF0000);
	wait_ms(400);
	tft_fill_area(0,0,320,240,0x00FF00);
	wait_ms(400);
	tft_fill_area(0,0,320,240,0x0000FF);
	wait_ms(400);
	term_init();
	
	while (1)
		{
		wait_ms(200);
		video_set_color(11,1);
		video_gotoxy(1,1);
		stdio_write("u ");
		video_gotoxy(24,1);
		stdio_write("p ");
		video_gotoxy(0,2);
		stdio_write("l r ");
		video_gotoxy(22,2);
		stdio_write("b b ");
		video_gotoxy(1,3);
		stdio_write("d ");
		video_gotoxy(22,3);
		stdio_write("_ d ");
		video_gotoxy(1,4);
		stdio_write("1 2 3 4 5 6 7 9 8 0 = ");
		video_gotoxy(1,5);
		stdio_write("Q W E R T Y U I O P ; ");
		video_gotoxy(1,6);
		stdio_write("? A S D F G H J K L e ");
		video_gotoxy(1,7);
		stdio_write("s Z X C V B N M < > s ");
		if (KEY_BRK==0)
			{
			video_gotoxy(22,2);
			video_set_color(11,4);
			stdio_write("  ");						
			}					
		if (K_SHIFTR==0)
			{
			video_gotoxy(1,7);
			video_set_color(11,4);
			stdio_write("  ");						
			}					
		if (K_SHIFTL==0)
			{
			video_gotoxy(21,7);
			video_set_color(11,4);
			stdio_write("  ");						
			}	
		if (stdio_get(&retval)!=0)
			{
			if ((retval>=' ')&(retval<=0x7F))
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
					video_set_color(11,4);
					stdio_write("  ");
					}
				else
					{
					if (retval=='_')
						{
						video_gotoxy(22,3);
						video_set_color(11,4);
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
				if (retval==NEWLINE) video_gotoxy(21,6);
				if (retval==K_DEL) video_gotoxy(24,3);
				video_set_color(11,4);
				stdio_write("  ");						
				}
			}
		}
	
	while(1);
	}


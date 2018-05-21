/************************************
 * This is the framework for those
 * who wish to write their own C
 * code for the basic badge
 ************************************/

#include "badge.h"
#include "badge_user.h"

void user_program_init(void)
	{
	/* This will run when User Program is first selected form the menu */
	clr_buffer();
	video_gotoxy(5,4);
    stdio_write("Use this example to write");
	video_gotoxy(5,5);
	stdio_write("your own C code for the badge.");
	}

void user_program_loop(void)
	{
	/* This should contain user program (will be run after init function)*/
	
	/* Write some stuff to the screen*/
	video_set_color(3,8);	//Set color before writing
	video_gotoxy(5,7);		//Move cursor
	stdio_write(" Behold, color! And ASCII: \xCC\xCD\xCD\xB9 "); // stdio_write will affect both color and character in a location
	video_set_color(15,0);
	video_gotoxy(5,10);
	stdio_write("Counting seconds:");
	video_gotoxy(5,14);
	stdio_write("Waiting for you to press any key...");
	
	while(1) //Loop forever
		{
		/* Example of a non-blocking delay */
		static uint32_t delay_until = 0;
		static uint8_t count = '0';
		if (millis()>=delay_until)
			{
				/* Writing directly to disp_buffer instead of stdio_write */
				/* Note that Y value is given before X: disp_buffer[Y][X]*/
				disp_buffer[10][24] = count++;
				if (count > '9')
					{
						count = '0';
					}
				delay_until = millis()+1000;
			}

		/* React to user input */
		uint8_t get_stat, char_out;
		get_stat = stdio_get(&char_out);
		if (get_stat!=0)
			{
			/* Show which letter was pressed just as an example: */
			/* Directly address color_buffer: first nibble is BG, second nibble is FG*/
			color_buffer[16][8] = 0x1E;
			if (char_out > 31) // Make sure it was a printable character
				{
				disp_buffer[16][8] = char_out;
				}
			else disp_buffer[16][8] = 0xEC;
			
			video_gotoxy(5,19);
			stdio_write("2s until manual screen writing demo");
			wait_ms(2000);	//Make sure user sees entered character
			
			/* Now do something interesting */
			enable_display_scanning(0); //Shut off auto-scanning of character buffer
			uint16_t x = 0;
			uint8_t y = 0;
			uint32_t colorshift = 0;
			while(1) //Loop forever
				{
				/* Draw a 1x1 box at x,y -- color is using XOR trick but should be format 0x00RRGGBB */
				tft_fill_area (x, y, 1, 1, (x^(uint32_t)y)+colorshift);
				++x; //Increment x
				
				/* Reset x/y as necessary */
				if (x >= 320)
					{
					x = 0;
					++y;
					}
				if (y >= 240)
					{
					y = 0;
					colorshift += 0x00123456;
					}
			
				/* Waste some time but less than 1 ms */
				uint16_t quickdelay = 2000;
				while (quickdelay)
					{
					--quickdelay;
					}
				}
			//Badge will need to be reset to continue
			}
		}
	}


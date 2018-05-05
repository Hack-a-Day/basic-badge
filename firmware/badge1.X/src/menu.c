#include "hw.h"
#include "menu.h"
#include <stdint.h>
#include "vt100.h"
#include <string.h>

void menu(void)
	{
	showmenu();
	
	char char_out;
	unsigned char menu_buff[30], menu_pointer;
	menu_pointer=0;
	int len, i;
	
	while (1)
		{
		unsigned char get_stat = stdio_get(&char_out);
		if (get_stat!=0)
			{
			stdio_c(char_out);	
			if (char_out==NEWLINE)
				{
				//Erase where the funny messages are written
				video_gotoxy(4,17);
				for (i=4; i<39; i++) stdio_write(" ");
				
				menu_buff[menu_pointer] = 0;	//add zero terminator
				//Check entry and react
				if (strcmp(menu_buff,"1")==0)
					{
					video_clrscr();
					init_basic();
					while (1) loop_basic();
					}
				else if (strcmp(menu_buff,"2")==0)
					{
					video_clrscr();
					init_z80_cpm();
					while (1) loop_z80_cpm();
					}			
				else if (strcmp(menu_buff,"3")==0)
					{
					init_8080_basic();
					while (1) loop_8080_basic();
					}
				else if (strcmp(menu_buff,"4")==0)
					{
					enable_display_scanning(0); //Shut off auto-scanning of character buffer
					tetrapuzz();
					}
				else if (strcmp(menu_buff,"5")==0)
					{
					init_userprog();
					while (1) loop_userprog();
					}			
				else if (strcmp(menu_buff,"mike")==0)
					{
					video_gotoxy(4,17);
					stdio_write("Mike wrote this");
					}
				else
					{
					video_gotoxy(4,17);
					stdio_write("Nice try, wise guy.");
					}
				
				//Clear prompt area
				video_gotoxy(4,13);
				for (i=4; i<39; i++) stdio_write(" ");
				video_gotoxy(4,13);
				stdio_write("> ");
				menu_pointer = 0;
				menu_buff[menu_pointer] = 0;
				}
				
			else
				{
				menu_buff[menu_pointer++] = char_out;
				if (menu_pointer >= 29) 
					{
					menu_pointer = 0;
					menu_buff[menu_pointer] = 0;
					}
				}
			}
		}
	}

void showmenu(void)
	{
	//Set some background boxes
	
	video_set_color(0,15);
	video_gotoxy(1,1);
	stdio_write("                                      ");
	video_gotoxy(1,2);
	stdio_write("                                      ");
	video_gotoxy(1,3);
	stdio_write("                                      ");
	
	video_set_color(15,8);
	video_gotoxy(3,5);
	stdio_write("                                  ");
	video_set_color(15,9);
	video_gotoxy(3,6);
	stdio_write("                                  ");
	video_gotoxy(3,7);
	stdio_write("                                  ");
	video_gotoxy(3,8);
	stdio_write("                                  ");
	video_gotoxy(3,9);
	stdio_write("                                  ");
	video_gotoxy(3,10);
	stdio_write("                                  ");
	
	//Draw frame
	video_set_color(12,0);
	fancyframe();
	
	//Print menu text
	video_set_color(0,15);
	video_gotoxy(4,2);
	stdio_write("Belegrade badge version 0.31");
	video_gotoxy(4,5);
	video_set_color(15,8);
	stdio_write("Type your choice and hit ENTER");
	video_gotoxy(4,6);
	video_set_color(15,9);
	stdio_write("1 - Hackaday BASIC");
	video_gotoxy(4,7);
	video_set_color(15,9);
	stdio_write("2 - CP/M @ Z80");
	video_gotoxy(4,8);
	video_set_color(15,9);
	stdio_write("3 - Tiny Basic @ 8080");
	video_gotoxy(4,9);
	video_set_color(15,9);
	stdio_write("4 - Play Badgetris!");
	video_gotoxy(4,10);
	video_set_color(15,9);
	stdio_write("5 - User Program");
	
	video_set_color(15,0);
	video_gotoxy(4,13);
	stdio_write("> ");
	}

void fancyframe(void)
	{
	unsigned int i;
	video_gotoxy(0,0);
	stdio_write("+");
	for (i=0; i<38; i++) stdio_write("*");
	stdio_write("+");
	video_gotoxy(0,39);
	stdio_write("+");
	for (i=0; i<38; i++) stdio_write("*");
	stdio_write("+");
	for (i=1; i<19; i++)
		{
		video_gotoxy(0,i);
		stdio_write("*");
		video_gotoxy(39,i);
		stdio_write("*");
		}
	}
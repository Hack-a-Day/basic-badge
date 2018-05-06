#include "hw.h"
#include "menu.h"
#include <stdint.h>
#include "vt100.h"
#include <string.h>

#define HASH_TABLE_LENGTH	10
const unsigned long hashtable[HASH_TABLE_LENGTH] =
	{
	0,				//0
	0,				//1
	4080429,		//2
	3914880981u,	//3
	1670,			//4
	121730,			//5
	4108351,		//6
	138097304,		//7
	110149,			//8
	3174374908u,	//9
	};

const unsigned char wrencher[18][41] = {
"    +mmy/                      /ymm+    ",
"      sMMh                    hMMs      ",
"h/  .hMMMM.                  -MMMMh.  /h",
"sMmyMMMMMMNo`              `oNMMMMMMymMs",
" :hNMMMMMMMMMo`:shmNNmhs:`oMMMMMMMMMNh: ",
"       :hMMN+sNMMMMMMMMMMNo+NMMh:       ",
"         -s:mMMMMMMMMMMMMMMm:s.         ",
"          `mMMMMMMMMMMMMMMMMm`          ",
"          +MMh.  -mMMd-  .hMM/          ",
"          oMM.  -+NMMN+-  .MMo          ",
"          -MMd/NMMMmmMMMN/dMM-          ",
"         -/sMMMMMMM::MMMMMMMs/-         ",
"       /dMMsoMMMMMMNNMMMMMNosMMd/       ",
" :hNMMMMMMMMd-hMMMMMMMMMMh-dMMMMMMMMNy: ",
"sMmyMMMMMMm+` /mNsoNm+sNm/ `omMMMMMNymMo",
"h:  .yMMMM.                  .MMMMy.  :h",
"      sMMh                    hMMs      ",
"   `ommh/                      /hmm+    "};

void menu(void)
	{
	showmenu();
	
	char char_out;
	unsigned char menu_buff[30], menu_pointer;
	menu_pointer=0;
	int len, i;
	unsigned char clear_flag = 0;
	unsigned long wait_to_clear = 0;
	
	while (1)
		{
		if (clear_flag)
			{
			if (millis() > wait_to_clear)
				{
				clear_flag = 0;
				int cursorx = video_getx();
				int cursory = video_gety();
				video_gotoxy(1,17);
				stdio_write("                                      ");
				video_gotoxy(cursorx,cursory);
				}
			}
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
				else
					{
					switch (get_command_index(hash(menu_buff)))
						{
						//case 0: break;
						case 2: clear_flag = wisecrack("Make your own sandwich",4,17); break;
						case 3: clear_flag = wisecrack("Existence itself is not a hack", 4,17); break;
						case 4: clear_flag = wisecrack("101010 *IS* the answer", 4,17); break;
						case 5: clear_flag = wisecrack("Hackers don't need manuals", 4,17); break;
						case 6: clear_flag = wisecrack("Han shot first", 4,17); break;
						case 7: clear_flag = wisecrack("You're in a room filled with hackers", 4,17); break;
						case 8: clear_flag = wisecrack("I'm sorry, Dave. I'm afraid I can't do that", 2,17); break;
						case 9: show_wrencher(); break;
						default: clear_flag = wisecrack("Nice try, wise guy",4,17); break;
						}
					}
				
				if (clear_flag)
					{
					wait_to_clear = millis() + 2000;
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

unsigned long hash(char *command)
	{
	unsigned long hash = 0;
	unsigned char infinite_loop_breaker = 0;
	unsigned char c;
	while (c = *command++)
		{
		hash = (hash*33)^c;
		if (infinite_loop_breaker++ > 100) return 0;	//In case we get stuck
		}
	//sprintf(stdio_buff, "has %d\n", hash);
	return hash;
	}

unsigned char get_command_index(unsigned long hash_value)
	{
	unsigned char i;
	for (i=0; i<HASH_TABLE_LENGTH; i++)
		{
		if (hashtable[i] == hash_value) return i;
		}
	return 0;
	}

unsigned char wisecrack(char * quip, unsigned int x, unsigned char y)
	{
	video_gotoxy(x,y);
	stdio_write(quip);
	return 1;
	}

void showmenu(void)
	{
	//Set some background boxes
	video_clrscr();
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

void show_wrencher(void)
	{
	char i;
	video_clrscr();
	video_set_color(2,0);
	video_gotoxy(0,1);
	for (i=0; i<18; i++)
		{
		stdio_write((char *)wrencher[i]);
		}
	char char_out;
	while (stdio_get(&char_out) == 0) {;;}
	showmenu();
	}


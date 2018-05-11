#include "hw.h"
#include "menu.h"
#include <stdint.h>
#include "vt100.h"
#include "tune_player.h"
#include <string.h>

#define HASH_TABLE_LENGTH	11
const uint32_t  hashtable[HASH_TABLE_LENGTH] =
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
	132593272,		//10
	};

//Prompt handling defines
#define COMMAND_MAX 32
#define TEXT_LEFT	4
#define PROMPT_Y	13
#define CRACK_Y		17
#define VERSION_X	33
#define VERSION_Y	18

//Menu color values
#define MENU_FRAME_FG	12
#define MENU_FRAME_BG	0
#define MENU_BANNER_FG	0
#define MENU_BANNER_BG	15
#define MENU_HEADER_FG	15
#define MENU_HEADER_BG	8
#define MENU_ENTRY_FG	15
#define MENU_ENTRY_BG	9
#define MENU_DEFAULT_FG 15
#define MENU_DEFAULT_BG 0
#define MENU_VERSION_FG	8

const uint8_t wrencher[18][41] = {
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
	
	int8_t char_out;
	uint8_t menu_buff[COMMAND_MAX], menu_pointer;
	menu_pointer=0;
	int16_t len, i;
	uint8_t clear_flag = 0;
	uint32_t  wait_to_clear = 0;
	int16_t cursorx, cursory;
	while (1)
		{
		if (clear_flag)
			{
			if (millis() > wait_to_clear)
				{
				clear_flag = 0;
				cursorx = video_getx();
				cursory = video_gety();
				video_gotoxy(1,CRACK_Y);
				stdio_write("                                      ");
				video_gotoxy(cursorx,cursory);
				}
			}
		uint8_t get_stat = stdio_get(&char_out);
		if (get_stat!=0)
			{
			stdio_c(char_out);
			if (char_out==BACKSPACE)
				{
				if (menu_pointer)
					{
					//We only need to do this if we're not at position 0
					--menu_pointer;
					menu_buff[menu_pointer] = 0;
					cursorx = video_getx();
					cursory = video_gety();
					stdio_write(" ");
					video_gotoxy(cursorx, cursory);
					}
				else
					{
					//Don't let backspace key move left of input position
					video_gotoxy(TEXT_LEFT+2,PROMPT_Y);
					}
				}
			else if (char_out==NEWLINE)
				{
				//Erase where the funny messages are written
				video_gotoxy(1,CRACK_Y);
				stdio_write("                                      ");
				
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
				else if (strcmp(menu_buff,"6")==0)
					{
					video_clrscr();
					stdio_write("...formatting FLASH...");
					init_first_x_sects(32);
					stdio_write("...done. Reset badge now.");
					while (1);
					}			
				else
					{
					switch (get_command_index(hash(menu_buff)))
						{
						//case 0: break;
						case 2: clear_flag = wisecrack("Make your own sandwich",TEXT_LEFT,CRACK_Y); break;
						case 3: clear_flag = wisecrack("Existence itself is not a hack", TEXT_LEFT,CRACK_Y); break;
						case 4: clear_flag = wisecrack("101010 *IS* the answer", TEXT_LEFT,CRACK_Y); break;
						case 5: clear_flag = wisecrack("Hackers don't need manuals", TEXT_LEFT,CRACK_Y); break;
						case 6: clear_flag = wisecrack("Han shot first", TEXT_LEFT,CRACK_Y); break;
						case 7: clear_flag = wisecrack("You're in a room filled with hackers", TEXT_LEFT,CRACK_Y); break;
						case 8: clear_flag = wisecrack("I am afraid I can't do that Dave", TEXT_LEFT,CRACK_Y); break;
						case 9: show_wrencher(); break;
						case 10: play_mario_tune(); break;
						default: clear_flag = wisecrack("Nice try, wise guy",TEXT_LEFT,CRACK_Y); break;
						}
					}

				
				clear_prompt();
				menu_pointer = 0;
				menu_buff[menu_pointer] = 0;
				}
				
			else
				{
				menu_buff[menu_pointer++] = char_out;
				if (menu_pointer >= COMMAND_MAX-1) 
					{
					menu_pointer = 0;
					menu_buff[menu_pointer] = 0;
					clear_flag = wisecrack("Boo, too big.",TEXT_LEFT,CRACK_Y);
					clear_prompt();
					}
				}
			if (clear_flag)
				{
				wait_to_clear = millis() + 2000;
				}
			}
		}
	}

uint32_t  hash(int8_t *command)
	{
	uint32_t  hash = 0;
	uint8_t infinite_loop_breaker = 0;
	uint8_t c;
	while (c = *command++)
		{
		hash = (hash*33)^c;
		if (infinite_loop_breaker++ > 100) return 0;	//In case we get stuck
		}
	//sprintf(stdio_buff, "has %d\n", hash);
	return hash;
	}

uint8_t get_command_index(uint32_t  hash_value)
	{
	uint8_t i;
	for (i=0; i<HASH_TABLE_LENGTH; i++)
		{
		if (hashtable[i] == hash_value) return i;
		}
	return 0;
	}

uint8_t wisecrack(int8_t * quip, uint16_t x, uint8_t y)
	{
	video_gotoxy(x,y);
	stdio_write(quip);
	return 1;
	}

void showmenu(void)
	{
	//Set some background boxes
	video_clrscr();
	video_set_color(MENU_BANNER_FG,MENU_BANNER_BG);
	video_gotoxy(1,1);
	stdio_write("                                      ");
	video_gotoxy(1,2);
	stdio_write("                                      ");
	video_gotoxy(1,3);
	stdio_write("                                      ");
	
	video_set_color(MENU_HEADER_FG,MENU_HEADER_BG);
	video_gotoxy(3,5);
	stdio_write("                                  ");
	video_set_color(MENU_ENTRY_FG,MENU_ENTRY_BG);
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
	video_set_color(MENU_FRAME_FG,MENU_FRAME_BG);
	fancyframe();
	
	//Print menu text
	video_set_color(MENU_BANNER_FG,MENU_BANNER_BG);
	video_gotoxy(TEXT_LEFT,2);
	stdio_write("Hackaday Belgrade Conference");
	video_gotoxy(TEXT_LEFT,5);
	video_set_color(MENU_HEADER_FG,MENU_HEADER_BG);
	stdio_write("Type a command and hit ENTER");
	video_gotoxy(TEXT_LEFT,6);
	video_set_color(MENU_ENTRY_FG,MENU_ENTRY_BG);
	stdio_write("1 - Hackaday BASIC");
	video_gotoxy(TEXT_LEFT,7);
	stdio_write("2 - CP/M @ Z80");
	video_gotoxy(TEXT_LEFT,8);
	stdio_write("3 - Tiny Basic @ 8080");
	video_gotoxy(TEXT_LEFT,9);
	stdio_write("4 - Play Badgetris!");
	video_gotoxy(TEXT_LEFT,10);
	stdio_write("5 - User Program");
	video_gotoxy(TEXT_LEFT,11);
	stdio_write("6 - !!!CP/M disk formatter!!!");
	
	video_set_color(MENU_VERSION_FG,MENU_DEFAULT_BG);
	video_gotoxy(VERSION_X,VERSION_Y);
	stdio_write("v");
	stdio_write((int8_t *)get_firmware_string());
	
	video_set_color(MENU_DEFAULT_FG,MENU_DEFAULT_BG);
	video_gotoxy(TEXT_LEFT,PROMPT_Y);
	stdio_write("> ");
	
	}

void fancyframe(void)
	{
	uint16_t i;
	video_gotoxy(0,0);
	stdio_c(201);
	for (i=0; i<38; i++) stdio_c(205);
	stdio_c(187);
	video_gotoxy(0,39);
	stdio_c(200);
	for (i=0; i<38; i++) stdio_c(205);
	stdio_c(188);
	for (i=1; i<19; i++)
		{
		video_gotoxy(0,i);
		stdio_c(186);
		video_gotoxy(39,i);
		stdio_c(186);
		}
	}

void clear_prompt(void)
	{
	//Clear prompt area
	video_gotoxy(TEXT_LEFT,PROMPT_Y);
	int16_t i;
	for (i=TEXT_LEFT; i<39; i++) stdio_write(" ");
	video_gotoxy(TEXT_LEFT,PROMPT_Y);
	stdio_write("> ");
	}

void show_wrencher(void)
	{
	int8_t i;
	video_clrscr();
	video_set_color(2,0);
	video_gotoxy(0,1);
	for (i=0; i<18; i++)
		{
		stdio_write((int8_t *)wrencher[i]);
		}
	wait_ms(200);
	i = 0;
	uint8_t loopbreak = 0;
	while(1)
		{
		loopbreak = playriff(i++);
		if (loopbreak) break;
		if (i > 3) i = 0;
		}
	showmenu();
	}

uint8_t playriff(uint8_t raisetop)
	{
	const uint16_t riffdelays[3] = {78,236,393};
	const uint8_t riff[14][4] =
		{
			{0,62,69,0},
			{0,0,0,0},
			{0,62,69,0},
			{0,0,0,0},
			{0,62,69,1},
			{0,0,0,2},
			{0,62,69,0},
			{0,0,0,0},
			{0,62,69,0},
			{0,0,0,0},
			{0,62,69,1},
			{0,0,0,0},
			{50,0,0,1},
			{0,0,0,0}
		};
	uint8_t i;
	for (i=0; i<14; i++)
		{
		uint8_t top = 0;
		if (riff[i][2] != 0)
			{
			top = riff[i][2] + raisetop;
			}
		sound_play_notes(riff[i][0],riff[i][1],top,riffdelays[riff[i][3]]);	
		}
		if (stdio_get(&i) != 0) { return 1; }
	return 0;
	}


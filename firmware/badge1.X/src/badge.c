#include <xc.h>
#include "basic/ubasic.h"
#include "basic/tokenizer.h"
#include <plib.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include "hw.h"
#include <setjmp.h>
#include "Z80/sim.h"
#include "Z80/simglb.h"
#include "Z80/hwz.h"
#include "vt100.h"
//#include "menu.h"
#include "splash.h"
#include "tetrapuzz.h"
#include "post.h"
#include "badge_settings.h"
#include "tune_player.h"
#include "badge.h"
#include "snake.h"

int8_t bprog[BPROG_LEN+1] =
"5 rem user value repeater\n\
10 let a = input \"Enter value\"\n\
19 print \"You entered: \"\n\
20 println a\n\
";

//a lot of magic numbers here, should be done properly
int8_t tprog[100],stdio_buff[50],key_buffer[10],char_out, stdio_local_buff[STDIO_LOCAL_BUFF_SIZE];
int8_t disp_buffer[DISP_BUFFER_HIGH+1][DISP_BUFFER_WIDE], color_buffer[DISP_BUFFER_HIGH+1][DISP_BUFFER_WIDE];
uint8_t get_stat,key_buffer_ptr =0,cmd_line_buff[30], cmd_line_pointer,cmd_line_key_stat_old,prompt;
uint8_t stdio_local_len=0;
uint16_t term_pointer,vertical_shift;
int16_t prog_ptr;
int32_t i,j,len;
jmp_buf jbuf;
volatile uint8_t handle_display = 1;
volatile int8_t brk_key,stdio_src;
extern volatile uint16_t bufsize;
volatile uint32_t ticks;			// millisecond timer incremented in ISR

extern const uint8_t ram_image[65536];
extern const uint8_t b2_rom[2048];
extern const uint8_t ram_init [30];
extern uint8_t ram_disk[RAMDISK_SIZE];

#define HASH_TABLE_LENGTH	12
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
	2802956003u		//11
	};

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


//B_BDG005
void wake_return(void)
	{
	//By default, this will be called after waking from sleep. It should do
	//noting. This is a placeholder for user programs to set the function pointer.
	return;
	}

void badge_init (void)
	{
	//B_BDG009
	start_after_wake = &wake_return; //Function pointer for waking from sleep
	ticks = 0;
	stdio_src = STDIO_LOCAL;
//	stdio_src = STDIO_TTY1;
	term_init();
	}

//B_BDG002
void badge_menu(void)
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
			if (char_out==K_UP) char_out='U';
			if (char_out==K_DN) char_out='D';
			if (char_out==K_LT) char_out='L';
			if (char_out==K_RT) char_out='R';
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
				//B_BDG006
				else if (strcmp(menu_buff,"5")==0)
					{
					init_userprog();
					while (1) loop_userprog();
					}			
				else if (strcmp(menu_buff,"6")==0)
					{
					stdio_local_buffer_puts("c:\nzork1\n");
					video_clrscr();
					video_set_color(0,14);
					stdio_write("\n\n\n\n\n\nPROTIP:\n\tUse Shift-Enter when playing ZORK!\n\n");
					video_set_color(15,0);
					wait_ms(2000);
					init_z80_cpm();
					while (1) loop_z80_cpm();
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
						case 11: 
							handle_display = 0;
							play_snake();
							break;
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
	video_gotoxy(3,11);
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
	stdio_write("6 - Zork @ CP/M @ Z80");
	
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


//housekeeping stuff. call this function often
void loop_badge(void)
	{
	volatile uint16_t dbg;
	static uint8_t brk_is_pressed;
	dbg = PORTD;
	if (K_PWR==0)
		{
		while (K_PWR==0);
		wait_ms(100);
		hw_sleep();
		wait_ms(30);
		while (K_PWR==0);
		wait_ms(300);
		}
	if (KEY_BRK==0)
		{
		if ((K_SHIFTL==0)&(brk_is_pressed==9))
			{
			serial_flush();
			if (stdio_src == STDIO_TTY1)
				stdio_src = STDIO_LOCAL;
			else
				stdio_src = STDIO_TTY1;
			}
		else
			brk_key = 1;
		if (brk_is_pressed<10) brk_is_pressed++;
		}
	else
		brk_is_pressed = 0;
	}

//B_BDG004
void enable_display_scanning(uint8_t onoff)
	{
	//Turns vt100 scanning on or off
	if (onoff) handle_display = 1;
	else handle_display = 0;
	}

uint32_t millis(void)
	{
	return ticks;
	}

void init_8080_basic (void)
	{
	video_set_color(15,0);
	for (i=0;i<2048;i++) ram[i] = b2_rom[i];
	for (i=0;i<30;i++) ram[i+0x1000] = ram_init[i];
	wrk_ram	= PC = STACK = ram;
	init_io(IO_BASIC_MODE);
	}

void loop_8080_basic (void)
	{
	cpu_error = NONE;
	cpu();	
	}

void init_z80_cpm (void)
	{
	video_set_color(15,0);
#ifdef	USE_RAM_IMAGE	
	for (i=0;i<65536;i++) ram[i] = ram_image[i];
#endif	
#ifdef	USE_RAMDISK
	for (i=0;i<RAMDISK_SIZE;i++) ram_disk[i] = 0xE5;
#endif
	wrk_ram	= PC = STACK = ram;
	init_io(IO_CPM_MODE);
	}

void loop_z80_cpm (void)
	{
	cpu_error = NONE;
	cpu();	
	}

//B_BAS005
void init_basic (void)
	{
	stdio_write("BASIC interpreter, type help for help\n");
	prompt = 1;
	brk_key = 0;
	cmd_line_pointer=0;
	cmd_line_buff[0] = 0;
	video_set_color(15,0);
	}
//B_BAS006
void loop_basic (void)
	{
	if (prompt==1)
	    {
	    stdio_write(">");	
	    prompt = 0;
	    }
	get_stat = stdio_get(&char_out);
	if (get_stat!=0)
	    {
	    stdio_c(char_out);	
	    if (char_out==NEWLINE) 
			{
			cmd_line_buff[cmd_line_pointer] = 0;
			cmd_exec (cmd_line_buff);
			cmd_line_pointer=0;
			cmd_line_buff[0] = 0;
			prompt = 1;			
			}
	    else
			{
			if (char_out>=' ') cmd_line_buff[cmd_line_pointer++] = char_out;
			else if (char_out==BACKSPACE)
				{
				if (cmd_line_pointer>0) 
					cmd_line_buff[--cmd_line_pointer]=0;
				}

			}
	    }	
	}

//B_BDG007
void init_userprog (void)
{
    clr_buffer();
    stdio_write("Press any key to show splash screen.\n");
}

//B_BDG008
void loop_userprog (void)
{
    get_stat = stdio_get(&char_out);
    if (get_stat!=0)
    {
	handle_display = 0; //Shut off auto-scanning of character buffer
	play_snake();
	animate_splash();
	//show_splash();
	while(stdio_get(&char_out) == 0) { ;; }  //wait for button press
	handle_display = 1; //Go back to character display
    }

    static uint16_t delay_until = 0;
    static uint8_t count = '0';

    if (ticks>=delay_until)
    {
	disp_buffer[10][10] = count++;
	if (count > '9')
	{
	    count = '0';
	}
	delay_until = ticks+1000;
    }
}

void clr_buffer (void)
	{
	for (i=0; i<DISP_BUFFER_HIGH+1; i++)
		{
		for (j=0; j<DISP_BUFFER_WIDE; j++) disp_buffer[i][j] = 0;
		}
	}

const char* get_firmware_string(void) {
	return FIRMWARE_VERSION;
	}

uint16_t get_free_mem(uint8_t * prog, uint16_t max_mem)
	{
	uint16_t prog_len;
	prog_len = strlen(prog);
	return (max_mem-prog_len);
	}

//B_BAS008
uint8_t add_prog_line (int8_t * line, int8_t * prog, int16_t linenum)
    {
    uint8_t * prog_ptr=prog, * prog_ptr_prev, * prog_ptr_dest;
    int16_t linenum_now,linenum_prev=0,line_exp_len,cnt, prog_len;
    int8_t line_rest[50],line_exp[50],ret;
    sprintf(line_exp,"%d %s\n",linenum,line);
    line_exp_len = strlen(line_exp);
	prog_len = strlen(prog);
	if ((prog_len + line_exp_len)>BPROG_LEN) return 1;
    while (1)
	{
	ret = sscanf(prog_ptr,"%d %[^\n]s",&linenum_now,line_rest);
	if (ret==2)
	    {
	    if ((linenum>linenum_prev)&(linenum<linenum_now))
			{
			cnt = strlen(prog_ptr) +1;
			prog_ptr_dest = prog_ptr + line_exp_len;
			memmove(prog_ptr_dest,prog_ptr,cnt);
			memcpy(prog_ptr,line_exp,line_exp_len);
			return 0;
			}
	    if (linenum==linenum_now)
			{
			prog_ptr_prev = prog_ptr;
			prog_ptr = strchr(prog_ptr,'\n')+1;
			cnt = strlen(prog_ptr)+1;
			memmove(prog_ptr_prev,prog_ptr,cnt);
			if (strlen(line)>1)
				{
				prog_ptr = prog_ptr_prev;
				cnt = strlen(prog_ptr);
				prog_ptr_dest = prog_ptr + line_exp_len;
				memmove(prog_ptr_dest,prog_ptr,cnt+1);
				memcpy(prog_ptr,line_exp,line_exp_len);		
				}
			return 0;
			}
	    }
	if (ret==-1)
	    {
	    strcat(prog,line_exp);
	    return 0;
	    }
	linenum_prev = linenum_now;
	prog_ptr_prev = prog_ptr;
	prog_ptr = strchr(prog_ptr,'\n')+1;
	}
    }

//B_BAS007
uint8_t cmd_exec (int8_t * cmd)
    {
    int8_t cmd_clean[25];
    int32_t linenum,prognum;
    if (isdigit(cmd[0]))
		{
		sscanf(cmd,"%d %[^\n]s",&linenum,cmd_clean);
		add_prog_line (cmd_clean,bprog, linenum);
		}
    else
		{
		if (strcmp("help",cmd)==0)
			{
			stdio_write("help - this help\n");
			stdio_write("clr - clear program buffer\n");
			stdio_write("run - run program in buffer\n");
			stdio_write("list - list program buffer\n");
			}
		else if (strcmp("list",cmd)==0) stdio_write(bprog);
		else if (strcmp("clr",cmd)==0) bprog[0]=0;
		else if (strcmp("free",cmd)==0)
			{
			sprintf(stdio_buff,"%d B of memory free\n",get_free_mem(bprog,BPROG_LEN));
			stdio_write(stdio_buff);
			}	
		else if (strcmp("more",cmd)==0) list_more();
		else if (strncmp("load",cmd,4)==0)
			{
			sscanf (cmd+4,"%d",&prognum);
			if ((prognum>=0)&(prognum<BASIC_SAVNUM))
				{
				stdio_write("loading...");
				basic_load_program(bprog,prognum);
				stdio_write("OK\n");
				}
			}
		else if (strncmp("save",cmd,4)==0)
			{
			sscanf (cmd+4,"%d",&prognum);
			if ((prognum>=0)&(prognum<BASIC_SAVNUM))
				{
				stdio_write("saving...");
				basic_save_program(bprog,prognum);
				stdio_write("OK\n");
				}
			}	
		else if (strncmp("ssave",cmd,5)==0)
			{
			stdio_write("Transmitting via serial port...\n");
			i = basic_saves(bprog,BPROG_LEN);
			sprintf(stdio_buff,"\nOK, transmitted %d bytes.\n",i);
			stdio_write(stdio_buff);
			}	
		else if (strncmp("sload",cmd,5)==0)
			{
			stdio_write("Loading new program from serial port\n");
			stdio_write("Press BRK to exit...\n");
			serial_flush();
			handle_display = 0;
			tft_disp_buffer_refresh(disp_buffer,color_buffer);
			i = basic_loads(bprog,BPROG_LEN);
			handle_display = 1;
			sprintf(stdio_buff,"\nOK, received %d bytes.\n",i);
			stdio_write(stdio_buff);
			}	
		else if (strcmp("run",cmd)==0)
			{
			ubasic_init(bprog);
			brk_key = 0;
			do 
				{
				if (brk_key) 
					{
					brk_key = 0;
					stdio_write("\nBRK pressed\n");
					break;
					}
				if (!setjmp(jbuf)) ubasic_run();
				else
					{
					stdio_write("\nBASIC error\n");
					break;
					}
				} 	while(!ubasic_finished());
			handle_display = 1;
			stdio_write("\n");
			}
		else 
			{
			if (strlen(cmd)>0)
				{
				sprintf(tprog,"0 %s\n",cmd);
				ubasic_init(tprog);
				do 
					{
					if (!setjmp(jbuf)) ubasic_run();
					else
						{
						stdio_write("BASIC error\n");
						break;
						}
					} 	while(!ubasic_finished());	
				}		
			}
		}
    }


uint16_t basic_loads (int8_t * data, uint16_t maxlen)
	{
	uint8_t rx_char;
	uint16_t len=0;
	while (1)
		{
		if (rx_sta())
			{
			rx_char = rx_read();
			if ((rx_char>=' ')|(rx_char==NEWLINE)|(rx_char==0))
				data[len++] = rx_char;
			if (rx_char==0) return len;
			if (len==maxlen) return len;
			}
		if (brk_key) 
			{
			if (len>0) data[len] = 0;
			brk_key = 0;
			return len;
			}
		}
	return 0;
	}

uint16_t basic_saves (int8_t * data, uint16_t maxlen)
	{
	uint16_t len=0;
	while (1)
		{
		tx_write(data[len]);
		if (data[len]==0) return len;
		if (len==maxlen) return len;
		len++;
		}
	return 0;
	}

//B_BAS011
uint8_t basic_save_program (uint8_t * data, uint8_t slot)
	{
	uint32_t  addr;
	addr = slot;
	addr = addr * 4096;
	addr = addr + BASIC_BASEADDR;
	fl_erase_4k(addr);
	fl_write_4k(addr,data);	
	return 1;
	}
//B_BAS012
uint8_t basic_load_program (uint8_t * data, uint8_t slot)
	{
	uint32_t  addr;
	addr = slot;
	addr = addr * 4096;
	addr = addr + BASIC_BASEADDR;
	fl_read_4k(addr,data);
	return 1;
	}

void list_more (void)
	{
	uint8_t retval;
	uint16_t list_cnt=0,list_nl_cnt=0;
	while (bprog[list_cnt]!=0)
		{
		if (bprog[list_cnt]==NEWLINE)
			list_nl_cnt++;
		if (list_nl_cnt==(DISP_BUFFER_HIGH-1))
			{
			stdio_c(NEWLINE);
			stdio_write("---hit any key for more, q to quit---");
			while (stdio_get(&retval)==0);
			stdio_c(NEWLINE);
			if (retval == 'q') return;
			list_nl_cnt = 0;
			video_clrscr();
			}
		stdio_c(bprog[list_cnt++]);
		}	
	}

//B_BDG003

//write null-terminated string to standard output
uint8_t stdio_write (int8_t * data)
	{
	if (stdio_src==STDIO_LOCAL)
		{
		while (*data!=0x00)
			{
			buf_enqueue (*data++);
			while (bufsize)
				receive_char(buf_dequeue());	
			}
		}
	else if (stdio_src==STDIO_TTY1)
		{
		while (*data!=0x00)
		tx_write(*data++);
		}
	}

//write one character to standard output
uint8_t stdio_c (uint8_t data)
	{
	int8_t tmp[3];
	if (stdio_src==STDIO_LOCAL)
		{
		tmp[0] = data;
		tmp[1] = 0;
		buf_enqueue (data);
		while (bufsize)
			receive_char(buf_dequeue());
		}
	else if (stdio_src==STDIO_TTY1)
		tx_write(data);
	}

//check, whether is there something to read from standard input
//zero is returned when empty, nonzero when character is available
int8_t stdio_get_state (void)
	{
	if (stdio_local_buffer_state()!=0)
		return 1;
	if (stdio_src==STDIO_LOCAL)
		return term_k_stat();
	else if (stdio_src==STDIO_TTY1)
		return rx_sta();
	}
//get character from stdio
//zero when there is nothing to read
int8_t stdio_get (int8_t * dat)
	{
	if (stdio_local_buffer_state()!=0)
		{
		*dat = stdio_local_buffer_get();
		return 1;
		}
	if (stdio_src==STDIO_LOCAL)
		{
		return term_k_char(dat);
		}
	else if (stdio_src==STDIO_TTY1)
		{
		if (rx_sta()!=0)
			{
			*dat=rx_read();
			return 1;
			}
		else
			return 0;
		}
	return 0;
	}


int8_t term_k_stat (void)
	{
	uint8_t key_len;
	IEC0bits.T2IE = 0;
	key_len = key_buffer_ptr;
	IEC0bits.T2IE = 1;
	if (key_len == 0)
		return 0;
	else 
		return 1;
	}

int8_t term_k_char (int8_t * out)
	{
	uint8_t retval;
	IEC0bits.T2IE = 0;
	retval = key_buffer_ptr;
	if (key_buffer_ptr>0)
		{
		strncpy(out,key_buffer,key_buffer_ptr);
		key_buffer_ptr = 0;
		}
	IEC0bits.T2IE = 1;
	return retval;
	}


void boot_animation(void)
	{
	handle_display = 0; //Shut off auto-scanning of character buffer
	animate_splash();
	uint16_t waitfor = ticks+1000;	//Wait for 1 second
	while (ticks<waitfor) { ;; }
	tft_fill_area(0,0,320,240,0x000000);    //Make display black
	handle_display = 1; //Go back to character display
}

uint8_t stdio_local_buffer_state (void)
	{
	if (stdio_local_len>0) return 1;
	else return 0;
	}

int8_t stdio_local_buffer_get (void)
	{
	int8_t retval=0;
	if (stdio_local_len>0)
		{
		retval = stdio_local_buff[0];
		for (i=1;i<STDIO_LOCAL_BUFF_SIZE;i++) stdio_local_buff[i-1] = stdio_local_buff[i];
		stdio_local_buff[STDIO_LOCAL_BUFF_SIZE-1]=0;
		stdio_local_len--;
		}
	return retval;
	}

void stdio_local_buffer_put (int8_t data)
	{
	if (stdio_local_len<(STDIO_LOCAL_BUFF_SIZE-1))
		stdio_local_buff[stdio_local_len++] = data;
	}

void stdio_local_buffer_puts (int8_t * data)
	{
	while (*data!=0) stdio_local_buffer_put(*data++);
	}

//************************************************************************
//some hardware stuff


//B_BDG003
void __ISR(_TIMER_5_VECTOR, IPL3AUTO) Timer5Handler(void)
{
    uint8_t key_temp;
    IFS0bits.T5IF = 0;
	loop_badge();
    if (handle_display)
		{
		tft_disp_buffer_refresh_part(disp_buffer,color_buffer);
		//tft_disp_buffer_refresh_part(disp_buffer,0xFFFFFF,0);
		}
    key_temp = keyb_tasks();
    if (key_temp>0)
		{
		key_buffer[key_buffer_ptr++] = key_temp;
		}
}

void __ISR(_TIMER_1_VECTOR, IPL4AUTO) Timer1Handler(void)
	{
    IFS0bits.T1IF = 0;
    ++ticks;
	}
void __ISR(_EXTERNAL_2_VECTOR, IPL4AUTO) Int2Handler(void)
	{
	IEC0bits.INT2IE = 0;
	}


int16_t get_user_value (void)
	{
	int8_t temp_arr[20];
	uint8_t temp_arr_p=0,char_val,stat;
	uint32_t retval;
	stdio_write(" :");
	while (1)
		{
		stat = stdio_get(&char_val);
		if ((char_val!=NEWLINE)&(stat!=0))
			{
			stdio_c(char_val);
			if (char_val>=' ') temp_arr[temp_arr_p++] = char_val;
			else if (char_val==BACKSPACE)
				{
				if (temp_arr_p>0) temp_arr[--temp_arr_p]=0;
				}
			}
	    if ((char_val==NEWLINE)&(stat!=0))
			{
			temp_arr[temp_arr_p] = 0;
			sscanf(temp_arr,"%d",&retval);
			stdio_c('\n');
			return retval;
			}
		if (brk_key) return 0;
		}
	}



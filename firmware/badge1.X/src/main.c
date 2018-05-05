#include <xc.h>
#include "basic/ubasic.h"
#include <plib.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "hw.h"
#include <setjmp.h>
#include "Z80/sim.h"
#include "Z80/simglb.h"
#include "Z80/hwz.h"
#include "vt100.h"
#include "menu.h"
#include "splash.h"
#include "tetrapuzz.h"

//Set SHOW_SPLASH to 0 to skip splash screen at boot
#define SHOW_SPLASH	0

/*
char bprog[4097] =
"\
1 tune 50,54,57,500\n\
2 tune 52,55,59,500\n\
2 tune 50,54,57,500\n\
5 a=0\n\
6 b=0\n\
10 print a\n\
11 print b\n\
12 a=a+1\n\
13 if a>15 then gosub 100\n\
14 color a,b\n\
20 wait 500\n\
30 goto 10\n\
100 a=0\n\
110 b=b+1\n\
120 return\n\
";
*/

char bprog[4097] =
"10 tune 50,66,76,149\n\
20 tune 50,66,76,149\n\
30 tune 0,0,0,149\n\
40 tune 50,66,76,149\n\
50 tune 0,0,0,149\n\
60 tune 50,66,72,149\n\
70 tune 50,66,76,149\n\
80 tune 0,0,0,149\n\
90 tune 67,71,79,149\n\
100 tune 0,0,0,447\n\
110 tune 0,55,67,149\n\
120 tune 0,0,0,447\n\
130 gosub 670\n\
140 gosub 810\n\
150 gosub 670\n\
160 gosub 810\n\
170 gosub 960\n\
180 gosub 1130\n\
190 gosub 960\n\
200 tune 48,0,0,149\n\
210 tune 0,0,0,149\n\
221 tune 56,68,75,149\n\
230 tune 0,0,0,298\n\
240 tune 58,65,74,149\n\
250 tune 0,0,0,298\n\
260 tune 60,64,72,149\n\
270 tune 0,0,0,298\n\
280 tune 55,0,0,149\n\
290 tune 55,0,0,149\n\
300 tune 0,0,0,149\n\
310 tune 48,0,0,149\n\
320 tune 0,0,0,149\n\
330 gosub 960\n\
340 gosub 1130\n\
350 gosub 960\n\
360 tune 48,0,0,149\n\
370 tune 0,0,0,149\n\
380 tune 56,68,75,149\n\
390 tune 0,0,0,298\n\
400 tune 56,65,74,149\n\
410 tune 0,0,0,298\n\
420 tune 60,64,72,149\n\
430 tune 0,0,0,298\n\
440 tune 55,0,0,149\n\
450 tune 55,0,0,149\n\
460 tune 0,0,0,149\n\
470 tune 48,0,0,149\n\
480 tune 0,0,0,149\n\
490 gosub 1300\n\
500 tune 44,68,72,149\n\
510 tune 0,68,72,149\n\
520 tune 0,0,0,149\n\
530 tune 51,68,72,149\n\
540 tune 0,0,0,149\n\
550 tune 0,68,72,149\n\
560 tune 56,70,74,149\n\
570 tune 0,67,76,149\n\
580 tune 55,0,0,149\n\
590 tune 0,0,0,298\n\
600 tune 48,0,0,149\n\
610 tune 0,0,0,298\n\
620 tune 43,0,0,149\n\
630 tune 0,0,0,149\n\
640 gosub 1300\n\
650 goto 10\n\
670 tune 55,64,72,149\n\
680 tune 0,0,0,298\n\
690 tune 52,60,67,149\n\
700 tune 0,0,0,298\n\
710 tune 48,55,64,149\n\
720 tune 0,0,0,298\n\
730 tune 53,60,69,149\n\
740 tune 0,0,0,149\n\
750 tune 55,62,71,149\n\
760 tune 0,0,0,149\n\
770 tune 54,61,70,149\n\
780 tune 53,60,69,149\n\
790 tune 0,0,0,149\n\
800 return\n\
810 tune 52,60,67,198\n\
820 tune 60,67,76,198\n\
830 tune 64,71,79,198\n\
840 tune 65,72,81,149\n\
850 tune 0,0,0,149\n\
860 tune 62,69,77,149\n\
870 tune 64,71,79,149\n\
880 tune 0,0,0,149\n\
890 tune 60,69,76,149\n\
900 tune 0,0,0,149\n\
910 tune 57,64,72,149\n\
920 tune 59,65,74,149\n\
930 tune 55,62,71,149\n\
940 tune 0,0,0,298\n\
950 return\n\
960 tune 48,0,0,149\n\
970 tune 0,0,0,149\n\
980 tune 0,76,79,149\n\
990 tune 55,75,78,149\n\
1000 tune 0,74,77,149\n\
1010 tune 0,71,75,149\n\
1020 tune 60,0,0,149\n\
1030 tune 0,72,76,149\n\
1040 tune 53,0,0,149\n\
1050 tune 0,64,68,149\n\
1060 tune 0,65,69,149\n\
1070 tune 60,67,72,149\n\
1080 tune 60,0,0,149\n\
1090 tune 0,60,69,149\n\
1100 tune 53,64,72,149\n\
1110 tune 0,65,74,149\n\
1120 return\n\
1130 tune 48,0,0,149\n\
1140 tune 0,0,0,149\n\
1150 tune 0,76,79,149\n\
1160 tune 52,75,78,149\n\
1170 tune 0,74,77,149\n\
1180 tune 0,71,75,149\n\
1190 tune 55,0,0,149\n\
1200 tune 60,72,76,149\n\
1210 tune 0,0,0,149\n\
1220 tune 77,79,84,149\n\
1230 tune 0,0,0,149\n\
1240 tune 77,79,84,149\n\
1250 tune 77,79,84,149\n\
1260 tune 0,0,0,149\n\
1270 tune 55,0,0,149\n\
1280 tune 0,0,0,149\n\
1290 return\n\
1300 tune 44,68,72,149\n\
1310 tune 0,68,72,149\n\
1320 tune 0,0,0,149\n\
1330 tune 51,68,72,149\n\
1340 tune 0,0,0,149\n\
1350 tune 0,68,72,149\n\
1360 tune 56,70,74,149\n\
1370 tune 0,0,0,149\n\
1380 tune 55,67,76,149\n\
1390 tune 0,64,72,149\n\
1400 tune 0,0,0,149\n\
1410 tune 48,64,69,149\n\
1420 tune 0,60,67,149\n\
1430 tune 0,0,0,149\n\
1440 tune 43,0,0,149\n\
1450 tune 0,0,0,149\n\
1460 return\n";

int prog_ptr;
char tprog[100];

void delay_us (unsigned long howmuch);
unsigned char add_prog_line (char * line, char * prog, int linenum);

void terminal_init(void);
unsigned char terminal_tasks (char * input, UINT16 * shift);
char term_k_stat (void);
char term_k_char (char * out);
unsigned char rx_sta (void);
unsigned char rx_read (void);
void tx_write (unsigned char data);
unsigned char cmd_exec (char * cmd);
unsigned char basic_save_program (unsigned char * data, unsigned char slot);
unsigned char basic_load_program (unsigned char * data, unsigned char slot);

char term_buffer[TBUF_LEN];
char term_screen_buffer[TERM_WIDTH*TERM_LINES];
//a lot of magic numbers here, should be done properly
char stdio_buff[25];
char term_input[50],term_input_p,term_key_stat_old;
UINT16 term_pointer,vertical_shift;
char key_buffer[10];
char stdio_src;
unsigned char key_buffer_ptr =0;

char disp_buffer[DISP_BUFFER_HIGH+1][DISP_BUFFER_WIDE];
char color_buffer[DISP_BUFFER_HIGH+1][DISP_BUFFER_WIDE];

int i,j,len;
unsigned char get_stat;
volatile char brk_key;
unsigned char cmd_line_buff[30], cmd_line_pointer,cmd_line_key_stat_old,prompt;

jmp_buf jbuf;
char char_out;
extern unsigned char flash_buff[4096];
extern const unsigned char ram_image[65536];
extern unsigned char ram_disk[RAMDISK_SIZE];

void init_z80_cpm (void);
void init_basic (void);
void init_userprog(void);
void loop_z80_cpm (void);
void loop_basic (void);
void loop_userprog(void);
void boot_animation(void);
void init_8080_basic (void);
void loop_8080_basic (void);
void clr_buffer(void);
void loop_badge(void);
void enable_display_scanning(unsigned char onoff);
uint32_t millis(void);

unsigned char flash_init = 0;
unsigned char handle_display = 1;

extern volatile uint16_t bufsize;
volatile uint32_t ticks;	// millisecond timer incremented in ISR

extern const unsigned char b2_rom[2048];
extern const unsigned char ram_init [30];

int main(void)
	{
    ticks = 0;
	start_after_wake = &wake_return; //Function pointer for waking from sleep
	hw_init();
	stdio_src = STDIO_LOCAL;
//	stdio_src = STDIO_TTY1;
	term_init();
	if (SHOW_SPLASH)
		boot_animation();
	if (flash_init==1)
		init_first_x_sects(32);
	menu();
	}


//housekeeping stuff. call this function often
void loop_badge(void)
	{
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
		brk_key = 1;
		}
	}

void enable_display_scanning(unsigned char onoff)
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
	for (i=0;i<RAMDISK_SIZE;i++) ram_disk[i] = 0xE5;
	wrk_ram	= PC = STACK = ram;
	init_io(IO_CPM_MODE);
	}

void loop_z80_cpm (void)
	{
	cpu_error = NONE;
	cpu();	
	}

void init_basic (void)
	{
	stdio_write("BASIC interpreter, type help for help\n");
	prompt = 1;
	brk_key = 0;
	cmd_line_pointer=0;
	cmd_line_buff[0] = 0;
	video_set_color(15,0);
	}

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
					cmd_line_buff[cmd_line_pointer--]=0;
				}

			}
	    }	
	}

void init_userprog (void)
{
    clr_buffer();
    stdio_write("Press any key to show splash screen.\n");
}

void loop_userprog (void)
{
    get_stat = stdio_get(&char_out);
    if (get_stat!=0)
    {
	handle_display = 0; //Shut off auto-scanning of character buffer
	animate_splash();
	//show_splash();
	while(stdio_get(&char_out) == 0) { ;; }  //wait for button press
	handle_display = 1; //Go back to character display
    }

    static unsigned int delay_until = 0;
    static unsigned char count = '0';

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
	for (j=0; j<DISP_BUFFER_WIDE; j++)
	{
	    disp_buffer[i][j] = 0;
	}
    }
}

unsigned char add_prog_line (char * line, char * prog, int linenum)
    {
    unsigned char * prog_ptr=prog, * prog_ptr_prev, * prog_ptr_dest;
    int linenum_now,linenum_prev=0,line_exp_len,cnt;
    char line_rest[50],line_exp[50],ret;
    sprintf(line_exp,"%d %s\n",linenum,line);
    line_exp_len = strlen(line_exp);
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
				memmove(prog_ptr_dest,prog_ptr,cnt);
				memcpy(prog_ptr,line_exp,line_exp_len);		
				}
			return 0;
			}
	    }
	else
	    {
	    strcat(prog,line_exp);
	    return 0;
	    }
	linenum_prev = linenum_now;
	prog_ptr_prev = prog_ptr;
	prog_ptr = strchr(prog_ptr,'\n')+1;
	}
    }


unsigned char cmd_exec (char * cmd)
    {
    char cmd_clean[25];
    int linenum,prognum;
    if (isdigit(cmd[0]))
		{
		sscanf(cmd,"%d %[^\n]s",&linenum,cmd_clean);
		add_prog_line (cmd_clean,bprog, linenum);
	//	sprintf(stdio_buff,"cmd L %d %s\n",linenum,cmd_clean);
	//	stdio_write(stdio_buff);
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
	else if (strcmp("list",cmd)==0)
	    {
	    stdio_write(bprog);
	    }
	else if (strcmp("clr",cmd)==0)
	    {
	    bprog[0]=0;
	    }
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
	else if (strcmp("run",cmd)==0)
	    {
	    ubasic_init(bprog);
	    do 
		    {
			if (brk_key) 
				{
				brk_key = 0;
				stdio_write("BRK pressed\n");
				break;
				}
		    if (!setjmp(jbuf))
				{
				ubasic_run();
				}
			else
				{
				stdio_write("BASIC error\n");
				break;
				}
		    } 	while(!ubasic_finished());	
	    stdio_write("\n");
	    }
	else 
	    {
		if (strlen(cmd)>0)
			{
			sprintf(tprog,"10 %s\n",cmd);
			ubasic_init(tprog);
			do 
				{
				if (!setjmp(jbuf))
					{
					ubasic_run();
					}
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


unsigned char basic_save_program (unsigned char * data, unsigned char slot)
	{
	unsigned long addr;
	addr = slot;
	addr = addr * 4096;
	addr = addr + BASIC_BASEADDR;
	fl_erase_4k(addr);
	fl_write_4k(addr,data);	
	return 1;
	}

unsigned char basic_load_program (unsigned char * data, unsigned char slot)
	{
	unsigned long addr;
	addr = slot;
	addr = addr * 4096;
	addr = addr + BASIC_BASEADDR;
	fl_read_4k(addr,data);
	return 1;
	}


void __ISR(_TIMER_2_VECTOR, ipl6) Timer2Handler(void)
	{
    IFS0bits.T2IF = 0;
	GEN_0_PIN = ~ GEN_0_PIN;
	}
void __ISR(_TIMER_3_VECTOR, ipl6) Timer3Handler(void)
	{
    IFS0bits.T3IF = 0;
	GEN_1_PIN = ~ GEN_1_PIN;
	}
void __ISR(_TIMER_4_VECTOR, ipl6) Timer4Handler(void)
	{
    IFS0bits.T4IF = 0;
	GEN_2_PIN = ~ GEN_2_PIN;
	}

void __ISR(_TIMER_5_VECTOR, ipl3) Timer5Handler(void)
{
    unsigned char key_temp;
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

void __ISR(_TIMER_1_VECTOR, ipl4) Timer1Handler(void)
{
    IFS0bits.T1IF = 0;
    ++ticks;
}

void __ISR(_EXTERNAL_2_VECTOR, ipl4) Int2Handler(void)
	{
	IEC0bits.INT2IE = 0;
	}

//*****************************************************************************/


unsigned char rx_sta (void)
{
volatile int u_sta;
if (U3STAbits.URXDA==1) 
	{
		u_sta++;
		return 0xFF;
	}
	else 
		return 0x00;
}
unsigned char rx_read (void)
{
	unsigned char data;
//	if (uart_buffer_ptr<500)
//		uart_buffer[uart_buffer_ptr++] = data;
	data = U3RXREG;
	return data;
}
void tx_write (unsigned char data)
{
    
U3TXREG = data;
while (U3STAbits.UTXBF==1);
    
}

unsigned char stdio_write (char * data)
{
if (stdio_src==STDIO_LOCAL)
	{
//jar
//	terminal_tasks(data,&vertical_shift);
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

unsigned char stdio_c (unsigned char data)
{
char tmp[3];
if (stdio_src==STDIO_LOCAL)
	{
	tmp[0] = data;
	tmp[1] = 0;
//jar
//	terminal_tasks(tmp,&vertical_shift);
	buf_enqueue (data);
	while (bufsize)
		receive_char(buf_dequeue());
	}
else if (stdio_src==STDIO_TTY1)
	tx_write(data);
}

char stdio_get_state (void)
	{
	if (stdio_src==STDIO_LOCAL)
		return term_k_stat();
	else if (stdio_src==STDIO_TTY1)
		return rx_sta();
	}

char stdio_get (char * dat)
{
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

char term_k_stat (void)
{
    unsigned char key_len;
IEC0bits.T2IE = 0;
key_len = key_buffer_ptr;
IEC0bits.T2IE = 1;
if (key_len == 0)
	return 0;
else 
	return 1;
}

char term_k_char (char * out)
{
unsigned char retval;
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


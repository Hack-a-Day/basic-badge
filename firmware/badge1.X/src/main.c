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
#include "menu.h"
#include "splash.h"
#include "tetrapuzz.h"

//Badge firmware version should be defined as a string here:
#define FIRMWARE_VERSION "0.43"

//Set SHOW_SPLASH to 0 to skip splash screen at boot
#define SHOW_SPLASH	0

int8_t bprog[4097] =
"1 poinless long program\n\
2 to sport MORE function\n\
3 chr 205\n\
4 chr 205\n\
5 setxy 2,2\n\
6 chr 206\n\
7 setxy 4,4\n\
8 chr 215\n\
9 wait 1000\n\
10 clrscr \n\
11 chr 205\n\
12 chr 205\n\
13 chr 205\n\
14 setxy 2,2\n\
15 chr 206\n\
16 setxy 4,4\n\
17 chr 215\n\
18 wait 1000\n\
19 clrscr \n\
20 chr 205\n\
21 chr 205\n\
22 chr 205\n\
23 setxy 2,2\n\
24 chr 206\n\
25 setxy 4,4\n\
26 chr 215\n\
27 wait 1000\n\
28 clrscr \n\
29 chr 205\n\
30 chr 205\n\
31 chr 205\n\
32 setxy 2,2\n\
33 chr 206\n\
34 setxy 4,4\n\
35 chr 215\n\
36 wait 1000\n\
37 clrscr \n\
38 chr 205\n\
39 chr 205\n\
40 chr 205\n\
41 setxy 2,2\n\
42 chr 206\n\
43 setxy 4,4\n\
44 chr 215\n\
45 wait 1000\n\
46 clrscr \n\
";

int16_t prog_ptr;
int8_t tprog[100];

void delay_us (uint32_t  howmuch);
uint8_t add_prog_line (int8_t * line, int8_t * prog, int16_t linenum);

void terminal_init(void);
uint8_t terminal_tasks (int8_t * input, uint16_t * shift);
int8_t term_k_stat (void);
int8_t term_k_char (int8_t * out);
uint8_t rx_sta (void);
uint8_t rx_read (void);
void tx_write (uint8_t data);
uint8_t cmd_exec (int8_t * cmd);
uint8_t basic_save_program (uint8_t * data, uint8_t slot);
uint8_t basic_load_program (uint8_t * data, uint8_t slot);

int8_t term_buffer[TBUF_LEN];
int8_t term_screen_buffer[TERM_WIDTH*TERM_LINES];
//a lot of magic numbers here, should be done properly
int8_t stdio_buff[25];
int8_t term_input[50],term_input_p,term_key_stat_old;
uint16_t term_pointer,vertical_shift;
int8_t key_buffer[10];
int8_t stdio_src;
uint8_t key_buffer_ptr =0;

int8_t disp_buffer[DISP_BUFFER_HIGH+1][DISP_BUFFER_WIDE];
int8_t color_buffer[DISP_BUFFER_HIGH+1][DISP_BUFFER_WIDE];

int32_t i,j,len;
uint8_t get_stat;
volatile int8_t brk_key;
uint8_t cmd_line_buff[30], cmd_line_pointer,cmd_line_key_stat_old,prompt;

jmp_buf jbuf;
int8_t char_out;
extern uint8_t flash_buff[4096];
extern const uint8_t ram_image[65536];
extern uint8_t ram_disk[RAMDISK_SIZE];

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
void enable_display_scanning(uint8_t onoff);
uint32_t millis(void);
void list_more (void);


uint8_t flash_init = 0;
uint8_t handle_display = 1;

extern volatile uint16_t bufsize;
volatile uint32_t ticks;	// millisecond timer incremented in ISR

extern const uint8_t b2_rom[2048];
extern const uint8_t ram_init [30];







int16_t main(void)
	{

    ticks = 0;
	start_after_wake = &wake_return; //Function pointer for waking from sleep
	hw_init();
					
	stdio_src = STDIO_LOCAL;
//	stdio_src = STDIO_TTY1;
	term_init();
	
	if (flash_init==1)
		init_first_x_sects(32);	
	if (SHOW_SPLASH)
		boot_animation();
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
	for (j=0; j<DISP_BUFFER_WIDE; j++)
	{
	    disp_buffer[i][j] = 0;
	}
    }
}

const char* get_firmware_string(void) {
	return FIRMWARE_VERSION;
	}

uint8_t add_prog_line (int8_t * line, int8_t * prog, int16_t linenum)
    {
    uint8_t * prog_ptr=prog, * prog_ptr_prev, * prog_ptr_dest;
    int16_t linenum_now,linenum_prev=0,line_exp_len,cnt;
    int8_t line_rest[50],line_exp[50],ret;
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
				memmove(prog_ptr_dest,prog_ptr,cnt+1);
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


uint8_t cmd_exec (int8_t * cmd)
    {
    int8_t cmd_clean[25];
    int32_t linenum,prognum;
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
	else if (strcmp("more",cmd)==0)
	    {
	    list_more();
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


void __ISR(_TIMER_5_VECTOR, ipl3) Timer5Handler(void)
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


uint8_t rx_sta (void)
{
volatile int16_t u_sta;
if (U3STAbits.URXDA==1) 
	{
		u_sta++;
		return 0xFF;
	}
	else 
		return 0x00;
}
uint8_t rx_read (void)
{
	uint8_t data;
//	if (uart_buffer_ptr<500)
//		uart_buffer[uart_buffer_ptr++] = data;
	data = U3RXREG;
	return data;
}
void tx_write (uint8_t data)
{
    
U3TXREG = data;
while (U3STAbits.UTXBF==1);
    
}

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

int8_t stdio_get_state (void)
	{
	if (stdio_src==STDIO_LOCAL)
		return term_k_stat();
	else if (stdio_src==STDIO_TTY1)
		return rx_sta();
	}

int8_t stdio_get (int8_t * dat)
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


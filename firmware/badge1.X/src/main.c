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

#include "splash.h"

//Set SHOW_SPLASH to 0 to skip splash screen at boot
#define SHOW_SPLASH	0

/* parameters for tune - note1(see below), note2(see below), note3(see below), duration (in ms)
 * 0 - no sound
 * 1 - G0
 * 2 - G#1
 * 3 - A1 (55Hz)
 * ...
 * 12 - F#1
 * 13 - G1
 * 14 - G$1
 * 15 - A2 (110Hz)
 * ...
 * 39 - A4 (440Hz)
 * ...
 * 61 - G6
 * 62 - G#6
 * 63 - A6 (1760Hz)
 */

char bprog[1000] =
"100 tune 25,0,44,120\n\
102 tune 32,41,44,100\n\
104 tune 0,0,44,20\n\
106 tune 32,41,44,100\n\
108 tune 0,0,44,20\n\
110 tune 20,0,41,120\n\
112 tune 29,37,41,100\n\
114 tune 0,0,41,20\n\
116 tune 29,37,41,100\n\
118 tune 29,37,41,20\n\
120 tune 0,0,37,120\n\
122 tune 17,0,37,100\n\
124 tune 25,32,37,20\n\
126 tune 0,0,37,100\n\
128 tune 25,32,37,20\n\
130 tune 0,0,32,120\n\
132 tune 13,0,32,100\n\
134 tune 25,29,32,20\n\
136 tune 0,0,32,100\n\
138 tune 25,29,32,20\n\
140 tune 0,0,34,120\n\
150 tune 0,0,36,120\n\
160 tune 0,0,37,120\n\
170 tune 0,0,34,240\n\
180 tune 0,0,37,120\n\
190 tune 0,0,32,600\n\
200 tune 0,0,0,120\n\
210 tune 0,0,39,360\n\
220 tune 0,0,44,360\n\
230 tune 0,0,41,360\n\
240 tune 0,0,37,360\n\
250 tune 0,0,34,120\n\
260 tune 0,0,36,120\n\
270 tune 0,0,37,120\n\
280 tune 0,0,39,240\n\
290 tune 0,0,41,120\n\
300 tune 0,0,39,480\n\
310 tune 0,0,0,5\n\
";
/*
char bprog[1000] =
"10 gosub 100\n\
20 for i = 1 to 5\n\
30 print i\n\
40 next i\n\
50 print \"end\"\n\
60 end\n\
100 print \"subroutine\"\n\
105 out 5\n\
110 return\n";
*/

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

int i,j,len;
unsigned char get_stat;
char sstr[3];
unsigned char cmd_line_buff[30], cmd_line_pointer,cmd_line_key_stat_old,prompt;

jmp_buf jbuf;

extern unsigned char flash_buff[4096];
extern const unsigned char ram_image[65536];
extern unsigned char ram_disk[RAMDISK_SIZE];

void init_z80 (void);
void init_basic (void);
void init_userprog(void);
void loop_z80 (void);
void loop_basic (void);
void loop_userprog(void);
void boot_animation(void);

void clr_buffer(void);

unsigned char flash_init = 0;
unsigned char handle_display = 1;

extern volatile uint16_t bufsize;

extern volatile uint32_t ticks;	// millisecond timer incremented in ISR

volatile int a,b,c,d;


int main(void)
{
    ticks = 0;
	hw_init();
	stdio_src = STDIO_LOCAL;
//	stdio_src = STDIO_TTY1;
	term_init();
	
	if (flash_init==1)
		init_first_x_sects(32);

	if (SHOW_SPLASH)
		boot_animation();

	stdio_write("\nBelegrade badge version 0.17\n");
	stdio_write("Type your choice and hit ENTER\n");
	stdio_write("1 - BASIC interpreter\n");
	stdio_write("2 - CP/M @ Z80\n");
	stdio_write("3 - User Program\n");
	while (1)
		{
		get_stat = stdio_get(sstr);
		if (get_stat!=0)
			{
			stdio_write(sstr);	
			if (sstr[0]==0x0A) 
				{
				cmd_line_buff[cmd_line_pointer] = 0;
				if (strcmp(cmd_line_buff,"1")==0)
					{
					init_basic();
					while (1) loop_basic();
					}
				if (strcmp(cmd_line_buff,"2")==0)
					{
					init_z80();
					while (1) loop_z80();
					}			
				if (strcmp(cmd_line_buff,"3")==0)
					{
					init_userprog();
					while (1) loop_userprog();
					}			
				}
			else
				{
				len = strlen(sstr);
				for (i=0;i<len;i++)
					{
					if (sstr[i]>0x1F) cmd_line_buff[cmd_line_pointer++] = sstr[i];
					else if (sstr[i]==BACKSPACE)
						{
						if (cmd_line_pointer>0) cmd_line_buff[cmd_line_pointer--]=0;
						}
					}			
				}
			}	
		}
	}

void init_z80 (void)
	{
#ifdef	USE_RAM_IMAGE	
	for (i=0;i<65536;i++) ram[i] = ram_image[i];
#endif	
	for (i=0;i<51200;i++) ram_disk[i] = 0xE5;
	wrk_ram	= PC = STACK = ram;
	}

void loop_z80 (void)
	{
	cpu_error = NONE;
	cpu();	
	}

void init_basic (void)
	{
	stdio_write("BASIC interpreter, type help for help\n");
	prompt = 1;
	cmd_line_pointer=0;
	cmd_line_buff[0] = 0;
	}

void loop_basic (void)
	{
	if (prompt==1)
	    {
	    stdio_write(">");	
	    prompt = 0;
	    }
	get_stat = stdio_get(sstr);
	if (get_stat!=0)
	    {
	    stdio_write(sstr);	
	    if (sstr[0]==0x0A) 
			{
			cmd_line_buff[cmd_line_pointer] = 0;
			cmd_exec (cmd_line_buff);
			cmd_line_pointer=0;
			cmd_line_buff[0] = 0;
			prompt = 1;			
			}
	    else
		{
		len = strlen(sstr);
		for (i=0;i<len;i++)
		    {
		    if (sstr[i]>0x1F) cmd_line_buff[cmd_line_pointer++] = sstr[i];
		    else if (sstr[i]==BACKSPACE)
				{
				if (cmd_line_pointer>0) 
					cmd_line_buff[cmd_line_pointer--]=0;
				}
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
    get_stat = stdio_get(sstr);
    if (get_stat!=0)
    {
	handle_display = 0; //Shut off auto-scanning of character buffer
	animate_splash();
	//show_splash();
	while(stdio_get(sstr) == 0) { ;; }  //wait for button press
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

void boot_animation(void)
	{
	handle_display = 0; //Shut off auto-scanning of character buffer
	animate_splash();
	uint16_t waitfor = ticks+1000;	//Wait for 1 second
	while (ticks<waitfor) { ;; }
	tft_fill_area(0,0,320,240,0x000000);    //Make display black
	handle_display = 1; //Go back to character display
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
    int linenum;
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
	else if (strcmp("run",cmd)==0)
	    {
	    ubasic_init(bprog);
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
	    stdio_write("\n");
	    }
	else 
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
    if (handle_display)
    {
	tft_disp_buffer_refresh_part(disp_buffer,0xFFFFFF,0);
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

unsigned char stdio_write (unsigned char * data)
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
		*dat++=rx_read();
		*dat++=0;
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



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

/*
char bprog[1000] =
"10 print 1+1\n"\
;
*/

int prog_ptr;

//char bprog[1000] = "print 1+7\n";
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
char stdio_buff[25];
char term_input[50],term_input_p,term_key_stat_old;
UINT16 term_pointer,vertical_shift;
char stdio_src;

char key_buffer[10];
unsigned char key_buffer_ptr =0;


char disp_buffer[800];
/*
char uart_buffer[500];
int uart_buffer_ptr = 0;
*/
/*
static const char program[] =
"10 out 9\n\
20 let a=5\n\
30 print a+7\n\
";
*/
//static const char program[] = "print 1+7";


/*
static const char program[] =
"10 gosub 100\n\
20 for i = 1 to 10\n\
30 print i\n\
40 next i\n\
50 print \"end\"\n\
60 end\n\
100 print \"subroutine\"\n\
110 return\n";
*/

int i,j,len;
unsigned char get_stat;
char sstr[3];
unsigned char cmd_line_buff[30], cmd_line_pointer,cmd_line_key_stat_old,prompt;

jmp_buf buf;

extern unsigned char flash_buff[4096];
extern const unsigned char ram_image[65536];
extern unsigned char ram_disk[65536];

void init_z80 (void);
void init_basic (void);
void loop_z80 (void);
void loop_basic (void);

unsigned char flash_init = 0;

int main(void)
{
   hw_init();
	CS_FLASH = 1;
	for (i=0;i<40;i++)
		for (j=0;j<20;j++)	
			disp_buffer[i+(j*40)] = ' ';
    tft_disp_buffer_refresh(disp_buffer,0xFFFFFF,0);
	stdio_src = STDIO_LOCAL;
//	stdio_src = STDIO_TTY1;

	fl_rst_pb();

	if (flash_init==1)
		{
		init_first_x_sects(32);
		}

	stdio_write("\nBelegrade badge version 0.14\n");
	stdio_write("Type your choice and hit ENTER\n");
	stdio_write("1 - BASIC interpreter\n");
	stdio_write("2 - CP/M @ Z80\n");
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
			if (cmd_line_pointer>0) cmd_line_buff[cmd_line_pointer--]=0;
			}
		    }			
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
		    if (!setjmp(buf))
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
		    if (!setjmp(buf))
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

void __ISR(_TIMER_2_VECTOR, ipl3) Timer2Handler(void)
{
    unsigned char key_temp;
	IFS0bits.T2IF = 0;
    tft_disp_buffer_refresh_part(disp_buffer,0xFFFFFF,0);
    key_temp = keyb_tasks();
    if (key_temp>0)
        {
        key_buffer[key_buffer_ptr++] = key_temp;
        }
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
	terminal_tasks(data,&vertical_shift);
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
	terminal_tasks(tmp,&vertical_shift);
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

//**********************************************************************************************
//**********************************************************************************************
unsigned char term_shift_out (char * dbuff, char * tbuff);

void terminal_init(void)
{
term_pointer = 0;
}

unsigned char terminal_tasks (char * input, UINT16 * shift)
{
unsigned char len,i;
UINT16 sh;
sh = *shift;
if (input!=0)
	{
	len = strlen(input);
	for (i=0;i<len;i++)
		{
		if (input[i]>(' '-1))
			term_screen_buffer[term_pointer++] = input[i];
		else if (input[i]==BACKSPACE) 
			{
			if (term_pointer>0)
				term_screen_buffer[--term_pointer] = ' ';
			}
		else if ((input[i]==K_UP)|(input[i]==K_DN)|(input[i]==K_LT)|(input[i]==K_RT))
			{
			switch (input[i])
				{
				case K_UP: sh++; break;
				case K_DN: if (sh>0) sh--; break;
				}
			}
		else
			{
			while (term_pointer%TERM_WIDTH!=0)
				term_screen_buffer[term_pointer++] = ' ';
			}
		if (term_pointer==TERM_WIDTH*TERM_LINES)
			{
			term_shift_out(term_screen_buffer,term_buffer);
			term_pointer = term_pointer - TERM_WIDTH;
			}
		}
	}
if (sh<TERM_LINES)
	{
	memcpy(disp_buffer,term_buffer+TBUF_LEN-(TERM_WIDTH*sh),TERM_WIDTH*sh);
	memcpy(disp_buffer+TERM_WIDTH*sh,term_screen_buffer,TERM_WIDTH*(TERM_LINES-sh));
	}
else
	memcpy(disp_buffer,term_buffer+TBUF_LEN-(TERM_WIDTH*sh),TERM_WIDTH*sh);
*shift = sh;
}

unsigned char term_shift_out (char * dbuff, char * tbuff)
{
memmove (tbuff,tbuff+TERM_WIDTH,TBUF_LEN-TERM_WIDTH);
memcpy(tbuff+TBUF_LEN-TERM_WIDTH,dbuff,TERM_WIDTH);
memmove(dbuff,dbuff+TERM_WIDTH,TERM_WIDTH*(TERM_LINES-1));
memset(dbuff + TERM_WIDTH*(TERM_LINES-1),' ',TERM_WIDTH);
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



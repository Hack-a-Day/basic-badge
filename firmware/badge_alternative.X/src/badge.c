#include <xc.h>
#include "badge.h"
#include <plib.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include <setjmp.h>

/************ Defines ****************************/



//a lot of magic numbers here, should be done properly
int8_t key_buffer[10], stdio_local_buff[STDIO_LOCAL_BUFF_SIZE];

uint8_t get_stat,key_buffer_ptr =0;
uint8_t stdio_local_len=0;
int32_t i,j,len;
volatile uint8_t handle_display = 1;
volatile int8_t brk_key,stdio_src;
extern volatile uint16_t bufsize;
volatile uint32_t ticks;			// millisecond timer incremented in ISR

int8_t disp_buffer[DISP_BUFFER_HIGH+1][DISP_BUFFER_WIDE];
int8_t color_buffer[DISP_BUFFER_HIGH+1][DISP_BUFFER_WIDE];

void badge_init (void)
	{
	//B_BDG009
	ticks = 0;
	stdio_src = STDIO_LOCAL;
//	stdio_src = STDIO_TTY1;
	term_init();
	set_cursor_state(1);
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


void clr_buffer (void)
	{
	for (i=0; i<DISP_BUFFER_HIGH+1; i++)
		{
		for (j=0; j<DISP_BUFFER_WIDE; j++) 
			{
			disp_buffer[i][j] = 0;		//Blank the buffer
			color_buffer[i][j] = 0x0F;	//White text on black background
			}
		}
	}

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
    if (handle_display)
		tft_disp_buffer_refresh_part((uint8_t *)(disp_buffer),(uint8_t *)color_buffer);
    key_temp = keyb_tasks();
    if (key_temp>0)
		key_buffer[key_buffer_ptr++] = key_temp;
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

uint16_t get_user_value (void)
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


void display_refresh_force (void)
	{
	tft_disp_buffer_refresh((uint8_t *)disp_buffer,(uint8_t *)color_buffer);
	}
#ifndef	__BADGE_H
#define	__BADGE_H

#include <stdint.h>
#include "vt100.h"
#include "hw.h"
#include "disp.h"


void terminal_init(void);
int8_t term_k_stat (void);
int8_t term_k_char (int8_t * out);
uint8_t rx_sta (void);
uint8_t rx_read (void);
void tx_write (uint8_t data);

uint8_t stdio_local_buffer_state (void);
int8_t stdio_local_buffer_get (void);
void stdio_local_buffer_put (int8_t data);
void stdio_local_buffer_puts (int8_t * data);

uint16_t get_user_value (void);
void enable_display_scanning(uint8_t onoff);
uint32_t millis(void);
void display_refresh_force (void);
void clr_buffer (void);

#define STDIO_LOCAL_BUFF_SIZE	25

/***************************************************************************************************/

#endif

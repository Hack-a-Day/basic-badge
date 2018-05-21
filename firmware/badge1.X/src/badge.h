#include <stdint.h>

/* This is a 40x20 character display. Write a char to this buffer and it will appear on the screen */
#define DISP_BUFFER_WIDE    40
#define DISP_BUFFER_HIGH    20
int8_t disp_buffer[DISP_BUFFER_HIGH+1][DISP_BUFFER_WIDE];
int8_t color_buffer[DISP_BUFFER_HIGH+1][DISP_BUFFER_WIDE];
/***************************************************************************************************/

/**** Useful functions for user programs ****/
void clr_buffer(void);							//Fill character buffer with spaces (blank the screen)
void enable_display_scanning(uint8_t onoff);	//Enable automatic screen refresh of character display (disp_buffer[])]
uint32_t millis(void);							//Return number of milliseconds passed since power on
/********************************************/
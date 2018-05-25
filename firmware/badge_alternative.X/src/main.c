#include <xc.h>
#include "badge.h"
#include "hw.h"

uint8_t char_out,char_last;
int16_t main(void)
	{
	hw_init();
	badge_init();
	stdio_write("keyboard repeater\n");
	while (1)
		{
		uint8_t get_stat = stdio_get(&char_out);	//get key status
		if (get_stat!=0)
			{
			stdio_c(char_out);						//if there is anything in here, print it and play
			//major/diminished/augumented chord with minor or major triad, depending on current and prev key
			sound_play_notes(char_out>>1,3+char_out>>1+(char_out&1),6+char_out>>1+(char_last&0x03),100);
			//also, turn on/off LEDs depending on current key
			set_led(((char_out>>1)&0x03),char_out&0x01);
			video_set_color(char_last,char_out);	//set some funny color next time
			char_last = char_out;
			}
		}
	}


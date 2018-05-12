#include <xc.h>
#include "badge.h"
#include "hw.h"


extern int8_t bprog[BPROG_LEN+1];
int16_t main(void)
	{
	hw_init();
	badge_init();
	term_init();
	if (KEY_BRK==0)
		post();
	if (SHOW_SPLASH)
		boot_animation();
	badge_menu();
	while (1); //menu is never left, but anyway
	}


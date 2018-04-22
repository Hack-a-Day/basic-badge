#include "splash.h"

void show_splash(void)
{
    unsigned int i,j;
    tft_fill_area(0,0,320,240,0x000000);    //Make display black
    for (i=0; i<12; i++)
    {
	for (j=0; j<12; j++)
	{
	    if (b_cipher[i] & 1<<j) {
		unsigned int curX = CIPHER_X0+((12-j)*CIPHER_CHAR_WIDTH)-((j/3)*CIPHER_SPACE);
		unsigned char curY = CIPHER_Y0+(i*CIPHER_CHAR_WIDTH)+((i/3)*CIPHER_SPACE);
		//FIXME: Why do I need these -1 adjustments?
		tft_fill_area(curX,curY,CIPHER_CHAR_WIDTH-1,CIPHER_CHAR_WIDTH-1,CIPHER_COLOR);
	    }
	}
    }
}



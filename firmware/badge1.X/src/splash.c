#include "splash.h"
#include <stdint.h>

volatile uint32_t ticks;

struct Cipher_box
{
    int16_t x;	    // Pixel location on screen (negative values for left overflow)
    uint8_t column; // Which of 12 columns in the splash logo this belongs to
};

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

void draw_vert_line(unsigned int x, unsigned char y, unsigned char height, unsigned int color)
{
    tft_fill_area(x,y,0,height-1,color);
}

void move_box_right(struct Cipher_box *b)
{
    uint8_t y = CIPHER_Y0 + (b->column * CIPHER_CHAR_WIDTH)+((b->column / 3) * CIPHER_SPACE);
    //FIXME: This should be passed a box object
    if (b->x<320)
    {
	//Draw leading edge
	draw_vert_line(b->x,y,CIPHER_CHAR_WIDTH,0xFFFFFF);
    }
    if (b->x>=CIPHER_CHAR_WIDTH)
    {
	//Erase trailing edge
	//FIXME: erase with red if this is within the splash area
	draw_vert_line(b->x-CIPHER_CHAR_WIDTH,y,CIPHER_CHAR_WIDTH,0x000000);
    }
    
    b->x = b->x + 1;
}

void animate_splash(void)
{
    unsigned int x = 0;
    unsigned int splash_waitfor = 0;
    
    struct Cipher_box box0 = { 0, 0 };
    struct Cipher_box box1 = { 0, 0 };
    struct Cipher_box box2 = { 0-(CIPHER_CHAR_WIDTH), 2 };
    struct Cipher_box box3 = { 0, 0 };
    struct Cipher_box box4 = { 0-(2*CIPHER_CHAR_WIDTH), 4 };
    struct Cipher_box box5 = { 0, 0 };
    struct Cipher_box box6 = { 0-(3*CIPHER_CHAR_WIDTH), 6 };
    struct Cipher_box box7 = { 0, 0 };
    struct Cipher_box box8 = { 0-(4*CIPHER_CHAR_WIDTH), 8 };
    struct Cipher_box box9 = { 0, 0 };
    struct Cipher_box box10 = { 0-(5*CIPHER_CHAR_WIDTH), 10 };
    struct Cipher_box box11 = { 0, 0 };
    
    while (1) {
	if (ticks>=splash_waitfor)
	{
	    splash_waitfor = ticks+2;
	    move_box_right(&box0);
	    move_box_right(&box2);
	    move_box_right(&box4);
	    move_box_right(&box6);
	    move_box_right(&box8);
	    move_box_right(&box10);
	    
	    if (box10.x>=(320+CIPHER_CHAR_WIDTH))
	    {
		break;
	    }
	}
    }
}

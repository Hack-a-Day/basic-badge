#include "box_game.h"
#include "hw.h"
#include <stdint.h>

/*
Program flow:
  -Initialize
  -Spawn piece
    -load from reference
    -calculate y_loc
    -draw piece
  -Wait for game input
*/

/**BOX_location[] Array********************
* Used to track where boxes are in the    *
* playing area.  One byte per column with *
* byte0 as the left column.  One bit per  *
* row with the LSB at the top.  If there  *
* are more than 8 rows, the array will be *
* Increased by BOX_board_right+1 bytes to *
* accomodate 8 more rows as needed.       *
*******************************************/

/*****************************************
* BOX_piece[] is a 4x4 representation of *
* the currently selected playing piece.  *
* When a piece spawns its shape is       *
* written to this array in the least     *
* significant nibble of each byte:       *
* Byte0= left, Byte3= right		         *
* The LSB of each nibble is the top of   *
* the display area.                      *
******************************************/

//Total rows-1 (max row-number, zero indexed)
#define BOX_BOARD_BOTTOM	19
//Total columns-1 (max column-number, zero indexed)
#define BOX_BOARD_RIGHT		9
//Pixel size of each box
#define BOX_MULTIPLIER          11
//Pixel offsets from left and top of screen
#define BOX_XOFFSET	44
#define BOX_YOFFSET 0
//Values for frame around grid
#define BOX_FRAMEX				40
#define BOX_FRAMEY				0
#define BOX_FRAME_THICKNESS		4
#define BOX_FRAMECOLOR			0xFFFFFF
//Values for frame around score
#define BOX_GAMETITLE_Y			30
#define BOX_GAMEOVER_Y			98
#define BOX_SCOREBOX_X			178
#define BOX_SCOREBOX_Y			64
#define BOX_SCOREBOX_WIDTH		110
#define BOX_SCOREBOX_HEIGHT		22
#define BOX_NEXT_PIECE_X                202
#define BOX_NEXT_PIECE_Y                100
#define BOX_NEXT_PIECE_WIDTH            (BOX_MULTIPLIER*5 + BOX_FRAME_THICKNESS*2)
#define BOX_NEXT_PIECE_HEIGHT           (BOX_MULTIPLIER*4 + BOX_FRAME_THICKNESS*2)

#define ARRAY_SIZE (((BOX_BOARD_BOTTOM+8)/8) * (BOX_BOARD_RIGHT + 1))

#define DEFAULT_FG_COLOR	0xFF0000
#define DEFAULT_BG_COLOR	0x000000
#define DEFAULT_STONE_COLOR	0x00FF00

#define DEFAULT_DROP_DELAY	400

static uint8_t cursor_x, cursor_y;

volatile uint8_t random_piece = 0;	// Used to select a piece "randomly" (but not really)
uint8_t next_piece = 4;                 // Updated on every spawn

uint8_t BOX_piece[4];

const uint8_t BOX_reference[7][4][4] = {
		//T
		{
				{
						0b00000010,
						0b00000011,
						0b00000010,
						0b00000000
				},

				{
						0b00000000,
						0b00000111,
						0b00000010,
						0b00000000
				},

				{
						0b00000001,
						0b00000011,
						0b00000001,
						0b00000000
				},

				{
						0b00000010,
						0b00000111,
						0b00000000,
						0b00000000
				}
		},

		// S
		{
				{
						0b00000010,
						0b00000011,
						0b00000001,
						0b00000000
				},

				{
						0b00000011,
						0b00000110,
						0b00000000,
						0b00000000
				},

				{
						0b00000010,
						0b00000011,
						0b00000001,
						0b00000000
				},

				{
						0b00000011,
						0b00000110,
						0b00000000,
						0b00000000
				}
		},

		// Z
		{
				{
						0b00000001,
						0b00000011,
						0b00000010,
						0b00000000
				},

				{
						0b00000110,
						0b00000011,
						0b00000000,
						0b00000000
				},

				{
						0b00000001,
						0b00000011,
						0b00000010,
						0b00000000
				},

				{
						0b00000110,
						0b00000011,
						0b00000000,
						0b00000000
				}
		},

		// L
		{
				{
						0b00000011,
						0b00000001,
						0b00000001,
						0b00000000
				},

				{
						0b00000000,
						0b00000001,
						0b00000111,
						0b00000000
				},

				{
						0b00000010,
						0b00000010,
						0b00000011,
						0b00000000
				},

				{
						0b00000000,
						0b00000111,
						0b00000100,
						0b00000000
				}
		},

		// J
		{
				{
						0b00000001,
						0b00000001,
						0b00000011,
						0b00000000
				},

				{
						0b00000000,
						0b00000100,
						0b00000111,
						0b00000000
				},

				{
						0b00000011,
						0b00000010,
						0b00000010,
						0b00000000
				},

				{
						0b00000000,
						0b00000111,
						0b00000001,
						0b00000000
				}
		},

		// Box
		{
				{
						0b00000011,
						0b00000011,
						0b00000000,
						0b00000000,
				},

				{
						0b00000011,
						0b00000011,
						0b00000000,
						0b00000000,
				},

				{
						0b00000011,
						0b00000011,
						0b00000000,
						0b00000000,
				},

				{
						0b00000011,
						0b00000011,
						0b00000000,
						0b00000000,
				}
		},

		// Line
		{
				{
						0b00000010,
						0b00000010,
						0b00000010,
						0b00000010
				},

				{
						0b00000000,
						0b00001111,
						0b00000000,
						0b00000000
				},

				{
						0b00000010,
						0b00000010,
						0b00000010,
						0b00000010
				},

				{
						0b00000000,
						0b00001111,
						0b00000000,
						0b00000000
				}
		}
};

//Variables
uint8_t BOX_location[ARRAY_SIZE];
uint8_t x_loc, y_loc;     //Bottom left index of each piece
uint8_t cur_piece = 0;	//Index for BOX_reference
uint8_t rotate = 0;		//Index for piece rotation
static uint8_t score = 0;		//Track the number of rows completed
static uint8_t game_over = 0;

//Messages
const uint8_t message1[] = { "Badgetris!" };
const uint8_t message2[] = { "click to start" };
const uint8_t message3[] = { "Game Over" };
const uint8_t message4[] = { "Lines:" };

//Functions

/*******************************
 * Display specific functions: *
 *   Change these to suit      *
 *   whatever display will     *
 *   be used.                  *
 *******************************/

void BOX_seed_random(uint8_t seed)
	{
	if (seed > 6) random_piece = 0;
	else random_piece = seed;
	}
void BOX_inc_random(void)
	{
	if (++random_piece > 6) random_piece = 0;
	}

uint8_t BOX_get_score(void)
	{
	return score;
	}

uint16_t BOX_get_delay(void)
	{
	if (BOX_get_score)
		{
		uint8_t level = BOX_get_score()/4;
		uint16_t dropdelay = DEFAULT_DROP_DELAY;
		while(level)
			{
			dropdelay -= (dropdelay/5);
			--level;
			if (dropdelay<200) return 200;	//Keep speed from becoming insane
			}
		return dropdelay;
		}
	return DEFAULT_DROP_DELAY;
	}

void BOX_clearscreen(void)
	{
	//Draw black on the display
	tft_fill_area(0, 0, 319, 239, 0x000000);
	}

void BOX_draw_ofs(uint16_t ofs_x, uint8_t ofs_y, uint8_t x, uint8_t y, uint32_t color) {
  //Draw box
  uint8_t row = y*BOX_MULTIPLIER;
  uint16_t col = x*BOX_MULTIPLIER;
	
  tft_fill_area(col+ofs_x, row+ofs_y, BOX_MULTIPLIER-1, BOX_MULTIPLIER-1, color);
}

void BOX_pregame(void)
	{
	//Draw fancy background
	tft_set_write_area(0,0,319,239);
	TFT_24_7789_Write_Command(0x2C);
	uint16_t patternx, patterny, hue;
	for (patterny = 0; patterny<240; patterny++)
		{
		for (patternx = 0; patternx<320; patternx++)
			{
			hue = patternx ^ patterny;
			TFT_24_7789_Write_Data3(hue,40,hue);
			}
		}
	//Draw frame around grid
	tft_fill_area(BOX_FRAMEX, BOX_FRAMEY, BOX_FRAME_THICKNESS-1, BOX_MULTIPLIER*(BOX_BOARD_BOTTOM+1), BOX_FRAMECOLOR);
	tft_fill_area(BOX_FRAMEX, BOX_FRAMEY+BOX_MULTIPLIER*(BOX_BOARD_BOTTOM+1), (BOX_MULTIPLIER*(BOX_BOARD_RIGHT + 1))+(BOX_FRAME_THICKNESS*2)-1, BOX_FRAME_THICKNESS-1, BOX_FRAMECOLOR);
	tft_fill_area(BOX_FRAMEX+(BOX_MULTIPLIER*(BOX_BOARD_RIGHT+1))+BOX_FRAME_THICKNESS, BOX_FRAMEY, BOX_FRAME_THICKNESS-1, BOX_MULTIPLIER*(BOX_BOARD_BOTTOM+1), BOX_FRAMECOLOR);
   
	//Show game title
	tft_fill_area(BOX_SCOREBOX_X, BOX_GAMETITLE_Y, BOX_SCOREBOX_WIDTH, BOX_SCOREBOX_HEIGHT, BOX_FRAMECOLOR);
	tft_fill_area(BOX_SCOREBOX_X+BOX_FRAME_THICKNESS, BOX_GAMETITLE_Y+BOX_FRAME_THICKNESS, BOX_SCOREBOX_WIDTH-(2*BOX_FRAME_THICKNESS), BOX_SCOREBOX_HEIGHT-(2*BOX_FRAME_THICKNESS), DEFAULT_BG_COLOR);
	BOX_print_string(message1,BOX_SCOREBOX_X+12,BOX_GAMETITLE_Y+BOX_FRAME_THICKNESS+2,0xFFFFFF,DEFAULT_BG_COLOR);
	
	//Get score area ready
	tft_fill_area(BOX_SCOREBOX_X, BOX_SCOREBOX_Y, BOX_SCOREBOX_WIDTH, BOX_SCOREBOX_HEIGHT, BOX_FRAMECOLOR);
	tft_fill_area(BOX_SCOREBOX_X+BOX_FRAME_THICKNESS, BOX_SCOREBOX_Y+BOX_FRAME_THICKNESS, BOX_SCOREBOX_WIDTH-(2*BOX_FRAME_THICKNESS), BOX_SCOREBOX_HEIGHT-(2*BOX_FRAME_THICKNESS), DEFAULT_BG_COLOR);

	// Next piece area
	tft_fill_area(BOX_NEXT_PIECE_X, BOX_NEXT_PIECE_Y,
		      BOX_NEXT_PIECE_WIDTH, BOX_NEXT_PIECE_HEIGHT,
		      BOX_FRAMECOLOR);
	tft_fill_area(BOX_NEXT_PIECE_X + BOX_FRAME_THICKNESS,
		      BOX_NEXT_PIECE_Y + BOX_FRAME_THICKNESS,
		      BOX_NEXT_PIECE_WIDTH - BOX_FRAME_THICKNESS*2,
		      BOX_NEXT_PIECE_HEIGHT - BOX_FRAME_THICKNESS*2,
		      DEFAULT_BG_COLOR);
	
	//Show game area
	BOX_rewrite_display(DEFAULT_FG_COLOR);
	
	BOX_update_score();
	}

void BOX_start_game(void)
	{
	score = 0; //Reset score
	game_over = 0;
	
	//Set up blank array
	uint16_t i;
	for (i=0; i<ARRAY_SIZE; i++) { BOX_location[i] = 0x00; }
	BOX_pregame();
	BOX_spawn();
	}

void BOX_show_gameover(void)
	{
	//Show game title
	tft_fill_area(BOX_SCOREBOX_X, BOX_GAMEOVER_Y, BOX_SCOREBOX_WIDTH, BOX_SCOREBOX_HEIGHT, BOX_FRAMECOLOR);
	tft_fill_area(BOX_SCOREBOX_X+BOX_FRAME_THICKNESS, BOX_GAMEOVER_Y+BOX_FRAME_THICKNESS, BOX_SCOREBOX_WIDTH-(2*BOX_FRAME_THICKNESS), BOX_SCOREBOX_HEIGHT-(2*BOX_FRAME_THICKNESS), 0xFF0000);
	BOX_print_string(message3,BOX_SCOREBOX_X+12,BOX_GAMEOVER_Y+BOX_FRAME_THICKNESS+2,0xFFFF00,0xFF0000);
	}

uint8_t BOX_end_game(void)
	{
	//FIXME: What happens when game ends?
	return game_over;
	}

void BOX_update_score(void)
	{
	//Show score on screen
	//FIXME: Make locations and colors #define values
	BOX_print_string(message4,BOX_SCOREBOX_X+12,BOX_SCOREBOX_Y+BOX_FRAME_THICKNESS+2,0xFFFFFF,DEFAULT_BG_COLOR);

	//This hack turns score numbers into a string
	int8_t mystring[4] = {'0',0,0,0};
	uint8_t i = 0;
	uint8_t hundred, ten, one;
	hundred = score/100;
	ten = (score%100)/10;
	one = (score%100)%10;
	if (hundred) mystring[i++] = hundred+'0';
	if (hundred || ten) mystring[i++] = ten+'0';
	if (hundred || ten || one) mystring[i++] = one+'0';
	
	BOX_print_string(mystring, BOX_SCOREBOX_X+76,BOX_SCOREBOX_Y+BOX_FRAME_THICKNESS+2,0xFFFFFF,DEFAULT_BG_COLOR);
	}

void BOX_print_string(const int8_t * buf, uint16_t x_pixel, uint8_t y_pixel, uint32_t fgcolor, uint32_t bgcolor)
	{
	uint8_t i=0;
	while (buf[i] != 0)
		{
		tft_print_char(buf[i],x_pixel+(i*8),y_pixel,fgcolor,bgcolor);
		++i;
		}
	}

/**********************************
 * End Display specific functions *
 **********************************/

/**********************************************
 * Functions that handle bits in BOX_location[]
 * BOX_loc_return_bit
 * BOX_loc_set_bit
 * BOX_loc_clear_bit
 ************************************************/

uint8_t BOX_loc_return_bit(uint8_t X, uint8_t Y)
	{
	//Calculate array index and shift amount
	uint8_t array_index_offset = ((Y)/8)*(BOX_BOARD_RIGHT+1);
	uint8_t shift_index = (Y)%8;		//How much to shift for our bit mask

	if (BOX_location[X+array_index_offset] & 1<<shift_index) return 1;
	else return 0;
	}

void BOX_loc_set_bit(uint8_t X, uint8_t Y)
	{
	//Calculate array index and shift amount
	uint8_t array_index_offset = ((Y)/8)*(BOX_BOARD_RIGHT+1);
	uint8_t shift_index = (Y)%8;		//How much to shift for our bit mask

	BOX_location[X+array_index_offset] |= 1<<shift_index;
	}

void BOX_loc_clear_bit(uint8_t X, uint8_t Y)
	{
	//Calculate array index and shift amount
	uint8_t array_index_offset = ((Y)/8)*(BOX_BOARD_RIGHT+1);
	uint8_t shift_index = (Y)%8;		//How much to shift for our bit mask

	BOX_location[X+array_index_offset] &= ~(1<<shift_index);
	}

/********************************
 * Functions that handle bits in BOX_piece[]
 * BOX_piece_return_bit()
 * BOX_piece_set_bit()
 * BOX_piece_clear_bit()
 */

void BOX_store_loc(void)
	{
	//Step through 4 columns
	uint8_t temp_col, temp_row;
	for (temp_col=0; temp_col<4; temp_col++)
		{
		//Only if x_loc is not out of bounds
		 if ((unsigned char)(x_loc+temp_col) <= BOX_BOARD_RIGHT)
			{
			//Step through 4 rows
			for (temp_row=0; temp_row<4; temp_row++)
				{
				//Only if y_loc is not out of bounds
				if (y_loc-temp_row <= BOX_BOARD_BOTTOM)
					{
					if (BOX_piece[temp_col] & 1<<(temp_row))	//Checks nibbles in Box_piece array
						{
						BOX_loc_set_bit((unsigned char)(x_loc+temp_col),y_loc-temp_row);
						}
					}
				}
			}
		}
	}

void BOX_clear_loc(void)
	{
	//Step through 4 columns
	uint8_t temp_col, temp_row;
	for (temp_col=0; temp_col<4; temp_col++)
		{
		//Only if x_loc is not out of bounds
		if ((unsigned char)(x_loc+temp_col) <= BOX_BOARD_RIGHT)
			{
			//Step through 4 rows
			for (temp_row=0; temp_row<4; temp_row++)
				{
				//Only if y_loc is not out of bounds
				if (y_loc-temp_row <= BOX_BOARD_BOTTOM)
					{
					if (BOX_piece[temp_col] & 1<<(temp_row))	//Checks nibbles in Box_piece array
						{
						BOX_loc_clear_bit((unsigned char)(x_loc+temp_col),y_loc-temp_row);
						}
					}
				}
			}
		}
	}

void BOX_load_reference(uint8_t piece, uint8_t rotation)
	{
	BOX_piece[0] = BOX_reference[piece][rotation][0];
	BOX_piece[1] = BOX_reference[piece][rotation][1];
	BOX_piece[2] = BOX_reference[piece][rotation][2];
	BOX_piece[3] = BOX_reference[piece][rotation][3];
	}

void BOX_rotate(uint8_t direction)
	{
	//TODO: Allow for adjustments if rotation is prevented due to proximity

	BOX_clear_loc(); //Clear current location so we don't have false compares

	//Load in the candidate rotation
	uint8_t new_rotate = rotate;
	if (++new_rotate > 3) new_rotate = 0;
	BOX_load_reference(cur_piece,new_rotate);

	//Check for overlaps
	if (BOX_check(0, 0))
		{
		//Overlap will be caused, restore piece settings and return
		BOX_load_reference(cur_piece, rotate);
		BOX_store_loc();
		return;
		}
	//No overlap found, allow new rotation
	else
		{
		//Load the current rotation back in and clear the piece from display
		BOX_load_reference(cur_piece, rotate);
		BOX_clear_piece();

		//Load new rotation, display, and save its location
		rotate = new_rotate;
		BOX_load_reference(cur_piece, rotate);

		BOX_store_loc();
		BOX_write_piece();
		}
	BOX_update_screen;
	}



void BOX_write_piece_ofs(const uint8_t* piece_ref, uint16_t ofs_x,
			 uint8_t ofs_y, uint8_t x, uint8_t y, uint32_t color)  //Writes piece to display
	{
	uint8_t i, j;
	for (i=0; i<4; i++)  //Step through each of 4 columns
		{
		for (j=0; j<4; j++) //Step through each of 4 rows
			{
			//prevent invalid indices from being written
			if ((y-j) >= 0)
				{
				if (piece_ref[i] & 1<<j)
					{
					//TODO: change this for different colored playing pieces
					  BOX_draw_ofs(ofs_x, ofs_y, x+i, y-j, color);
					}
				}
			}
		}
	}

void BOX_write_piece(void) {
  BOX_write_piece_ofs(BOX_piece, BOX_XOFFSET, BOX_YOFFSET, x_loc, y_loc, DEFAULT_FG_COLOR);
}

void BOX_clear_piece(void) {
  BOX_write_piece_ofs(BOX_piece, BOX_XOFFSET, BOX_YOFFSET, x_loc, y_loc, DEFAULT_BG_COLOR);
}

void BOX_show_next_piece(uint8_t piece_id) {
  BOX_write_piece_ofs(BOX_reference[piece_id][0],
		      BOX_NEXT_PIECE_X + BOX_FRAME_THICKNESS + BOX_MULTIPLIER,
		      BOX_NEXT_PIECE_Y + BOX_FRAME_THICKNESS + BOX_MULTIPLIER, 0, 1, DEFAULT_FG_COLOR);
}

void BOX_clear_next_piece(uint8_t piece_id) {
  BOX_write_piece_ofs(BOX_reference[piece_id][0],
		      BOX_NEXT_PIECE_X + BOX_FRAME_THICKNESS + BOX_MULTIPLIER,
		      BOX_NEXT_PIECE_Y + BOX_FRAME_THICKNESS + BOX_MULTIPLIER, 0, 1, DEFAULT_BG_COLOR);
}

void BOX_rewrite_display(uint32_t fgcolor)	//Rewrites entire playing area
	{
	//printf(cls);
	
	uint8_t cols, rows;
	for (cols=0; cols<=BOX_BOARD_RIGHT; cols++)
		{
		for (rows=0; rows<=BOX_BOARD_BOTTOM; rows++)
			{
			  if(BOX_loc_return_bit(cols,rows))
			    BOX_draw_ofs(BOX_XOFFSET, BOX_YOFFSET, cols,rows,fgcolor);
			  else
			    BOX_draw_ofs(BOX_XOFFSET, BOX_YOFFSET, cols,rows, DEFAULT_BG_COLOR);
			}
		}
	}

void BOX_update_screen(void)
	{
	//This function call is not needed when a screen buffer isn't used
	//BOX_rewrite_display(default_fg_color, default_bg_color);
	}

void BOX_spawn(void)
	{
	x_loc = 4;
	y_loc = 1;
      	BOX_clear_next_piece(next_piece);
	cur_piece = next_piece;
	next_piece = random_piece;
	rotate = 0;

	BOX_load_reference(cur_piece, rotate);  //load from reference

	//Check to see if we've filled the screen
	if (BOX_check(0,0))
		{
		//BOX_end_game();
		++game_over;
		}

	BOX_store_loc(); //Store new location
	BOX_write_piece(); //draw piece
	BOX_show_next_piece(next_piece);
	BOX_update_screen();
	}

uint8_t BOX_check(int8_t X_offset, int8_t Y_offset)
	{
	uint8_t temp_area[4] = { 0x00, 0x00, 0x00, 0x00 };
	uint8_t i;
	//Build compare mask in temp_area[]

	//Clear the current piece from BOX_location[] so we don't have a false overlap
	//Do this only if X_offset and Y_offset aren't both 0 (used for BOX_spawn)
	if (X_offset || Y_offset) BOX_clear_loc();
	//mask will be 4 sets of nibbles (2 bytes)
	for (i=0; i<4; i++)
		{
		//if out of bounds on the x axis
		if ((unsigned char)(x_loc+X_offset+i) > BOX_BOARD_RIGHT) temp_area[i] = 0x0F;
		else
			{
			uint8_t j;
			for (j=0; j<4; j++)
				{
				//if we're out of bounds on the y axis
				if (((unsigned char)(y_loc+Y_offset-j) > BOX_BOARD_BOTTOM) ||
					(BOX_loc_return_bit((unsigned char)(x_loc+X_offset+i),(unsigned char)(y_loc+Y_offset-j))))
					{
					temp_area[i] |= 1<<j;
					}
				}
			}
		}
	if (X_offset || Y_offset) BOX_store_loc(); //Restore the location we cleared earlier

	if ((temp_area[0] & BOX_piece[0]) | (temp_area[1] & BOX_piece[1]) | (temp_area[2] & BOX_piece[2]) | (temp_area[3] & BOX_piece[3]))
		{
		//Conflict has been found
		return 1;
		}
	else return 0;
	}

void BOX_line_check(void)
	{
	//TODO: Tweak this to enable scoring

	//Check every line on the playing area for complete rows and record them in an array
	uint8_t complete_lines[4];	//There will never be more than 4 complete rows
	uint8_t temp_index = 0;		//Index for complete_lines[]
  
	uint8_t board_rows;
	for (board_rows=0; board_rows<=BOX_BOARD_BOTTOM; board_rows++)
		{
		uint8_t board_cols=0;
		while ((board_cols<=BOX_BOARD_RIGHT) && (BOX_loc_return_bit(board_cols,board_rows)))
			{
			//Complete row found, record in complete_lines[]
			if (board_cols == BOX_BOARD_RIGHT) complete_lines[temp_index++] = board_rows;
			++board_cols;
			}
		}
	if (temp_index == 0) return;  //No complete lines found, return


	//If there are complete rows
		//TODO: Disable interrupts to pause game flow
		//TODO: Add an arbitrary delay, perhaps make complete lines flash?

	score += temp_index; //Add the completed rows to our score

	--temp_index;	//This was incremented one too many times earlier, get it back to the proper index.

	//Rewrite BOX_location[] data without completed lines
	uint8_t read_from_row = BOX_BOARD_BOTTOM;
	uint8_t write_to_row = BOX_BOARD_BOTTOM;

	//When we have read from all rows, this will be set to 0 and
	//remaining bits cleared from BOX_location[]
	uint8_t rows_left_to_read = 1;

	//Use variable i to iterate through every row of the board
	uint8_t i=0;
	while (i <= BOX_BOARD_BOTTOM)
		{
		//If the current row is a complete line
		if (read_from_row == complete_lines[temp_index])
			{
			//Decrement indexes
			if (read_from_row == 0)
				{
				rows_left_to_read = 0;

				//Change complete_lines[0] so we don't do this again
				complete_lines[0] = BOX_BOARD_BOTTOM;
				}
			else
				{
				--read_from_row;
				if (temp_index) --temp_index;
				}
			}

		else
			{
			//Write data to all columns of current row
			uint8_t col;
			for (col=0; col<=BOX_BOARD_RIGHT; col++)
				{
				//If there are rows left to read from, do so.
				if (rows_left_to_read)
					{
					if (BOX_loc_return_bit(col,read_from_row)) BOX_loc_set_bit(col, write_to_row);
					else BOX_loc_clear_bit(col, write_to_row);
					}
				//There are no rows left to read from, fill with 0
				else
					{
					BOX_loc_clear_bit(col, write_to_row);
					}
				}

			//A row has now been read from, decrement the counter
			if (read_from_row) --read_from_row;
			else rows_left_to_read = 0;

			//A row has now been written to, increment the counter, decrement the tracker
			++i;
			--write_to_row;
			}
		}

	BOX_rewrite_display(DEFAULT_STONE_COLOR);
	BOX_update_score();
	}


void BOX_up(void)
	{
	BOX_clear_loc();
	BOX_clear_piece();

	if (++cur_piece > 6) cur_piece = 0;
	x_loc = 4;
	y_loc = 0;

	BOX_spawn();
	}

void BOX_dn(void)
	{
	if (BOX_check(0, 1))
		{
		//Set piece here and spawn a new one
		BOX_rewrite_display(DEFAULT_STONE_COLOR);
		BOX_line_check();
		BOX_spawn();
		BOX_update_score();
		return;
		}

	BOX_clear_loc();
	BOX_clear_piece();
	++y_loc;

	BOX_store_loc();
	BOX_write_piece();
	BOX_update_screen();
	}

void BOX_drop(void)
	{
	BOX_clear_loc();
	BOX_clear_piece();

	while (!BOX_check(0, 0)) {
	  y_loc++;
	}
	y_loc--;

	BOX_store_loc();
	BOX_write_piece();

	BOX_rewrite_display(DEFAULT_STONE_COLOR);
	BOX_line_check();
	BOX_spawn();
	BOX_update_score();
	}

void BOX_lt(void)
	{
	if (BOX_check(-1, 0)) return; //Do nothing if moving causes an overlap
	BOX_clear_loc();
	BOX_clear_piece();
	x_loc--;

	BOX_store_loc();
	BOX_write_piece();
	BOX_update_screen();
	}

void BOX_rt(void)
	{
	if (BOX_check(1, 0)) return; //Do nothing if moving causes an overlap
	BOX_clear_loc();
	BOX_clear_piece();
	++x_loc;

	BOX_store_loc();
	BOX_write_piece();
	BOX_update_screen();
	}

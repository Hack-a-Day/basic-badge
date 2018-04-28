#include "box_game.h"
#include "hw.h"

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
#define BOX_board_bottom	19
//Total columns-1 (max column-number, zero indexed)
#define BOX_board_right		9
//Pixel size of each box
#define BOX_multiplier          11
//Pixel offsets from left and top of screen
#define BOX_xoffset	4
#define BOX_yoffset 0

#define array_size (((BOX_board_bottom+8)/8) * (BOX_board_right + 1))

#define default_fg_color	0xFF0000
#define default_bg_color	0x000000

static unsigned char cursor_x, cursor_y;

volatile unsigned char random_piece = 0;	//Used to select a piece "randomly" (but not really)

unsigned char BOX_piece[4];

const unsigned char BOX_reference[7][4][4] = {
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
unsigned char BOX_location[array_size];
unsigned char x_loc, y_loc;     //Bottom left index of each piece
unsigned char cur_piece = 0;	//Index for BOX_reference
unsigned char rotate = 0;		//Index for piece rotation
static unsigned char score = 0;		//Track the number of rows completed
static unsigned char game_over = 0;

//Messages
const unsigned char message1[] = { "Tetrapuzz!" };
const unsigned char message2[] = { "click to start" };
const unsigned char message3[] = { "Game Over" };
const unsigned char message4[] = { "Lines:" };

//Functions

/*******************************
 * Display specific functions: *
 *   Change these to suit      *
 *   whatever display will     *
 *   be used.                  *
 *******************************/

void BOX_inc_random(void)
	{
	if (++random_piece > 6) random_piece = 0;
	}

unsigned char BOX_get_score(void)
	{
	return score;
	}

void BOX_draw(unsigned char X, unsigned char Y, unsigned int color)
	{
	//Draw box
	unsigned char row = Y*BOX_multiplier;
	unsigned int col = X*BOX_multiplier;
	
	tft_fill_area(col+BOX_xoffset, row+BOX_yoffset, BOX_multiplier-1, BOX_multiplier-1, color);
	}

void BOX_erase(unsigned char X, unsigned char Y)
	{
	//Erase box
	unsigned char row = Y*BOX_multiplier;
	unsigned int col = X*BOX_multiplier;
	
	tft_fill_area(col+BOX_xoffset, row+BOX_yoffset, BOX_multiplier-1, BOX_multiplier-1, default_bg_color);
	}

void BOX_pregame(void)
	{
	//TODO: Clear screen and setup board
	//printf(top butcol "EXIT" tabx4 taby4 whi "Click to play");
	//printf(tabx4 taby5 "BadgeTris" butcol bot "<-      Rotate     ->");
	}

void BOX_start_game(void)
	{
	score = 0; //Reset score
	game_over = 0;

	unsigned int i;
  
	//Draw grid area background
	tft_fill_area(0, 0, BOX_board_right-1, BOX_board_bottom-1, default_bg_color);
	
	//Draw frame around grid -- this is coded with magic numbers and will need changing if #defines change
	tft_fill_area(0, 0, 3, 220, 0xFFFFFF);
	tft_fill_area(0, 220, 117, 3, 0xFFFFFF);
	tft_fill_area(114, 0, 3, 220, 0xFFFFFF);
   
	for (i=0; i<array_size; i++) { BOX_location[i] = 0x00; }

	BOX_rewrite_display(0x00FF00, 0xFFFFFF);
	BOX_spawn();
	BOX_update_score();
	}

	unsigned char BOX_end_game(void)
	{
	//FIXME: What happens when game ends?
	return game_over;
	}

void BOX_update_score(void)
	{
	//Show score on screen
	//FIXME: Make locations and colors #define values
	
	unsigned char i = 0;
	while (message4[i] != 0)
		{
		tft_print_char(message4[i],160+(i*8),48,0xFFFFFF,0x000000);
		++i;
		}
	
	//This hack turns score numbers into a string
	char mystring[4] = {'0',0,0,0};
	i=0;
	unsigned char hundred, ten, one;
	hundred = score/100;
	ten = (score%100)/10;
	one = (score%100)%10;
	if (hundred) mystring[i++] = hundred+'0';
	if (ten) mystring[i++] = ten+'0';
	if (one) mystring[i++] = one+'0';
	
	i=0;
	while (mystring[i] != 0)
		{
		tft_print_char(mystring[i],160+(8*8)+(i*8),48,0xFFFFFF,0x000000);
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

unsigned char BOX_loc_return_bit(unsigned char X, unsigned char Y)
	{
	//Calculate array index and shift amount
	unsigned char array_index_offset = ((Y)/8)*(BOX_board_right+1);
	unsigned char shift_index = (Y)%8;		//How much to shift for our bit mask

	if (BOX_location[X+array_index_offset] & 1<<shift_index) return 1;
	else return 0;
	}

void BOX_loc_set_bit(unsigned char X, unsigned char Y)
	{
	//Calculate array index and shift amount
	unsigned char array_index_offset = ((Y)/8)*(BOX_board_right+1);
	unsigned char shift_index = (Y)%8;		//How much to shift for our bit mask

	BOX_location[X+array_index_offset] |= 1<<shift_index;
	}

void BOX_loc_clear_bit(unsigned char X, unsigned char Y)
	{
	//Calculate array index and shift amount
	unsigned char array_index_offset = ((Y)/8)*(BOX_board_right+1);
	unsigned char shift_index = (Y)%8;		//How much to shift for our bit mask

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
	unsigned char temp_col, temp_row;
	for (temp_col=0; temp_col<4; temp_col++)
		{
		//Only if x_loc is not out of bounds
		 if ((unsigned char)(x_loc+temp_col) <= BOX_board_right)
			{
			//Step through 4 rows
			for (temp_row=0; temp_row<4; temp_row++)
				{
				//Only if y_loc is not out of bounds
				if (y_loc-temp_row <= BOX_board_bottom)
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
	unsigned char temp_col, temp_row;
	for (temp_col=0; temp_col<4; temp_col++)
		{
		//Only if x_loc is not out of bounds
		if ((unsigned char)(x_loc+temp_col) <= BOX_board_right)
			{
			//Step through 4 rows
			for (temp_row=0; temp_row<4; temp_row++)
				{
				//Only if y_loc is not out of bounds
				if (y_loc-temp_row <= BOX_board_bottom)
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

void BOX_load_reference(unsigned char piece, unsigned char rotation)
	{
	BOX_piece[0] = BOX_reference[piece][rotation][0];
	BOX_piece[1] = BOX_reference[piece][rotation][1];
	BOX_piece[2] = BOX_reference[piece][rotation][2];
	BOX_piece[3] = BOX_reference[piece][rotation][3];
	}

void BOX_rotate(unsigned char direction)
	{
	//TODO: Allow for adjustments if rotation is prevented due to proximity

	BOX_clear_loc(); //Clear current location so we don't have false compares

	//Load in the candidate rotation
	unsigned char new_rotate = rotate;
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



void BOX_write_piece(void)  //Writes piece to display
	{
	unsigned char i, j;
	for (i=0; i<4; i++)  //Step through each of 4 columns
		{
		for (j=0; j<4; j++) //Step through each of 4 rows
			{
			//prevent invalid indices from being written
			if ((y_loc-j) >= 0)
				{
				if (BOX_piece[i] & 1<<j)
					{
					//TODO: change this for different colored playing pieces
					BOX_draw(x_loc+i, y_loc-j, default_fg_color);
					}
				}
			}
		}
	}

void BOX_clear_piece(void)  //Clears piece from display
	{
	unsigned char i, j;
	for (i=0; i<4; i++)  //Step through each of 4 columns
		{
		for (j=0; j<4; j++) //Step through each of 4 rows
			{
			//prevent invalid indices from being written
			if ((y_loc-j) >= 0)
				{
				if (BOX_piece[i] & 1<<j)
					{
					//TODO: change this for different colored playing pieces
					BOX_erase(x_loc+i, y_loc-j);
					}
				}
			}
		}
	}

void BOX_rewrite_display(unsigned int fgcolor, unsigned int bgcolor)	//Rewrites entire playing area
	{
	//printf(cls);
	
	unsigned char cols, rows;
	for (cols=0; cols<=BOX_board_right; cols++)
		{
		for (rows=0; rows<=BOX_board_bottom; rows++)
			{
			if(BOX_loc_return_bit(cols,rows)) BOX_draw(cols,rows,fgcolor);
			else BOX_erase(cols,rows);
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
	cur_piece = random_piece;
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
	BOX_update_screen();
	}

unsigned char BOX_check(signed char X_offset, signed char Y_offset)
	{
	unsigned char temp_area[4] = { 0x00, 0x00, 0x00, 0x00 };
	unsigned char i;
	//Build compare mask in temp_area[]

	//Clear the current piece from BOX_location[] so we don't have a false overlap
	//Do this only if X_offset and Y_offset aren't both 0 (used for BOX_spawn)
	if (X_offset || Y_offset) BOX_clear_loc();
	//mask will be 4 sets of nibbles (2 bytes)
	for (i=0; i<4; i++)
		{
		//if out of bounds on the x axis
		if ((unsigned char)(x_loc+X_offset+i) > BOX_board_right) temp_area[i] = 0x0F;
		else
			{
			unsigned char j;
			for (j=0; j<4; j++)
				{
				//if we're out of bounds on the y axis
				if (((unsigned char)(y_loc+Y_offset-j) > BOX_board_bottom) ||
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
	unsigned char complete_lines[4];	//There will never be more than 4 complete rows
	unsigned char temp_index = 0;		//Index for complete_lines[]
  
	unsigned char board_rows;
	for (board_rows=0; board_rows<=BOX_board_bottom; board_rows++)
		{
		unsigned char board_cols=0;
		while ((board_cols<=BOX_board_right) && (BOX_loc_return_bit(board_cols,board_rows)))
			{
			//Complete row found, record in complete_lines[]
			if (board_cols == BOX_board_right) complete_lines[temp_index++] = board_rows;
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
	unsigned char read_from_row = BOX_board_bottom;
	unsigned char write_to_row = BOX_board_bottom;

	//When we have read from all rows, this will be set to 0 and
	//remaining bits cleared from BOX_location[]
	unsigned char rows_left_to_read = 1;

	//Use variable i to iterate through every row of the board
	unsigned char i=0;
	while (i <= BOX_board_bottom)
		{
		//If the current row is a complete line
		if (read_from_row == complete_lines[temp_index])
			{
			//Decrement indexes
			if (read_from_row == 0)
				{
				rows_left_to_read = 0;

				//Change complete_lines[0] so we don't do this again
				complete_lines[0] = BOX_board_bottom;
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
			unsigned char col;
			for (col=0; col<=BOX_board_right; col++)
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

	BOX_rewrite_display(0x00FF00, default_bg_color);
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
		BOX_rewrite_display(0x00FF00, default_bg_color);
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

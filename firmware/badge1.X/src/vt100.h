#define DISP_BUFFER_WIDE    40
#define DISP_BUFFER_HIGH    20
#define MAX_ESC_LEN 48 
#define  NOT_IN_ESC 	0
#define    ESC_GOT_1B	1
#define    ESC_CSI		2
#define    ESC_NONCSI	3



typedef struct
{
  int8_t cx;
  int8_t cy;
  uint8_t graphicchars;
  uint8_t revvideo;
} termstate_t; 

void escseq_process(char c);
void escseq_process_noncsi(char c);
void escseq_process_csi(char c);
void escseq_csi_start(void);
uint8_t escseq_get_param(uint8_t defaultval);
void receive_char(uint8_t c);
void save_term_state(void);
void restore_term_state(void);
void reset_term(void); 
uint8_t buf_dequeue(void);
void buf_enqueue(uint8_t c);
/* Set the top and bottom margins. The cursor is moved to the first column
 * of the top margin. */
void video_set_margins(int8_t top, int8_t bottom);
/* Resets the top margin to the top line of the screen and the bottom margin
 * to the bottom line of the screen. */
void video_reset_margins(void);
/* Returns the line number of the top margin. */
int8_t video_top_margin(void);
/* Returns the line number of the bottom margin. */
int8_t video_bottom_margin(void);
/* Sets whether or not the screen should be displayed in reverse video. */
void video_set_reverse(uint8_t val);
/* Clears the screen, returns the cursor to (0,0), and resets the margins
 * to the full size of the screen. */
void video_clrscr(void);
/* Clears the current line and returns the cursor to he start of the line. */
void video_clrline(void);
/* Clears the rest of the line from the cursor position to the end of the line
 * without moving the cursor. */
void video_clreol(void);
/* erasemode = 0: erase from the cursor (inclusive) to the end of the screen.
 * erasemode = 1: erase from the start of the screen to the cursor (inclusive).
 * erasemode = 2: erase the entire screen.
 * The cursor does not move.
 * This call corresponds to the ANSI "Erase in Display" escape sequence. */
void video_erase(uint8_t erasemode);
/* erasemode = 0: erase from the cursor (inclusive) to the end of the line.
 * erasemode = 1: erase from the start of the line to the cursor (inclusive).
 * erasemode = 2: erase the entire line.
 * The cursor does not move.
 * This call corresponds to the ANSI "Erase in Line" escape sequence. */
void video_eraseline(uint8_t erasemode);
/* Overwrites the character at the cursor position without moving it. */
void video_setc(char c);
/* Prints a character at the cursor position and advances the cursor.
 * Carriage returns and newlines are interpreted. */
void video_putc(char c);
/* Prints a character at the cursor position and advances the cursor.
 * Carriage returns and newlines are not interpreted. */
void video_putc_raw(char c);
/* Prints a string at the cursor position and advances the cursor.
 * The screen will be scrolled if necessary. */
void video_puts(char *str);
/* Prints a string from program memory at the cursor position and advances
 * the cursor. The screen will be scrolled if necessary. */
void video_putcxy(int8_t x, int8_t y, char c);
/* Prints a string at the specified position. Escape characters are not
 * interpreted. The cursor is not advanced and the screen is not scrolled. */
void video_putsxy(int8_t x, int8_t y, char *str);
/* Prints a string from program memory at the specified position.
 * Escape characters are not interpreted. The cursor is not advanced and the
 * screen is not scrolled. */
void video_putline(int8_t y, char *str);
/* Prints a string from program memory on the specified line.
 * The previous contents of the line are erased. Escape characters are not
 * interpreted. The cursor is not advanced and the screen is not scrolled. */
void video_gotoxy(int8_t x, int8_t y);
/* Moves the cursor left/right by the specified number of columns. */
void video_movex(int8_t dx);
/* Moves the cursor up/down the specified number of lines. 
 * The cursor does not move beyond the top/bottom margins. */
void video_movey(int8_t dy);
/* Moves the cursor to the start of the current line. */
void video_movesol(void);
/* Sets the horizontal position of the cursor. */
void video_setx(int8_t x);
/* Advances the cursor one character to the right, advancing to a new line if
 * necessary. */
void video_cfwd(void);
/* Advances the cursor one line down and moves it to the start of the new line.
 * The screen is scrolled if the bottom margin is exceeded. */
void video_lfwd(void);
/* Advances the cursor one line down but does not return the cursor to the start
 * of the new line. The screen is scrolled if the bottom margin is exceeded. */
void video_lf(void);
/* Moves the cursor one character back, moving to the end of the previous line
 * if necessary. */
void video_cback(void);
/* Moves the cursor to the end of the previous line, or to the first column
 * of the top margin if the top margin is exceeded. */
void video_lback(void);
/* Scrolls the region between the top and bottom margins up one line.
 * A blank line is added at the bottom. The cursor is not moved. */
void video_scrollup(void);
/* Scrolls the region between the top and bottom margins down one line.
 * A blank lines is added at the top. The cursor is not moved. */
void video_scrolldown(void);
/* Returns the x coordinate of the cursor. */
int8_t video_getx(void);
/* Returns the y coordinate of the cursor. */
int8_t video_gety(void);
/* Returns the character at the specified position. */
char video_charat(int8_t x, int8_t y);
/* Shows the cursor. Off by default. */
void video_show_cursor(void);
/* Hides the cursor. */
void video_hide_cursor(void);
/* Returns 1 if the cursor is visible, 0 if it is hidden. */
uint8_t video_cursor_visible();
/* Set inverse video for the character range specified. */
void video_invert_range(int8_t x, int8_t y, uint8_t rangelen);  
static void CURSOR_INVERT(void);
static void _video_scrollup(void);
static void _video_scrolldown(void);
static void _video_lfwd(void);
static void _video_cfwd(void);
static void _video_lback(void);
static void _video_scrollup_lin(unsigned char lin);
static void _video_scrolldown_lin(unsigned char line);


void write_direct(unsigned int * x, unsigned int * y, unsigned char * str);
void term_init (void);

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vt100.h"

#define MAX_BUF 50

uint8_t msg1[50];


uint8_t color_composite;
uint16_t count;
volatile uint16_t bufhead;
volatile uint16_t buftail; 
volatile uint16_t bufsize;
static int8_t cx;
static int8_t cy;
static uint8_t showcursor;
/* parameters from setup */
static uint8_t newlineseq;
static uint8_t process_escseqs;
static uint8_t local_echo;
/* escape sequence processing */
static uint8_t in_esc;
static int8_t paramstr[MAX_ESC_LEN+1];
static int8_t *paramptr;
static uint8_t paramch;
/* current attributes */
static uint8_t graphicchars;  /* set to 1 with an SI and set to 0 with an SO */
static uint8_t revvideo;      /* reverse video attribute */
static termstate_t savedstate;/* state used for save/restore sequences */ 
/* Vertical margins */
static int8_t mtop;
static int8_t mbottom;

/* reverse video */
static uint8_t revvideo;

uint8_t cur_type,cur_blink;

extern int8_t disp_buffer[DISP_BUFFER_HIGH+1][DISP_BUFFER_WIDE];
extern int8_t color_buffer[DISP_BUFFER_HIGH+1][DISP_BUFFER_WIDE];
volatile uint8_t buf[MAX_BUF];

void video_set_color(uint8_t fg, uint8_t bg)
	{
	color_composite = (fg&0xF) | ((bg&0xF)<<4);
	}


void write_direct(uint16_t * x, uint16_t * y, uint8_t * str)
	{
	uint16_t xt,yt;
	xt = *x;
	yt = *y;
	while (*str>=' ')
		{
		disp_buffer[yt][xt] = *str++;
		color_buffer[yt][xt] = color_composite;
		xt++;
		if (xt==DISP_BUFFER_WIDE)
			{
			xt=0;
			yt++;
			}
		}
	*x = xt;
	*y = yt;
	}

void term_init (void)
	{
	video_reset_margins();
	reset_term();
	process_escseqs = 1;
	buftail = 0;
	bufhead = 0;
	bufsize = 0;
	cur_blink = 1;
	cur_type = 2;	
	video_set_color(15,0);
	}

void buf_enqueue(uint8_t c)
{
    if (bufsize < MAX_BUF)
    {
      buf[buftail] = c;
      if (++buftail >= MAX_BUF) buftail = 0;
      bufsize++;
    }
}

uint8_t buf_dequeue()
{
  uint8_t ret = 0;
    if (bufsize > 0)
    {
      uint8_t c = buf[bufhead];
      if (++bufhead >= MAX_BUF) bufhead = 0;
      bufsize--;
      ret = c;
    }
  return ret;
}

uint8_t buf_size()
{
  uint8_t sz;
   sz = bufsize;
  return sz;
} 

//jar
/*
ISR(USART_RX_vect)
{
	buf_enqueue(RCREG);
}

*/



void receive_char(uint8_t c)
{
  if (!process_escseqs)
  {
    video_putc_raw(c);
    return;
  }

  if (c)
  {
    if (in_esc)
      escseq_process(c);
    else switch (c)
    {
      case 0x07: /* BEL */
        break;   /* ignore bells */
      case '\b': /* backspace */
        video_cback();
		video_putc_raw(' ');
		video_cback();
        break;
      case 0x0A: /* LF, VT, and FF all print a linefeed */
      case 0x0B:
      case 0x0C:
		  video_setx(0);
		  video_lf();
        break;
      case 0x0D: /* CR */
 //       video_setx(0);
        break;
      case 0x0E: /* SO; enable box-drawing characters */
        graphicchars = 1;
        break;
      case 0x0F: /* SI; return to normal characters */
        graphicchars = 0;
        break;
      case 0x1B: /* ESC */
        in_esc = ESC_GOT_1B;
        break;
      case 0x7F: /* DEL */
        break;
      default:
        if (c >= ' ')
        {
//jar
/*
          if (graphicchars && c >= '_' && c <= '~')
            c -= 95;
          c |= revvideo;
*/
          video_putc_raw(c);
        }
    }
  }
}


void escseq_process(int8_t c)
{
  /* CAN and SUB interrupt escape sequences */
  if (c == 0x18 || c == 0x1A)
  {
    in_esc = NOT_IN_ESC;
    return;
  }

  if (in_esc == ESC_CSI)
    escseq_process_csi(c);
  else if (in_esc == ESC_NONCSI)
  {
    /* received a non-CSI sequence that requires a parameter
     * (ESC #, ESC %, etc)
     * These aren't supported, so eat this character */
    in_esc = NOT_IN_ESC;
  } 
  else if (in_esc == ESC_GOT_1B)
  {
    in_esc = ESC_NONCSI;
    escseq_process_noncsi(c);
  }
}

/* Process sequences that begin with ESC */
void escseq_process_noncsi(int8_t c)
{
  switch (c)
  {
    case '[': /* got the [; this is a CSI sequence */
      escseq_csi_start();
      in_esc = ESC_CSI;
      break;
    case '%': /* non-CSI codes that require parameters */
    case '#': /* (we don't support these) */
    case '(':
    case ')':
      break;  /* return without setting in_esc to NOT_IN_ESC */
    case '7': /* save cursor position and attributes */
      save_term_state();
      goto esc_done;
    case '8': /* restore cursor position and attributes */
      restore_term_state();
      goto esc_done;
    case 'E': /* next line */
      video_movesol(); /* fall through */
    case 'D': /* index */

      if (video_gety() == video_bottom_margin())
        video_scrollup();
      else
        video_movey(1);
      goto esc_done;
    case 'M': /* reverse index */
      if (video_gety() == video_top_margin())
        video_scrolldown();
      else
        video_movey(-1);
      goto esc_done;
    case 'c': /* reset */
      video_clrscr();
      reset_term();
      goto esc_done;
    default: /* other non-CSI codes */
    esc_done:
      in_esc = NOT_IN_ESC; /* unimplemented */
      break;
  }
}

/* Process sequences that begin with ESC [ */
void escseq_process_csi(int8_t c)
{
  if ((c >= '0' && c <= '9') || c == ';' || c == '?') /* digit or separator */
  {
    /* save the character */
    if (paramch >= MAX_ESC_LEN) /* received too many characters */
    {
      in_esc = NOT_IN_ESC;
      return;
    }
    paramstr[paramch] = c;
    paramch++;
  }
  else
  {
    /* take the appropriate action */
    switch (c)
    {
      case 'A': /* cursor up */
        video_movey(-escseq_get_param(1));
        break;
      case 'B': /* cursor down */
        video_movey(escseq_get_param(1));
        break;
      case 'C': /* cursor forward */
        video_movex(escseq_get_param(1));
        break;
      case 'D': /* cursor back */
        video_movex(-escseq_get_param(1));
        break;
      case 'E': /* cursor to next line */
        video_movey(escseq_get_param(1));
        video_movesol();
        break;
      case 'F': /* cursor to previous line */
        video_movey(-escseq_get_param(1));
        video_movesol();
        break;
      case 'G': /* cursor horizontal absolute */
        video_setx(escseq_get_param(1)-1); /* one-indexed */
        break;
      case 'H': case 'f': /* horizonal and vertical position */
      {
        uint8_t y = escseq_get_param(1);
        uint8_t x = escseq_get_param(1);
       video_gotoxy(x-1, y-1);
        break;
      }
      case 'J': /* erase */
        video_erase(escseq_get_param(0));
        break;
      case 'K': /* erase in line */
        video_eraseline(escseq_get_param(0));
        break;
      case 'L': /* erase in line */
//jar
        _video_scrolldown_lin(video_gety());
        break;
      case 'M': /* erase in line */
//jar
        _video_scrollup_lin(video_gety());
        break;

      case 'm': /* set graphic rendition */
        while (paramptr) /* read attributes until we reach the end */
        {
          uint8_t attr = escseq_get_param(0);
          if (attr == 0 || attr == 27)
            revvideo = 0;
          else if (attr == 7)
            revvideo = 0x80;
        }
        break;
      case 'r': /* set top and bottom margins */
      {
        uint8_t top = escseq_get_param(1);
        uint8_t bottom = escseq_get_param(DISP_BUFFER_HIGH);
        video_set_margins(top-1, bottom-1);
        break;
      }
      default: /* unknown */
        break;
    }

    in_esc = NOT_IN_ESC;
  }
}

void escseq_csi_start()
{
  paramch = 0;
  memset(paramstr, 0, MAX_ESC_LEN+1);
  paramptr = &paramstr[0];
}

uint8_t escseq_get_param(uint8_t defaultval)
{
uint8_t val;
 int8_t *startptr;
int8_t *endptr;

  if (!paramptr)
    return defaultval;

   val = defaultval;
  startptr = paramptr;
 /* get everything up to the semicolon, move past it */
 endptr = strchr(paramptr, ';');
  if (endptr)
  {
    *endptr = '\0'; /* replace the semicolon to make atoi stop here */
    paramptr = endptr+1;
  }
  else
    paramptr = NULL;

  /* ascii to integer, as long as the string isn't empty */
  /* default value is given if the string is empty */
  if (*startptr)
    val = atoi(startptr); /* will read up to the null */
  

  return val;
}

void save_term_state()
{
//jar
//  savedstate.cx = video_getx();
//  savedstate.cy = video_gety();
  savedstate.graphicchars = graphicchars;
  savedstate.revvideo = revvideo;
}

void restore_term_state()
{
//jar
//  video_gotoxy(savedstate.cx, savedstate.cy);
  graphicchars = savedstate.graphicchars;
  revvideo = savedstate.revvideo;
} 

void reset_term()
{
  graphicchars = 0;
  revvideo = 0;
  in_esc = 0;
  save_term_state();
} 


void video_reset_margins()
{
  video_set_margins(0, DISP_BUFFER_HIGH-1);
}

void video_set_margins(int8_t top, int8_t bottom)
{
  /* sanitize input */
  if (top < 0) top = 0;
  if (bottom >= DISP_BUFFER_HIGH) bottom = DISP_BUFFER_HIGH-1;
  if (top >= bottom) { top = 0; bottom = DISP_BUFFER_HIGH-1; }

  mtop = top;
  mbottom = bottom;
  video_gotoxy(mtop, 0);
}

int8_t video_top_margin()
{
  return mtop;
}

int8_t video_bottom_margin()
{
  return mbottom;
}

void video_set_reverse(uint8_t val)
{
  revvideo = (val) ? 0x80 : 0;
}

static void _video_scrollup()
{
uint8_t i,j;
//jar
/*
  memmove(&disp_buffer[mtop], &disp_buffer[mtop+1], (mbottom-mtop)*DISP_BUFFER_WIDE);
  memset(&disp_buffer[mbottom], revvideo, DISP_BUFFER_WIDE);
*/
for (j=1;j<(mbottom+1);j++)
	{
	for (i=0;i<DISP_BUFFER_WIDE;i++)
		{
		disp_buffer[j-1][i] = disp_buffer[j][i];
		color_buffer[j-1][i] = color_buffer[j][i];
		}
	}	
for (i=0;i<DISP_BUFFER_WIDE;i++)
	{
	disp_buffer[mbottom][i] = ' ';
	color_buffer[mbottom][i] = 0;
	}

}

static void _video_scrollup_lin(uint8_t lin)
{
uint8_t i,j;
//jar
/*
  memmove(&disp_buffer[mtop], &disp_buffer[mtop+1], (mbottom-mtop)*DISP_BUFFER_WIDE);
  memset(&disp_buffer[mbottom], revvideo, DISP_BUFFER_WIDE);
*/
for (j=(lin+1);j<(mbottom+1);j++)
	{
	for (i=0;i<DISP_BUFFER_WIDE;i++)
		{
		disp_buffer[j-1][i] = disp_buffer[j][i];
		color_buffer[j-1][i] = color_buffer[j][i];
		}
	}	
/*
for (i=0;i<DISP_BUFFER_WIDE;i++)
	{
	disp_buffer[lin][i] = ' ';
	}
*/
}

static void _video_scrolldown()
{
uint8_t i,j;
//  memmove(&disp_buffer[mtop+1], &disp_buffer[mtop], (mbottom-mtop)*DISP_BUFFER_WIDE);
//  memset(&disp_buffer[mtop], revvideo, DISP_BUFFER_WIDE);
for (j=mbottom;j>0;j--)
	{
	for (i=0;i<DISP_BUFFER_WIDE;i++)
		{
		disp_buffer[j][i] = disp_buffer[j-1][i];
		color_buffer[j][i] = color_buffer[j-1][i];
		}
	}	
for (i=0;i<DISP_BUFFER_WIDE;i++)
	{
	disp_buffer[mtop][i] = ' ';
	color_buffer[mtop][i] = 0;
	}
}

static void _video_scrolldown_lin(uint8_t line)
{
uint8_t i,j;
//  memmove(&disp_buffer[mtop+1], &disp_buffer[mtop], (mbottom-mtop)*DISP_BUFFER_WIDE);
//  memset(&disp_buffer[mtop], revvideo, DISP_BUFFER_WIDE);
for (j=mbottom;j>(line);j--)
	{
	for (i=0;i<DISP_BUFFER_WIDE;i++)
		{
		disp_buffer[j][i] = disp_buffer[j-1][i];
		color_buffer[j][i] = color_buffer[j-1][i];
		}
	}	
/*
for (i=0;i<DISP_BUFFER_WIDE;i++)
	{
	disp_buffer[mtop][i] = '-';
	}
*/
}



void video_scrollup()
{
  CURSOR_INVERT();
  _video_scrollup();
  CURSOR_INVERT();
}

void video_scrolldown()
{
  CURSOR_INVERT();
  _video_scrolldown();
  CURSOR_INVERT();
}

void video_movesol()
{
  CURSOR_INVERT();
  cx = 0;
  CURSOR_INVERT();
}

void video_setx(int8_t x)
{
  CURSOR_INVERT();
  cx = x;
  if (cx < 0) cx = 0;
  if (cx >= DISP_BUFFER_WIDE) cx = DISP_BUFFER_WIDE-1;
  CURSOR_INVERT();
}

/* Absolute positioning does not respect top/bottom margins */
void video_gotoxy(int8_t x, int8_t y)
{
  CURSOR_INVERT();
  cx = x;
  if (cx < 0) cx = 0;
  if (cx >= DISP_BUFFER_WIDE) cx = DISP_BUFFER_WIDE-1;
  cy = y;
  if (cy < 0) cy = 0;
  if (cy >= DISP_BUFFER_HIGH) cy = DISP_BUFFER_HIGH-1;
  CURSOR_INVERT();
}

void video_movex(int8_t dx)
{
  CURSOR_INVERT();
  cx += dx;
  if (cx < 0) cx = 0;
  if (cx >= DISP_BUFFER_WIDE) cx = DISP_BUFFER_WIDE-1;
  CURSOR_INVERT();
}

void video_movey(int8_t dy)
{
  CURSOR_INVERT();
  cy += dy;
  if (cy < mtop) cy = mtop;
  if (cy > mbottom) cy = mbottom;
  CURSOR_INVERT();
}

static void _video_lfwd()
{
  cx = 0;
  if (++cy > mbottom)
  {
    cy = mbottom;
    _video_scrollup();
  }
}

static void _video_cfwd()
{
  if (++cx > DISP_BUFFER_WIDE)
    _video_lfwd();
}

void video_cfwd()
{
  CURSOR_INVERT();
  _video_cfwd();
  CURSOR_INVERT();
}

void video_lfwd()
{
  CURSOR_INVERT();
  cx = 0;
  if (++cy > mbottom)
  {
    cy = mbottom;
    _video_scrollup();
  }
  CURSOR_INVERT();
}

void video_lf()
{
  CURSOR_INVERT();
  if (++cy > mbottom)
  {
    cy = mbottom;
    _video_scrollup();
  }
  CURSOR_INVERT();
}

static void _video_lback()
{
  cx = DISP_BUFFER_WIDE-1;
  if (--cy < 0)
  { cx = 0; cy = mtop; }
}

void video_lback()
{
  CURSOR_INVERT();
  cx = DISP_BUFFER_WIDE-1;
  if (--cy < 0)
  { cx = 0; cy = mtop; }
  CURSOR_INVERT();
}

void video_cback()
{
  CURSOR_INVERT();
  if (--cx < 0)
    _video_lback();
  CURSOR_INVERT();
}

int8_t video_getx()
{
  return cx;
}

int8_t video_gety(void)
{
  return cy;
}

int8_t video_charat(int8_t x, int8_t y)
{
  return disp_buffer[cy][cx];
}

void video_clrscr()
{
  CURSOR_INVERT();
  video_reset_margins(); 
  memset(disp_buffer, revvideo, DISP_BUFFER_WIDE*DISP_BUFFER_HIGH);
  memset(color_buffer, 0, DISP_BUFFER_WIDE*DISP_BUFFER_HIGH);
  cx = cy = 0;
  CURSOR_INVERT();
}

void video_clrline()
{
  CURSOR_INVERT();
  memset(&disp_buffer[cy], revvideo, DISP_BUFFER_WIDE);
  memset(&color_buffer[cy], 0, DISP_BUFFER_WIDE);
  cx = 0;
  CURSOR_INVERT();
}

void video_clreol()
{
  memset(&disp_buffer[cy][cx], revvideo, DISP_BUFFER_WIDE-cx);
  memset(&color_buffer[cy][cx], 0, DISP_BUFFER_WIDE-cx);
}

void video_erase(uint8_t erasemode)
{
uint8_t x,y;
  CURSOR_INVERT();
  switch(erasemode)
  {
    case 0: /* erase from cursor to end of screen */
//jar
//      memset(&disp_buffer[cy][cx], revvideo,
//          (DISP_BUFFER_WIDE*DISP_BUFFER_HIGH)-(cy*DISP_BUFFER_WIDE+cx));
	for (x=cx;x<DISP_BUFFER_WIDE;x++) 
		for (y=cy;y<DISP_BUFFER_HIGH;y++) 
			{
			disp_buffer[y][x] = ' ';
			color_buffer[y][x] = 0;
			}
      break;
    case 1: /* erase from beginning of screen to cursor */
//jar
//      memset(disp_buffer, revvideo, cy*DISP_BUFFER_WIDE+cx+1);
	for (x=0;x<cx;x++) 
		for (y=0;y<cy;y++) 
			{
			disp_buffer[y][x] = ' ';
			color_buffer[y][x] = 0;
			}
      break;
    case 2: /* erase entire screen */
//jar
//      memset(disp_buffer, revvideo, DISP_BUFFER_WIDE*DISP_BUFFER_HIGH);
	for (x=0;x<DISP_BUFFER_WIDE;x++) 
		for (y=0;y<DISP_BUFFER_HIGH;y++) 
			{
			disp_buffer[y][x] = ' ';
			color_buffer[y][x] = ' ';
			}
      break;
  }
  CURSOR_INVERT();
}

void video_eraseline(uint8_t erasemode)
{
 uint8_t y,x;
  CURSOR_INVERT();
  switch(erasemode)
  {
    case 0: /* erase from cursor to end of line */
//jar
	for (x=cx;x<DISP_BUFFER_WIDE;x++) 
		{
		disp_buffer[cy][x] = ' ';
		color_buffer[cy][x] = 0;
		}
//      memset(&disp_buffer[cy][cx], revvideo, DISP_BUFFER_WIDE-cx);
      break;
    case 1: /* erase from beginning of line to cursor */
//jar
	for (x=0;x<cx;x++)
				{
		disp_buffer[cy][x] = ' ';
		color_buffer[cy][x] = 0;
		}

//      memset(&disp_buffer[cy], revvideo, cx+1);
      break;
    case 2: /* erase entire line */
//jar
	for (x=0;x<DISP_BUFFER_WIDE;x++)
				{
		disp_buffer[cy][x] = ' ';
		color_buffer[cy][x] = 0;
		}

//      memset(&disp_buffer[cy], revvideo, DISP_BUFFER_WIDE);
      break;
  }
  CURSOR_INVERT();
}

/* Does not respect top/bottom margins */
void video_putcxy(int8_t x, int8_t y, int8_t c)
{
  if (x < 0 || x >= DISP_BUFFER_WIDE) return;
  if (y < 0 || y >= DISP_BUFFER_HIGH) return;
  disp_buffer[y][x] = c ^ revvideo;
  color_buffer[y][x] = color_composite;
}

/* Does not respect top/bottom margins */
void video_putsxy(int8_t x, int8_t y, int8_t *str)
{
	int16_t len;
  if (x < 0 || x >= DISP_BUFFER_WIDE) return;
  if (y < 0 || y >= DISP_BUFFER_HIGH) return;
  len = strlen(str);
  if (len > DISP_BUFFER_WIDE-x) len = DISP_BUFFER_WIDE-x;
  memcpy((int8_t *)(&disp_buffer[y][x]), str, len);
  memset((int8_t *)(&color_buffer[y][x]), color_composite, len);
  if (revvideo) video_invert_range(x, y, len);
}


/* Does not respect top/bottom margins */
void video_putline(int8_t y, int8_t *str)
{
  if (y < 0 || y >= DISP_BUFFER_HIGH) return;
  /* strncpy fills unused bytes in the destination with nulls */
  strncpy((int8_t *)(&disp_buffer[y]), str, DISP_BUFFER_WIDE);
  memset((int8_t *)(&color_buffer[y]), color_composite, strlen(str));
  if (revvideo) video_invert_range(0, y, DISP_BUFFER_WIDE);
}

void video_setc(int8_t c)
{
  CURSOR_INVERT();
  disp_buffer[cy][cx] = c ^ revvideo;
  color_buffer[cy][cx] = color_composite;
  CURSOR_INVERT();
}

static void _video_putc(int8_t c)
{
  /* If the last character printed exceeded the right boundary,
   * we have to go to a new line. */
  if (cx >= DISP_BUFFER_WIDE) _video_lfwd();

  if (c == '\r') cx = 0;
  else if (c == '\n') _video_lfwd();
  else
  {
    disp_buffer[cy][cx] = c ^ revvideo;
	color_buffer[cy][cx] = color_composite;
    _video_cfwd();
  }
}

void video_putc(int8_t c)
{
  CURSOR_INVERT();
  _video_putc(c);
  CURSOR_INVERT();
}

void video_putc_raw(int8_t c)
{
  CURSOR_INVERT();
  
  /* If the last character printed exceeded the right boundary,
   * we have to go to a new line. */
  if (cx >= DISP_BUFFER_WIDE) _video_lfwd();
//jar
//  disp_buffer[cy][cx] = c ^ revvideo;
  disp_buffer[cy][cx] = c;
  color_buffer[cy][cx] = color_composite;
  _video_cfwd();
  CURSOR_INVERT();
}

void video_puts(int8_t *str)
{
  /* Characters are interpreted and printed one at a time. */
  int8_t c;
  CURSOR_INVERT();
  while ((c = *str++))
    _video_putc(c);
  CURSOR_INVERT();
}

void video_show_cursor()
{
  if (!showcursor)
  {
    showcursor = 0x80;
    CURSOR_INVERT();
  }
}

void video_hide_cursor()
{
  if (showcursor)
  {
    CURSOR_INVERT();
    showcursor = 0;
  }
}

uint8_t video_cursor_visible()
{
  return showcursor != 0;
}

void video_invert_range(int8_t x, int8_t y, uint8_t rangelen)
{
  int8_t *start = &disp_buffer[y][x];
  uint8_t i;
  for (i = 0; i < rangelen; i++)
  {
    *start ^= 0x80;
    start++;
  }
} 

static void CURSOR_INVERT(void)
{
//jar
//  disp_buffer[cy][cx] ^= showcursor;
}

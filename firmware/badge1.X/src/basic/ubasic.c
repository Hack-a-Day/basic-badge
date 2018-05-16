/*
 * Copyright (c) 2006, Adam Dunkels
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the author nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#define DEBUG 0

#if DEBUG
#define DEBUG_PRINTF(...)  printf(__VA_ARGS__)
#else
#define DEBUG_PRINTF(...)
#endif


#include "ubasic.h"
#include "../hw.h"
#include "../vt100.h"
#include "tokenizer.h"

#include <stdio.h> /* printf() */
#include <stdlib.h> /* exit() */
#include <setjmp.h>

extern jmp_buf jbuf;
extern uint8_t handle_display;
extern int8_t disp_buffer[DISP_BUFFER_HIGH+1][DISP_BUFFER_WIDE];
extern int8_t color_buffer[DISP_BUFFER_HIGH+1][DISP_BUFFER_WIDE];

char err_msg[40];
int last_linenum;

static char const *program_ptr;
#define MAX_STRINGLEN 40
static char string[MAX_STRINGLEN];

#define MAX_GOSUB_STACK_DEPTH 10
static int gosub_stack[MAX_GOSUB_STACK_DEPTH];
static int gosub_stack_ptr;

struct for_state {
  int line_after_for;
  int for_variable;
  int to;
};
#define MAX_FOR_STACK_DEPTH 4
static struct for_state for_stack[MAX_FOR_STACK_DEPTH];
static int for_stack_ptr;

#define MAX_VARNUM 26
static int variables[MAX_VARNUM];

static int ended;

uint8_t term_vt100=1;
unsigned int term_x=0,term_y=0;

static int expr(void);
static void line_statement(void);
static void statement(void);
extern volatile int8_t brk_key;


//B_BAS009
/*---------------------------------------------------------------------------*/
void
ubasic_init(const char *program)
{
  program_ptr = program;
  for_stack_ptr = gosub_stack_ptr = 0;
  tokenizer_init(program);
  ended = 0;
  term_vt100=1;
  last_linenum = 0;
}
/*---------------------------------------------------------------------------*/
static void
accept(int token)
{
  if(token != tokenizer_token()) {
	sprintf(err_msg,"Bad token %d at line %d\n", token,last_linenum);
	stdio_write(err_msg);
    tokenizer_error_print();
	longjmp(jbuf,1);             // jumps back to where setjmp was called - making setjmp now return 1
    exit(1);
  }
  DEBUG_PRINTF("Expected %d, got it\n", token);
  tokenizer_next();
}
/*---------------------------------------------------------------------------*/
static int
varfactor(void)
{
  int r;
  DEBUG_PRINTF("varfactor: obtaining %d from variable %d\n", variables[tokenizer_variable_num()], tokenizer_variable_num());
  r = ubasic_get_variable(tokenizer_variable_num());
  accept(TOKENIZER_VARIABLE);
  return r;
}

//B_BAS004
/*---------------------------------------------------------------------------*/
static int
factor(void)
{
  int r;
  long temp;
  DEBUG_PRINTF("factor: token %d\n", tokenizer_token());
  switch(tokenizer_token()) {
  case TOKENIZER_NUMBER:
    r = tokenizer_num();
    DEBUG_PRINTF("factor: number %d\n", r);
    accept(TOKENIZER_NUMBER);
    break;
  case TOKENIZER_LEFTPAREN:
    accept(TOKENIZER_LEFTPAREN);
    r = expr();
    accept(TOKENIZER_RIGHTPAREN);
    break;
  case TOKENIZER_RND:
	accept(TOKENIZER_RND);
    r = expr();
	temp = get_rnd();
	temp = temp * (r+1);
	r = temp / 65535;
    break;	
   case TOKENIZER_EIN:
	accept(TOKENIZER_EIN);
    r = expr();
	r = exp_get(r);
    break;
	
   case TOKENIZER_UIN:
	accept(TOKENIZER_UIN);
    r = expr();
	if (r==0)
		{
		if (rx_sta()!=0)
			r = rx_read();
		else
			r = 0;
		}
	else
		{
		while (1)
			{
			if (rx_sta()!=0)
				{
				r = rx_read();
				break;
				}
			if (brk_key)
				break;
			}
		}
    break;

	  case TOKENIZER_INPUT:
		accept(TOKENIZER_INPUT);
		if(tokenizer_token() == TOKENIZER_STRING) 
			{
			tokenizer_string(string, sizeof(string));
			stdio_write(string);
			tokenizer_next();
			}		  
		  r = get_user_value();
		  break;

   default:
	r = varfactor();
	break;
	}
return r;
}
/*---------------------------------------------------------------------------*/
static int
term(void)
{
  int f1, f2;
  int op;

  f1 = factor();
  op = tokenizer_token();
  DEBUG_PRINTF("term: token %d\n", op);
  while(op == TOKENIZER_ASTR ||
	op == TOKENIZER_SLASH ||
	op == TOKENIZER_MOD) {
    tokenizer_next();
    f2 = factor();
    DEBUG_PRINTF("term: %d %d %d\n", f1, op, f2);
    switch(op) {
    case TOKENIZER_ASTR:
      f1 = f1 * f2;
      break;
    case TOKENIZER_SLASH:
      f1 = f1 / f2;
      break;
    case TOKENIZER_MOD:
      f1 = f1 % f2;
      break;
    }
    op = tokenizer_token();
  }
  DEBUG_PRINTF("term: %d\n", f1);
  return f1;
}
/*---------------------------------------------------------------------------*/
static int
expr(void)
{
  int t1, t2;
  int op;
  
  t1 = term();
  op = tokenizer_token();
  DEBUG_PRINTF("expr: token %d\n", op);
  while(op == TOKENIZER_PLUS ||
	op == TOKENIZER_MINUS ||
	op == TOKENIZER_AND ||
	op == TOKENIZER_OR) {
    tokenizer_next();
    t2 = term();
    DEBUG_PRINTF("expr: %d %d %d\n", t1, op, t2);
    switch(op) {
    case TOKENIZER_PLUS:
      t1 = t1 + t2;
      break;
    case TOKENIZER_MINUS:
      t1 = t1 - t2;
      break;
    case TOKENIZER_AND:
      t1 = t1 & t2;
      break;
    case TOKENIZER_OR:
      t1 = t1 | t2;
      break;
    }
    op = tokenizer_token();
  }
  DEBUG_PRINTF("expr: %d\n", t1);
  return t1;
}
/*---------------------------------------------------------------------------*/
static int
relation(void)
{
  int r1, r2;
  int op;
  
  r1 = expr();
  op = tokenizer_token();
  DEBUG_PRINTF("relation: token %d\n", op);
  while(op == TOKENIZER_LT ||
	op == TOKENIZER_GT ||
	op == TOKENIZER_EQ) {
    tokenizer_next();
    r2 = expr();
    DEBUG_PRINTF("relation: %d %d %d\n", r1, op, r2);
    switch(op) {
    case TOKENIZER_LT:
      r1 = r1 < r2;
      break;
    case TOKENIZER_GT:
      r1 = r1 > r2;
      break;
    case TOKENIZER_EQ:
      r1 = r1 == r2;
      break;
    }
    op = tokenizer_token();
  }
  return r1;
}
/*---------------------------------------------------------------------------*/
static void
jump_linenum(int linenum)
{
  tokenizer_init(program_ptr);
  while(tokenizer_num() != linenum) {
    do {
      do {
	tokenizer_next();
      } while(tokenizer_token() != TOKENIZER_CR &&
	      tokenizer_token() != TOKENIZER_ENDOFINPUT);
      if(tokenizer_token() == TOKENIZER_CR) {
	tokenizer_next();
      }
    } while(tokenizer_token() != TOKENIZER_NUMBER);
    DEBUG_PRINTF("jump_linenum: Found line %d\n", tokenizer_num());
  }
}
/*---------------------------------------------------------------------------*/
static void
goto_statement(void)
{
  accept(TOKENIZER_GOTO);
  jump_linenum(tokenizer_num());
}
/*---------------------------------------------------------------------------*/
static void
print_statement(void)
{
char str_out[40];
sprintf(str_out," ");
  accept(TOKENIZER_PRINT);
  do {
    DEBUG_PRINTF("Print loop\n");
    if(tokenizer_token() == TOKENIZER_STRING) {
      tokenizer_string(string, sizeof(string));
      sprintf(str_out,"%s", string);
      tokenizer_next();
    } else if(tokenizer_token() == TOKENIZER_COMMA) {
      sprintf(str_out," ");
      tokenizer_next();
    } else if(tokenizer_token() == TOKENIZER_SEMICOLON) {
      tokenizer_next();
    } else  {
      sprintf(str_out,"%d", expr());
    } 
  } while(tokenizer_token() != TOKENIZER_CR &&
	  tokenizer_token() != TOKENIZER_ENDOFINPUT);
	stdio_write(str_out);
  DEBUG_PRINTF("End of print\n");
  tokenizer_next();
}
/*---------------------------------------------------------------------------*/
println_statement(void)
{
char str_out[40];
sprintf(str_out," ");
  accept(TOKENIZER_PRINTLN);
  do {
    DEBUG_PRINTF("Print loop\n");
    if(tokenizer_token() == TOKENIZER_STRING) {
      tokenizer_string(string, sizeof(string));
      sprintf(str_out,"%s\n", string);
      tokenizer_next();
    } else if(tokenizer_token() == TOKENIZER_COMMA) {
      sprintf(str_out," \n");
      tokenizer_next();
    } else if(tokenizer_token() == TOKENIZER_SEMICOLON) {
      tokenizer_next();
    } else{
      sprintf(str_out,"%d\n", expr());
    }
  } while(tokenizer_token() != TOKENIZER_CR &&
	  tokenizer_token() != TOKENIZER_ENDOFINPUT);
	stdio_write(str_out);
  DEBUG_PRINTF("End of print\n");
  tokenizer_next();
}
/*---------------------------------------------------------------------------*/
static void
if_statement(void)
{
  int r;
  
  accept(TOKENIZER_IF);

  r = relation();
  DEBUG_PRINTF("if_statement: relation %d\n", r);
  accept(TOKENIZER_THEN);
  if(r) {
    statement();
  } else {
    do {
      tokenizer_next();
    } while(tokenizer_token() != TOKENIZER_ELSE &&
	    tokenizer_token() != TOKENIZER_CR &&
	    tokenizer_token() != TOKENIZER_ENDOFINPUT);
    if(tokenizer_token() == TOKENIZER_ELSE) {
      tokenizer_next();
      statement();
    } else if(tokenizer_token() == TOKENIZER_CR) {
      tokenizer_next();
    }
  }
}
/*---------------------------------------------------------------------------*/
static void
let_statement(void)
{
  int var;

  var = tokenizer_variable_num();

  accept(TOKENIZER_VARIABLE);
  accept(TOKENIZER_EQ);
  ubasic_set_variable(var, expr());
  DEBUG_PRINTF("let_statement: assign %d to %d\n", variables[var], var);
  accept(TOKENIZER_CR);

}
/*---------------------------------------------------------------------------*/
static void
gosub_statement(void)
{
  int linenum;
  accept(TOKENIZER_GOSUB);
  linenum = tokenizer_num();
  accept(TOKENIZER_NUMBER);
  accept(TOKENIZER_CR);
  if(gosub_stack_ptr < MAX_GOSUB_STACK_DEPTH) {
    gosub_stack[gosub_stack_ptr] = tokenizer_num();
    gosub_stack_ptr++;
    jump_linenum(linenum);
  } else {
    DEBUG_PRINTF("gosub_statement: gosub stack exhausted\n");
  }
}
/*---------------------------------------------------------------------------*/
static void
return_statement(void)
{
  accept(TOKENIZER_RETURN);
  if(gosub_stack_ptr > 0) {
    gosub_stack_ptr--;
    jump_linenum(gosub_stack[gosub_stack_ptr]);
  } else {
    DEBUG_PRINTF("return_statement: non-matching return\n");
  }
}
/*---------------------------------------------------------------------------*/
static void
next_statement(void)
{
  int var;
  
  accept(TOKENIZER_NEXT);
  var = tokenizer_variable_num();
  accept(TOKENIZER_VARIABLE);
  if(for_stack_ptr > 0 &&
     var == for_stack[for_stack_ptr - 1].for_variable) {
    ubasic_set_variable(var,
			ubasic_get_variable(var) + 1);
    if(ubasic_get_variable(var) <= for_stack[for_stack_ptr - 1].to) {
      jump_linenum(for_stack[for_stack_ptr - 1].line_after_for);
    } else {
      for_stack_ptr--;
      accept(TOKENIZER_CR);
    }
  } else {
    DEBUG_PRINTF("next_statement: non-matching next (expected %d, found %d)\n", for_stack[for_stack_ptr - 1].for_variable, var);
    accept(TOKENIZER_CR);
  }

}
/*---------------------------------------------------------------------------*/
static void
for_statement(void)
{
  int for_variable, to;
  
  accept(TOKENIZER_FOR);
  for_variable = tokenizer_variable_num();
  accept(TOKENIZER_VARIABLE);
  accept(TOKENIZER_EQ);
  ubasic_set_variable(for_variable, expr());
  accept(TOKENIZER_TO);
  to = expr();
  accept(TOKENIZER_CR);

  if(for_stack_ptr < MAX_FOR_STACK_DEPTH) {
    for_stack[for_stack_ptr].line_after_for = tokenizer_num();
    for_stack[for_stack_ptr].for_variable = for_variable;
    for_stack[for_stack_ptr].to = to;
    DEBUG_PRINTF("for_statement: new for, var %d to %d\n",
		 for_stack[for_stack_ptr].for_variable,
		 for_stack[for_stack_ptr].to);
		 
    for_stack_ptr++;
  } else {
    DEBUG_PRINTF("for_statement: for stack depth exceeded\n");
  }
}
/*---------------------------------------------------------------------------*/
static void
end_statement(void)
{
  accept(TOKENIZER_END);
  ended = 1;
}
/*---------------------------------------------------------------------------*/
static void
out_statement(void)
{
char str_out[40];

  accept(TOKENIZER_OUT);
   if(tokenizer_token() == TOKENIZER_VARIABLE ||
	      tokenizer_token() == TOKENIZER_NUMBER)
	   {
		sprintf(str_out,"out - %d\n", expr());
	   }
  DEBUG_PRINTF("End of out\n");
  tokenizer_next();
}
/*---------------------------------------------------------------------------*/
static void
tune_statement(void)
{
char str_out[40];
int tone1,tone2,tone3,duration;

	accept(TOKENIZER_TUNE);
	if(tokenizer_token() == TOKENIZER_VARIABLE || tokenizer_token() == TOKENIZER_NUMBER) tone1 =  expr();
	accept(TOKENIZER_COMMA);
	if(tokenizer_token() == TOKENIZER_VARIABLE || tokenizer_token() == TOKENIZER_NUMBER) tone2 =  expr();
	accept(TOKENIZER_COMMA);
	if(tokenizer_token() == TOKENIZER_VARIABLE || tokenizer_token() == TOKENIZER_NUMBER) tone3 =  expr();
	accept(TOKENIZER_COMMA);
	if(tokenizer_token() == TOKENIZER_VARIABLE || tokenizer_token() == TOKENIZER_NUMBER) duration =  expr();
	//sprintf(str_out,"playing %d %d %d for %d\n",tone1,tone2,tone3,duration);
	//stdio_write(str_out);
	//now perform the actual beep
	sound_play_notes(tone1,tone2,tone3,duration);
	tokenizer_next();
}
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static void
setxy_statement(void)
{
	int x_temp,y_temp;
	accept(TOKENIZER_SETXY);
	if(tokenizer_token() == TOKENIZER_VARIABLE || tokenizer_token() == TOKENIZER_NUMBER) x_temp =  expr();
	accept(TOKENIZER_COMMA);
	if(tokenizer_token() == TOKENIZER_VARIABLE || tokenizer_token() == TOKENIZER_NUMBER) y_temp =  expr();
	tokenizer_next();
	video_gotoxy(x_temp,y_temp);
}
/*---------------------------------------------------------------------------*/
static void
termt_statement(void)
{
int type;
	accept(TOKENIZER_TERMT);
	if(tokenizer_token() == TOKENIZER_VARIABLE || tokenizer_token() == TOKENIZER_NUMBER) type =  expr();
	if (type==0) 
		{
		term_vt100 = 0;
		handle_display = 0;
		}
	else 
		{
		term_vt100 = 1;
		handle_display = 1;
		}
	tokenizer_next();
}
/*---------------------------------------------------------------------------*/
static void
clrscr_statement(void)
{
int type;
	accept(TOKENIZER_CLRSCR);
	video_clrscr();
	tokenizer_next();
}
/*---------------------------------------------------------------------------*/
static void
wait_statement(void)
{
int time;
	accept(TOKENIZER_WAIT);
	if(tokenizer_token() == TOKENIZER_VARIABLE || tokenizer_token() == TOKENIZER_NUMBER) time =  expr();
	wait_ms(time);
	tokenizer_next();
}
/*---------------------------------------------------------------------------*/
static void
led_statement(void)
{
int led_p,led_v;
	accept(TOKENIZER_LED);
	if(tokenizer_token() == TOKENIZER_VARIABLE || tokenizer_token() == TOKENIZER_NUMBER) led_p =  expr();
	accept(TOKENIZER_COMMA);
	if(tokenizer_token() == TOKENIZER_VARIABLE || tokenizer_token() == TOKENIZER_NUMBER) led_v =  expr();
	//now set up led_p to value led_v
	set_led(led_p,led_v);
	tokenizer_next();
}
/*---------------------------------------------------------------------------*/
static void
color_statement(void)
{
int c1,c2;
	accept(TOKENIZER_COLOR);
	if(tokenizer_token() == TOKENIZER_VARIABLE || tokenizer_token() == TOKENIZER_NUMBER) c1 =  expr();
	accept(TOKENIZER_COMMA);
	if(tokenizer_token() == TOKENIZER_VARIABLE || tokenizer_token() == TOKENIZER_NUMBER) c2 =  expr();
	//now set up led_p to value led_v
	video_set_color(c1,c2);
	tokenizer_next();
}
/*---------------------------------------------------------------------------*/
static void
chr_statement(void)
{
int var,c1;
long temp;
	accept(TOKENIZER_CHR);
	if(tokenizer_token() == TOKENIZER_VARIABLE || tokenizer_token() == TOKENIZER_NUMBER) c1 =  expr();
	stdio_c(c1);
	tokenizer_next();
}
/*---------------------------------------------------------------------------*/
static void
edr_statement(void)
{
int c1,c2;
	accept(TOKENIZER_EDR);
	if(tokenizer_token() == TOKENIZER_VARIABLE || tokenizer_token() == TOKENIZER_NUMBER) c1 =  expr();
	accept(TOKENIZER_COMMA);
	if(tokenizer_token() == TOKENIZER_VARIABLE || tokenizer_token() == TOKENIZER_NUMBER) c2 =  expr();
	exp_ddr(c1,c2);
	tokenizer_next();
}
/*---------------------------------------------------------------------------*/
static void
eout_statement(void)
{
int c1,c2;
	accept(TOKENIZER_EOUT);
	if(tokenizer_token() == TOKENIZER_VARIABLE || tokenizer_token() == TOKENIZER_NUMBER) c1 =  expr();
	accept(TOKENIZER_COMMA);
	if(tokenizer_token() == TOKENIZER_VARIABLE || tokenizer_token() == TOKENIZER_NUMBER) c2 =  expr();
	exp_set(c1,c2);
	tokenizer_next();
}
/*---------------------------------------------------------------------------*/
static void
termup_statement(void)
{
	accept(TOKENIZER_TERMUP);
	if (handle_display==0)
		tft_disp_buffer_refresh(disp_buffer,color_buffer);
	tokenizer_next();
}
/*---------------------------------------------------------------------------*/
static void
rem_statement(void)
{
	accept(TOKENIZER_REM);
     do {
	tokenizer_next();
      } while(tokenizer_token() != TOKENIZER_CR &&
	      tokenizer_token() != TOKENIZER_ENDOFINPUT);
      if(tokenizer_token() == TOKENIZER_CR) {
	tokenizer_next();
      }
//	tokenizer_next();
}
/*---------------------------------------------------------------------------*/
static void
uout_statement(void)
{
int c1;
	accept(TOKENIZER_UOUT);
	if(tokenizer_token() == TOKENIZER_VARIABLE || tokenizer_token() == TOKENIZER_NUMBER) c1 =  expr();
	tx_write(c1);
	tokenizer_next();
}

//B_BAS002
/*---------------------------------------------------------------------------*/
static void
statement(void)
{
  int token;
  
  token = tokenizer_token();
  
  switch(token) {
  case TOKENIZER_PRINT:
    print_statement();
    break;
  case TOKENIZER_PRINTLN:
    println_statement();
    break;
  case TOKENIZER_IF:
    if_statement();
    break;
  case TOKENIZER_GOTO:
    goto_statement();
    break;
  case TOKENIZER_GOSUB:
    gosub_statement();
    break;
  case TOKENIZER_RETURN:
    return_statement();
    break;
  case TOKENIZER_FOR:
    for_statement();
    break;
  case TOKENIZER_NEXT:
    next_statement();
    break;
  case TOKENIZER_END:
    end_statement();
    break;
  case TOKENIZER_OUT:
    out_statement();
    break;
  case TOKENIZER_TUNE:
    tune_statement();
    break;
  case TOKENIZER_LET:
    accept(TOKENIZER_LET);
    /* Fall through. */
  case TOKENIZER_VARIABLE:
    let_statement();
    break;
  case TOKENIZER_SETXY:
    setxy_statement();
    break;
  case TOKENIZER_TERMT:
    termt_statement();
    break;
  case TOKENIZER_CLRSCR:
    clrscr_statement();
    break;
  case TOKENIZER_WAIT:
    wait_statement();
    break;	
  case TOKENIZER_LED:
    led_statement();
    break;	  
  case TOKENIZER_COLOR:
    color_statement();
    break;
  case TOKENIZER_CHR:
    chr_statement();
    break;
  case TOKENIZER_EDR:
    edr_statement();
    break;
  case TOKENIZER_EOUT:
    eout_statement();
    break;
  case TOKENIZER_TERMUP:
    termup_statement();
    break;
  case TOKENIZER_REM:
    rem_statement();
    break;
  case TOKENIZER_UOUT:
    uout_statement();
    break;
  default:
    sprintf(err_msg,"Bad token %d at line %d\n", token,last_linenum);
	stdio_write(err_msg);
    //exit(1);
	longjmp(jbuf,1);             // jumps back to where setjmp was called - making setjmp now return 1
  }
}
/*---------------------------------------------------------------------------*/
static void
line_statement(void)
{
  DEBUG_PRINTF("----------- Line number %d ---------\n", tokenizer_num());
  last_linenum = tokenizer_num();
  accept(TOKENIZER_NUMBER);
  statement();
  return;
}
//B_BAS010
/*---------------------------------------------------------------------------*/
void
ubasic_run(void)
{
  if(tokenizer_finished()) {
    DEBUG_PRINTF("uBASIC program finished\n");
    return;
  }

  line_statement();
}
/*---------------------------------------------------------------------------*/
int
ubasic_finished(void)
{
  return ended || tokenizer_finished();
}
/*---------------------------------------------------------------------------*/
void
ubasic_set_variable(int varnum, int value)
{
  if(varnum <= MAX_VARNUM) {
    variables[varnum] = value;
  }
}
/*---------------------------------------------------------------------------*/
int
ubasic_get_variable(int varnum)
{
  if(varnum <= MAX_VARNUM) {
    return variables[varnum];
  }
  return 0;
}
/*---------------------------------------------------------------------------*/




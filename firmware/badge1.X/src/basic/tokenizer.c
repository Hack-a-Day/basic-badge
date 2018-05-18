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

#include "tokenizer.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

static char const *ptr, *nextptr;

#define MAX_NUMLEN 5

struct keyword_token {
  char *keyword;
  int token;
};

static int current_token = TOKENIZER_ERROR;

//B_BAS001 - list of tokens
static const struct keyword_token keywords[] = {
  {"let", TOKENIZER_LET},
  {"print", TOKENIZER_PRINT},
  {"println", TOKENIZER_PRINTLN},
  {"if", TOKENIZER_IF},
  {"then", TOKENIZER_THEN},
  {"else", TOKENIZER_ELSE},
  {"for", TOKENIZER_FOR},
  {"to", TOKENIZER_TO},
  {"next", TOKENIZER_NEXT},
  {"goto", TOKENIZER_GOTO},
  {"gosub", TOKENIZER_GOSUB},
  {"return", TOKENIZER_RETURN},
  {"call", TOKENIZER_CALL},
  {"end", TOKENIZER_END},
  {"out", TOKENIZER_OUT},
  {"tune", TOKENIZER_TUNE},
  {"termt", TOKENIZER_TERMT},
  {"setxy", TOKENIZER_SETXY},
  {"clrscr", TOKENIZER_CLRSCR},
  {"wait", TOKENIZER_WAIT},
  {"led", TOKENIZER_LED},
  {"color", TOKENIZER_COLOR},
  {"rnd", TOKENIZER_RND},
  {"chr", TOKENIZER_CHR},
  {"ein", TOKENIZER_EIN},
  {"eout", TOKENIZER_EOUT},
  {"edr", TOKENIZER_EDR},
  {"termup", TOKENIZER_TERMUP},
  {"LET", TOKENIZER_LET},
  {"PRINT", TOKENIZER_PRINT},
  {"PRINTLN", TOKENIZER_PRINTLN},
  {"IF", TOKENIZER_IF},
  {"THEN", TOKENIZER_THEN},
  {"ELSE", TOKENIZER_ELSE},
  {"FOR", TOKENIZER_FOR},
  {"TO", TOKENIZER_TO},
  {"NEXT", TOKENIZER_NEXT},
  {"GOTO", TOKENIZER_GOTO},
  {"GOSUB", TOKENIZER_GOSUB},
  {"RETURN", TOKENIZER_RETURN},
  {"CALL", TOKENIZER_CALL},
  {"END", TOKENIZER_END},
  {"OUT", TOKENIZER_OUT},
  {"TUNE", TOKENIZER_TUNE},
  {"TERMT", TOKENIZER_TERMT},
  {"SETXY", TOKENIZER_SETXY},
  {"CLRSCR", TOKENIZER_CLRSCR},
  {"WAIT", TOKENIZER_WAIT},
  {"LED", TOKENIZER_LED},
  {"COLOR", TOKENIZER_COLOR},
  {"RND", TOKENIZER_RND},
  {"CHR", TOKENIZER_CHR},
  {"EIN", TOKENIZER_EIN},
  {"EOUT", TOKENIZER_EOUT},
  {"EDR", TOKENIZER_EDR},
  {"TERMUP", TOKENIZER_TERMUP},
  {"rem", TOKENIZER_REM},
  {"REM", TOKENIZER_REM},
  {"uin", TOKENIZER_UIN},
  {"UIN", TOKENIZER_UIN},
  {"uout", TOKENIZER_UOUT},
  {"UOUT", TOKENIZER_UOUT},
  {"input", TOKENIZER_INPUT},
  {"INPUT", TOKENIZER_INPUT},
//aliases
  {"clr", TOKENIZER_CLRSCR},
  {"CLR", TOKENIZER_CLRSCR},
  {"pnt", TOKENIZER_PRINT},
  {"PNT", TOKENIZER_PRINT},
  {"ptl", TOKENIZER_PRINTLN},
  {"Ptl", TOKENIZER_PRINTLN},
  {"inp", TOKENIZER_INPUT},
  {"INP", TOKENIZER_INPUT},
  {"sxy", TOKENIZER_SETXY},
  {"SXY", TOKENIZER_SETXY},
  {"ret", TOKENIZER_RETURN},
  {"RET", TOKENIZER_RETURN},  
  {NULL, TOKENIZER_ERROR}
};

/*---------------------------------------------------------------------------*/
static int
singlechar(void)
{
  if(*ptr == '\n') {
    return TOKENIZER_CR;
  } else if(*ptr == '\r') {
  	*ptr++;
    return TOKENIZER_CR;
  } else if(*ptr == ',') {
    return TOKENIZER_COMMA;
  } else if(*ptr == ';') {
    return TOKENIZER_SEMICOLON;
  } else if(*ptr == '+') {
    return TOKENIZER_PLUS;
  } else if(*ptr == '-') {
    return TOKENIZER_MINUS;
  } else if(*ptr == '&') {
    return TOKENIZER_AND;
  } else if(*ptr == '|') {
    return TOKENIZER_OR;
  } else if(*ptr == '*') {
    return TOKENIZER_ASTR;
  } else if(*ptr == '/') {
    return TOKENIZER_SLASH;
  } else if(*ptr == '%') {
    return TOKENIZER_MOD;
  } else if(*ptr == '(') {
    return TOKENIZER_LEFTPAREN;
  } else if(*ptr == ')') {
    return TOKENIZER_RIGHTPAREN;
  } else if(*ptr == '<') {
    return TOKENIZER_LT;
  } else if(*ptr == '>') {
    return TOKENIZER_GT;
  } else if(*ptr == '=') {
    return TOKENIZER_EQ;
  }
  return 0;
}
/*---------------------------------------------------------------------------*/
static int
get_next_token(void)
{
  struct keyword_token const *kt;
  char token_string[20];
  int i;
  char *ptr2;
  
  strncpy(token_string,ptr,sizeof(token_string));
  ptr2 = strchr(token_string, ' ');
  if (ptr2!=0) *ptr2 = 0;
  ptr2 = strchr(token_string, '\n');
  if (ptr2!=0) *ptr2 = 0;
  DEBUG_PRINTF("get_next_token(): '%s'\n", ptr);

  if(*ptr == 0) {
    return TOKENIZER_ENDOFINPUT;
  }
  
  if(isdigit(*ptr)) {
    for(i = 0; i < MAX_NUMLEN; ++i) {
      if(!isdigit(ptr[i])) {
	if(i > 0) {
	  nextptr = ptr + i;
	  return TOKENIZER_NUMBER;
	} else {
	  DEBUG_PRINTF("get_next_token: error due to too short number\n");
	  return TOKENIZER_ERROR;
	}
      }
      if(!isdigit(ptr[i])) {
	DEBUG_PRINTF("get_next_token: error due to malformed number\n");
	return TOKENIZER_ERROR;
      }
    }
    DEBUG_PRINTF("get_next_token: error due to too long number\n");
    return TOKENIZER_ERROR;
  } else if(singlechar()) {
    nextptr = ptr + 1;
    return singlechar();
  } else if(*ptr == '"') {
    nextptr = ptr;
    do {
      ++nextptr;
    } while(*nextptr != '"');
    ++nextptr;
    return TOKENIZER_STRING;
  } else {
    for(kt = keywords; kt->keyword != NULL; ++kt) {
      if(strncmp(token_string, kt->keyword, strlen(token_string)) == 0) 
		  {
		  if (strlen(token_string)==strlen(kt->keyword))
			  {
				nextptr = ptr + strlen(kt->keyword);
				return kt->token;
			  }
			}
		}
  }

  if((*ptr >= 'a' && *ptr <= 'z')|(*ptr >= 'A' && *ptr <= 'Z')) {
    nextptr = ptr + 1;
    return TOKENIZER_VARIABLE;
  }
  nextptr = ptr + 1;
  
  return TOKENIZER_ERROR;
}
/*---------------------------------------------------------------------------*/
void
tokenizer_init(const char *program)
{
  ptr = program;
  current_token = get_next_token();
}
/*---------------------------------------------------------------------------*/
int
tokenizer_token(void)
{
  return current_token;
}
/*---------------------------------------------------------------------------*/
void
tokenizer_next(void)
{

  if(tokenizer_finished()) {
    return;
  }

  DEBUG_PRINTF("tokenizer_next: %p\n", nextptr);
  ptr = nextptr;
  while(*ptr == ' ') {
    ++ptr;
  }
  current_token = get_next_token();
  DEBUG_PRINTF("tokenizer_next: '%s' %d\n", ptr, current_token);
  return;
}
/*---------------------------------------------------------------------------*/
int
tokenizer_num(void)
{
  return atoi(ptr);
}
/*---------------------------------------------------------------------------*/
void
tokenizer_string(char *dest, int len)
{
  char *string_end;
  int string_len;
  
  if(tokenizer_token() != TOKENIZER_STRING) {
    return;
  }
  string_end = strchr(ptr + 1, '"');
  if(string_end == NULL) {
    return;
  }
  string_len = string_end - ptr - 1;
  if(len < string_len) {
    string_len = len;
  }
  memcpy(dest, ptr + 1, string_len);
  dest[string_len] = 0;
}
/*---------------------------------------------------------------------------*/
void
tokenizer_error_print(void)
{
  DEBUG_PRINTF("tokenizer_error_print: '%s'\n", ptr);
}
/*---------------------------------------------------------------------------*/
int
tokenizer_finished(void)
{
  return *ptr == 0 || current_token == TOKENIZER_ENDOFINPUT;
}
/*---------------------------------------------------------------------------*/
int
tokenizer_variable_num(void)
{
 if (*ptr >= 'a' && *ptr <= 'z') return *ptr - 'a';
 else return *ptr - 'A';
}
/*---------------------------------------------------------------------------*/

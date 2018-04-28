#include "hw.h"
#include "tetrapuzz.h"

extern volatile unsigned long ticks;

//FIXME: these should probably not be globals
unsigned long wait_until;
unsigned char drop_timer_flag = 0;
unsigned int state;

void tetrapuzz(void)
	{

	tetrapuzz_init();
	
	while(1)
		{
		
		//This is just a rolling increment kind of fake random
		//because it gets called more often than the boxes are dropped
		BOX_inc_random();
		
		//TODO:Service button inputs as necessary
		/*
		if(butpress & but1) BOX_lt();
		if(butpress & but2) BOX_rotate(1);
		if(butpress & but3) BOX_rt();
		if(butpress & but5) BOX_dn();
		if(butpress & powerbut) return(""); // exit with nonzero value to indicate we want to quit
		*/
		
		//TODO: There needs to be a delayed loop here
		//TODO: Service the loop on a non-blocking delay here
		//if(!tick) return(0); //This is deprecated form camera badge
		if (ticks > wait_until)
			{
			wait_until = ticks+1000;
			tetrapuzz_loop();
			}
		}
	}
 
void tetrapuzz_init(void)
	{
	BOX_pregame();
	drop_timer_flag = 0;
	wait_until = ticks+1000;
	}

void tetrapuzz_loop(void)
	{
	unsigned char droplimit;
	if (BOX_get_score()/4 < DROPRATE) droplimit = DROPRATE - (BOX_get_score()/4);
	else droplimit = 1;

	if (++drop_timer_flag > droplimit)
		{
			drop_timer_flag = 0;
			BOX_dn();
		}
	if (BOX_end_game())
		{
			//FIXME: print game ending information
			//printf(tabx14 taby6 "Game");
			//printf(tabx14 taby7 "Over");
			state = s_gameover;
		}
	}

void tetrapuzz_pause(void)
	{
	//FIXME: we probably don't need a pause mode.
	/* deprecated camera badge code: 
	if(butpress)
		{
		BOX_start_game();
		state=s_run;
		return(0);
		}
	*/
	}

void tetrapuzz_gameover(void)
	{
	//FIXME: we probably don't need this gameover mode.
	/* deprecated camera badge code:
	}
	if(butpress) {
		BOX_start_game();
		state=s_run;
	}
	*/
	}
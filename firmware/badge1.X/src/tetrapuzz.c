// template demonstration application for camera badge
// new apps need to be added to list in applist.h

#include "cambadge.h"
#include "globals.h"

// states used by this application

#define s_start 0
#define s_run 1
#define s_freeze 2
#define s_gameover 3

#define DROPRATE  19

char* tetrapuzz(unsigned int action)
{
  static unsigned int state;
  static unsigned char drop_timer_flag;
  
  switch(action) {
     case act_name : return("BADGETRIS");
     case act_help : return("Puzzle game of\nfour-box shapes");  
     case act_init :
         // add any code here that needs to run once at powerup - e.g. hardware detection/initialisation      
            return(0);   
     
     case act_powerdown :
          // add any code here that needs to run before powerdown
         return(0);
         
     case act_start :  
         // called once when app is selected from menu
         state=s_start;
         return(0);         
    } //switch
 
 if (action!=act_poll) return(0);
 
  // do anything that needs to be faster than tick here.
 BOX_inc_random();
 if(!tick) return(0);
 
 unsigned char droplimit;
 switch(state) {
     case s_start :
       BOX_pregame();
       drop_timer_flag = 0;
       state=s_freeze;
       
  
     case s_run :
       if (BOX_get_score()/4 < DROPRATE) droplimit = DROPRATE - (BOX_get_score()/4);
       else droplimit = 1;
       
       if (++drop_timer_flag > droplimit)
       {
         drop_timer_flag = 0;
         BOX_dn();
       }
       if (BOX_end_game()) {
         printf(tabx14 taby6 "Game");
         printf(tabx14 taby7 "Over");
         state = s_gameover;
       }
       break;
 
     case s_freeze :
       if(butpress) {
         BOX_start_game();
         state=s_run;
         return(0);
       }
       break;
       
     case s_gameover :
       if(butpress) {
         BOX_start_game();
         state=s_run;
       }
       break;
 
 } // switch state
 
 if(butpress & but1) BOX_lt();
 if(butpress & but2) BOX_rotate(1);
 if(butpress & but3) BOX_rt();
 if(butpress & but5) BOX_dn();
 
 
     
       
 
 if(butpress & powerbut) return(""); // exit with nonzero value to indicate we want to quit

 return(0);
 
}
 
 


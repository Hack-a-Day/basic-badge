#include "tune_player.h"

void play_music_array(const unsigned char * arr, unsigned char length, const unsigned int * tempo_arr)
	{
	unsigned char i;
	for (i=0; i<(length/4); i++)
		{
		unsigned char index = i*4;
		sound_play_notes(*(arr+index), *(arr+index+1), *(arr+index+2), *(tempo_arr+*(arr+index+3)));
		}
	}

void play_mario_tune(void)
	{
	play_music_array(mario_main0,mario_array_limits[0],mario_tempos);
	
	play_music_array(mario_clip0,mario_array_limits[4],mario_tempos);
	play_music_array(mario_clip1,mario_array_limits[5],mario_tempos);
	play_music_array(mario_clip0,mario_array_limits[4],mario_tempos);
	play_music_array(mario_clip1,mario_array_limits[5],mario_tempos);
	play_music_array(mario_clip2,mario_array_limits[6],mario_tempos);
	play_music_array(mario_clip3,mario_array_limits[7],mario_tempos);
	play_music_array(mario_clip2,mario_array_limits[6],mario_tempos);
	
	play_music_array(mario_main1,mario_array_limits[1],mario_tempos);
	
	play_music_array(mario_clip2,mario_array_limits[6],mario_tempos);
	play_music_array(mario_clip3,mario_array_limits[7],mario_tempos);
	play_music_array(mario_clip2,mario_array_limits[6],mario_tempos);
	
	play_music_array(mario_main2,mario_array_limits[2],mario_tempos);
	play_music_array(mario_clip4,mario_array_limits[7],mario_tempos);
	play_music_array(mario_main3,mario_array_limits[3],mario_tempos);
	play_music_array(mario_clip4,mario_array_limits[7],mario_tempos);
	}

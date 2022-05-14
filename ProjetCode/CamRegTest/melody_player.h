#ifndef MELODY_PLAYER_H
#define MELODY_PLAYER_H

#include <ch.h>
#include <hal.h>

typedef enum{
	//internal songs available
	MEGALOVANIA = 0,
	POKEMON_TRAINER_BATTLE,
	VICTORY_FANFARE,
	THE_TAVERN_SONG,

	NB_EXTERNAL_SONGS	//tell the number of internal songs
}song_selection_ext;

typedef enum {
	NO_SONG = 0x00,
	TAVERN_SONG = 0x01,
    PURSUIT_SONG = 0x02,
    ENEMY_DETECTION_SONG = 0x03,
	VICTORY_SONG = 0x04
} music;

void melody_player_start(void);
music get_music_to_play(void);
void set_music_to_play(uint8_t music_to_set);

#endif /* MELODY_PLAYER_H */

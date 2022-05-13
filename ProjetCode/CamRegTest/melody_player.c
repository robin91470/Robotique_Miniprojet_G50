#include <ch.h>
#include <hal.h>
#include <audio/play_sound_file.h>
#include <audio/play_melody.h>
#include <selector.h>
#include <melody_player.h>

static music music_to_play = 0;

static THD_WORKING_AREA(waMelodyPlayer, 1024);
static THD_FUNCTION(Melody_player, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

    while (1) {
    	//waits 1 second
    	static music temp = NO_SONG;

    	switch (music_to_play){
    		default:
    			if(getPlay()){
    				stopCurrentMelody();
    			}
    			temp = NO_SONG;
    			break;
    		case TAVERN_SONG:
    			if(temp!=TAVERN_SONG || !getPlay()){
    			playMelody(THE_TAVERN_SONG, ML_FORCE_CHANGE, NULL);
    			temp = TAVERN_SONG;
    			}
    			break;
    		case PURSUIT_SONG:
    			if(temp!=PURSUIT_SONG || !getPlay()){
    			playMelody(MEGALOVANIA, ML_FORCE_CHANGE, NULL);
    			temp = PURSUIT_SONG;
    			}
    			break;
    		case ENEMY_DETECTION_SONG:
    			if(temp!=ENEMY_DETECTION_SONG || !getPlay()){
    			playMelody(POKEMON_TRAINER_BATTLE, ML_FORCE_CHANGE, NULL);
    			temp = ENEMY_DETECTION_SONG;
    			}
    			break;
    		case VICTORY_SONG:
    			if(temp!=VICTORY_SONG || !getPlay()){
    			playMelody(VICTORY_FANFARE, ML_FORCE_CHANGE, NULL);
    			temp = VICTORY_SONG;
    			}
    			break;
    		}


        chThdSleepMilliseconds(100);
    }
}

void melody_player_start(void){
	chThdCreateStatic(waMelodyPlayer, sizeof(waMelodyPlayer), NORMALPRIO, Melody_player, NULL);
}
music get_music_to_play(void){
	return music_to_play;
}

void set_music_to_play(uint8_t music_to_set){
	music_to_play = music_to_set;
}

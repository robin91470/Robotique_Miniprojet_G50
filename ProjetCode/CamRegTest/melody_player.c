#include <ch.h>
#include <hal.h>
#include <audio/play_sound_file.h>
#include <audio/play_melody.h>
#include <selector.h>


static THD_WORKING_AREA(waMelodyPlayer, 1024);
static THD_FUNCTION(Melody_player, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;
    systime_t time;

    while (1) {
    	//waits 1 second
    	static uint8_t temp = 0;

    	switch (get_selector()){
    		default:
    			if(getPlay()){
    				stopCurrentMelody();
    			}
    			temp = 0;
    			break;
    		case 1:
    			if(temp!=1){
    			playMelody(THE_TAVERN_SONG, ML_FORCE_CHANGE, NULL);
    			temp = 1;
    			}
    			break;
    		case 2:
    			if(temp!=2){
    			playMelody(MEGALOVANIA, ML_FORCE_CHANGE, NULL);
    			temp = 2;
    			}
    			break;
    		case 3:
    			if(temp!=3){
    			playMelody(POKEMON_TRAINER_BATTLE, ML_FORCE_CHANGE, NULL);
    			temp = 3;
    			}
    			break;
    		case 4:
    			if(temp!=4){
    			playMelody(VICTORY_FANFARE, ML_FORCE_CHANGE, NULL);
    			temp = 4;
    			}
    			break;
    		}


        chThdSleepMilliseconds(100);
    }
}

void melody_player_start(void){
	chThdCreateStatic(waMelodyPlayer, sizeof(waMelodyPlayer), NORMALPRIO, Melody_player, NULL);
}

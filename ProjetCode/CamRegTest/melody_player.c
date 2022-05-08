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
    	while(get_selector() != 8){
    		if(getPlay()){
    			stopCurrentMelody();
    		}
    	}
    	playMelody(SANDSTORMS, ML_SIMPLE_PLAY, NULL);

        chThdSleepMilliseconds(1000);



    }
}

void melody_player_start(void){
	chThdCreateStatic(waMelodyPlayer, sizeof(waMelodyPlayer), NORMALPRIO, Melody_player, NULL);
}

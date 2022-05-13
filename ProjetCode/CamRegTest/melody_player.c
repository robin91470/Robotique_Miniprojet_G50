#include <ch.h>
#include <hal.h>
#include <audio/play_sound_file.h>
#include <audio/play_melody.h>
#include <selector.h>

#include "chschd.h"

static thread_t* ptr_melody_player;

static bool is_paused = false;

static THD_WORKING_AREA(waMelodyPlayer, 1024);
static THD_FUNCTION(thd_melody_player, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;
    systime_t time;

    while (!chThdShouldTerminateX()) {
		time = chVTGetSystemTime();

    	chSysLock();
    			if (is_paused){
    			  chSchGoSleepS(CH_STATE_SUSPENDED);
    			}
    	chSysUnlock();

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
	chThdExit(0);
}

void melody_player_start_thd(void){
	ptr_melody_player = chThdCreateStatic(waMelodyPlayer, sizeof(waMelodyPlayer), NORMALPRIO, thd_melody_player, NULL);
	is_paused = false;
}

void melody_player_stop_thd(void){
		chThdTerminate(ptr_melody_player);
		chThdWait(ptr_melody_player);
		is_paused = false;
}

void melody_player_pause_thd(void){
	if (getPlay())
		is_paused = true;
}

void melody_player_resume_thd(void){
	chSysLock();
	if (getPlay() && is_paused){
	  chSchWakeupS(ptr_melody_player, CH_STATE_READY);
	  is_paused = false;
	}
	chSysUnlock();
}

bool melody_player_get_state(void){
	return getPlay();
}



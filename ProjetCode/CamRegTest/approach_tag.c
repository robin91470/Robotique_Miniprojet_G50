#include <ch.h>
#include <hal.h>

#include "chschd.h"

static thread_t* ptr_approach_tag;


static bool is_approaching = false;
static bool is_paused = false;

static THD_WORKING_AREA(waApproachTag, 1024);
static THD_FUNCTION(thd_approach_tag, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;
    systime_t time;
	while(!chThdShouldTerminateX()){
		time = chVTGetSystemTime();


		chSysLock();
		if (is_paused){
		  chSchGoSleepS(CH_STATE_SUSPENDED);
		}
		chSysUnlock();
		chThdSleepUntilWindowed(time, time + MS2ST(10));//refresh at 100 Hz
    }
	is_approaching = false;
	chThdExit(0);
}

void approach_tag_start_thd(void){
	ptr_approach_tag = chThdCreateStatic(waApproachTag, sizeof(waApproachTag), NORMALPRIO, thd_approach_tag, NULL);
	is_paused = false;
	is_approaching = true;
}

void approach_stop_thd(void){
		chThdTerminate(ptr_approach_tag);
		chThdWait(ptr_approach_tag);
		is_approaching = false;
		is_paused = false;
}


void approach_pause_thd(void){
	if (is_approaching)
		is_paused = true;
}

void approach_resume_thd(void){
	chSysLock();
	if (is_approaching && is_paused){
	  chSchWakeupS(ptr_approach_tag, CH_STATE_READY);
	  is_paused = false;
	}
	chSysUnlock();
}

bool approach_get_state(void){
	return is_approaching;
}




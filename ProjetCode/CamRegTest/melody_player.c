#include <ch.h>
#include <hal.h>

static THD_WORKING_AREA(waMelodyPlayer, 1024);
static THD_FUNCTION(Melody_player, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;
    systime_t time;
	while(1){

    chThdSleepUntilWindowed(time, time + MS2ST(10));//refresh at 100 Hz
    }
}

void melody_player_start(void){
	chThdCreateStatic(waMelodyPlayer, sizeof(waMelodyPlayer), NORMALPRIO, Melody_player, NULL);
}

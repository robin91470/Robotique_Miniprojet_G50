#include <ch.h>
#include <hal.h>

static THD_WORKING_AREA(waApproachTag, 1024);
static THD_FUNCTION(Approach_tag, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;
    systime_t time;
	while(1){

    chThdSleepUntilWindowed(time, time + MS2ST(10));//refresh at 100 Hz
    }
}

void approach_tag_start(void){
	chThdCreateStatic(waApproachTag, sizeof(waApproachTag), NORMALPRIO, Approach_tag, NULL);
}

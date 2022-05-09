#include <ch.h>
#include <hal.h>

static THD_WORKING_AREA(waDistance, 1024);
static THD_FUNCTION(Distance, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;
    systime_t time;
	while(1){

    chThdSleepUntilWindowed(time, time + MS2ST(10));//refresh at 100 Hz
    }
}

void distance_start(void){
	chThdCreateStatic(waDistance, sizeof(waDistance), NORMALPRIO, Distance, NULL);
}

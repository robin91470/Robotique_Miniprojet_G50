#include "ch.h"
#include "hal.h"
#include <math.h>
#include <usbcfg.h>


#include <main.h>
#include <motors.h>
#include <walk.h>
#include <selector.h>


#define SPEED_TRIANGLE		1000//step*s^-1
#define TIME_TURN_TRIANGLE	1000*(PERIMETER_EPUCK/6)*(NSTEP_ONE_TURN/WHEEL_DISTANCE)/(SPEED_TRIANGLE) //ms
#define TIME_SEGMENT_TRIANGLE	1000*(PERIMETER_EPUCK/2)*(NSTEP_ONE_TURN/WHEEL_DISTANCE)/(SPEED_TRIANGLE) //ms


static THD_WORKING_AREA(waWalk, 1024);
static THD_FUNCTION(Walk, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;
    systime_t time;
	while(1){
    for(unsigned int i=0; i<3; i++){
    	if(get_selector()==0){
    				//chThdTerminate(Walk);
    				break;
    			}
    	left_motor_set_speed(SPEED_TRIANGLE);
		right_motor_set_speed(-SPEED_TRIANGLE);
		chThdSleepMilliseconds(TIME_TURN_TRIANGLE);
		left_motor_set_speed(SPEED_TRIANGLE);
		right_motor_set_speed(SPEED_TRIANGLE);
		chThdSleepMilliseconds(TIME_SEGMENT_TRIANGLE);
		left_motor_set_speed(0);
		right_motor_set_speed(0);


    }
	chThdSleepMilliseconds(5000);//pause entre les triangles, à enlever plus tard
	if(get_selector()==0){
				//chThdTerminate(Walk);
				break;
			}






    chThdSleepUntilWindowed(time, time + MS2ST(10));//refresh at 100 Hz
}
}

void walk_start(void){
	chThdCreateStatic(waWalk, sizeof(waWalk), NORMALPRIO, Walk, NULL);
}

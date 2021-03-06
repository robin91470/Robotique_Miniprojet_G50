#include "ch.h"
#include "hal.h"
#include <math.h>
#include <usbcfg.h>


#include <main.h>
#include <motors.h>
#include <walk.h>
#include <selector.h>

//The main goal of this module is to achieve "random-like" shapes that take the form of a butterfly (or "criss-cross" shape)
#define SPEED_BUTTERFLY		1000//[step*s^-1]
#define TIME_TURN_BUTTERFLY	1000*(PERIMETER_EPUCK/6)*(NSTEP_ONE_TURN/WHEEL_DISTANCE)/(SPEED_BUTTERFLY) //[ms]
#define TIME_SEGMENT_BUTTERFLY	1000*(PERIMETER_EPUCK/2)*(NSTEP_ONE_TURN/WHEEL_DISTANCE)/(SPEED_BUTTERFLY) //[ms]
#define BUTTERFLY_WAIT_TIME	1000 //[s]

static bool thd_is_created = false;
static bool is_paused = false;


static THD_WORKING_AREA(waWalk, 2048);
static THD_FUNCTION(thd_walk, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;
    systime_t time;
    while(1){
		time = chVTGetSystemTime();
		if (!is_paused){

			for(unsigned int i=0; i<3; i++){

				left_motor_set_speed(SPEED_BUTTERFLY);
				right_motor_set_speed(-SPEED_BUTTERFLY);
				chThdSleepMilliseconds(TIME_TURN_BUTTERFLY);
				left_motor_set_speed(SPEED_BUTTERFLY);
				right_motor_set_speed(SPEED_BUTTERFLY);
				chThdSleepMilliseconds(TIME_SEGMENT_BUTTERFLY);
				left_motor_set_speed(SPEED_STOP);
				right_motor_set_speed(SPEED_STOP);
			}
			chThdSleepMilliseconds(BUTTERFLY_WAIT_TIME);//pause entre les triangles, ? enlever plus tard
		}
    chThdSleepUntilWindowed(time, time + MS2ST(10));//refresh at 100 Hz
    }
}

void walk_start_thd(void){
	if(!thd_is_created){
		chThdCreateStatic(waWalk, sizeof(waWalk), NORMALPRIO, thd_walk, NULL);
		is_paused = false;
		thd_is_created = true;
	}
}


void walk_pause_thd(void){
	if(!is_paused && thd_is_created){
			is_paused = true;
		}
}

void walk_resume_thd(void){
	if (is_paused && thd_is_created){
	  is_paused = false;
	}
}


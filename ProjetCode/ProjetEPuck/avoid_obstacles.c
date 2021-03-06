#include <ch.h>
#include <hal.h>
#include <stdio.h>
#include <stdlib.h>

#include "chschd.h"
#include <sensors/proximity.h>
#include <motors.h>
#include <avoid_obstacles.h>
#include <math.h>
#include <main.h>
#include <walk.h>

#define PROXIMITY_THRESHOLD 150 //Very sensible sensors, adjust this parameter if needed
#define AVOIDANCE_SPEED 700 //speed at which the robot bypasses the obstacle [step/s]
#define AVOIDANCE_TIME 200//time during which the robot moves away from the obstacle before stopping and exiting thread [ms]
#define AVOIDANCE_DURATION 3//time given to bypass obstacle before forcibly moving on [s]

messagebus_t bus;//declares the messagebus (to proper sensor communication)
MUTEX_DECL(bus_lock);
CONDVAR_DECL(bus_condvar);

static bool thd_is_created = false;
static bool is_paused = false;//checkable boolean to pause/unpause the thread as necessary

static THD_WORKING_AREA(waAvoidObstacles, 1024);
static THD_FUNCTION(avoid_obstacles, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;
    systime_t time;


    messagebus_init(&bus, &bus_lock, &bus_condvar);//inits the messagebus

	proximity_start();
	calibrate_ir();

    messagebus_topic_t *prox_topic = messagebus_find_topic_blocking(&bus, "/proximity");
    proximity_msg_t prox_values;
	while(1){
		time = chVTGetSystemTime();
		if (!is_paused){
			messagebus_topic_wait(prox_topic, &prox_values, sizeof(prox_values));//proximity sensor readings
			for(unsigned int i = 0; i<PROXIMITY_NB_CHANNELS - 1; i++){
				if (abs(prox_values.delta[i]) > PROXIMITY_THRESHOLD){//obstacle is detected
					left_motor_set_speed(SPEED_STOP);
					right_motor_set_speed(SPEED_STOP);
					if(i<PROXIMITY_NB_HALF){//obstacle detection on the right
						left_motor_set_speed(-SPEED_ROTATION);
						right_motor_set_speed(SPEED_ROTATION);
					}
					else{//obstacle detection on the left
						left_motor_set_speed(SPEED_ROTATION);
						right_motor_set_speed(-SPEED_ROTATION);
					}
					systime_t time_avoid = chVTGetSystemTime();
					while((abs(prox_values.delta[PROXIMITY_SENSOR_3_ID]) <= PROXIMITY_THRESHOLD)&&((abs(prox_values.delta[PROXIMITY_SENSOR_4_ID]) <= PROXIMITY_THRESHOLD))){//
						if(((chVTGetSystemTime() - time_avoid) > S2ST(AVOIDANCE_DURATION))
								&&(abs(prox_values.delta[PROXIMITY_SENSOR_0_ID]) <= PROXIMITY_THRESHOLD)
								&&(abs(prox_values.delta[PROXIMITY_SENSOR_1_ID]) <= PROXIMITY_THRESHOLD)
								&&(abs(prox_values.delta[PROXIMITY_SENSOR_6_ID]) <= PROXIMITY_THRESHOLD)
								&&(abs(prox_values.delta[PROXIMITY_SENSOR_7_ID]) <= PROXIMITY_THRESHOLD)){
							break;
						}
						messagebus_topic_wait(prox_topic, &prox_values, sizeof(prox_values));
					}
					left_motor_set_speed(AVOIDANCE_SPEED);
					right_motor_set_speed(AVOIDANCE_SPEED);
					chThdSleepMilliseconds(AVOIDANCE_TIME);
					left_motor_set_speed(SPEED_STOP);
					right_motor_set_speed(SPEED_STOP);
				}
			}
		}
		chThdSleepUntilWindowed(time, time + MS2ST(10));//refresh at 100 Hz
    }
}

void avoid_obstacles_start_thd(void){
	if(!thd_is_created){
		chThdCreateStatic(waAvoidObstacles, sizeof(waAvoidObstacles), NORMALPRIO+8, avoid_obstacles, NULL);
		is_paused = false;
		thd_is_created = true;
	}

}


void avoid_obstacles_pause_thd(void){
	if(!is_paused && thd_is_created){
		is_paused = true;
	}
}

void avoid_obstacles_resume_thd(void){
	if (is_paused && thd_is_created){
	  is_paused = false;
	}
}


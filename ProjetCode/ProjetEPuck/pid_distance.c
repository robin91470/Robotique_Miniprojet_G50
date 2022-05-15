/*
 * pid_distance.c
 *
 *  Created on: 29 avr. 2022
 *      Author: alexis
 */


#include "ch.h"
#include "hal.h"
#include <math.h>
#include <usbcfg.h>
#include <chprintf.h>


#include <main.h>
#include <motors.h>
#include <pid_distance.h>
#include <sensors/VL53L0X/VL53L0X.h>
#include <process_image.h>

// Booléen de complétition de la tache
static bool is_done = false;
// Booléen d'arret d'urgence de la fonction
static bool must_stop = false;

//Booléen indiquant si la fonction est crée ou non
static bool thd_exist = false;

static uint16_t get_mean_distance_mm(void){
	uint16_t mean_distance = 0;
	static uint16_t previous_distance[NB_SAMPLES] = {0};
	static uint8_t circular_buffer = 0;
	previous_distance[circular_buffer] = VL53L0X_get_dist_mm();
	for(uint8_t i = 0;i<NB_SAMPLES;i++){
		mean_distance += previous_distance[i];
	}
	mean_distance /= NB_SAMPLES;
	circular_buffer = (circular_buffer + 1) % NB_SAMPLES;
	return(mean_distance);
}

//simple PID regulator implementation
int16_t pid_regulator(uint16_t distance, float goal){

	float error = 0;
	float speed = 0;

	static float sum_error = 0;
	static float previous_error = 0;

	error = distance - goal;

	//disables the PID regulator if the error is to small
	if(fabs(error) < ERROR_THRESHOLD){
		return 0;
	}

	sum_error += error;

	//we set a maximum and a minimum for the sum to avoid an uncontrolled growth
	if(sum_error > MAX_SUM_ERROR){
		sum_error = MAX_SUM_ERROR;
	}else if(sum_error < -MAX_SUM_ERROR){
		sum_error = -MAX_SUM_ERROR;
	}

	speed = KP * error + KI * sum_error + KD * (error-previous_error);
	previous_error = error ;
    return (int16_t)speed;
}
// animation de coup d'épée
static void animation(void){
	float time_animation = 0;
	uint16_t step_animation = 0;
	step_animation = (NSTEP_ONE_TURN/2)/(FRACTION_OF_THE_ROAD);
	time_animation = 1000*step_animation/SPEED_ANIMATION1;
	right_motor_set_speed(SPEED_ANIMATION1);
	left_motor_set_speed(-SPEED_ANIMATION1);
	chThdSleepMilliseconds(time_animation);
	time_animation = 1000*step_animation/SPEED_ANIMATION2;
	right_motor_set_speed(-SPEED_ANIMATION2);
	left_motor_set_speed(SPEED_ANIMATION2);
	chThdSleepMilliseconds(time_animation);
	right_motor_set_speed(SPEED_STOP);
	left_motor_set_speed(SPEED_STOP);
}

static THD_WORKING_AREA(waPidRegulator, 256);
static THD_FUNCTION(PidRegulator, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

    systime_t time;
    static systime_t stable_time;
    int16_t speed = 0;

    while(1){
    	if(must_stop){
			right_motor_set_speed(SPEED_STOP);
			left_motor_set_speed(SPEED_STOP);
			must_stop = false;
			thd_exist = false;
			chThdExit(0);
		}
    	time = chVTGetSystemTime();
    	if(!stable_time){
    	    stable_time = chVTGetSystemTime();
    	}
        //computes the speed to give to the motors
        speed = pid_regulator(get_mean_distance_mm(), GOAL_DIST);


        //applies the speed from the PID regulator
		right_motor_set_speed(speed);
		left_motor_set_speed(speed);

		if(!speed){
			//si le robot est arreté pendant assez de temps, il peut attaquer l'ennemi
			if((chVTGetSystemTime() - stable_time) > S2ST(STABLE_DURATION)){
				is_done = true;
				animation();
				thd_exist = false;
				chThdExit(0);
			}
		}else{
			stable_time = chVTGetSystemTime();
		}

        //100Hz
        chThdSleepUntilWindowed(time, time + MS2ST(10));
    }
}

void pid_distance_start(void){
	if(!thd_exist){
		must_stop = false;
		is_done = false;
		thd_exist = true;
		chThdCreateStatic(waPidRegulator, sizeof(waPidRegulator), NORMALPRIO, PidRegulator, NULL);
	}
}

bool get_job_is_done(void){
	return is_done;
}

void stop_pid(void){
	must_stop = true;
}

/*
 * beer.c
 *
 *  Created on: 13 mai 2022
 *      Author: alexis
 */
#include <ch.h>
#include <hal.h>
#include <sensors/VL53L0X/VL53L0X.h>
#include <chprintf.h>

#include <motors.h>
#include <scan.h>
#include <math.h>
#include <beer.h>
#include <main.h>

static bool beer_gotten = 0;

// Booléen d'arret d'urgence de la fonction
static bool must_stop = false;

//Booléen indiquant si la fonction est crée ou non
static bool thd_exist = false;

static THD_WORKING_AREA(waBeer, 1024);
static THD_FUNCTION(Beer, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;
    systime_t time;

	while(1){
		if(must_stop){
			must_stop = 0;
			thd_exist = 0;
			chThdExit(0);
		}
		time = chVTGetSystemTime();
		int16_t dist_approach = 0;
		dist_approach = VL53L0X_get_dist_mm() - BAR_DIST;
		if(dist_approach < MAX_SCAN_DIST && dist_approach > MIN_SCAN_DIST){
			distance_approach(dist_approach);
			chThdSleepSeconds(BEER_WAIT);
			distance_approach(-dist_approach);
			beer_gotten = true;
			thd_exist = 0;
			chThdExit(0);
		}

		chThdSleepUntilWindowed(time, time + MS2ST(10));//refresh at 100 Hz
    }
}

void beer_start(void){
	if(!thd_exist){
		thd_exist  = true;
		must_stop = false;
		beer_gotten = 0;
		chThdCreateStatic(waBeer, sizeof(waBeer), NORMALPRIO, Beer, NULL);
	}
}

bool get_beer_served(void){
	return beer_gotten;
}

void stop_beer(void){
	must_stop = true;
}

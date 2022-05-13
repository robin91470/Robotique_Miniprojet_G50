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

static bool beer_gotten = 0;


static THD_WORKING_AREA(waBeer, 1024);
static THD_FUNCTION(Beer, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;
    systime_t time;

	while(1){
		time = chVTGetSystemTime();
		int16_t dist_approach = 0;
		dist_approach = VL53L0X_get_dist_mm() - BAR_DIST;
		if(dist_approach < MAX_SCAN_DIST && dist_approach > MIN_SCAN_DIST){
			distance_approach(dist_approach);
			chThdSleepSeconds(BEER_WAIT);
			distance_approach(-dist_approach);
			beer_gotten = true;
			chThdExit(0);
		}

		chThdSleepUntilWindowed(time, time + MS2ST(10));//refresh at 100 Hz
    }
}

void beer_start(void){
    beer_gotten = 0;
	chThdCreateStatic(waBeer, sizeof(waBeer), NORMALPRIO, Beer, NULL);
}

bool get_beer_served(void){
	return beer_gotten;
}

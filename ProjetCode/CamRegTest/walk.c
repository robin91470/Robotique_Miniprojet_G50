#include "ch.h"
#include "hal.h"
#include <math.h>
#include <usbcfg.h>
#include <chprintf.h>


#include <main.h>
#include <motors.h>
#include <walk.h>

#define WALK_SPEED		500
#define TRIANGLE_ANGLE
#define NSTEP_ONE_TURN      1000 // number of step for 1 turn of the motor

#define PI                  3.1415926536f
//TO ADJUST IF NECESSARY. NOT ALL THE E-PUCK2 HAVE EXACTLY THE SAME WHEEL DISTANCE
#define WHEEL_DISTANCE      5.35f    //cm
#define PERIMETER_EPUCK     (PI * WHEEL_DISTANCE)


static THD_WORKING_AREA(waWalk, 1024);
static THD_FUNCTION(Walk, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;
    systime_t time;



    chThdYield();
    //chThdSleepUntilWindowed(time, time + MS2ST(10));//refresh at 100 Hz
}


void walk_start(void){
	chThdCreateStatic(waWalk, sizeof(waWalk), NORMALPRIO, Walk, NULL);
}

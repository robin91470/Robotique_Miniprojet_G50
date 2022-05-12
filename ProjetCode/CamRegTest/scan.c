#include <ch.h>
#include <hal.h>
#include <sensors/VL53L0X/VL53L0X.h>

#include <process_image.h>
#include <main.h>
#include <motors.h>
#include <scan.h>
#include <math.h>

void scan_approach(void){
	int16_t dist_approach = 0;
	dist_approach = VL53L0X_get_dist_mm() - SCAN_DIST;
	step_approach = dist_approach*NSTEP_ONE_TURN / PERIMETER_EPUCK;
	time_approach = abs(step_approach)/SPEED_APPROACH

}

static THD_WORKING_AREA(waScan, 1024);
static THD_FUNCTION(Scan, arg) {

    chRegSetThreadName(_FUNCTION_);
    (void)arg;
    systime_t time;
	while(1){
		while(VL53L0X_get_dist_mm() > MAX_SCAN_DIST){
			left_motor_set_speed(ROTATION_SPEED);
			right_motor_set_speed(ROTATION_SPEED);
		}


		chThdSleepUntilWindowed(time, time + MS2ST(10));//refresh at 100 Hz
    }
}

void scan_start(void){
	chThdCreateStatic(wasScan, sizeof(waScan), NORMALPRIO, Scan, NULL);
}

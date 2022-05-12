#include <ch.h>
#include <hal.h>
#include <sensors/VL53L0X/VL53L0X.h>

#include <process_image.h>
#include <main.h>
#include <motors.h>
#include <scan.h>
#include <math.h>

static bool scan_mode = 0;
// set the color to track.
static couleur color_mode = 0;

static bool good_color = 0;

static void scan_approach(int16_t dist_approach){

	int16_t step_approach = 0;
	uint16_t time_approach = 0;

	step_approach = dist_approach*NSTEP_ONE_TURN / PERIMETER_EPUCK;
	time_approach = abs(step_approach)/SPEED_APPROACH;
	if(dist_approach >= 0){
		left_motor_set_speed(SPEED_APPROACH);
		right_motor_set_speed(SPEED_APPROACH);
	}else{
		//si négatif il faut reculer
		left_motor_set_speed(-SPEED_APPROACH);
		right_motor_set_speed(-SPEED_APPROACH);
	}
	if(time_approach){
		chThdSleepMilliseconds(time_approach);
	}
	left_motor_set_speed(SPEED_STOP);
	right_motor_set_speed(SPEED_STOP);
}

static couleur color_scan(void){
	couleur color_detected = COULEUR_AUTRE;


	if(get_line_detection_red() && !get_line_detection_blue()){
		color_detected = COULEUR_BLEU;
	}else if(!get_line_detection_red() && get_line_detection_blue()){
		color_detected = COULEUR_ROUGE;
	}else{
		color_detected =  COULEUR_AUTRE;
	}
	return color_detected;




}



static THD_WORKING_AREA(waScan, 1024);
static THD_FUNCTION(Scan, arg) {

    chRegSetThreadName(_FUNCTION_);
    (void)arg;
    systime_t time;
	while(1){
		int16_t dist_approach = 0;
		couleur color_scanned = 0;
		if(!scan_mode && (VL53L0X_get_dist_mm() > MAX_SCAN_DIST)){
			left_motor_set_speed(ROTATION_SPEED);
			right_motor_set_speed(-ROTATION_SPEED);
		}else{
			scan_mode = 1;
			dist_approach = VL53L0X_get_dist_mm() - SCAN_DIST;
			scan_approach(dist_approach);
			color_scanned = color_scan();
			if((color_mode == COULEUR_ROUGE && color_scanned == COULEUR_ROUGE) ||
					(color_mode == COULEUR_BLEU && color_scanned == COULEUR_BLEU)){
				good_color = 1;
				chThdExit(0);
			}
			else{
				good_color = 0;
				scan_approach(-dist_approach);
				scan_mode = 0;
			}

		}


		chThdSleepUntilWindowed(time, time + MS2ST(10));//refresh at 100 Hz
    }
}

couleur get_color_mode(void){
	return color_mode;
}

void set_color_mode(couleur color_set){
	color_mode = color_set;
}

bool get_good_color(void){
	return good_color;
}


void scan_start(void){
	chThdCreateStatic(wasScan, sizeof(waScan), NORMALPRIO, Scan, NULL);
}

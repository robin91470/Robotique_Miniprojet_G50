/*
 * pid_distance.h
 *
 *  Created on: 29 avr. 2022
 *      Author: alexis
 */

#ifndef PID_DISTANCE_H_
#define PID_DISTANCE_H_

// These parameters can be change for different use
#define MAX_DIST 				1000.0f //[mm]
#define GOAL_DIST				50.0f   //[mm]
#define ERROR_THRESHOLD			20.0f	//[mm] because of the noise of the camera
#define KP						20.0f
#define KI 						1.4f	//must not be zero
#define KD						1.4f
#define MAX_SUM_ERROR 			(MOTOR_SPEED_LIMIT/KI)
#define NB_SAMPLES				10
#define STABLE_DURATION			3 //[s]
#define SPEED_ANIMATION1		700 //[step/s]
#define SPEED_ANIMATION2		250 //[step/s]
#define ANGLE_OF_THE_ROAD_TO_REACH			180//[°]
#define FRACTION_OF_THE_ROAD	360/ANGLE_OF_THE_ROAD_TO_REACH //[%]

void pid_distance_start(void);
// Permet de savoir si le PID s'est stabilisé
bool get_job_is_done(void);
void stop_pid(void);

#endif /* PID_DISTANCE_H_ */

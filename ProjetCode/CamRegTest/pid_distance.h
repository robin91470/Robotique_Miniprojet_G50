/*
 * pid_distance.h
 *
 *  Created on: 29 avr. 2022
 *      Author: alexi
 */

#ifndef PID_DISTANCE_H_
#define PID_DISTANCE_H_

#define MAX_DIST 				1000.0f //[mm]
#define GOAL_DIST				50.0f	//[mm]
#define ERROR_THRESHOLD			10.0f	//[mm] because of the noise of the sensor
#define KP						20.0f
#define KI 						1.4f	//must not be zero
#define KD						1.4f
#define MAX_SUM_ERROR 			(MOTOR_SPEED_LIMIT/KI)
#define NB_SAMPLES				10
#define SCAN_DIST				250.0f  //[mm]
#define MAX_SCAN_DIST			600.0f  //[mm]
#define ROTATION_SPEED			700 	//[step/s]
#define SPEED_APPROACH			1000	//[step/s]

void pid_distance_start(void);


#endif /* PID_DISTANCE_H_ */

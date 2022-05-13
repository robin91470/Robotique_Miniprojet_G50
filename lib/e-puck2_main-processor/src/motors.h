#ifndef MOTORS_H
#define MOTORS_H

#include <stdint.h>
#include <hal.h>

#define MOTOR1_A	GPIOE, 9
#define MOTOR1_B	GPIOE, 8
#define MOTOR1_C	GPIOE, 11
#define MOTOR1_D	GPIOE, 10
#define MOTOR2_A	GPIOE, 13
#define MOTOR2_B	GPIOE, 12
#define MOTOR2_C	GPIOE, 14
#define MOTOR2_D	GPIOE, 15


#define MOTOR_SPEED_LIMIT 1100 // [step/s]

#define NSTEP_ONE_TURN      1000

#define PI                  3.1415926536f
//TO ADJUST IF NECESSARY. NOT ALL THE E-PUCK2 HAVE EXACTLY THE SAME WHEEL DISTANCE
#define WHEEL_DISTANCE      5.35f    //cm
#define PERIMETER_EPUCK     (PI * WHEEL_DISTANCE)
#define SPEED_STOP			0

 /**
 * @brief   Sets the speed of the left motor
 * 
 * @param speed     speed desired in step/s
 */
void left_motor_set_speed(int speed);

 /**
 * @brief   Sets the speed of the right motor
 * 
 * @param speed     speed desired in step/s
 */
void right_motor_set_speed(int speed);

 /**
 * @brief   Reads the position counter of the left motor
 * 
 * @return          position counter of the left motor in step
 */
int32_t left_motor_get_pos(void);

 /**
 * @brief   Reads the position counter of the right motor
 * 
 * @return          position counter of the right motor in step
 */
int32_t right_motor_get_pos(void);

/**
 * @brief 	sets the position counter of the left motor to the given value
 * 
 * @param counter_value 	value to store in the position counter of the motor in step
 */
void left_motor_set_pos(int32_t counter_value);

/**
 * @brief 	sets the position counter of the right motor to the given value
 * 
 * @param counter_value value to store in the position counter of the motor in step
 */
void right_motor_set_pos(int32_t counter_value);

 /**
 * @brief   Initializes the control of the motors.
 */
void motors_init(void);


//void motors_set_speed(int speed_r, int speed_l);//added function, uses both motors at given speed.

//void motors_set_pos(int32_t counter_value_r, int32_t counter_value_l);//added function, uses both motors to reach given length at given speed.

//void motors_set_position(int32_t position_r, int32_t position_l, int speed_r, int speed_l);


#endif /* MOTOR_H */

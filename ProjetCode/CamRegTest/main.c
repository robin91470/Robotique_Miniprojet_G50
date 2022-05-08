#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ch.h"
#include "hal.h"
#include "memory_protection.h"
#include <usbcfg.h>
#include <main.h>
#include <motors.h>
#include <camera/po8030.h>
#include <chprintf.h>
#include <sensors/proximity.h>
#include <sensors/VL53L0X/VL53L0X.h>
#include <audio/play_sound_file.h>
#include <audio/play_melody.h>

#include <pi_regulator.h>
#include <process_image.h>
#include <walk.h>


#define NSTEP_ONE_TURN      1000

#define PI                  3.1415926536f
//TO ADJUST IF NECESSARY. NOT ALL THE E-PUCK2 HAVE EXACTLY THE SAME WHEEL DISTANCE
#define WHEEL_DISTANCE      5.35f    //cm
#define PERIMETER_EPUCK     (PI * WHEEL_DISTANCE)
#define SPEED_TRIANGLE		1000
#define TIME_TURN_TRIANGLE	1000*(PERIMETER_EPUCK/6)*(NSTEP_ONE_TURN/WHEEL_DISTANCE)/(SPEED_TRIANGLE) //ms
#define TIME_SEGMENT_TRIANGLE	1000*(PERIMETER_EPUCK/2)*(NSTEP_ONE_TURN/WHEEL_DISTANCE)/(SPEED_TRIANGLE) //ms

int32_t HALF_TURN = (PERIMETER_EPUCK/2)*(NSTEP_ONE_TURN/WHEEL_DISTANCE);
void SendUint8ToComputer(uint8_t* data, uint16_t size) 
{
	chSequentialStreamWrite((BaseSequentialStream *)&SD3, (uint8_t*)"START", 5);
	chSequentialStreamWrite((BaseSequentialStream *)&SD3, (uint8_t*)&size, sizeof(uint16_t));
	chSequentialStreamWrite((BaseSequentialStream *)&SD3, (uint8_t*)data, size);
}

static void serial_start(void)
{
	static SerialConfig ser_cfg = {
	    115200,
	    0,
	    0,
	    0,
	};

	sdStart(&SD3, &ser_cfg); // UART3.
}

int main(void)
{

    halInit();
    chSysInit();
    mpu_init();

    //starts the serial communication
    serial_start();
    //start the USB communication
    usb_start();
    //starts the camera
    dcmi_start();
	po8030_start();
	//inits the motors
	motors_init();
	//uint8_t RAF = 2+2;
	//stars the threads for the pi regulator and the processing of the image
	//pi_regulator_start();
	//process_image_start();
	//walk_start();




    /* Infinite loop. */
    while (1) {
    	//waits 1 second
    	for(unsigned int i=0; i<3; i++){
    		left_motor_set_speed(SPEED_TRIANGLE);
    		right_motor_set_speed(-SPEED_TRIANGLE);
    		chThdSleepMilliseconds(TIME_TURN_TRIANGLE);
    		left_motor_set_speed(SPEED_TRIANGLE);
    		right_motor_set_speed(SPEED_TRIANGLE);
    		chThdSleepMilliseconds(TIME_SEGMENT_TRIANGLE);
    	}
    	left_motor_set_speed(0);
    	right_motor_set_speed(0);
    	chThdSleepMilliseconds(5000);
    }
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}

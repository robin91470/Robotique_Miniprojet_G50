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
#include <leds.h>

#include <pi_regulator.h>
#include <process_image.h>

messagebus_t bus;
MUTEX_DECL(bus_lock);
CONDVAR_DECL(bus_condvar);

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
    messagebus_init(&bus, &bus_lock, &bus_condvar);

    //starts the serial communication
    serial_start();

    //start the USB communication
    usb_start();
    //starts the camera
//    dcmi_start();
//	po8030_start();
	//inits the motors
	motors_init();
	//stars the threads for the pi regulator and the processing of the image
//	pi_regulator_start();
//	process_image_start();
    //chprintf((BaseSequentialStream *)&SD3,"test2 \n");
    proximity_start();
    calibrate_ir();
    messagebus_topic_t *prox_topic = messagebus_find_topic_blocking(&bus, "/proximity");
    proximity_msg_t prox_values;


    /* Infinite loop. */
    while (1) {
    	//waits 1 second
		messagebus_topic_wait(prox_topic, &prox_values, sizeof(prox_values));
    	chprintf((BaseSequentialStream *)&SD3, "PROXIMITY\r\n");
    	chprintf((BaseSequentialStream *)&SD3, "%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d\r\n\n", prox_values.delta[0], prox_values.delta[1], prox_values.delta[2], prox_values.delta[3], prox_values.delta[4], prox_values.delta[5], prox_values.delta[6], prox_values.delta[7]);
    	chThdSleepMilliseconds(100);
    	for(unsigned int i = 0; i<7; i++){
    		if((i!=2) && (i!=4)){ //ignoring defective sensors
				if (abs(prox_values.delta[i]) > 100){
					//palClearPad(GPIOD, GPIOD_LED_FRONT);

					left_motor_set_speed(0);
					right_motor_set_speed(0);
					set_front_led(1);
					break;
				}
				else {
				    left_motor_set_speed(500);
				    right_motor_set_speed(500);
				    set_front_led(0);
				}
    		}
    	}

    }
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}

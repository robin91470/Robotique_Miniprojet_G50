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
#include <selector.h>

#include <audio/play_sound_file.h>
#include <audio/play_melody.h>
#include <audio/audio_thread.h>
#include <melody_player.h>

#include <process_image.h>
#include <walk.h>

#include <pid_distance.h>
#include <avoid_obstacles.h>




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
	VL53L0X_start();

	//process_image_start();
	//inits the motors
	motors_init();
	dac_start();
	setSoundFileVolume(5);
	playMelodyStart();//lance le module
	//melody_player_start_thd();
	setSoundFileVolume(10);
	//melody_player_start();
	avoid_obstacles_start_thd();
	walk_start_thd();


    /* Infinite loop. */
    while (1) {
    	//if(get_selector()==0){
    	    				//break;
    		//walk_stop_thd();
    	static bool lock = 0;

    	if(get_selector() == 0 && !lock){
    		lock = 1;
    		avoid_obstacles_resume_thd();
    	}
    	else if((get_selector() != 0 && lock)){
    		lock = 0;
    		avoid_obstacles_pause_thd();
    	}
    	chThdSleepMilliseconds(1000);

    }
    		//		else{walk_resume_thd();}

    	//waits 1 second
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}

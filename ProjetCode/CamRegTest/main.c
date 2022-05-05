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
<<<<<<< Updated upstream

=======
#include <leds.h>
#include <audio/audio_thread.h>//ajout� pour l'audio
#include <audio/play_melody.h>
>>>>>>> Stashed changes
#include <pi_regulator.h>
#include <process_image.h>

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
<<<<<<< Updated upstream
	pi_regulator_start();
	process_image_start();
=======
//	pi_regulator_start();
//	process_image_start();
    //chprintf((BaseSequentialStream *)&SD3,"test2 \n");
	dac_start();//pas sur qu'il faille le mettre
	playMelodyStart();//lance le module
	playSoundFileStart();//
	setSoundFileVolume(40);
	char* path = "sound.wav";
	//playMelody(MARIO, ML_SIMPLE_PLAY, NULL);



>>>>>>> Stashed changes

    /* Infinite loop. */
    while (1) {
    	//waits 1 second
<<<<<<< Updated upstream
        chThdSleepMilliseconds(1000);
=======
    	playSoundFile(path, SF_SIMPLE_PLAY);
    	waitSoundFileHasFinished();
    	chThdSleepMilliseconds(1000);


>>>>>>> Stashed changes
    }
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}

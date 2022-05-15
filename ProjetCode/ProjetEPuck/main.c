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
#include <scan.h>
#include <pid_distance.h>
#include <avoid_obstacles.h>
#include <beer.h>


#define TIMEOUR_BEER_STROLL 15//[s] time before the epucks gets thirsty
#define TAVERN_MODE_SELECTION 0//Nominal position the selector must be in to engage the tavern stroll
#define HUNT_MODE_SELECTION 1 //Nominal position the selector must be in to engage the enemy hunting
#define TRANSITION_SONG_DURATION	5//[s]

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

static bool tavern_phase(bool tavern_threads_resumed){
	static systime_t beer_time;
	static bool thirsty = false;
	if(!tavern_threads_resumed){
		set_music_to_play(TAVERN_SONG);
		walk_resume_thd();
		avoid_obstacles_resume_thd();
		tavern_threads_resumed = true;
	}
	thirsty = true;
	while(get_selector()==TAVERN_MODE_SELECTION){//While the selector isn't switched manually, the epuck is locked in its tavern stroll
		if((!beer_time)||!thirsty){//checks if time has been initialized or if a beer has been gotten
			beer_time = chVTGetSystemTime();
		}
		if((chVTGetSystemTime() - beer_time) > S2ST(TIMEOUR_BEER_STROLL)){//The beer timeout delay has been reached, the epuck is thirsty
			thirsty = true;
			walk_pause_thd();
			avoid_obstacles_pause_thd();
			tavern_threads_resumed = false;
			set_color_mode(COULEUR_BLEU);
			scan_start();
			while(!get_good_color()){//Waits for the scan to identify the enemy
							chThdSleepMilliseconds(100);//10Hz Refresh
			}
			beer_start();//Gets a beer
			while(!get_beer_served()){
				chThdSleepMilliseconds(100);//10Hz Refresh
			}//A beer has been served and the epuck has  gotten away from the bar => can revert to tavern stroll
			thirsty = false;
			if(!tavern_threads_resumed){
				walk_resume_thd();
				avoid_obstacles_resume_thd();
				tavern_threads_resumed = true;
			}

		}
	}
	return tavern_threads_resumed;
}

static bool tracking_phase(bool tavern_threads_resumed){
	set_music_to_play(NO_SONG);
	walk_pause_thd();
	avoid_obstacles_pause_thd();
	if(tavern_threads_resumed){
	tavern_threads_resumed = false;
	}
	set_color_mode(COULEUR_ROUGE);
	set_music_to_play(RADAR_SONG);
	scan_start();//Scan for enemy: the epuck scans nearby objects using its TOF sensor
	while(!get_good_color()){//Waits for the scan to identify the enemy
		chThdSleepMilliseconds(100);//10Hz Refresh
	}//The while loop ends => Enemy is detected
	set_music_to_play(ENEMY_DETECTION_SONG);
	chThdSleepSeconds(TRANSITION_SONG_DURATION);
	set_music_to_play(PURSUIT_SONG);
	pid_distance_start();//Pursuit initialized
	while(!get_job_is_done()){//Waits for the pursuit to finish
		chThdSleepMilliseconds(100);//10Hz Refresh
	}
	set_music_to_play(VICTORY_SONG);//Enemy Hit. Victory is ours !
	chThdSleepSeconds(TRANSITION_SONG_DURATION);
	set_music_to_play(NO_SONG);
	while((get_selector() == HUNT_MODE_SELECTION)  && get_job_is_done() ){
		chThdSleepMilliseconds(100);//10Hz Refresh; Stays locked in if HUNT_MODE isn't manually disabled (through selector)
	}
	return tavern_threads_resumed;
}

static bool reinit(bool tavern_threads_resumed){
	set_music_to_play(NO_SONG);
	walk_pause_thd();
	avoid_obstacles_pause_thd();
	if(tavern_threads_resumed){
		tavern_threads_resumed = false;
	}
	stop_beer();
	stop_scan();
	stop_pid();
	return tavern_threads_resumed;
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
	//inits the motors
	motors_init();
	dac_start();
	//Creation of diverse threads prior to their use in the scenario. They are immediately paused.
	ProcessImage_start_thd();
	ProcessImage_pause_thd();
	playMelodyStart();
	setSoundFileVolume(10);
	melody_player_start();
	walk_start_thd();
	walk_pause_thd();
	avoid_obstacles_start_thd();
	avoid_obstacles_pause_thd();

	static bool tavern_threads_resumed = false;


    while (1) {

    	if(get_selector() == TAVERN_MODE_SELECTION){//Initialisation of the tavern stroll
    		tavern_threads_resumed = tavern_phase(tavern_threads_resumed);
			chThdSleepMilliseconds(1000);//1Hz Refresh
    	}else if(get_selector() == HUNT_MODE_SELECTION){//Initialisation of the hunt
    		tavern_threads_resumed = tracking_phase(tavern_threads_resumed);
    	}
    	else{
    		tavern_threads_resumed = reinit(tavern_threads_resumed);
    	}
    	chThdSleepMilliseconds(100);//10Hz Refresh;
    }
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}

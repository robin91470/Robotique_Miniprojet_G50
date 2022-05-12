#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "camera/dcmi_camera.h"
#include "msgbus/messagebus.h"
#include "parameter/parameter.h"


//constants for the differents parts of the project
#define IMAGE_BUFFER_SIZE		640
#define WIDTH_SLOPE				5
#define LINE_WIDTH_MIN			30
#define PXTOCM					1570.0f
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

/** Robot wide IPC bus. */
extern messagebus_t bus;

extern parameter_namespace_t parameter_root;

void SendUint8ToComputer(uint8_t* data, uint16_t size);

#ifdef __cplusplus
}
#endif

#endif

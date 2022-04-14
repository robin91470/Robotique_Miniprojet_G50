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
#define MAX_DIST 				25.0f
#define GOAL_DIST				10.0f
#define ERROR_THRESHOLD			0.5f	//[cm] because of the noise of the camera
#define KP						400.0f
#define KI 						7.0f	//must not be zero
#define KD						7.0f
#define MAX_SUM_ERROR 			(MOTOR_SPEED_LIMIT/KI)
#define ROTATION_THRESHOLD		10
#define ROTATION_COEFF			2
/** Robot wide IPC bus. */
extern messagebus_t bus;

extern parameter_namespace_t parameter_root;

void SendUint8ToComputer(uint8_t* data, uint16_t size);

#ifdef __cplusplus
}
#endif

#endif

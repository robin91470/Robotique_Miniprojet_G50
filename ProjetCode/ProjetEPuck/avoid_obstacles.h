#ifndef AVOID_OBSTACLES_H
#define AVOID_OBSTACLES_H
#include "msgbus/messagebus.h"
extern messagebus_t bus;


#define PROXIMITY_NB_HALF 4// Used to differentiate left/right side sensors
#define PROXIMITY_SENSOR_0_ID 0
#define PROXIMITY_SENSOR_1_ID 1
#define PROXIMITY_SENSOR_2_ID 2
#define PROXIMITY_SENSOR_3_ID 3
#define PROXIMITY_SENSOR_4_ID 4
#define PROXIMITY_SENSOR_5_ID 5
#define PROXIMITY_SENSOR_6_ID 6
#define PROXIMITY_SENSOR_7_ID 7//constants used to identify individual sensors (refer to epuck wiki)


void avoid_obstacles_start_thd(void);

void avoid_obstacles_pause_thd(void);

void avoid_obstacles_resume_thd(void);

#endif /* AVOID_OBSTACLES_H */

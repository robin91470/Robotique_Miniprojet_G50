#ifndef AVOID_OBSTACLES_H
#define AVOID_OBSTACLES_H
#include "msgbus/messagebus.h"
extern messagebus_t bus;

void avoid_obstacles_start_thd(void);

void avoid_obstacles_pause_thd(void);

void avoid_obstacles_resume_thd(void);

#endif /* AVOID_OBSTACLES_H */

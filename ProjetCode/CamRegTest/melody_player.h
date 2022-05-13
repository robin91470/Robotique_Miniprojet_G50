#ifndef MELODY_PLAYER_H
#define MELODY_PLAYER_H

#include <ch.h>
#include <hal.h>

void melody_player_start_thd(void);

void melody_player_stop_thd(void);

void melody_player_pause_thd(void);

void melody_player_resume_thd(void);

bool melody_player_get_state(void);


#endif /* MELODY_PLAYER_H */

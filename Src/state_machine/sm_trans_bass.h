#ifndef __TRAMS_FM_SM_TRAMS_BASS_H__
#define __TRAMS_FM_SM_TRAMS_BASS_H__

#include <stdint.h>

#include "task.h"

enum sm_states_trans_bass
{
  SM_TRANS_BASS_INIT,
  SM_TRANS_BASS_MAIN,
};

const struct sm_state_slot code sm_function_trans_bass[];

extern void do_trans_bass_init(uint8_t to_func, uint8_t to_state, enum task_events ev);

#endif

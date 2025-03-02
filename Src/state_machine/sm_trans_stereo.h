#ifndef __TRAMS_FM_SM_TRAMS_STEREO_H__
#define __TRAMS_FM_SM_TRAMS_STEREO_H__

#include <stdint.h>

#include "task.h"

enum sm_states_trans_stereo
{
  SM_TRANS_STEREO_INIT,
  SM_TRANS_STEREO_MAIN,  
};

const struct sm_state_slot code sm_function_trans_stereo[];

extern void do_trans_stereo_init(uint8_t to_func, uint8_t to_state, enum task_events ev);

#endif

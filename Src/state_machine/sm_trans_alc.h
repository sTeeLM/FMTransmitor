#ifndef __TRAMS_FM_SM_TRAMS_ALC_H__
#define __TRAMS_FM_SM_TRAMS_ALC_H__

#include <stdint.h>

#include "task.h"

enum sm_states_trans_alc
{
  SM_TRANS_ALC_INIT,
  SM_TRANS_ALC_ONOFF,
  SM_TRANS_ALC_DELAY,
  SM_TRANS_ALC_ATTACK,
  SM_TRANS_ALC_HOLD,
  SM_TRANS_ALC_HIGH,
  SM_TRANS_ALC_LOW,
  SM_TRANS_ALC_COMP_GAIN
};

const struct sm_state_slot code sm_function_trans_alc[];
extern void do_trans_alc_init(uint8_t to_func, uint8_t to_state, enum task_events ev);
#endif

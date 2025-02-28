#ifndef __TRAMS_FM_SM_TRAMS_BRIGHT_H__
#define __TRAMS_FM_SM_TRAMS_BRIGHT_H__

#include <stdint.h>

#include "task.h"

enum sm_states_trans_bright
{
  SM_TRANS_BRIGHT_INIT,
};

const struct sm_state_slot code sm_function_trans_bright[];

extern void do_trans_bright_init(uint8_t to_func, uint8_t to_state, enum task_events ev);

#endif

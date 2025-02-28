#ifndef __TRAMS_FM_SM_TRAMS_PTA_H__
#define __TRAMS_FM_SM_TRAMS_PTA_H__

#include <stdint.h>

#include "task.h"

enum sm_states_trans_pta
{
  SM_TRANS_PTA_INIT,
};

const struct sm_state_slot code sm_function_trans_pta[];

extern void do_trans_pta_init(uint8_t to_func, uint8_t to_state, enum task_events ev);

#endif

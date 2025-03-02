#ifndef __TRAMS_FM_SM_TRAMS_PA_H__
#define __TRAMS_FM_SM_TRAMS_PA_H__

#include <stdint.h>

#include "task.h"

enum sm_states_trans_pa
{
  SM_TRANS_PA_INIT,
  SM_TRANS_PA_PDPA,
  SM_TRANS_PA_PADN, 
  SM_TRANS_PA_BIAS 
};

const struct sm_state_slot code sm_function_trans_pa[];

extern void do_trans_pa_init(uint8_t to_func, uint8_t to_state, enum task_events ev);

#endif

#ifndef __TRAMS_FM_SM_TRAMS_FDD_H__
#define __TRAMS_FM_SM_TRAMS_FDD_H__

#include <stdint.h>

#include "task.h"

enum sm_states_trans_fdd
{
  SM_TRANS_FDD_INIT,
  SM_TRANS_FDD_MAIN,  
};

const struct sm_state_slot code sm_function_trans_fdd[];

extern void do_trans_fdd_init(uint8_t to_func, uint8_t to_state, enum task_events ev);

#endif

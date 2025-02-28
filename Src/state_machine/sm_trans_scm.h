#ifndef __TRAMS_FM_SM_TRAMS_SCM_H__
#define __TRAMS_FM_SM_TRAMS_SCM_H__

#include <stdint.h>

#include "task.h"

enum sm_states_trans_scm
{
  SM_TRANS_SCM_INIT,
};

const struct sm_state_slot code sm_function_trans_scm[];

extern void do_trans_scm_init(uint8_t to_func, uint8_t to_state, enum task_events ev);

#endif

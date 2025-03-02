#ifndef __TRAMS_FM_SM_TRAMS_AFRE_H__
#define __TRAMS_FM_SM_TRAMS_AFRE_H__

#include <stdint.h>

#include "task.h"

enum sm_states_trans_afre
{
  SM_TRANS_AFRE_INIT,
  SM_TRANS_AFRE_MAIN,  
};

const struct sm_state_slot code sm_function_trans_afre[];

extern void do_trans_afre_init(uint8_t to_func, uint8_t to_state, enum task_events ev);

#endif

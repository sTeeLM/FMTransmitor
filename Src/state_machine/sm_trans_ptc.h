#ifndef __TRAMS_FM_SM_TRAMS_PTC_H__
#define __TRAMS_FM_SM_TRAMS_PTC_H__

#include <stdint.h>

#include "task.h"

enum sm_states_trans_ptc
{
  SM_TRANS_PTC_INIT,
};

const struct sm_state_slot code sm_function_trans_ptc[];

#endif

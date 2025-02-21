#ifndef __TRAMS_FM_SM_TRAMS_FM_H__
#define __TRAMS_FM_SM_TRAMS_FM_H__

#include <stdint.h>

#include "task.h"

enum sm_states_trans_fm
{
  SM_TRANS_FM_INIT,
};

const struct sm_state_slot code sm_function_trans_fm[];

#endif

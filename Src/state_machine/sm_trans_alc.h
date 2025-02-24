#ifndef __TRAMS_FM_SM_TRAMS_ALC_H__
#define __TRAMS_FM_SM_TRAMS_ALC_H__

#include <stdint.h>

#include "task.h"

enum sm_states_trans_alc
{
  SM_TRANS_ALC_INIT,
};

const struct sm_state_slot code sm_function_trans_alc[];

#endif

#ifndef __TRAMS_FM_SM_TRAMS_RF_GAIN_H__
#define __TRAMS_FM_SM_TRAMS_RF_GAIN_H__

#include <stdint.h>

#include "task.h"

enum sm_states_trans_rf_gain
{
  SM_TRANS_RF_GAIN_INIT,
};

const struct sm_state_slot code sm_function_trans_rf_gain[];

#endif

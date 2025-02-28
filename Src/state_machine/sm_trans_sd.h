#ifndef __TRAMS_FM_SM_TRAMS_SD_H__
#define __TRAMS_FM_SM_TRAMS_SD_H__

#include <stdint.h>

#include "task.h"

enum sm_states_trans_sd
{
  SM_TRANS_SD_INIT,
  SM_TRANS_SD_ONOFF,
  SM_TRANS_SD_THH, // Silence Detection High Threshold
  SM_TRANS_SD_THL, // Silence Detection Low Threshold
  SM_TRANS_SD_CTH,
  SM_TRANS_SD_CTL,  
};

const struct sm_state_slot code sm_function_trans_sd[];
extern void do_trans_sd_init(uint8_t to_func, uint8_t to_state, enum task_events ev);

#endif

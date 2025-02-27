#ifndef __TRAMS_FM_SM_TRAMS_FM_H__
#define __TRAMS_FM_SM_TRAMS_FM_H__

#include <stdint.h>

#include "task.h"

enum sm_states_trans_fm
{
  SM_TRANS_FM_INIT,
  SM_TRANS_FM_MAIN,    // Channel Selection
  SM_TRANS_FM_VOLUME,  // Volume Selection
  SM_TRANS_FM_MENU,    // Menu Selection  
  SM_TRANS_FM_PW_OFF,    // Power OFF  
};

const struct sm_state_slot code sm_function_trans_fm[];

extern void do_trans_fm_init(uint8_t to_func, uint8_t to_state, enum task_events ev);

#endif

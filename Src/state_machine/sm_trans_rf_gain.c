#include "sm_trans_rf_gain.h"
#include "task.h"
#include "sm.h"
#include "cext.h"
#include "debug.h"
#include "power.h"
#include "gpio.h"
#include "delay.h"

static void do_trans_rf_gain_init(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  
}

static const struct sm_trans_slot code  sm_trans_rf_gain_init[] = {
  {EV_INIT, SM_TRANS_RF_GAIN, SM_TRANS_RF_GAIN_INIT, do_trans_rf_gain_init},
  {NULL, NULL, NULL, NULL}
};


const struct sm_state_slot code sm_function_trans_rf_gain[] = {
  {"SM_TRANS_RF_GAIN_INIT", sm_trans_rf_gain_init},
};
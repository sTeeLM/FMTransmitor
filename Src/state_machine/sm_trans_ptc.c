#include "sm_trans_ptc.h"
#include "task.h"
#include "sm.h"
#include "cext.h"
#include "debug.h"
#include "power.h"
#include "gpio.h"
#include "delay.h"

void do_trans_ptc_init(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  
}

static const struct sm_trans_slot code  sm_trans_ptc_init[] = {
  {EV_INIT, SM_TRANS_PTC, SM_TRANS_PTC_INIT, do_trans_ptc_init},
  {NULL, NULL, NULL, NULL}
};


const struct sm_state_slot code sm_function_trans_ptc[] = {
  {"SM_TRANS_PDC_INIT", sm_trans_ptc_init},
};
#include "sm_trans_scm.h"
#include "task.h"
#include "sm.h"
#include "cext.h"
#include "debug.h"
#include "power.h"
#include "gpio.h"
#include "delay.h"

void do_trans_scm_init(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  
}

static const struct sm_trans_slot code  sm_trans_scm_init[] = {
  {EV_INIT, SM_TRANS_SCM, SM_TRANS_SCM_INIT, do_trans_scm_init},
  {NULL, NULL, NULL, NULL}
};


const struct sm_state_slot code sm_function_trans_scm[] = {
  {"SM_TRANS_SCM_INIT", sm_trans_scm_init},
};
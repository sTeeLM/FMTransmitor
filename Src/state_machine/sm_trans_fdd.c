#include "sm_trans_fdd.h"
#include "task.h"
#include "sm.h"
#include "cext.h"
#include "debug.h"
#include "power.h"
#include "gpio.h"
#include "delay.h"

static void do_trans_fdd_init(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  
}

static const struct sm_trans_slot code  sm_trans_fdd_init[] = {
  {EV_INIT, SM_TRANS_FDD, SM_TRANS_FDD_INIT, do_trans_fdd_init},
  {NULL, NULL, NULL, NULL}
};


const struct sm_state_slot code sm_function_trans_fdd[] = {
  {"SM_TRANS_FDD_INIT", sm_trans_fdd_init},
};
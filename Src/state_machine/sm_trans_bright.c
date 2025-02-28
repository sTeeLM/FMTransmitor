#include "sm_trans_bright.h"
#include "task.h"
#include "sm.h"
#include "cext.h"
#include "debug.h"
#include "power.h"
#include "gpio.h"
#include "delay.h"

void do_trans_bright_init(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  
}

static const struct sm_trans_slot code  sm_trans_bright_init[] = {
  {EV_INIT, SM_TRANS_BRIGHT, SM_TRANS_BRIGHT_INIT, do_trans_bright_init},
  {NULL, NULL, NULL, NULL}
};


const struct sm_state_slot code sm_function_trans_bright[] = {
  {"SM_TRANS_BRIGHT_INIT", sm_trans_bright_init},
};
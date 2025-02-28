#include "sm_trans_afre.h"
#include "task.h"
#include "sm.h"
#include "cext.h"
#include "debug.h"
#include "power.h"
#include "gpio.h"
#include "delay.h"

void do_trans_afre_init(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  
}

static const struct sm_trans_slot code  sm_trans_afre_init[] = {
  {EV_INIT, SM_TRANS_AFRE, SM_TRANS_AFRE_INIT, do_trans_afre_init},
  {NULL, NULL, NULL, NULL}
};


const struct sm_state_slot code sm_function_trans_afre[] = {
  {"SM_TRANS_AFRE_INIT", sm_trans_afre_init},
};
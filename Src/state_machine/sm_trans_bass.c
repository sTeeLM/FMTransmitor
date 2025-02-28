#include "sm_trans_bass.h"
#include "task.h"
#include "sm.h"
#include "cext.h"
#include "debug.h"
#include "power.h"
#include "gpio.h"
#include "delay.h"

void do_trans_bass_init(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  
}

static const struct sm_trans_slot code  sm_trans_bass_init[] = {
  {EV_INIT, SM_TRANS_BASS, SM_TRANS_BASS_INIT, do_trans_bass_init},
  {NULL, NULL, NULL, NULL}
};


const struct sm_state_slot code sm_function_trans_bass[] = {
  {"SM_TRANS_BASS_INIT", sm_trans_bass_init},
};
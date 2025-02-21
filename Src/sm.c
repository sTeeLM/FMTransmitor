#include "sm.h"

#include "debug.h"

#include "sm_trans_fm.h"

uint8_t sm_cur_function;
uint8_t sm_cur_state;

static const struct sm_function_slot code sm_function[] =
{
  {"SM_TRANS_FM", sm_function_trans_fm}, 
};

uint8_t sm_cur_function;
uint8_t sm_cur_state;

void sm_initialize(void)
{
  CDBG(("sm_initialize\n"));
  sm_cur_function = SM_TRANS_FM;
  sm_cur_state = SM_TRANS_FM_INIT;
  task_set(EV_INIT);
}

void sm_enter_shell(void)
{
  CDBG(("sm_enter_shell\n"));
}

void sm_leave_shell(void)
{
  CDBG(("sm_leave_shell\n"));
  sm_cur_function = SM_TRANS_FM;
  sm_cur_state = SM_TRANS_FM_INIT;
  task_set(EV_INIT);
}

void sm_enter_control(void)
{
  CDBG(("sm_enter_control\n"));
}

void sm_leave_control(void)
{
  CDBG(("sm_leave_control\n"));
  sm_cur_function = SM_TRANS_FM;
  sm_cur_state = SM_TRANS_FM_INIT;
  task_set(EV_INIT);
}

void sm_run(enum task_events ev)
{
  struct sm_trans_slot * p_trans = NULL;
  
  p_trans = sm_function[sm_cur_function].function_states_array[sm_cur_state].state_trans_array;
  
  while(p_trans && p_trans->sm_proc) {
    if(p_trans->event == ev) {
      p_trans->sm_proc(p_trans->to_function, p_trans->to_state, ev);
      CDBG("[%s][%s][%s]->[%s][%s]\n",
        task_names[ev],
        sm_function[sm_cur_function].function_name,
        sm_function[sm_cur_function].function_states_array[sm_cur_state].state_name,
        sm_function[p_trans->to_function].function_name,
        sm_function[p_trans->to_function].function_states_array[p_trans->to_state].state_name
      );
      sm_cur_function = p_trans->to_function;
      sm_cur_state    = p_trans->to_state;
      break;
    }
    p_trans ++;
  }
}



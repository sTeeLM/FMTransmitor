#include "sm.h"

#include "debug.h"

#include "sm_trans_fm.h"
#include "sm_trans_alc.h"
#include "sm_trans_sd.h"
#include "sm_trans_pta.h"
#include "sm_trans_rf_gain.h"
#include "sm_trans_ptc.h"
#include "sm_trans_stereo.h"
#include "sm_trans_scm.h"
#include "sm_trans_bass.h"
#include "sm_trans_fdd.h"
#include "sm_trans_afre.h"

uint8_t sm_cur_function;
uint8_t sm_cur_state;

/*
  SM_TRANS_FM  = 0,   // 主功能（含频道选择）
  SM_TRANS_ALC,       // Automatic Level Control调节
  SM_TRANS_SD,        // Silence Detection
  SM_TRANS_RF_GAIN,   // RF Gain
  SM_TRANS_PDA,       // Pilot Tone Amplitude
  SM_TRANS_PTC,       // Pre-emphasis Time-Constant
  SM_TRANS_STEREO,    // Stereo or Mono
  SM_TRANS_BASS,      // Bass Boost Control
  SM_TRANS_FDD,       // Frequency Deviation Delection
  SM_TRANS_AFRE,      // Audio Frequency Response Enhancement
  SM_TRANS_SCM,       // Switching Channel Mode Selection
*/
static const struct sm_function_slot code sm_function[] =
{
  {"SM_TRANS_FM", sm_function_trans_fm},
  {"SM_TRANS_ALC", sm_function_trans_alc}, 
  {"SM_TRANS_SD", sm_function_trans_sd},
  {"SM_TRANS_RF_GAIN", sm_function_trans_rf_gain},
  {"SM_TRANS_PTA", sm_function_trans_pta}, 
  {"SM_TRANS_PTC", sm_function_trans_ptc},
  {"SM_TRANS_STEREO", sm_function_trans_stereo}, 
  {"SM_TRANS_BASS", sm_function_trans_bass},
  {"SM_TRANS_FDD", sm_function_trans_fdd},
  {"SM_TRANS_AFRE", sm_function_trans_afre}, 
  {"SM_TRANS_SCM", sm_function_trans_scm}  
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



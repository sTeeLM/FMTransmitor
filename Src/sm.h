#ifndef __FM_TRANS_SM_H__
#define __FM_TRANS_SM_H__

#include <stdint.h>
#include <stddef.h>
#include "task.h"


enum sm_functions
{
  SM_TRANS_FM  = 0,   // 主功能（含频道选择）
  SM_TRANS_ALC,       // Automatic Level Control调节
  SM_TRANS_SD,        // Silence Detection
  SM_TRANS_RF_GAIN,   // RF Gain
  SM_TRANS_PTA,       // Pilot Tone Amplitude
  SM_TRANS_PTC,       // Pre-emphasis Time-Constant
  SM_TRANS_STEREO,    // Stereo or Mono
  SM_TRANS_BASS,      // Bass Boost Control
  SM_TRANS_FDD,       // Frequency Deviation Delection
  SM_TRANS_AFRE,      // Audio Frequency Response Enhancement
  SM_TRANS_SCM,       // Switching Channel Mode Selection
};

typedef void (*SM_PROC)(uint8_t to_func, uint8_t to_state, enum task_events ev);


/* function: array of state */
/* state: array of trans */

struct sm_trans_slot
{
  enum task_events event;
  uint8_t to_function;
  uint8_t to_state;
  SM_PROC sm_proc;
};

struct sm_state_slot {
  const char * state_name;
  struct sm_trans_slot * state_trans_array;
};

struct sm_function_slot {
  const char * function_name;
  struct sm_state_slot * function_states_array;
};

extern uint8_t sm_cur_function;
extern uint8_t sm_cur_state;

void sm_initialize(void);

void sm_run(enum task_events ev);

void sm_enter_control(void);
void sm_leave_control(void);

void sm_enter_shell(void);
void sm_leave_shell(void);

#endif

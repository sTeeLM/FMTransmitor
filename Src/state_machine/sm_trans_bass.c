#include "sm_trans_bass.h"
#include "task.h"
#include "sm.h"
#include "cext.h"
#include "debug.h"
#include "power.h"
#include "gpio.h"
#include "delay.h"
#include "kt0803.h"
#include "tm1650.h"

#include "sm_trans_fm.h"


void do_trans_bass_init(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  tm1650_clear();
}

static void trans_bass_main_display(uint8_t bass)
{
  const char * str = "    ";
  switch(bass) {
    case KT0803_BASS_DISABLE: str = " OFF"; break;
    case KT0803_BASS_5DB:     str = "  5D"; break;
    case KT0803_BASS_11DB:    str = " 11D"; break;
    case KT0803_BASS_17DB:    str = " 17D"; break;
  }
  tm1650_set_str(str);
}

static void do_trans_bass_main(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  if(to_func != sm_cur_function || to_state != sm_cur_state) {
    trans_bass_main_display(kt0803_get_bass());
    return;
  }
  
  if(ev == EV_KEY_PLUS_PRESS) {
    trans_bass_main_display(kt0803_next_bass());
  } else if(ev == EV_KEY_NEG_PRESS) {
    trans_bass_main_display(kt0803_prev_bass());
  }
}

static const struct sm_trans_slot code  sm_trans_bass_init[] = {
  {EV_KEY_UP, SM_TRANS_BASS, SM_TRANS_BASS_MAIN, do_trans_bass_main},
  {NULL, NULL, NULL, NULL}
};

static const struct sm_trans_slot code  sm_trans_bass_main[] = {
  {EV_KEY_PLUS_PRESS, SM_TRANS_BASS, SM_TRANS_BASS_MAIN, do_trans_bass_main},
  {EV_KEY_NEG_PRESS, SM_TRANS_BASS, SM_TRANS_BASS_MAIN, do_trans_bass_main},
  {EV_KEY_MENU_PRESS, SM_TRANS_FM, SM_TRANS_FM_INIT, do_trans_fm_init}, 
};

const struct sm_state_slot code sm_function_trans_bass[] = {
  {"INIT", sm_trans_bass_init},
  {"MAIN", sm_trans_bass_main},  
};
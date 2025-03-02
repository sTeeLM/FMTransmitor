#include "sm_trans_rf_gain.h"
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


void do_trans_rf_gain_init(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  tm1650_clear();
}

static void trans_fm_display_rf_gain(kt0803_rf_gain_t gain)
{
  const char * str = "    ";
  switch(gain) {
    case KT0803_RF_GAIN_955:  str = " 955"; break;
    case KT0803_RF_GAIN_965:  str = " 965"; break;
    case KT0803_RF_GAIN_975:  str = " 975"; break;
    case KT0803_RF_GAIN_982:  str = " 982"; break;
    case KT0803_RF_GAIN_989:  str = " 989"; break;
    case KT0803_RF_GAIN_1000: str = "1000"; break;
    case KT0803_RF_GAIN_1015: str = "1015"; break;
    case KT0803_RF_GAIN_1028: str = "1028"; break;
    case KT0803_RF_GAIN_1051: str = "1051"; break;
    case KT0803_RF_GAIN_1056: str = "1056"; break;
    case KT0803_RF_GAIN_1062: str = "1062"; break;
    case KT0803_RF_GAIN_1065: str = "1065"; break;
    case KT0803_RF_GAIN_1070: str = "1070"; break;
    case KT0803_RF_GAIN_1074: str = "1074"; break;
    case KT0803_RF_GAIN_1077: str = "1077"; break;
    case KT0803_RF_GAIN_1080: str = "1080"; break;
  }
  tm1650_clr_dp();
  tm1650_set_dp(1);
}

static void do_trans_rf_gain_main(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  if(to_func != sm_cur_function || to_state != sm_cur_state) {
    trans_fm_display_rf_gain(kt0803_get_rf_gain());
    return;
  }
  
  if(ev == EV_KEY_PLUS_PRESS) {
    trans_fm_display_rf_gain(kt0803_next_rf_gain());
  } else if(ev == EV_KEY_NEG_PRESS) {
    trans_fm_display_rf_gain(kt0803_prev_rf_gain());
  }
}


static const struct sm_trans_slot code  sm_trans_rf_gain_init[] = {
  {EV_KEY_UP, SM_TRANS_RF_GAIN, SM_TRANS_RF_GAIN_MAIN, do_trans_rf_gain_main},
  {NULL, NULL, NULL, NULL}
};

static const struct sm_trans_slot code  sm_trans_rf_gain_main[] = {
  {EV_KEY_PLUS_PRESS, SM_TRANS_RF_GAIN, SM_TRANS_RF_GAIN_MAIN, do_trans_rf_gain_main},
  {EV_KEY_NEG_PRESS, SM_TRANS_RF_GAIN, SM_TRANS_RF_GAIN_MAIN, do_trans_rf_gain_main},
  {EV_KEY_MENU_PRESS, SM_TRANS_FM, SM_TRANS_FM_INIT, do_trans_fm_init},
  {NULL, NULL, NULL, NULL}
};


const struct sm_state_slot code sm_function_trans_rf_gain[] = {
  {"INIT", sm_trans_rf_gain_init},
  {"MAIN", sm_trans_rf_gain_main},  
};
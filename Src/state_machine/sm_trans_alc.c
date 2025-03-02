#include "sm_trans_alc.h"
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

void do_trans_alc_init(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  tm1650_clear();
}

static void do_trans_alc_onoff(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  if(to_func != sm_cur_function || to_state != sm_cur_state) {
    tm1650_set_str(kt0803_cfg.flag & KT0803_CFG_FLAG_ALC_ENABLE ? "  ON" : " OFF");
    return;
  }
  
  if(ev == EV_KEY_PLUS_PRESS || ev == EV_KEY_NEG_PRESS) {
    bit enable = kt0803_get_alc_enable();
    kt0803_cfg.flag &= ~KT0803_CFG_FLAG_ALC_ENABLE;
    kt0803_cfg.flag |= enable ? 0 : KT0803_CFG_FLAG_ALC_ENABLE;
    tm1650_set_str(enable ? " OFF" : "  ON");
    kt0803_set_alc_enable(!enable);
  }
}

static void trans_fm_main_display_delay_attack(uint8_t value)
{
  const char * str = "    ";
  switch(value) {
    case 0:  str = " 25U"; break;
    case 1:  str = " 50U"; break;
    case 2:  str = " 75U"; break; 
    case 3: str = "100U"; break; 
    case 4: str = "125U"; break; 
    case 5: str = "150U"; break; 
    case 6: str = "175U"; break; 
    case 7: str = "200U"; break; 
    case 8:  str = " 50M"; break; 
    case 9: str = "100M"; break; 
    case 10: str = "150M"; break; 
    case 11: str = "200M"; break; 
    case 12: str = "250M"; break; 
    case 13: str = "300M"; break; 
    case 14: str = "350M"; break; 
    case 15: str = "400M"; break;
  }
  tm1650_set_str(str);
}

static void trans_fm_main_display_delay(uint8_t delay)
{
  trans_fm_main_display_delay_attack(delay);
}

static void do_trans_alc_delay(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  if(to_func != sm_cur_function || to_state != sm_cur_state) {
    tm1650_set_str("DELY");
    delay_ms(1000);
    trans_fm_main_display_delay(kt0803_get_alc_delay());
    return;
  }
  
  if(ev == EV_KEY_PLUS_PRESS) {
    trans_fm_main_display_delay(kt0803_next_alc_delay());
  } else if(ev == EV_KEY_NEG_PRESS) {
    trans_fm_main_display_delay(kt0803_prev_alc_delay());
  }
}

static void trans_fm_main_display_attack(uint8_t attack)
{
  trans_fm_main_display_delay_attack(attack);
}

static void do_trans_alc_attack(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  if(to_func != sm_cur_function || to_state != sm_cur_state) {
    tm1650_set_str("ATTC");
    delay_ms(1000);
    trans_fm_main_display_attack(kt0803_get_alc_attack());
    return;
  }
  
  if(ev == EV_KEY_PLUS_PRESS) {
    trans_fm_main_display_attack(kt0803_next_alc_attack());
  } else if(ev == EV_KEY_NEG_PRESS) {
    trans_fm_main_display_attack(kt0803_prev_alc_attack());
  }
}

static void trans_fm_main_display_hold(kt0803_alc_hold_t hold)
{
  const char * str = "    ";
  switch(hold) {
    case KT0803_ALC_HOLD_50MS:  str = " 50M"; break;
    case KT0803_ALC_HOLD_100MS: str = "100M"; break;
    case KT0803_ALC_HOLD_150MS: str = "150M"; break;
    case KT0803_ALC_HOLD_200MS: str = "200M"; break;
    case KT0803_ALC_HOLD_1S:    str = "  1S"; break;
    case KT0803_ALC_HOLD_5S:    str = "  5S"; break;
    case KT0803_ALC_HOLD_10S:   str = " 10S"; break;
    case KT0803_ALC_HOLD_15S:   str = " 15S"; break;
  }
  tm1650_set_str(str);
}

static void do_trans_alc_hold(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  if(to_func != sm_cur_function || to_state != sm_cur_state) {
    tm1650_set_str("HOLD");
    delay_ms(1000);
    trans_fm_main_display_hold(kt0803_get_alc_hold());
    return;
  }
  
  if(ev == EV_KEY_PLUS_PRESS) {
    trans_fm_main_display_hold(kt0803_next_alc_hold());
  } else if(ev == EV_KEY_NEG_PRESS) {
    trans_fm_main_display_hold(kt0803_prev_alc_hold());
  }
}

static void trans_fm_main_display_high_th(kt0803_alc_high_th_t high_th)
{
  const char * str = "    ";
  uint8_t dp = 0;
  switch(high_th) {
    case KT0803_ALC_HIGH_TH_0P6:  str = "  06"; dp = 1; break;
    case KT0803_ALC_HIGH_TH_0P5:  str = "  05"; dp = 1; break;
    case KT0803_ALC_HIGH_TH_0P4:  str = "  04"; dp = 1; break;
    case KT0803_ALC_HIGH_TH_0P3:  str = "  03"; dp = 1; break;
    case KT0803_ALC_HIGH_TH_0P2:  str = "  02"; dp = 1; break;
    case KT0803_ALC_HIGH_TH_0P1:  str = "  01"; dp = 1; break;
    case KT0803_ALC_HIGH_TH_0P05: str = " 005"; dp = 2; break;
    case KT0803_ALC_HIGH_TH_0P01: str = " 001"; dp = 2; break;
  }
  tm1650_set_str(str);
  tm1650_clr_dp();
  tm1650_set_dp(dp);
}

static void do_trans_alc_high(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  if(to_func != sm_cur_function || to_state != sm_cur_state) {
    tm1650_set_str("HIGH");
    delay_ms(1000);
    trans_fm_main_display_high_th(kt0803_get_alc_high_th());
    return;
  }
  
  if(ev == EV_KEY_PLUS_PRESS) {
    trans_fm_main_display_high_th(kt0803_next_alc_high_th());
  } else if(ev == EV_KEY_NEG_PRESS) {
    trans_fm_main_display_high_th(kt0803_prev_alc_high_th());
  }
}

static void trans_fm_main_display_low_th(kt0803_alc_low_th_t low_th)
{
  const char * str = "    ";
  uint8_t dp = 0;
  switch(low_th) {
    case KT0803_ALC_LOW_TH_0P25:   str = " 025"; dp = 2; break;
    case KT0803_ALC_LOW_TH_0P2:    str = " 020"; dp = 2; break;
    case KT0803_ALC_LOW_TH_0P15:   str = " 015"; dp = 2; break;
    case KT0803_ALC_LOW_TH_0P1:    str = " 010"; dp = 2; break;
    case KT0803_ALC_LOW_TH_0P05:   str = " 005"; dp = 2; break;
    case KT0803_ALC_LOW_TH_0P03:   str = " 003"; dp = 2; break;
    case KT0803_ALC_LOW_TH_0P02:   str = " 002"; dp = 2; break;
    case KT0803_ALC_LOW_TH_0P01:   str = " 001"; dp = 2; break;
    case KT0803_ALC_LOW_TH_0P005:  str = "0005"; dp = 3; break;
    case KT0803_ALC_LOW_TH_0P001:  str = "0001"; dp = 3; break;
    case KT0803_ALC_LOW_TH_0P0005: str = "0-35"; dp = 3; break;
    case KT0803_ALC_LOW_TH_0P0001: str = "0-31"; dp = 3; break;
  }
  tm1650_set_str(str);
  tm1650_clr_dp();
  tm1650_set_dp(dp);
}

static void do_trans_alc_low(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  if(to_func != sm_cur_function || to_state != sm_cur_state) {
    tm1650_set_str("  LO");
    delay_ms(1000);
    trans_fm_main_display_low_th(kt0803_get_alc_low_th());
    return;
  }
  
  if(ev == EV_KEY_PLUS_PRESS) {
    trans_fm_main_display_low_th(kt0803_next_alc_low_th());
  } else if(ev == EV_KEY_NEG_PRESS) {
    trans_fm_main_display_low_th(kt0803_prev_alc_low_th());
  }
}

static void trans_fm_main_display_comp_gain(kt0803_alc_comp_gain_t comp_gain)
{
  const char * str = "    ";
  switch(comp_gain) {
    case KT0803_ALC_COMP_GAIN_N6DB:  str=" -6D"; break;
    case KT0803_ALC_COMP_GAIN_N9DB:  str=" -9D"; break;
    case KT0803_ALC_COMP_GAIN_N12DB: str="-12D"; break;
    case KT0803_ALC_COMP_GAIN_N15DB: str="-15D"; break; 
    case KT0803_ALC_COMP_GAIN_6DB:   str="  6D"; break; 
    case KT0803_ALC_COMP_GAIN_3DB:   str="  3D"; break; 
    case KT0803_ALC_COMP_GAIN_0DB:   str="  0D"; break; 
    case KT0803_ALC_COMP_GAIN_N3DB:  str=" -3D"; break;  
  }
  tm1650_set_str(str);
}

static void do_trans_alc_comp_gain(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  if(to_func != sm_cur_function || to_state != sm_cur_state) {
    tm1650_set_str("GAIN");
    delay_ms(1000);
    trans_fm_main_display_comp_gain(kt0803_get_alc_comp_gain());
    return;
  }
  
  if(ev == EV_KEY_PLUS_PRESS) {
    trans_fm_main_display_comp_gain(kt0803_next_alc_comp_gain());
  } else if(ev == EV_KEY_NEG_PRESS) {
    trans_fm_main_display_comp_gain(kt0803_prev_alc_comp_gain());
  }
}

static const struct sm_trans_slot code  sm_trans_alc_init[] = {
  {EV_KEY_UP, SM_TRANS_ALC, SM_TRANS_ALC_ONOFF, do_trans_alc_onoff},
  {NULL, NULL, NULL, NULL}
};

static const struct sm_trans_slot code  sm_trans_alc_onoff[] = {
  {EV_KEY_PLUS_PRESS, SM_TRANS_ALC, SM_TRANS_ALC_ONOFF, do_trans_alc_onoff},
  {EV_KEY_NEG_PRESS, SM_TRANS_ALC, SM_TRANS_ALC_ONOFF, do_trans_alc_onoff},
  {EV_KEY_OK_PRESS, SM_TRANS_ALC, SM_TRANS_ALC_DELAY, do_trans_alc_delay},
  {EV_KEY_MENU_PRESS, SM_TRANS_FM, SM_TRANS_FM_INIT, do_trans_fm_init},  
  {NULL, NULL, NULL, NULL}
};

static const struct sm_trans_slot code  sm_trans_alc_delay[] = {
  {EV_KEY_PLUS_PRESS, SM_TRANS_ALC, SM_TRANS_ALC_DELAY, do_trans_alc_delay},
  {EV_KEY_NEG_PRESS, SM_TRANS_ALC, SM_TRANS_ALC_DELAY, do_trans_alc_delay},
  {EV_KEY_OK_PRESS, SM_TRANS_ALC, SM_TRANS_ALC_ATTACK, do_trans_alc_attack},
  {EV_KEY_MENU_PRESS, SM_TRANS_FM, SM_TRANS_FM_INIT, do_trans_fm_init},  
  {NULL, NULL, NULL, NULL}
};

static const struct sm_trans_slot code  sm_trans_alc_attack[] = {
  {EV_KEY_PLUS_PRESS, SM_TRANS_ALC, SM_TRANS_ALC_ATTACK, do_trans_alc_attack},
  {EV_KEY_NEG_PRESS, SM_TRANS_ALC, SM_TRANS_ALC_ATTACK, do_trans_alc_attack},
  {EV_KEY_OK_PRESS, SM_TRANS_ALC, SM_TRANS_ALC_HOLD, do_trans_alc_hold},
  {EV_KEY_MENU_PRESS, SM_TRANS_FM, SM_TRANS_FM_INIT, do_trans_fm_init},  
  {NULL, NULL, NULL, NULL}
};

static const struct sm_trans_slot code  sm_trans_alc_hold[] = {
  {EV_KEY_PLUS_PRESS, SM_TRANS_ALC, SM_TRANS_ALC_HOLD, sm_trans_alc_hold},
  {EV_KEY_NEG_PRESS, SM_TRANS_ALC, SM_TRANS_ALC_HOLD, sm_trans_alc_hold},
  {EV_KEY_OK_PRESS, SM_TRANS_ALC, SM_TRANS_ALC_HIGH, do_trans_alc_high},
  {EV_KEY_MENU_PRESS, SM_TRANS_FM, SM_TRANS_FM_INIT, do_trans_fm_init},  
  {NULL, NULL, NULL, NULL}
};

static const struct sm_trans_slot code  sm_trans_alc_high[] = {
  {EV_KEY_PLUS_PRESS, SM_TRANS_ALC, SM_TRANS_ALC_HIGH, do_trans_alc_high},
  {EV_KEY_NEG_PRESS, SM_TRANS_ALC, SM_TRANS_ALC_HIGH, do_trans_alc_high},
  {EV_KEY_OK_PRESS, SM_TRANS_ALC, SM_TRANS_ALC_LOW, do_trans_alc_low},
  {EV_KEY_MENU_PRESS, SM_TRANS_FM, SM_TRANS_FM_INIT, do_trans_fm_init},  
  {NULL, NULL, NULL, NULL}
};

static const struct sm_trans_slot code  sm_trans_alc_low[] = {
  {EV_KEY_PLUS_PRESS, SM_TRANS_ALC, SM_TRANS_ALC_LOW, do_trans_alc_low},
  {EV_KEY_NEG_PRESS, SM_TRANS_ALC, SM_TRANS_ALC_LOW, do_trans_alc_low},
  {EV_KEY_OK_PRESS, SM_TRANS_ALC, SM_TRANS_ALC_COMP_GAIN, do_trans_alc_comp_gain},
  {EV_KEY_MENU_PRESS, SM_TRANS_FM, SM_TRANS_FM_INIT, do_trans_fm_init},  
  {NULL, NULL, NULL, NULL}
};

static const struct sm_trans_slot code  sm_trans_alc_comp_gain[] = {
  {EV_KEY_PLUS_PRESS, SM_TRANS_ALC, SM_TRANS_ALC_COMP_GAIN, do_trans_alc_comp_gain},
  {EV_KEY_NEG_PRESS, SM_TRANS_ALC, SM_TRANS_ALC_COMP_GAIN, do_trans_alc_comp_gain},
  {EV_KEY_OK_PRESS, SM_TRANS_ALC, SM_TRANS_ALC_ONOFF, do_trans_alc_onoff},
  {EV_KEY_MENU_PRESS, SM_TRANS_FM, SM_TRANS_FM_INIT, do_trans_fm_init},  
  {NULL, NULL, NULL, NULL}
};

const struct sm_state_slot code sm_function_trans_alc[] = {
  {"INIT", sm_trans_alc_init},
  {"ONOFF", sm_trans_alc_onoff},
  {"DELAY", sm_trans_alc_delay}, 
  {"ATTACK", sm_trans_alc_attack}, 
  {"HOLD", sm_trans_alc_hold}, 
  {"HIGH", sm_trans_alc_high},
  {"LOW", sm_trans_alc_low},  
  {"COMP_GAIN", sm_trans_alc_comp_gain},   
};
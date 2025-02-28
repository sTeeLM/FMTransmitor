#include "sm_trans_sd.h"
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


void do_trans_sd_init(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  tm1650_clear();
}

static void do_trans_sd_onoff(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  if(to_func != sm_cur_function || to_state != sm_cur_state) {
    tm1650_set_str(kt0803_cfg.flag & KT0803_CFG_FLAG_SLNCDIS ? " OFF" : " ON");
    return;
  }
  
  if(ev == EV_KEY_PLUS_PRESS || ev == EV_KEY_NEG_PRESS) {
    bit disable = kt0803_cfg.flag & KT0803_CFG_FLAG_SLNCDIS ? 1 : 0;
    kt0803_cfg.flag &= ~KT0803_CFG_FLAG_SLNCDIS;
    kt0803_cfg.flag |= disable ? 0 : 1;
    tm1650_set_str(disable ? "  ON" : " OFF");
    kt0803_set_slncdis(!disable);
  }
}
static void trans_fm_main_display_thh(kt0803_slncthh_t thh)
{
  const char * str = "    ";
  tm1650_clr_dp();
  switch(thh) {
    case KT0803_SLNCTHH_050MV: str = "  05"; tm1650_set_dp(1); break;
    case KT0803_SLNCTHH_1MV:   str = "   1"; break;
    case KT0803_SLNCTHH_2MV:   str = "   2"; break;
    case KT0803_SLNCTHH_4MV:   str = "   4"; break;
    case KT0803_SLNCTHH_6MV:   str = "   6"; break;
    case KT0803_SLNCTHH_8MV:   str = "   8"; break;
    case KT0803_SLNCTHH_16MV:  str = "  16"; break;
    case KT0803_SLNCTHH_32MV:  str = "  32"; break;
    case KT0803_SLNCTHH_64MV:  str = "  64"; break;
  }
  tm1650_set_str(str);
}

static void do_trans_sd_thh(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  if(to_func != sm_cur_function || to_state != sm_cur_state) {
    tm1650_set_str(" THH");
    delay_ms(1000);
    trans_fm_main_display_thh(kt0803_get_slncthh());
    return;
  }
  
  if(ev == EV_KEY_PLUS_PRESS) {
    trans_fm_main_display_thh(kt0803_next_slncthh());
  } else if(ev == EV_KEY_NEG_PRESS) {
    trans_fm_main_display_thh(kt0803_prev_slncthh());
  }
}

static void trans_fm_main_display_thl(kt0803_slncthl_t thl)
{
  const char * str = "    ";
  tm1650_clr_dp();
  switch(thl) {
    case KT0803_SLNCTHL_025MV: str = " 025"; tm1650_set_dp(2); break;
    case KT0803_SLNCTHL_050MV: str = "  05"; tm1650_set_dp(1); break;
    case KT0803_SLNCTHL_1MV:   str = "   1"; break;
    case KT0803_SLNCTHL_2MV:   str = "   2"; break;
    case KT0803_SLNCTHL_4MV:   str = "   4"; break;
    case KT0803_SLNCTHL_6MV:   str = "   6"; break;
    case KT0803_SLNCTHL_8MV:   str = "   8"; break;
    case KT0803_SLNCTHL_16MV:  str = "  16"; break;
    case KT0803_SLNCTHL_32MV:  str = "  32"; break;
  }
  tm1650_set_str(str);
}

static void do_trans_sd_thl(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  if(to_func != sm_cur_function || to_state != sm_cur_state) {
    tm1650_set_str(" THL");
    delay_ms(1000);
    trans_fm_main_display_thl(kt0803_get_slncthl());
    return;
  }
  
  if(ev == EV_KEY_PLUS_PRESS) {
    trans_fm_main_display_thl(kt0803_next_slncthl());
  } else if(ev == EV_KEY_NEG_PRESS) {
    trans_fm_main_display_thl(kt0803_prev_slncthl());
  }
}

static void trans_fm_main_display_cth(kt0803_slnccnt_high_t cth)
{
  const char * str = "    ";
  switch(cth) {
    case KT0803_SLNCCNT_HIGH_15:   str = "  15"; break;
    case KT0803_SLNCCNT_HIGH_31:   str = "  31"; break;
    case KT0803_SLNCCNT_HIGH_63:   str = "  63"; break;
    case KT0803_SLNCCNT_HIGH_127:  str = " 127"; break;
    case KT0803_SLNCCNT_HIGH_255:  str = " 255"; break; 
    case KT0803_SLNCCNT_HIGH_511:  str = " 511"; break; 
    case KT0803_SLNCCNT_HIGH_1023: str = "1023"; break; 
    case KT0803_SLNCCNT_HIGH_2047: str = "2047"; break; 
  }
  tm1650_set_str(str);
}

static void do_trans_sd_cth(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  if(to_func != sm_cur_function || to_state != sm_cur_state) {
    tm1650_set_str(" CTH");
    delay_ms(1000);
    trans_fm_main_display_cth(kt0803_get_slnccnt_high());
    return;
  }
  
  if(ev == EV_KEY_PLUS_PRESS) {
    trans_fm_main_display_cth(kt0803_next_slnccnt_high());
  } else if(ev == EV_KEY_NEG_PRESS) {
    trans_fm_main_display_cth(kt0803_prev_slnccnt_high());
  }
}

static void trans_fm_main_display_ctl(kt0803_slnccnt_low_t ctl)
{
  const char * str = "    ";
  switch(ctl) {
    case KT0803_SLNCCNT_LOW_1:  str = "   1"; break;
    case KT0803_SLNCCNT_LOW_2:  str = "   2"; break;
    case KT0803_SLNCCNT_LOW_4:  str = "   4"; break;
    case KT0803_SLNCCNT_LOW_8:  str = "   8"; break; 
    case KT0803_SLNCCNT_LOW_16: str = "  16"; break; 
    case KT0803_SLNCCNT_LOW_32: str = "  32"; break; 
    case KT0803_SLNCCNT_LOW_64: str = "  64"; break; 
    case KT0803_SLNCCNT_LOW_128:str = " 128"; break;  
  }
  tm1650_set_str(str);
}

static void do_trans_sd_ctl(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  if(to_func != sm_cur_function || to_state != sm_cur_state) {
    tm1650_set_str(" CTL");
    delay_ms(1000);
    trans_fm_main_display_ctl(kt0803_get_slnccnt_low());
    return;
  }
  
  if(ev == EV_KEY_PLUS_PRESS) {
    trans_fm_main_display_ctl(kt0803_next_slnccnt_low());
  } else if(ev == EV_KEY_NEG_PRESS) {
    trans_fm_main_display_ctl(kt0803_prev_slnccnt_low());
  }
}

static const struct sm_trans_slot code  sm_trans_sd_init[] = {
  {EV_KEY_UP, SM_TRANS_SD, SM_TRANS_SD_ONOFF, do_trans_sd_onoff},
  {NULL, NULL, NULL, NULL}
};

static const struct sm_trans_slot code  sm_trans_sd_onoff[] = {
  {EV_KEY_PLUS_PRESS, SM_TRANS_SD, SM_TRANS_SD_ONOFF, do_trans_sd_onoff},
  {EV_KEY_NEG_PRESS, SM_TRANS_SD, SM_TRANS_SD_ONOFF, do_trans_sd_onoff},
  {EV_KEY_OK_PRESS, SM_TRANS_SD, SM_TRANS_SD_THH, do_trans_sd_thh},
  {EV_KEY_MENU_PRESS, SM_TRANS_FM, SM_TRANS_FM_INIT, do_trans_fm_init},  
  {NULL, NULL, NULL, NULL}
};

static const struct sm_trans_slot code  sm_trans_sd_thh[] = {
  {EV_KEY_PLUS_PRESS, SM_TRANS_SD, SM_TRANS_SD_THH, do_trans_sd_thh},
  {EV_KEY_NEG_PRESS, SM_TRANS_SD, SM_TRANS_SD_THH, do_trans_sd_thh},
  {EV_KEY_OK_PRESS, SM_TRANS_SD, SM_TRANS_SD_THL, do_trans_sd_thl},
  {EV_KEY_MENU_PRESS, SM_TRANS_FM, SM_TRANS_FM_INIT, do_trans_fm_init},  
  {NULL, NULL, NULL, NULL}
};

static const struct sm_trans_slot code  sm_trans_sd_thl[] = {
  {EV_KEY_PLUS_PRESS, SM_TRANS_SD, SM_TRANS_SD_THL, do_trans_sd_thl},
  {EV_KEY_NEG_PRESS, SM_TRANS_SD, SM_TRANS_SD_THL, do_trans_sd_thl},
  {EV_KEY_OK_PRESS, SM_TRANS_SD, SM_TRANS_SD_CTH, do_trans_sd_cth},
  {EV_KEY_MENU_PRESS, SM_TRANS_FM, SM_TRANS_FM_INIT, do_trans_fm_init},  
  {NULL, NULL, NULL, NULL}
};

static const struct sm_trans_slot code  sm_trans_sd_cth[] = {
  {EV_KEY_PLUS_PRESS, SM_TRANS_SD, SM_TRANS_SD_CTH, do_trans_sd_cth},
  {EV_KEY_NEG_PRESS, SM_TRANS_SD, SM_TRANS_SD_CTH, do_trans_sd_cth},
  {EV_KEY_OK_PRESS, SM_TRANS_SD, SM_TRANS_SD_CTL, do_trans_sd_ctl},
  {EV_KEY_MENU_PRESS, SM_TRANS_FM, SM_TRANS_FM_INIT, do_trans_fm_init},  
  {NULL, NULL, NULL, NULL}
};

static const struct sm_trans_slot code  sm_trans_sd_ctl[] = {
  {EV_KEY_PLUS_PRESS, SM_TRANS_SD, SM_TRANS_SD_CTL, do_trans_sd_ctl},
  {EV_KEY_NEG_PRESS, SM_TRANS_SD, SM_TRANS_SD_CTL, do_trans_sd_ctl},
  {EV_KEY_OK_PRESS, SM_TRANS_SD, SM_TRANS_SD_ONOFF, do_trans_sd_onoff},
  {EV_KEY_MENU_PRESS, SM_TRANS_FM, SM_TRANS_FM_INIT, do_trans_fm_init},  
  {NULL, NULL, NULL, NULL}
};

const struct sm_state_slot code sm_function_trans_sd[] = {
  {"SM_TRANS_SD_INIT", sm_trans_sd_init},
  {"SM_TRANS_SD_ONOFF", sm_trans_sd_onoff},
  {"SM_TRANS_SD_THH", sm_trans_sd_thh},
  {"SM_TRANS_SD_THL", sm_trans_sd_thl},
  {"SM_TRANS_SD_CTH", sm_trans_sd_cth},
  {"SM_TRANS_SD_CTL", sm_trans_sd_ctl},  
};
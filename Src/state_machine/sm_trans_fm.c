#include "sm_trans_fm.h"
#include "task.h"
#include "sm.h"
#include "cext.h"
#include "debug.h"
#include "power.h"
#include "gpio.h"
#include "delay.h"
#include "tm1650.h"
#include "kt0803.h"
#include "eerom.h"
#include "power.h"

#include "sm_trans_alc.h"
#include "sm_trans_sd.h"
#include "sm_trans_pa.h"
#include "sm_trans_rf_gain.h"
#include "sm_trans_ptc.h"
#include "sm_trans_stereo.h"
#include "sm_trans_fdd.h"
#include "sm_trans_bass.h"
#include "sm_trans_afre.h"
#include "sm_trans_scm.h"
#include "sm_trans_pta.h"
#include "sm_trans_bright.h"


void do_trans_fm_init(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  tm1650_clear();
}

static void trans_fm_main_display_freq(uint16_t freq)
{
  if(kt0803_cfg.flag & KT0803_CFG_FLAG_MUTE) {
    tm1650_set_dig(0, 0, '-');
    tm1650_set_dig(1, 0, '-'); 
    tm1650_set_dig(2, 0, '-');
    tm1650_set_dig(3, 0, '-');    
  } else {
    tm1650_set_dig(0, 0, freq / 1000 ? (freq / 1000 + 0x30) : ' ');
    freq = freq % 1000;
    tm1650_set_dig(1, 0, freq / 100 + 0x30);
    freq = freq % 100;
    tm1650_set_dig(2, 1, freq / 10 + 0x30);
    freq = freq % 10;
    tm1650_set_dig(3, 0, freq + 0x30);
  }
}

static void do_trans_fm_main(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  if(to_func != sm_cur_function || to_state != sm_cur_state) {
    trans_fm_main_display_freq(kt0803_cfg.freq);
    return;
  }
  
  if(ev == EV_KEY_PLUS_PRESS) {
    kt0803_cfg.freq = kt0803_next_ch(0);
  } else if(ev == EV_KEY_PLUS_LPRESS) {
    kt0803_cfg.freq = kt0803_next_ch(1);
  } else if(ev == EV_KEY_NEG_PRESS) {
    kt0803_cfg.freq = kt0803_prev_ch(0);
  } else if(ev == EV_KEY_NEG_LPRESS) {
    kt0803_cfg.freq = kt0803_prev_ch(1);
  } else if(ev == EV_KEY_OK_PRESS) {
    bit mute = (kt0803_cfg.flag & KT0803_CFG_FLAG_MUTE) ? 1 : 0;
    kt0803_cfg.flag &= ~KT0803_CFG_FLAG_MUTE;
    kt0803_cfg.flag |= mute ? 0 : KT0803_CFG_FLAG_MUTE;
    kt0803_set_mute(kt0803_cfg.flag & KT0803_CFG_FLAG_MUTE);
  }
  trans_fm_main_display_freq(kt0803_cfg.freq);
}

static void trans_fm_main_display_vol(uint8_t vol)
{
  tm1650_set_dig(0, 0, 'U');
  tm1650_set_dig(1, 0, 'O');
  tm1650_set_dig(2, 0, vol / 10 ? (vol / 10 + 0x30) : ' ');
  vol %= 10;
  tm1650_set_dig(3, 0, vol + 0x30);
}

static void do_trans_fm_volume(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  uint8_t vol;
  if(to_func != sm_cur_function || to_state != sm_cur_state) {
    tm1650_clear();
    vol = kt0803_get_vol();
    trans_fm_main_display_vol(vol);
    return;
  }
  
  if(ev == EV_KEY_PLUS_PRESS) {
    vol = kt0803_next_vol(0);
  } else if(ev == EV_KEY_PLUS_LPRESS) {
    vol = kt0803_next_vol(1);
  } else if(ev == EV_KEY_NEG_PRESS) {
    vol = kt0803_prev_vol(0);
  } else if(ev == EV_KEY_NEG_LPRESS) {
    vol = kt0803_prev_vol(1);
  }
  
  trans_fm_main_display_vol(vol);
}

#define FM_MENU_ITEM 12
static char * code menu_array[FM_MENU_ITEM] = 
{
  "ALC ",
  "SD  ",
  "PA  ",
  "RFG ",
  "PTC ", 
  "STER", 
  "BASS",
  "FDD ",
  "AFRE", 
  "SCM ",
  "PTA ",
  "BR  ",  
};


static uint8_t trans_fm_main_menu;
static void trans_fm_main_display_menu(uint8_t menu)
{
  CDBG("trans_fm_main_display_menu %bu\n", menu);
  tm1650_set_str(menu_array[menu % FM_MENU_ITEM]);
}

static void do_trans_fm_menu(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  
  if(to_func != sm_cur_function || to_state != sm_cur_state) {
    tm1650_clear();
    trans_fm_main_menu = 0;
    trans_fm_main_display_menu(trans_fm_main_menu);
    return;
  }
  
  if(ev == EV_KEY_PLUS_PRESS) {
    trans_fm_main_menu ++;
    if(trans_fm_main_menu ==  FM_MENU_ITEM)
      trans_fm_main_menu = 0;
  } else if(ev == EV_KEY_NEG_PRESS) {
    trans_fm_main_menu --;
    if(trans_fm_main_menu ==  255)
      trans_fm_main_menu = FM_MENU_ITEM - 1;
  } else if(ev == EV_KEY_OK_PRESS) {
    /* task_set(xx) */
    task_set(trans_fm_main_menu + EV_MENU_ALC);
  }
  
  trans_fm_main_display_menu(trans_fm_main_menu);
}

void do_trans_fm_pwoff(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  tm1650_clear();
  if(ev == EV_KEY_POWER_PRESS) {
    tm1650_set_str(" OFF");
    eerom_save_config();
    delay_ms(1000);
    power_set_enable(0);
  }
}

static const struct sm_trans_slot code  sm_trans_fm_init[] = {
  {EV_INIT, SM_TRANS_FM, SM_TRANS_FM_MAIN, do_trans_fm_main},
  {EV_KEY_UP, SM_TRANS_FM, SM_TRANS_FM_MAIN, do_trans_fm_main},
  {NULL, NULL, NULL, NULL}
};

static const struct sm_trans_slot code  sm_trans_fm_main[] = {
  {EV_KEY_PLUS_PRESS, SM_TRANS_FM, SM_TRANS_FM_MAIN, do_trans_fm_main},  // next ch, 0.1 Mhz
  {EV_KEY_PLUS_LPRESS, SM_TRANS_FM, SM_TRANS_FM_MAIN, do_trans_fm_main}, // next ch, 1 MhZ
  {EV_KEY_NEG_PRESS, SM_TRANS_FM, SM_TRANS_FM_MAIN, do_trans_fm_main},   // prev ch, 0.1 Mhz
  {EV_KEY_NEG_LPRESS, SM_TRANS_FM, SM_TRANS_FM_MAIN, do_trans_fm_main},  // prev ch, 1 MhZ
  {EV_KEY_OK_PRESS, SM_TRANS_FM, SM_TRANS_FM_MAIN, do_trans_fm_main},    // Mute on/off
  {EV_KEY_MENU_PRESS, SM_TRANS_FM, SM_TRANS_FM_VOLUME, do_trans_fm_volume}, // set volume
  {EV_KEY_MENU_LPRESS, SM_TRANS_FM, SM_TRANS_FM_MENU, do_trans_fm_menu}, // select menu
  {EV_KEY_POWER_PRESS, SM_TRANS_FM, SM_TRANS_FM_PW_OFF, do_trans_fm_pwoff}, // Power OFF
  {NULL, NULL, NULL, NULL}
};

static const struct sm_trans_slot code  sm_trans_fm_volume[] = {
  {EV_KEY_PLUS_PRESS, SM_TRANS_FM, SM_TRANS_FM_VOLUME, do_trans_fm_volume},  // volume +
  {EV_KEY_PLUS_LPRESS, SM_TRANS_FM, SM_TRANS_FM_VOLUME, do_trans_fm_volume}, // volume ++
  {EV_KEY_NEG_PRESS, SM_TRANS_FM, SM_TRANS_FM_VOLUME, do_trans_fm_volume},   // volume -
  {EV_KEY_NEG_LPRESS, SM_TRANS_FM, SM_TRANS_FM_VOLUME, do_trans_fm_volume},  // volume --
  {EV_KEY_OK_PRESS, SM_TRANS_FM, SM_TRANS_FM_MAIN, do_trans_fm_main},        // OK
  {NULL, NULL, NULL, NULL}
};

static const struct sm_trans_slot code  sm_trans_fm_menu[] = {
  {EV_KEY_PLUS_PRESS, SM_TRANS_FM, SM_TRANS_FM_MENU, do_trans_fm_menu}, // next menu item
  {EV_KEY_NEG_PRESS, SM_TRANS_FM, SM_TRANS_FM_MENU, do_trans_fm_menu},  // prev menu item
  {EV_KEY_MENU_PRESS, SM_TRANS_FM, SM_TRANS_FM_MAIN, do_trans_fm_main}, // return main
  {EV_KEY_OK_PRESS, SM_TRANS_FM, SM_TRANS_FM_MENU, do_trans_fm_menu},   // select
  
  /* Menu Items */
  {EV_MENU_ALC, SM_TRANS_ALC, SM_TRANS_ALC_INIT, do_trans_alc_init},  
  {EV_MENU_SD, SM_TRANS_SD, SM_TRANS_SD_INIT, do_trans_sd_init},
  {EV_MENU_PA, SM_TRANS_PA, SM_TRANS_PA_INIT, do_trans_pa_init}, 
  {EV_MENU_RFG, SM_TRANS_RF_GAIN, SM_TRANS_RF_GAIN_INIT, do_trans_rf_gain_init},
  {EV_MENU_PTC, SM_TRANS_PTC, SM_TRANS_PTC_INIT, do_trans_ptc_init},
  {EV_MENU_STER, SM_TRANS_STEREO, SM_TRANS_STEREO_INIT, do_trans_stereo_init},
  {EV_MENU_BASS, SM_TRANS_BASS, SM_TRANS_BASS_INIT, do_trans_bass_init},
  {EV_MENU_FDD, SM_TRANS_FDD, SM_TRANS_FDD_INIT, do_trans_fdd_init},
  {EV_MENU_AFRE, SM_TRANS_AFRE, SM_TRANS_AFRE_INIT, do_trans_afre_init},
  {EV_MENU_SCM, SM_TRANS_SCM, SM_TRANS_SCM_INIT, do_trans_scm_init},
  {EV_MENU_PTA, SM_TRANS_PTA, SM_TRANS_PTA_INIT, do_trans_pta_init}, 
  {EV_MENU_BR, SM_TRANS_BRIGHT, SM_TRANS_BRIGHT_INIT, do_trans_bright_init},  
  
  {NULL, NULL, NULL, NULL}
};

static const struct sm_trans_slot code  sm_trans_fm_pwoff[] = {
  {NULL, NULL, NULL, NULL}
};

const struct sm_state_slot code sm_function_trans_fm[] = {
  {"SM_TRANS_FM_INIT", sm_trans_fm_init},
  {"SM_TRANS_FM_MAIN", sm_trans_fm_main}, 
  {"SM_TRANS_FM_VOLUME", sm_trans_fm_volume},
  {"SM_TRANS_FM_MENU", sm_trans_fm_menu},
  {"SM_TRANS_FM_PW_OFF", sm_trans_fm_pwoff},
};
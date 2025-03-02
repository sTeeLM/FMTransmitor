#include "sm_trans_fdd.h"
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

void do_trans_fdd_init(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  tm1650_clear();
}

static void do_trans_fdd_main(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  if(to_func != sm_cur_function || to_state != sm_cur_state) {
    tm1650_set_str(kt0803_cfg.flag & KT0803_CFG_FLAG_FDD ? "  75" : "1125");
    if(kt0803_cfg.flag & KT0803_CFG_FLAG_FDD)
      tm1650_set_dp(1);
    return;
  }
  
  if(ev == EV_KEY_PLUS_PRESS || ev == EV_KEY_NEG_PRESS) {
    bit is1125 = kt0803_get_fedv() == KT0803_FEDV_112P5KHZ;
    kt0803_cfg.flag &= ~KT0803_CFG_FLAG_FDD;
    kt0803_cfg.flag |= is1125 ? 0 : KT0803_CFG_FLAG_FDD;
    tm1650_set_str(is1125 ? "1125" : "  75");
    if(!is1125){
      tm1650_set_dp(1);
    }
    kt0803_set_fedv(is1125 ? KT0803_FEDV_75KHZ : KT0803_FEDV_112P5KHZ);
  }
}

static const struct sm_trans_slot code  sm_trans_fdd_init[] = {
  {EV_KEY_UP, SM_TRANS_FDD, SM_TRANS_FDD_MAIN, do_trans_fdd_main},
  {NULL, NULL, NULL, NULL}
};

static const struct sm_trans_slot code  sm_trans_fdd_main[] = {
  {EV_KEY_PLUS_PRESS, SM_TRANS_FDD, SM_TRANS_FDD_MAIN, do_trans_fdd_main},
  {EV_KEY_NEG_PRESS, SM_TRANS_FDD, SM_TRANS_FDD_MAIN, do_trans_fdd_main},
  {EV_KEY_MENU_PRESS, SM_TRANS_FM, SM_TRANS_FM_INIT, do_trans_fm_init},  
  {NULL, NULL, NULL, NULL}
};

const struct sm_state_slot code sm_function_trans_fdd[] = {
  {"INIT", sm_trans_fdd_init},
  {"MAIN", sm_trans_fdd_main},  
};
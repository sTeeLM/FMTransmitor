#include "sm_trans_ptc.h"
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

void do_trans_ptc_init(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  tm1650_clear();
}
/*
  KT0803_PHTCNST_75US = 0,
  KT0803_PHTCNST_50US = 1
*/
static void do_trans_ptc_main(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  if(to_func != sm_cur_function || to_state != sm_cur_state) {
    tm1650_set_str(kt0803_cfg.flag & KT0803_CFG_FLAG_PHTCNST ? "50US" : "75US");
    return;
  }
  
  if(ev == EV_KEY_PLUS_PRESS || ev == EV_KEY_NEG_PRESS) {
    bit is50 = kt0803_get_phtcnst() == KT0803_PHTCNST_50US;
    kt0803_cfg.flag &= ~KT0803_CFG_FLAG_PHTCNST;
    kt0803_cfg.flag |= is50 ? 0 : KT0803_CFG_FLAG_PHTCNST;
    tm1650_set_str(is50 ? "75US" : "50US");
    kt0803_set_phtcnst(is50 ? KT0803_PHTCNST_75US : KT0803_PHTCNST_50US);
  }
}

static const struct sm_trans_slot code  sm_trans_ptc_init[] = {
  {EV_KEY_UP, SM_TRANS_PTC, SM_TRANS_PTC_MAIN, do_trans_ptc_main},
  {NULL, NULL, NULL, NULL}
};

static const struct sm_trans_slot code  sm_trans_ptc_main[] = {
  {EV_KEY_PLUS_PRESS, SM_TRANS_PTC, SM_TRANS_PTC_MAIN, do_trans_ptc_main},
  {EV_KEY_NEG_PRESS, SM_TRANS_PTC, SM_TRANS_PTC_MAIN, do_trans_ptc_main},
  {EV_KEY_MENU_PRESS, SM_TRANS_FM, SM_TRANS_FM_INIT, do_trans_fm_init},  
  {NULL, NULL, NULL, NULL}
};


const struct sm_state_slot code sm_function_trans_ptc[] = {
  {"INIT", sm_trans_ptc_init},
  {"MAIN", sm_trans_ptc_main},
};
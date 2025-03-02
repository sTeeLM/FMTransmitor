#include "sm_trans_scm.h"
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

void do_trans_scm_init(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  tm1650_clear();
}
/*
  KT0803_SWCH_MOD_MUTE = 0,
  KT0803_SWCH_MOD_PA
*/
static void do_trans_scm_main(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  if(to_func != sm_cur_function || to_state != sm_cur_state) {
    tm1650_set_str(kt0803_cfg.flag & KT0803_CFG_FLAG_SCM ? "  PA" : "MUTE");
    return;
  }
  
  if(ev == EV_KEY_PLUS_PRESS || ev == EV_KEY_NEG_PRESS) {
    bit ispa = kt0803_get_swch_mod() == KT0803_SWCH_MOD_PA;
    kt0803_cfg.flag &= ~KT0803_CFG_FLAG_SCM;
    kt0803_cfg.flag |= ispa ? 0 : KT0803_CFG_FLAG_SCM;
    tm1650_set_str(ispa ? "MUTE" : "  PA");
    kt0803_set_swch_mod(ispa ? KT0803_SWCH_MOD_MUTE : KT0803_SWCH_MOD_PA);
  }
}

static const struct sm_trans_slot code  sm_trans_scm_init[] = {
  {EV_KEY_UP, SM_TRANS_SCM, SM_TRANS_SCM_MAIN, do_trans_scm_main},
  {NULL, NULL, NULL, NULL}
};

static const struct sm_trans_slot code  sm_trans_scm_main[] = {
  {EV_KEY_PLUS_PRESS, SM_TRANS_SCM, SM_TRANS_SCM_MAIN, do_trans_scm_main},
  {EV_KEY_NEG_PRESS, SM_TRANS_SCM, SM_TRANS_SCM_MAIN, do_trans_scm_main},
  {EV_KEY_MENU_PRESS, SM_TRANS_FM, SM_TRANS_FM_INIT, do_trans_fm_init},  
  {NULL, NULL, NULL, NULL}
};

const struct sm_state_slot code sm_function_trans_scm[] = {
  {"INIT", sm_trans_scm_init},
  {"MAIN", sm_trans_scm_main},  
};
#include "sm_trans_afre.h"
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

void do_trans_afre_init(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  tm1650_clear();
}

static void do_trans_afre_main(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  if(to_func != sm_cur_function || to_state != sm_cur_state) {
    tm1650_set_str(kt0803_cfg.flag & KT0803_CFG_FLAG_AFRE ? "  ON" : " OFF");
    return;
  }
  
  if(ev == EV_KEY_PLUS_PRESS || ev == EV_KEY_NEG_PRESS) {
    bit enable = kt0803_get_au_enhancement();
    kt0803_cfg.flag &= ~KT0803_CFG_FLAG_AFRE;
    kt0803_cfg.flag |= enable ? 0 : KT0803_CFG_FLAG_AFRE;
    tm1650_set_str(enable ? " OFF" : "  ON");
    kt0803_set_au_enhancement(!enable);
  }
}

static const struct sm_trans_slot code  sm_trans_afre_init[] = {
  {EV_KEY_UP, SM_TRANS_AFRE, SM_TRANS_AFRE_MAIN, do_trans_afre_main},
  {NULL, NULL, NULL, NULL}
};

static const struct sm_trans_slot code  sm_trans_afre_main[] = {
  {EV_KEY_PLUS_PRESS, SM_TRANS_AFRE, SM_TRANS_AFRE_MAIN, do_trans_afre_main},
  {EV_KEY_NEG_PRESS, SM_TRANS_AFRE, SM_TRANS_AFRE_MAIN, do_trans_afre_main},
  {EV_KEY_MENU_PRESS, SM_TRANS_FM, SM_TRANS_FM_INIT, do_trans_fm_init},  
  {NULL, NULL, NULL, NULL}
};

const struct sm_state_slot code sm_function_trans_afre[] = {
  {"INIT", sm_trans_afre_init},
  {"MAIN", sm_trans_afre_main},
};
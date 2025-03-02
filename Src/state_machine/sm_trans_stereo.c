#include "sm_trans_stereo.h"
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

void do_trans_stereo_init(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  tm1650_clear();
}

void do_trans_stereo_main(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  if(to_func != sm_cur_function || to_state != sm_cur_state) {
    tm1650_set_str(kt0803_cfg.flag & KT0803_CFG_FLAG_MONO ? "MONO" : "STER");
    return;
  }
  
  if(ev == EV_KEY_PLUS_PRESS || ev == EV_KEY_NEG_PRESS) {
    bit enable = kt0803_get_mono();
    kt0803_cfg.flag &= ~KT0803_CFG_FLAG_MONO;
    kt0803_cfg.flag |= enable ? 0 : KT0803_CFG_FLAG_MONO;
    tm1650_set_str(enable ? "STER" : "MONO");
    kt0803_set_mono(!enable);
  }
}

static const struct sm_trans_slot code  sm_trans_stereo_init[] = {
  {EV_KEY_UP, SM_TRANS_STEREO, SM_TRANS_STEREO_MAIN, do_trans_stereo_main},
  {NULL, NULL, NULL, NULL}
};

static const struct sm_trans_slot code  sm_trans_stereo_main[] = {
  {EV_KEY_PLUS_PRESS, SM_TRANS_STEREO, SM_TRANS_STEREO_MAIN, do_trans_stereo_main},
  {EV_KEY_NEG_PRESS, SM_TRANS_STEREO, SM_TRANS_STEREO_MAIN, do_trans_stereo_main},
  {EV_KEY_MENU_PRESS, SM_TRANS_FM, SM_TRANS_FM_INIT, do_trans_fm_init},  
  {NULL, NULL, NULL, NULL}
};


const struct sm_state_slot code sm_function_trans_stereo[] = {
  {"INIT", sm_trans_stereo_init},
  {"MAIN", sm_trans_stereo_main},  
};
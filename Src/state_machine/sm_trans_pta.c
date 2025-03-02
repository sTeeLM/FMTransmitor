#include "sm_trans_pta.h"
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

/*
  KT0803_PLTADJ_LOW = 0,
  KT0803_PLTADJ_HIGH
*/

void do_trans_pta_init(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  tm1650_clear();
}

static void do_trans_pta_main(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  if(to_func != sm_cur_function || to_state != sm_cur_state) {
    tm1650_set_str(kt0803_cfg.flag & KT0803_CFG_FLAG_PLTADJ ? "HIGH" : " LOW");
    return;
  }
  
  if(ev == EV_KEY_PLUS_PRESS || ev == EV_KEY_NEG_PRESS) {
    bit high = kt0803_get_pltadj() == KT0803_PLTADJ_HIGH;
    kt0803_cfg.flag &= ~KT0803_CFG_FLAG_PLTADJ;
    kt0803_cfg.flag |= high ? 0 : KT0803_CFG_FLAG_PLTADJ;
    tm1650_set_str(high ? "LOW" : "HIGH");
    kt0803_set_pltadj(high ? KT0803_PLTADJ_LOW : KT0803_PLTADJ_HIGH);
  }
}

static const struct sm_trans_slot code  sm_trans_pta_init[] = {
  {EV_KEY_UP, SM_TRANS_PTA, SM_TRANS_PTA_MAIN, do_trans_pta_main},
  {NULL, NULL, NULL, NULL}
};

static const struct sm_trans_slot code  sm_trans_pta_main[] = {
  {EV_KEY_PLUS_PRESS, SM_TRANS_PTA, SM_TRANS_PTA_MAIN, do_trans_pta_main},
  {EV_KEY_NEG_PRESS, SM_TRANS_PTA, SM_TRANS_PTA_MAIN, do_trans_pta_main},
  {EV_KEY_MENU_PRESS, SM_TRANS_FM, SM_TRANS_FM_INIT, do_trans_fm_init},  
  {NULL, NULL, NULL, NULL}
};


const struct sm_state_slot code sm_function_trans_pta[] = {
  {"INIT", sm_trans_pta_init},
  {"MAIN", sm_trans_pta_main},  
};
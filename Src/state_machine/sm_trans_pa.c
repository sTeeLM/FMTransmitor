#include "sm_trans_pa.h"
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

void do_trans_pa_init(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  tm1650_clear();
}

static void do_trans_pa_pdpa(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  if(to_func != sm_cur_function || to_state != sm_cur_state) {
    tm1650_set_str("PDPA");
    delay_ms(1000);
    tm1650_set_str(kt0803_cfg.flag & KT0803_CFG_FLAG_PDPA ? "  ON" : " OFF");
    return;
  }
  
  if(ev == EV_KEY_PLUS_PRESS || ev == EV_KEY_NEG_PRESS) {
    bit enable = kt0803_get_pdpa();
    kt0803_cfg.flag &= ~KT0803_CFG_FLAG_PDPA;
    kt0803_cfg.flag |= enable ? 0 : KT0803_CFG_FLAG_PDPA;
    tm1650_set_str(enable ? " OFF" : "  ON");
    kt0803_set_pdpa(!enable);
  }
}

static void do_trans_pa_padn(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  if(to_func != sm_cur_function || to_state != sm_cur_state) {
    tm1650_set_str("PADN");
    delay_ms(1000);
    tm1650_set_str(kt0803_cfg.flag & KT0803_CFG_FLAG_PADN ? "  ON" : " OFF");
    return;
  }
  
  if(ev == EV_KEY_PLUS_PRESS || ev == EV_KEY_NEG_PRESS) {
    bit enable = kt0803_get_padn();
    kt0803_cfg.flag &= ~KT0803_CFG_FLAG_PADN;
    kt0803_cfg.flag |= enable ? 0 : KT0803_CFG_FLAG_PADN;
    tm1650_set_str(enable ? " OFF" : "  ON");
    kt0803_set_padn(!enable);
  }
}

static void do_trans_pa_bias(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  if(to_func != sm_cur_function || to_state != sm_cur_state) {
    tm1650_set_str("BIAS");
    delay_ms(1000);
    tm1650_set_str(kt0803_cfg.flag & KT0803_CFG_FLAG_PA_BIAS ? "  ON" : " OFF");
    return;
  }
  
  if(ev == EV_KEY_PLUS_PRESS || ev == EV_KEY_NEG_PRESS) {
    bit enable = kt0803_get_pa_bias();
    kt0803_cfg.flag &= ~KT0803_CFG_FLAG_PA_BIAS;
    kt0803_cfg.flag |= enable ? 0 : KT0803_CFG_FLAG_PA_BIAS;
    tm1650_set_str(enable ? " OFF" : "  ON");
    kt0803_set_pa_bias(!enable);
  }
}

static const struct sm_trans_slot code  sm_trans_pa_init[] = {
  {EV_KEY_UP, SM_TRANS_PA, SM_TRANS_PA_PDPA, do_trans_pa_pdpa},
  {NULL, NULL, NULL, NULL}
};

static const struct sm_trans_slot code  sm_trans_pa_pdpa[] = {
  {EV_KEY_PLUS_PRESS, SM_TRANS_PA, SM_TRANS_PA_PDPA, do_trans_pa_pdpa},
  {EV_KEY_NEG_PRESS, SM_TRANS_PA, SM_TRANS_PA_PDPA, do_trans_pa_pdpa},
  {EV_KEY_OK_PRESS, SM_TRANS_PA, SM_TRANS_PA_PADN, do_trans_pa_padn},
  {EV_KEY_MENU_PRESS, SM_TRANS_FM, SM_TRANS_FM_INIT, do_trans_fm_init},  
  {NULL, NULL, NULL, NULL}
};

static const struct sm_trans_slot code  sm_trans_pa_padn[] = {
  {EV_KEY_PLUS_PRESS, SM_TRANS_PA, SM_TRANS_PA_PADN, do_trans_pa_padn},
  {EV_KEY_NEG_PRESS, SM_TRANS_PA, SM_TRANS_PA_PADN, do_trans_pa_padn},
  {EV_KEY_OK_PRESS, SM_TRANS_PA, SM_TRANS_PA_BIAS, do_trans_pa_bias},
  {EV_KEY_MENU_PRESS, SM_TRANS_FM, SM_TRANS_FM_INIT, do_trans_fm_init},  
  {NULL, NULL, NULL, NULL}
};
  
static const struct sm_trans_slot code  sm_trans_pa_bias[] = {
  {EV_KEY_PLUS_PRESS, SM_TRANS_PA, SM_TRANS_PA_BIAS, do_trans_pa_bias},
  {EV_KEY_NEG_PRESS, SM_TRANS_PA, SM_TRANS_PA_BIAS, do_trans_pa_padn},
  {EV_KEY_OK_PRESS, SM_TRANS_PA, SM_TRANS_PA_PDPA, do_trans_pa_pdpa},
  {EV_KEY_MENU_PRESS, SM_TRANS_FM, SM_TRANS_FM_INIT, do_trans_fm_init},  
  {NULL, NULL, NULL, NULL}
};

const struct sm_state_slot code sm_function_trans_pa[] = {
  {"INIT", sm_trans_pa_init},
  {"PDPA", sm_trans_pa_pdpa},
  {"PADN", sm_trans_pa_padn}, 
  {"BIAS", sm_trans_pa_bias},   
};
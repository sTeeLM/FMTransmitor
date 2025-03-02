#include "sm_trans_bright.h"
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


void do_trans_bright_init(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  tm1650_clear();
}

static void trans_bright_display(uint8_t br)
{
  if(br == 0) br = 8;
  tm1650_set_dig(0, 1, br + 0x30);
}

void do_trans_bright_main(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  if(to_func != sm_cur_function || to_state != sm_cur_state) {
    trans_bright_display(tm1650_get_brightness());
    return;
  }
  
  if(ev == EV_KEY_PLUS_PRESS) {
    trans_bright_display(tm1650_next_brightness());
  } else if(ev == EV_KEY_NEG_PRESS) {
    trans_bright_display(tm1650_prev_brightness());
  }
}

static const struct sm_trans_slot code  sm_trans_bright_init[] = {
  {EV_KEY_UP, SM_TRANS_BRIGHT, SM_TRANS_BRIGHT_MAIN, do_trans_bright_main},
  {NULL, NULL, NULL, NULL}
};

static const struct sm_trans_slot code  sm_trans_bright_main[] = {
  {EV_KEY_PLUS_PRESS, SM_TRANS_BRIGHT, SM_TRANS_BRIGHT_MAIN, do_trans_bright_main},
  {EV_KEY_NEG_PRESS, SM_TRANS_BRIGHT, SM_TRANS_BRIGHT_MAIN, do_trans_bright_main},
  {EV_KEY_MENU_PRESS, SM_TRANS_FM, SM_TRANS_FM_INIT, do_trans_fm_init},  
  {NULL, NULL, NULL, NULL}
};

const struct sm_state_slot code sm_function_trans_bright[] = {
  {"INIT", sm_trans_bright_init},
  {"MAIN", sm_trans_bright_main}, 
};
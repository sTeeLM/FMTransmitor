#include "button.h"
#include "sm.h"
#include "debug.h"
#include "gpio.h"
#include "tm1650.h"
#include "clock.h"

#define BUTTON_DIV 4
#define BUTTON_LPRESS 16

static uint8_t button_power_key_down_cnt;
static uint16_t button_key_down_cnt;
static uint8_t button_prev_key_code;
static bit button_lpress_sent;

void button_initialize (void)
{
  CDBG("button_initialize\n");
  button_power_key_down_cnt = 0;
  button_key_down_cnt = 0;
  button_prev_key_code = 0;
}

static bit button_scan_key(uint8_t key_code, enum task_events ev1, enum task_events ev2)
{
  bit ret = 0;
  uint8_t current_key_code;
  current_key_code = tm1650_get_scan_code();
  if(current_key_code == key_code) {
    button_key_down_cnt ++;
    if(button_key_down_cnt > BUTTON_LPRESS) {
      if((button_key_down_cnt % BUTTON_DIV) == (BUTTON_DIV - 1)) {
        task_set(ev2);
        button_lpress_sent = 1;
      }
      ret = 1;
    }
    button_prev_key_code = key_code;
  } else if(current_key_code == 0 && button_prev_key_code == key_code){
    if(button_key_down_cnt) {
      if(!button_lpress_sent)
        task_set(ev1);
      task_set(EV_KEY_UP);
      button_key_down_cnt = 0;
      ret = 1;
      button_prev_key_code = 0;
      button_lpress_sent = 0;
    }
  }
  return ret;
}

void button_scan_proc(enum task_events ev)
{
  /* scan power key */
  if(POWER_KEY) {
    if(button_power_key_down_cnt == 0) {
      task_set(EV_KEY_POWER_PRESS);
    }
    button_power_key_down_cnt ++;
  } else {
    if(button_power_key_down_cnt) {
      button_power_key_down_cnt = 0;
      task_set(EV_KEY_UP);
    }
  }
  do {
    if(button_scan_key(BUTTON_PLUS_KEY_CODE, EV_KEY_PLUS_PRESS, EV_KEY_PLUS_LPRESS))
      break;
    if(button_scan_key(BUTTON_NEG_KEY_CODE, EV_KEY_NEG_PRESS, EV_KEY_NEG_LPRESS))
      break;
    if(button_scan_key(BUTTON_OK_KEY_CODE, EV_KEY_OK_PRESS, EV_KEY_OK_LPRESS))
      break;
    if(button_scan_key(BUTTON_MENU_KEY_CODE, EV_KEY_MENU_PRESS, EV_KEY_MENU_LPRESS))
      break;
  }while(0);
}

void button_proc(enum task_events ev)
{
  switch(ev) {
    case EV_KEY_POWER_PRESS:
      CDBG("EV_KEY_POWER_PRESS\n"); break;
    case EV_KEY_PLUS_PRESS:
      CDBG("EV_KEY_PLUS_PRESS\n"); break;
    case EV_KEY_PLUS_LPRESS:
      CDBG("EV_KEY_PLUS_LPRESS\n"); break;
    case EV_KEY_NEG_PRESS:
      CDBG("EV_KEY_NEG_PRESS\n"); break;
    case EV_KEY_NEG_LPRESS:
      CDBG("EV_KEY_NEG_LPRESS\n"); break;
    case EV_KEY_OK_PRESS:
      CDBG("EV_KEY_OK_PRESS\n"); break;
    case EV_KEY_OK_LPRESS:
      CDBG("EV_KEY_OK_LPRESS\n"); break;
    case EV_KEY_MENU_PRESS:
      CDBG("EV_KEY_MENU_PRESS\n"); break;
    case EV_KEY_MENU_LPRESS:
      CDBG("EV_KEY_MENU_LPRESS\n"); break;
    case EV_KEY_UP:
      CDBG("EV_KEY_UP\n"); break;
    default:
      CDBG("button_proc unknown ev %bu\n", ev); break;
  }
  sm_run(ev);
}

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "clock.h"
#include "task.h"
#include "debug.h"
#include "sm.h"

/* hardware*/
#include "button.h"

/*
  EV_INIT                = 0,
  EV_KEY_SCAN            = 1,  // 扫描按键
  EV_TIMEO               = 2,  // 定时器超时
  EV_250MS               = 3,  // 大约每250ms转一下
  EV_1S                  = 4,  // 大约每1s转一下
  EV_KEY_POWER_PRESS     = 5,  // power按下 
  EV_KEY_PLUS_PRESS      = 6,  // +键按下
  EV_KEY_PLUS_LPRESS     = 7,  // +键长按 
  EV_KEY_NEG_PRESS       = 8,  // -键按下
  EV_KEY_NEG_LPRESS      = 9,  // -键长按 
  EV_KEY_OK_PRESS        = 10, // OK键按下
  EV_KEY_OK_LPRESS       = 11, // OK键长按 
  EV_KEY_MENU_PRESS      = 12, // Menu键按下
  EV_KEY_MENU_LPRESS     = 13, // Menu键长按
  EV_KEY_UP              = 14, // 按键抬起
  
  EV_MENU_ALC,
  EV_MENU_SD,
  EV_MENU_PA,
  EV_MENU_RFG,
  EV_MENU_PTC, 
  EV_MENU_STER, 
  EV_MENU_BASS,
  EV_MENU_FDD,
  EV_MENU_AFRE, 
  EV_MENU_SCM,
  EV_MENU_PTA,
  EV_MENU_BR, 
  EV_CNT  
*/

const char * code task_names[] =
{
  "INIT",
  "KS", 
  "TIMEO",  
  "250MS",
  "1S",
  "POWER_P",
  "PLUS_P",
  "PLUS_LP",
  "NEG_P",
  "NEG_LP", 
  "OK_P",
  "OK_LP", 
  "M_P",
  "M_LP", 
  "M_ALC", 
  "M_SD", 
  "M_PA", 
  "M_RFG",
  "M_STER",
  "M_BR",  
  "M_PTC", 
  "M_BASS",
  "M_FDD",
  "M_AFRE",
  "M_SCM",
  "M_PTA", 
  "KEY_UP",  
};

static void null_proc(enum task_events ev)
{
  sm_run(ev);
}


static const TASK_PROC task_procs[EV_CNT] = 
{
  null_proc,
  button_scan_proc,
  clock_time_proc,
  clock_time_proc,
  /* EV_1S*/
  clock_time_proc,
  button_proc,
  button_proc,
  button_proc,
  button_proc,
  button_proc,
  button_proc,
  button_proc,
  button_proc,
  button_proc,
  null_proc,
  null_proc,
  null_proc,
  null_proc,
  null_proc,
  null_proc,
  null_proc,
  null_proc, 
  null_proc,
  null_proc,
  null_proc,
  null_proc,
  button_proc,  
};


uint16_t ev_bits0;
uint16_t ev_bits1;

void task_initialize (void)
{
  ev_bits0 = 0;
  ev_bits1 = 0;
}

void task_run(void)
{
  uint8_t c;
  for(c = 0; c < EV_CNT; c++) {
    if(task_test(c)) {
      task_clr(c);
      task_procs[c](c);
    }
  }
}

void task_dump(void)
{
  uint8_t i;
  for (i = 0 ; i < EV_CNT; i ++) {
    CDBG("[%02bd][%s] %c\n", i, task_names[i], task_test(i) ? '1' : '0');
  }
}

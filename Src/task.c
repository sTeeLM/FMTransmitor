#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "clock.h"
#include "task.h"
#include "debug.h"
#include "sm.h"

/* hardware*/


/*
  EV_INIT                = 0,
  EV_KEY_SCAN            = 1, // 扫描按键
  EV_TIMEO               = 2, // 定时器超时
  EV_250MS               = 3, // 大约每250ms转一下
  EV_1S                  = 4, // 大约每1s转一下
  EV_KEY_POWER_PRESS     = 5, // power按下 
  EV_KEY_VPLUS_PRESS     = 6, // Volue+键按下
  EV_KEY_VPLUS_LPRESS    = 7, // Volue+键长按
  EV_KEY_VNEG_PRESS      = 8, // Volue-键按下
  EV_KEY_VNEG_LPRESS     = 9, // Volue-键长按
  EV_KEY_CPLUS_PRESS     = 10, // Chanel+键按下
  EV_KEY_CPLUS_LPRESS    = 11, // Chanel+键长按
  EV_KEY_CNEG_PRESS      = 12, // Chanel-键按下
  EV_KEY_CNEG_LPRESS     = 13, // Chanel-键长按
  EV_KEY_MUTE_PRESS      = 14, // Mute键按下
  EV_CNT  
*/

const char * code task_names[] =
{
  "EV_INIT",
  "EV_KEY_SCAN", 
  "EV_TIMEO",  
  "EV_250MS",
  "EV_1S",
  "EV_KEY_POWER_PRESS",
  "EV_KEY_VPLUS_PRESS",
  "EV_KEY_VPLUS_LPRESS",
  "EV_KEY_VNEG_PRESS",
  "EV_KEY_VNEG_LPRESS", 
  "EV_KEY_CPLUS_PRESS",
  "EV_KEY_CPLUS_LPRESS", 
  "EV_KEY_CNEG_PRESS",
  "EV_KEY_CNEG_LPRESS", 
  "EV_KEY_MUTE_PRESS",     
};

static void null_proc(enum task_events ev)
{
  sm_run(ev);
}


static const TASK_PROC task_procs[EV_CNT] = 
{
  null_proc,
  null_proc,
  null_proc,
  clock_time_proc,
  /* EV_1S*/
  clock_time_proc,
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
};


uint16_t ev_bits0;
uint16_t ev_bits1;

void task_initialize (void)
{
  CDBG("task_initialize\n");
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

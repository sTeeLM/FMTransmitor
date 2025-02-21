#ifndef __FM_TRANS_TASK_H__
#define __FM_TRANS_TASK_H__

#include <stdint.h>

// max 32
enum task_events
{
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
};

extern uint16_t ev_bits0;
extern uint16_t ev_bits1;

extern const char * code task_names[];

typedef void (*TASK_PROC)(enum task_events);

void task_initialize (void);

void task_dump(void);

// 这些宏也在中断里被调用，所以不能是带参数函数，只能拿宏实现了
#define task_set(ev1)             \
  do{                             \
    if(ev1 < 16)                  \
      ev_bits0 |= 1<<ev1;         \
    else                          \
      ev_bits1 |= 1<<(ev1 - 16);  \
  }while(0)

#define task_clr(ev1)               \
  do{                               \
    if(ev1 < 16)                    \
      ev_bits0 &= ~(1<<ev1);        \
    else                            \
      ev_bits1 &= ~(1<<(ev1 - 16)); \
  }while(0)
    
#define  task_test(ev1)             \
  (ev1 < 16 ? (ev_bits0 & (1<<ev1)) : (ev_bits1 & (1<<(ev1 - 16))))

void task_run(void);

#endif
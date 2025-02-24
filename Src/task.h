#ifndef __FM_TRANS_TASK_H__
#define __FM_TRANS_TASK_H__

#include <stdint.h>

// max 32
enum task_events
{
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
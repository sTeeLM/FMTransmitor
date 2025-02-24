#include <STC8G.H>

#include "debug.h"
#include "gpio.h"
#include "power.h"
#include "com.h"
#include "eerom.h"
#include "delay.h"
#include "task.h"
#include "clock.h"
#include "button.h"
#include "power.h"
#include "tm1650.h"
#include "kt0803.h"
#include "sm.h"
#include "task.h"

void main(void)
{
  uint8_t i = '-';
  
  EA = 1;
  
  debug_initialize();
  gpio_initialize();
  com_initialize();
  power_initialize();
  power_set_enable(1);
  eerom_initialize();
  
  tm1650_initialize();  
  kt0803_initialize();
  
  button_initialize();
  clock_initialize();
  task_initialize();
  sm_initialize();
  
  delay_ms(1000);
  
  kt0803_set_ch(1000);
  
  while(1) {
    uint8_t sc;
    delay_ms(1000);
    tm1650_set_dig(0, 0, i);
    tm1650_set_dig(1, 0, i);
    tm1650_set_dig(2, 0, i);
    tm1650_set_dig(3, 0, i);
    i = (i + 1);
    if(i == 'Z')
      i = '-';
    sc = tm1650_get_scan_code();
    CDBG("tm1650_get_scan_code %bx\n", sc);
    CDBG("kt0803_get_ch %x\n", kt0803_get_ch());
  }
  
  /*
  while(1) {
    task_run();
  }
  */  
}
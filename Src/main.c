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
  
  tm1650_initialize(); 
  
  eerom_initialize();
  tm1650_set_brightness(tm1650_cfg.brightness);
  
  kt0803_initialize();
  
  button_initialize();
  clock_initialize();
  task_initialize();
  sm_initialize();
  
  delay_ms(1000);

  while(1) {
    task_run();
  }
 
}
#include <STC8G.H>

#include "power.h"
#include "gpio.h"
#include "debug.h"
#include "task.h"
#include "delay.h"
#include "sm.h"

void power_initialize(void)
{
  
}

void power_set_enable(bit enable)
{
  POWER_EN = enable;
}



#include "button.h"
#include "sm.h"
#include "debug.h"
#include "gpio.h"
#include "tm1650.h"
#include "clock.h"

void button_initialize (void)
{
  CDBG("button_initialize\n");
}

void button_scan_proc(enum task_events ev)
{
  
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

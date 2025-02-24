#ifndef __FM_TRANS_BUTTON_H__
#define __FM_TRANS_BUTTON_H__

#include "task.h"

void button_initialize (void);
void button_proc(enum task_events ev);
void button_scan_proc(enum task_events ev);
#endif
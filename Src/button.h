#ifndef __FM_TRANS_BUTTON_H__
#define __FM_TRANS_BUTTON_H__

#include "task.h"

#define BUTTON_NEG_KEY_CODE     0x44 // Prev Channel / Negitive
#define BUTTON_PLUS_KEY_CODE    0x4C // Next Channel / Plus
#define BUTTON_OK_KEY_CODE     0x54 // Mute / OK
#define BUTTON_MENU_KEY_CODE   0x5C // Volume / Menu

void button_initialize (void);
void button_proc(enum task_events ev);
void button_scan_proc(enum task_events ev);
#endif
#ifndef __FM_TRANS_TM1650_H__
#define __FM_TRANS_TM1650_H__

#include <stdint.h>

typedef struct _tm1650_cfg_t
{
  uint8_t brightness;
}tm1650_cfg_t;

extern tm1650_cfg_t tm1650_cfg;

typedef enum _tm1650_mod_t
{
  TM1650_MOD_7_DIG = 0,
  TM1650_MOD_8_DIG
} tm1650_mod_t;

void tm1650_factory_reset(void);
void tm1650_initialize(void);
void tm1650_set_mod(tm1650_mod_t mod);
tm1650_mod_t tm1650_get_mod(void);
void tm1650_set_brightness(uint8_t brightness);
uint8_t tm1650_get_brightness(void);
void tm1650_enable_display(bit enable);
void tm1650_set_dig(uint8_t index, bit dp, uint8_t dat);
uint8_t tm1650_get_scan_code(void);
void tm1650_clear(void);
void tm1650_set_str(const char * str);
#endif
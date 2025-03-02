#include "kt0803.h"
#include "debug.h"
#include "i2c.h"
#include "delay.h"

kt0803_cfg_t kt0803_cfg;

void kt0803_factory_reset(void)
{
  // Channel: 100.0MHz
  kt0803_cfg.freq = 1000;
  
  kt0803_cfg.flag = 0;
  
  // Automatic Level Control: disabled
  kt0803_cfg.flag &= ~KT0803_CFG_FLAG_ALC_ENABLE; 
  kt0803_cfg.alc_delay = (uint8_t)KT0803_ALC_DELAY_25US;
  kt0803_cfg.alc_attack = (uint8_t)KT0803_ALC_ATTACK_25US;
  kt0803_cfg.alc_comp_gain = (uint8_t)KT0803_ALC_COMP_GAIN_N3DB;
  kt0803_cfg.alc_hold = (uint8_t)KT0803_ALC_HOLD_5S;
  kt0803_cfg.alc_high_th = (uint8_t)KT0803_ALC_HIGH_TH_0P6;
  kt0803_cfg.alc_low_th = (uint8_t)KT0803_ALC_LOW_TH_0P25; 

  // Silence Detection: disable
  kt0803_cfg.flag |= KT0803_CFG_FLAG_SLNCDIS;
  kt0803_cfg.slncthl = (uint8_t)KT0803_SLNCTHL_025MV;
  kt0803_cfg.slncthh = (uint8_t)KT0803_SLNCTHH_050MV;
  kt0803_cfg.slnccnt_high = (uint8_t)KT0803_SLNCCNT_HIGH_15;
  kt0803_cfg.slnccnt_low = (uint8_t)KT0803_SLNCCNT_LOW_1;
  
  // RF Gain: 108.0dB
  kt0803_cfg.rf_gain = (uint8_t)KT0803_RF_GAIN_1080;
  
  // Pilot Tone Amplitude: KT0803_PLTADJ_LOW
  kt0803_cfg.flag &= ~KT0803_CFG_FLAG_PLTADJ;
  
  // Pre-emphasis Time-Constant: KT0803_PHTCNST_75US
  kt0803_cfg.flag &= ~KT0803_CFG_FLAG_PHTCNST;
  
  // Stereo or Mono: Stereo
  kt0803_cfg.flag &= ~KT0803_CFG_FLAG_MONO;
  
  // Bass Boost Control: KT0803_BASS_DISABLE
  kt0803_cfg.bass = (uint8_t)KT0803_BASS_DISABLE;
  
  // Frequency Deviation Delection: disabled
  kt0803_cfg.flag &= ~KT0803_CFG_FLAG_FDD;
  
  // Audio Frequency Response Enhancement: disabled
  kt0803_cfg.flag &= ~KT0803_CFG_FLAG_AFRE;
  
  // Switching Channel Mode Selection: Mute
  kt0803_cfg.flag &= ~KT0803_CFG_FLAG_SCM;
  
  // PGA: 0dB, 1dB step
  kt0803_cfg.pga_gain = 0;
  
  // Power Amplifier: disable PDPA, PADN, PA_BIAS
  kt0803_cfg.flag &= ~(KT0803_CFG_FLAG_PDPA|KT0803_CFG_FLAG_PADN);
  kt0803_cfg.flag |= KT0803_CFG_FLAG_PA_BIAS;
  
  // Mute: disable
  kt0803_cfg.flag &= ~KT0803_CFG_FLAG_MUTE;
  
}

static uint8_t kt0803_read_reg(uint8_t addr);

static void kt0803_dump_device(void)
{
  uint8_t i, dat;
  
  
  CDBG("    ");
  for(i = 0 ; i <= 0xF ; i ++ ) {
    CDBG("  %bx", i);
  }
  CDBG("\n");
  for(i = 0 ; i <= 0x27 ; i ++ ) {
    if(i % 16 == 0) {
      if(i != 0) {
        CDBG("\n");
      }      
      CDBG("[%02bx] ", i);
    }
    dat = kt0803_read_reg(i);
    CDBG("%02bx ", dat);
  }
  if(i % 16 != 0) {
    CDBG("\n");
  }
}

static void kt0803_dump_cfg(void)
{
  CDBG("kt0803 cfg:\n");
  
  CDBG("flag 0x%04x\n", kt0803_cfg.flag);
  CDBG("freq %u\n", kt0803_cfg.freq);
  
  CDBG("alc_delay %bu\n", kt0803_cfg.alc_delay); 
  CDBG("alc_attack %bu\n", kt0803_cfg.alc_attack);
  CDBG("alc_comp_gain %bu\n", kt0803_cfg.alc_comp_gain);  
  CDBG("alc_hold %bu\n", kt0803_cfg.alc_hold);
  CDBG("alc_high_th %bu\n", kt0803_cfg.alc_high_th);
  CDBG("alc_low_th %bu\n", kt0803_cfg.alc_low_th);

  CDBG("slncthl %bu\n", kt0803_cfg.slncthl); 
  CDBG("slncthh %bu\n", kt0803_cfg.slncthh);
  CDBG("slnccnt_high %bu\n", kt0803_cfg.slnccnt_high);  
  CDBG("slnccnt_low %bu\n", kt0803_cfg.slnccnt_low);
  
  CDBG("pga_gain %bd\n", kt0803_cfg.pga_gain); 
  
  CDBG("rf_gain %bu\n", kt0803_cfg.rf_gain);

  CDBG("bass %bu\n", kt0803_cfg.bass);  
}

void kt0803_initialize(void)
{
  //kt0803_dump_cfg();
  
  /* wait device ready.. */
  while(!kt0803_get_pw_ok()){
    delay_ms(100);
  }
  
  /*
  Power amplifier structure selection
    0 = Internal power supply, KT0803 compatible
    1 = External power supply via external inductor
    Note : When an external inductor is used, this bit
    must be set to 1 immediately after the Power OK
    indicator Reg 0x0F[4] is set to 1. Otherwise, the
    device may be destroyed!  
  */
  
  //CDBG("kt0803 before\n");
  //kt0803_dump_device();
  
  /* load cfg into chip */
  // Channel:
  kt0803_set_ch(kt0803_cfg.freq);
  kt0803_set_mute(0);
  
  // Automatic Level Control:
  kt0803_set_alc_enable((kt0803_cfg.flag & KT0803_CFG_FLAG_ALC_ENABLE) ? 1 : 0);
  kt0803_set_alc_delay(kt0803_cfg.alc_delay);
  kt0803_set_alc_attack(kt0803_cfg.alc_attack);
  kt0803_set_alc_comp_gain(kt0803_cfg.alc_comp_gain);
  kt0803_set_alc_hold(kt0803_cfg.alc_hold);
  kt0803_set_alc_high_th(kt0803_cfg.alc_high_th);
  kt0803_set_alc_low_th(kt0803_cfg.alc_low_th);
  
  // Silence Detection
  kt0803_set_slncdis((kt0803_cfg.flag & KT0803_CFG_FLAG_SLNCDIS) ? 1 : 0);
  kt0803_set_slncthl(kt0803_cfg.slncthl);
  kt0803_set_slncthh(kt0803_cfg.slncthh);
  kt0803_set_slnccnt_high(kt0803_cfg.slnccnt_high);
  kt0803_set_slnccnt_low(kt0803_cfg.slnccnt_low);
  
  // RF Gain
  kt0803_set_rf_gain(kt0803_cfg.rf_gain);
  
  // Pilot Tone Amplitude
  kt0803_set_pltadj((kt0803_cfg.flag & KT0803_CFG_FLAG_PLTADJ) ? 1 : 0);
  
  // Pre-emphasis Time-Constant
  kt0803_set_phtcnst((kt0803_cfg.flag & KT0803_CFG_FLAG_PHTCNST) ? 1 : 0);
  
  // Stereo or Mono
  kt0803_set_mono((kt0803_cfg.flag & KT0803_CFG_FLAG_MONO) ? 1 : 0);
  
  // Bass Boost Control
  kt0803_set_bass(kt0803_cfg.bass);
  
  // Frequency Deviation Delection
  kt0803_set_fedv((kt0803_cfg.flag & KT0803_CFG_FLAG_FDD) ? 1 : 0);
  
  // Audio Frequency Response Enhancement
  kt0803_set_au_enhancement((kt0803_cfg.flag & KT0803_CFG_FLAG_AFRE) ? 1 : 0);
  
  // Switching Channel Mode Selection
  kt0803_set_swch_mod((kt0803_cfg.flag & KT0803_CFG_FLAG_SCM) ? 1 : 0);
  
  // PGA, 1db step with PGA_LSB
  kt0803_set_pga_mod(KT0803_PGA_MOD_1DB_STEP);
  kt0803_set_pga_gain(kt0803_cfg.pga_gain);
  
  // Power Amplifier
  kt0803_set_pdpa((kt0803_cfg.flag & KT0803_CFG_FLAG_PDPA) ? 1 : 0);
  kt0803_set_padn((kt0803_cfg.flag & KT0803_CFG_FLAG_PADN) ? 1 : 0);
  kt0803_set_pa_bias((kt0803_cfg.flag & KT0803_CFG_FLAG_PA_BIAS) ? 1 : 0);

  // Mute:
  kt0803_set_mute((kt0803_cfg.flag & KT0803_CFG_FLAG_MUTE) ? 1 : 0);

  //CDBG("kt0803 after\n");
  //kt0803_dump_device();
}

// ----------------------------------------------------------------------------
uint16_t kt0803_next_ch(bit coarse)
{
  if(coarse)
    kt0803_cfg.freq += 10;
  else
    kt0803_cfg.freq ++;
  if(kt0803_cfg.freq > 1080) kt0803_cfg.freq = 700;
  kt0803_set_ch(kt0803_cfg.freq);
  kt0803_set_mute(0);
  return kt0803_cfg.freq;
}

uint16_t kt0803_prev_ch(bit coarse)
{
  if(coarse)
    kt0803_cfg.freq -= 10;
  else
    kt0803_cfg.freq --;
  if(kt0803_cfg.freq < 700) kt0803_cfg.freq = 1080;
  kt0803_set_ch(kt0803_cfg.freq);
  kt0803_set_mute(0);
  return kt0803_cfg.freq;
}

uint8_t kt0803_get_vol(void)
{
  return kt0803_cfg.pga_gain + 15;
}

uint8_t kt0803_next_vol(bit coarse)
{
  if(coarse)
    kt0803_cfg.pga_gain += 4;
  else
    kt0803_cfg.pga_gain ++;
  if(kt0803_cfg.pga_gain > 12)
    kt0803_cfg.pga_gain = -15;
  kt0803_set_pga_gain(kt0803_cfg.pga_gain);
  kt0803_set_ch(kt0803_cfg.freq);
  kt0803_set_mute(0);
  return kt0803_cfg.pga_gain + 15;
}

uint8_t kt0803_prev_vol(bit coarse)
{
  if(coarse)
    kt0803_cfg.pga_gain -= 4;
  else
    kt0803_cfg.pga_gain --;
  if(kt0803_cfg.pga_gain < -15)
    kt0803_cfg.pga_gain = 12;
  kt0803_set_pga_gain(kt0803_cfg.pga_gain);
  kt0803_set_ch(kt0803_cfg.freq);
  kt0803_set_mute(0);
  return kt0803_cfg.pga_gain + 15;
}
typedef void (*KT0803_VALUE_PROC)(uint16_t dat);

static void kt0803_next_value(KT0803_VALUE_PROC proc, uint8_t * val, uint8_t min, uint8_t max)
{
  (*val)++;
  if((*val) > max)
    (*val) = min;
  proc((uint16_t)(*val));
}

static void kt0803_prev_value(KT0803_VALUE_PROC proc, uint8_t * val, uint8_t min, uint8_t max)
{
  if(*val == min)
    *val = max;
  else
    (*val) --;
  proc(*val);
}

uint8_t kt0803_next_alc_delay(void)
{
  kt0803_next_value(kt0803_set_alc_delay, &kt0803_cfg.alc_delay, KT0803_ALC_DELAY_25US, KT0803_ALC_DELAY_400MS);
  return kt0803_cfg.alc_delay;
}

uint8_t kt0803_prev_alc_delay(void)
{
  kt0803_prev_value(kt0803_set_alc_delay, &kt0803_cfg.alc_delay, KT0803_ALC_DELAY_25US, KT0803_ALC_DELAY_400MS);
  return kt0803_cfg.alc_delay;
}

kt0803_alc_attack_t kt0803_next_alc_attack(void)
{
  kt0803_next_value(kt0803_set_alc_attack, &kt0803_cfg.alc_attack, KT0803_ALC_ATTACK_25US, KT0803_ALC_ATTACK_400MS);
  return kt0803_cfg.alc_attack;
}

kt0803_alc_attack_t kt0803_prev_alc_attack(void)
{
  kt0803_prev_value(kt0803_set_alc_attack, &kt0803_cfg.alc_attack, KT0803_ALC_ATTACK_25US, KT0803_ALC_ATTACK_400MS);
  return kt0803_cfg.alc_attack;
}

kt0803_alc_hold_t kt0803_next_alc_hold(void)
{
  kt0803_next_value(kt0803_set_alc_hold, &kt0803_cfg.alc_hold, KT0803_ALC_HOLD_50MS, KT0803_ALC_HOLD_15S);
  return kt0803_cfg.alc_hold;
}

kt0803_alc_hold_t kt0803_prev_alc_hold(void)
{
  kt0803_prev_value(kt0803_set_alc_hold, &kt0803_cfg.alc_hold, KT0803_ALC_HOLD_50MS, KT0803_ALC_HOLD_15S);
  return kt0803_cfg.alc_hold;
}

kt0803_alc_high_th_t kt0803_next_alc_high_th(void)
{
  kt0803_next_value(kt0803_set_alc_high_th, &kt0803_cfg.alc_high_th, KT0803_ALC_HIGH_TH_0P6, KT0803_ALC_HIGH_TH_0P01);
  return kt0803_cfg.alc_high_th;
}

kt0803_alc_high_th_t kt0803_prev_alc_high_th(void)
{
  kt0803_prev_value(kt0803_set_alc_high_th, &kt0803_cfg.alc_high_th, KT0803_ALC_HIGH_TH_0P6, KT0803_ALC_HIGH_TH_0P01);
  return kt0803_cfg.alc_high_th;
}

kt0803_alc_low_th_t kt0803_next_alc_low_th(void)
{
  kt0803_next_value(kt0803_set_alc_low_th, &kt0803_cfg.alc_low_th, KT0803_ALC_LOW_TH_0P25, KT0803_ALC_LOW_TH_0P0001);
  return kt0803_cfg.alc_low_th;
}

kt0803_alc_low_th_t kt0803_prev_alc_low_th(void)
{
  kt0803_prev_value(kt0803_set_alc_low_th, &kt0803_cfg.alc_low_th, KT0803_ALC_LOW_TH_0P25, KT0803_ALC_LOW_TH_0P0001);
  return kt0803_cfg.alc_low_th;
}

kt0803_alc_comp_gain_t kt0803_next_alc_comp_gain(void)
{
  kt0803_next_value(kt0803_set_alc_comp_gain, &kt0803_cfg.alc_comp_gain, KT0803_ALC_COMP_GAIN_N6DB, KT0803_ALC_COMP_GAIN_N3DB);
  return kt0803_cfg.alc_comp_gain;
}

kt0803_alc_comp_gain_t kt0803_prev_alc_comp_gain(void)
{
  kt0803_prev_value(kt0803_set_alc_comp_gain, &kt0803_cfg.alc_comp_gain, KT0803_ALC_COMP_GAIN_N6DB, KT0803_ALC_COMP_GAIN_N3DB);
  return kt0803_cfg.alc_comp_gain;
}

kt0803_slncthh_t kt0803_next_slncthh(void)
{
  kt0803_next_value(kt0803_set_slncthh, &kt0803_cfg.slncthh, KT0803_SLNCTHH_050MV, KT0803_SLNCTHH_64MV);
  return kt0803_cfg.slncthh;
}

kt0803_slncthh_t kt0803_prev_slncthh(void)
{
  kt0803_prev_value(kt0803_set_slncthh, &kt0803_cfg.slncthh, KT0803_SLNCTHH_050MV, KT0803_SLNCTHH_64MV);
  return kt0803_cfg.slncthh;
}

kt0803_slncthl_t kt0803_next_slncthl(void)
{
  kt0803_next_value(kt0803_set_slncthl, &kt0803_cfg.slncthl, KT0803_SLNCTHL_025MV, KT0803_SLNCTHL_32MV);
  return kt0803_cfg.slncthl;
}

kt0803_slncthl_t kt0803_prev_slncthl(void)
{
  kt0803_prev_value(kt0803_set_slncthl, &kt0803_cfg.slncthl, KT0803_SLNCTHL_025MV, KT0803_SLNCTHL_32MV);
  return kt0803_cfg.slncthl;
}

kt0803_slnccnt_high_t kt0803_next_slnccnt_high(void)
{
  kt0803_next_value(kt0803_set_slnccnt_high, &kt0803_cfg.slnccnt_high, KT0803_SLNCCNT_HIGH_15, KT0803_SLNCCNT_HIGH_2047);
  return kt0803_cfg.slnccnt_high;
}

kt0803_slnccnt_high_t kt0803_prev_slnccnt_high(void)
{
  kt0803_prev_value(kt0803_set_slnccnt_high, &kt0803_cfg.slnccnt_high, KT0803_SLNCCNT_HIGH_15, KT0803_SLNCCNT_HIGH_2047);
  return kt0803_cfg.slnccnt_high;
}

kt0803_slnccnt_low_t kt0803_next_slnccnt_low(void)
{
  kt0803_next_value(kt0803_set_slnccnt_low, &kt0803_cfg.slnccnt_low, KT0803_SLNCCNT_LOW_1, KT0803_SLNCCNT_LOW_128);
  return kt0803_cfg.slnccnt_low;
}

kt0803_slnccnt_low_t kt0803_prev_slnccnt_low(void)
{
  kt0803_prev_value(kt0803_set_slnccnt_low, &kt0803_cfg.slnccnt_low, KT0803_SLNCCNT_LOW_1, KT0803_SLNCCNT_LOW_128);
  return kt0803_cfg.slnccnt_low;
}

kt0803_rf_gain_t kt0803_next_rf_gain(void)
{
  kt0803_next_value(kt0803_set_rf_gain, &kt0803_cfg.rf_gain, KT0803_RF_GAIN_955, KT0803_RF_GAIN_1080);
  return kt0803_cfg.rf_gain;
}

kt0803_rf_gain_t kt0803_prev_rf_gain(void)
{
  kt0803_prev_value(kt0803_set_rf_gain, &kt0803_cfg.rf_gain, KT0803_RF_GAIN_955, KT0803_RF_GAIN_1080);
  return kt0803_cfg.rf_gain;
}

kt0803_bass_t kt0803_next_bass(void)
{
  kt0803_next_value(kt0803_set_bass, &kt0803_cfg.bass, KT0803_BASS_DISABLE, KT0803_BASS_17DB);
  return kt0803_cfg.bass;
}

kt0803_bass_t kt0803_prev_bass(void)
{
  kt0803_prev_value(kt0803_set_bass, &kt0803_cfg.bass, KT0803_BASS_DISABLE, KT0803_BASS_17DB);
  return kt0803_cfg.bass;
}

// -----------------------------------------------------------------------------
static uint8_t kt0803_read_reg(uint8_t addr)
{
  uint8_t dat;
  if(I2C_Get(0x7c, addr, &dat)) {
    CDBG("kt0803_read_reg %bx failed!\n", addr);
    dat = 0;
  }
  return dat;
}

static void kt0803_write_reg(uint8_t addr, uint8_t dat)
{
  if(I2C_Put(0x7c, addr, dat)) {
    CDBG("kt0803_write_reg %bx failed!\n", addr);
  }
}

static uint8_t kt0803_get_reg(uint8_t addr, uint8_t bit_index, uint8_t bit_len)
{
  uint8_t bit_mask = 0, i, dat, bit_off;
  
  if(bit_index > 7 || bit_len > 8 || (bit_index + 1) < bit_len) {
    CDBG("kt0803_get_reg wrong param!\n", bit_index, bit_len);
    return 0;
  }
  
  if(bit_len) {
    for(i = 0; i < bit_len; i ++){
      bit_mask <<= 1;
      bit_mask |= 1;
    }
  }
  
  bit_off = (bit_index + 1 - bit_len);
  
  bit_mask <<= bit_off;
  
  dat = kt0803_read_reg(addr);
  
  dat = (dat & bit_mask) >> bit_off;
  
  // CDBG("kt0803_get_reg [%02bx][%02bx][%02bx] : [%02bx]\n", addr, bit_index, bit_len, dat);
  
  return dat;
}

static void kt0803_set_reg(uint8_t addr, uint8_t bit_index, uint8_t bit_len, uint8_t dat)
{
  uint8_t bit_mask = 0, i, old_dat, tmp, bit_off;
  
  if(bit_index > 7 || bit_len > 8 || (bit_index + 1) < bit_len) {
    CDBG("kt0803_set_reg wrong param!\n", bit_index, bit_len);
    return;
  }
  
  if(bit_len) {
    for(i = 0; i < bit_len; i ++){
      bit_mask <<= 1;
      bit_mask |= 1;
    }
  }
  
  bit_off = (bit_index + 1 - bit_len);
  
  bit_mask <<= bit_off;
    
  tmp = kt0803_read_reg(addr);
  old_dat = (tmp & bit_mask) >> bit_off;
  
  tmp = (tmp & ~bit_mask) | (dat << bit_off);
  kt0803_write_reg(addr, tmp);
  
  // CDBG("kt0803_set_reg [%02bx][%02bx][%02bx] : [%02bx] -> [%02bx]\n", addr, bit_index, bit_len, old_dat, dat);
}

/* 
  70MHz ~ 108MHz 
  700~1080 * 20
*/
uint16_t kt0803_get_ch(void)
{
  uint16_t ch;
  
  ch = kt0803_get_reg(0x1, 2, 3);
  ch <<= 8;
  ch |= kt0803_get_reg(0x0, 7, 8);
  ch <<= 1;
  ch |= kt0803_get_reg(0x2, 7, 1);
  return ch / 2;
}

void kt0803_set_ch(uint16_t ch)
{
  if(ch < 700) ch = 700;
  else if (ch > 1080) ch = 1080;
  
  ch *= 2;
  
  kt0803_set_reg(0x2, 7, 1, (ch & 0x1));
  ch >>= 1;
  kt0803_set_reg(0x0, 7, 8, (ch & 0xFF));
  ch >>= 8;
  kt0803_set_reg(0x1, 2, 3, (ch & 0x7));  
}
/*
  Switching Channel Mode Selection.
  0 = Mute when changing channel (default setting)
  1 = PA off when changing channel
*/
kt0803_swch_mod_t kt0803_get_swch_mod(void)
{
  return kt0803_get_reg(0x12, 0, 1) ? KT0803_SWCH_MOD_PA : KT0803_SWCH_MOD_MUTE;
}

void kt0803_set_swch_mod(kt0803_swch_mod_t mod)
{
  kt0803_set_reg(0x12, 0, 1, mod == KT0803_SWCH_MOD_PA ? 1 : 0);
}

/*
  0000 95.5 dBuV
  0001 96.5 dBuV
  0010 97.5 dBuV
  0011 98.2 dBuV
  0100 98.9 dBuV
  0101 100 dBuV
  0110 101.5 dBuV
  0111 102.8 dBuV
  1000 105.1 dBuV (107.2dBuV PA_BIAS=1)
  1001 105.6 dBuV (108dBuV, PA_BIAS=1)
  1010 106.2 dBuV (108.7dBuV, PA_BIAS=1)
  1011 106.5 dBuV (109.5dBuV, PA_BIAS=1)
  1100 107 dBuV   (110.3dBuV, PA_BIAS=1)
  1101 107.4 dBuV (111dBuV, PA_BIAS=1)
  1110 107.7 dBuV (111.7dBuV, PA_BIAS=1)
  1111 (default setting) 108 dBuV (112.5dBuV, PA_BIAS=1)
  bin: 0000 ~ 1111
  hex: 0~0xf
*/
kt0803_rf_gain_t kt0803_get_rf_gain(void)
{
  uint8_t gain;
  kt0803_rf_gain_t ret = KT0803_RF_GAIN_955;
 
  // RFGAIN[1:0]
  gain = kt0803_get_reg(0x1, 7, 2);
  
  // RFGAIN[2]
  gain <<= 2;
  gain |= kt0803_get_reg(0x13, 7, 1);
  
  
  // RFGAIN[3]
  gain <<= 1;
  gain |= kt0803_get_reg(0x2, 6, 1);
  
  return (kt0803_rf_gain_t)gain;
}

void kt0803_set_rf_gain(kt0803_rf_gain_t gain)
{
  uint8_t tmp;
  tmp = ((uint8_t)gain & 0xF);
  
  // RFGAIN[1:0]
  kt0803_set_reg(0x1, 7, 2, (tmp & 0x3));
  
  
  // RFGAIN[2]
  tmp >>= 2;
  kt0803_set_reg(0x13, 7, 1, (tmp & 0x1));
  
  // RFGAIN[3]
  tmp >>= 1;
  kt0803_set_reg(0x2, 6, 1, (tmp & 0x1));
}

/*
  Software Mute
  0: MUTE Disabled (default setting)
  1: MUTE Enabled
*/

bit kt0803_get_mute(void)
{
  return kt0803_get_reg(0x2, 3, 1) ? 1 : 0;
}

void kt0803_set_mute(bit enable)
{
  kt0803_set_reg(0x2, 3, 1, enable ? 1 : 0);
}

/*
  Pilot Tone Amplitude Adjustment
  0: Amplitude low (default setting) 
  1: Amplitude high
*/
kt0803_pltadj_t kt0803_get_pltadj(void)
{
  return kt0803_get_reg(0x2, 2, 1) ? 1 : 0;
}


void kt0803_set_pltadj(kt0803_pltadj_t adj)
{
  kt0803_set_reg(0x2, 2, 1, adj ? 1 : 0);
}


/*
  Pre-emphasis Time-Constant Set
  0: 75 μs (USA, Japan, default setting)
  1: 50 μs (Europe, Australia)
*/

kt0803_phtcnst_t kt0803_get_phtcnst(void)
{
  return (kt0803_phtcnst_t) kt0803_get_reg(0x2, 0, 1);
}

void kt0803_set_phtcnst(kt0803_phtcnst_t cnst)
{
  kt0803_set_reg(0x2, 0, 1, cnst == KT0803_PHTCNST_50US ? 1 : 0);
}


/*
  Automatic Level Control Enable Control
  0 = Disable ALC (default setting) 
  1 = Enable ALC
*/
bit kt0803_get_alc_enable(void)
{
  return kt0803_get_reg(0x4, 7, 1);
}

void kt0803_set_alc_enable(bit enable)
{
  kt0803_set_reg(0x4, 7, 1, enable ? 1 : 0);
}
/*
  0000 = 25us
  0001 = 50us
  0010 = 75us
  0011 = 100us
  0100 = 125us
  0101 = 150us
  0110 = 175us
  0111 = 200us
  1000 = 50ms
  1001 = 100ms
  1010 = 150ms
  1011 = 200ms
  1100 = 250ms
  1101 = 300ms
  1110 = 350ms
  1111 = 400ms
*/
kt0803_alc_delay_t kt0803_get_alc_delay(void)
{
  return (kt0803_alc_delay_t) kt0803_get_reg(0xC, 7, 4);
}

void kt0803_set_alc_delay(kt0803_alc_delay_t delay)
{
  kt0803_set_reg(0xc, 7, 4, ((uint8_t)delay & 0xF));
}

kt0803_alc_attack_t kt0803_get_alc_attack(void)
{
  return (kt0803_alc_attack_t) kt0803_get_reg(0xC, 3, 4);
}

void kt0803_set_alc_attack(kt0803_alc_attack_t attack)
{
  kt0803_set_reg(0xC, 3, 4, ((uint8_t) attack & 0xF));
}

/*
  ALC Compressed Gain Setting
  100 = 06 (6dB) 
  101 = 03 (3dB)
  110 = 00 (0dB)
  111= 1D (-3dB) (default setting) 
  000 = 1A(-6dB)
  001 = 17(-9dB)
  010 = 14(-12dB)
  011 = 11(-15dB)
*/
kt0803_alc_comp_gain_t kt0803_get_alc_comp_gain(void)
{
  return (kt0803_alc_comp_gain_t) kt0803_get_reg(0x15, 7, 3);
}

void kt0803_set_alc_comp_gain(kt0803_alc_comp_gain_t gain)
{
  kt0803_set_reg(0x15, 7, 3, ((uint8_t) gain & 0x7));
}

/*
  ALC Hold Time Selection
  000 = 50ms
  001 = 100ms
  010 = 150ms
  011 = 200ms
  100 = 1s
  101 = 5s (default setting)
  110 = 10s
  111 = 15s
*/
kt0803_alc_hold_t kt0803_get_alc_hold(void)
{
  return (kt0803_alc_hold_t) kt0803_get_reg(0x26, 7, 3);
}

void kt0803_set_alc_hold(kt0803_alc_hold_t hold)
{
  kt0803_set_reg(0x26, 7, 3, ((uint8_t) hold & 0x7));
}

/*
  ALC High Threshold Selection
  000 = 0.6 (default setting)
  001 = 0.5
  010 = 0.4
  011 = 0.3
  100 = 0.2
  101 = 0.1
  110 = 0.05
  111 = 0.01
*/
kt0803_alc_high_th_t kt0803_get_alc_high_th(void)
{
  return (kt0803_alc_high_th_t) kt0803_get_reg(0x26, 3, 3);
}

void kt0803_set_alc_high_th(kt0803_alc_high_th_t th)
{
  kt0803_set_reg(0x26, 3, 3, ((uint8_t) th & 0x7));
}

/*
  ALC Low Threshold
  0000 = 0.25 (default setting)
  0001 = 0.2
  0010 = 0.15
  0011 = 0.1
  0100 = 0.05
  0101 = 0.03
  0110 = 0.02
  0111 = 0.01
  1000 = 0.005
  1001 = 0.001
  1010 = 0.0005
  1011 = 0.0001
*/
kt0803_alc_low_th_t kt0803_get_alc_low_th(void)
{
  return (kt0803_alc_high_th_t) kt0803_get_reg(0x27, 3, 4);
}

void kt0803_set_alc_low_th(kt0803_alc_low_th_t th)
{
  kt0803_set_reg(0x27, 3, 4, ((uint8_t) th & 0xF));
}

/*
  0 = Stereo (default setting) 
  1 = Mono
*/
bit kt0803_get_mono(void)
{
  return kt0803_get_reg(0x4, 6, 1) ? 1 : 0;
}

void kt0803_set_mono(bit enable)
{
  kt0803_set_reg(0x4, 6, 1, enable ? 1 : 0);
}


/*
  01111~11111
  ret: -15dB~12dB
*/
int8_t kt0803_get_pga_gain(void)
{
  uint8_t pga;
  int8_t ret = 0;
  
  // PGA_LSB[1:0]
  pga = kt0803_get_reg(0x4, 5, 2);
  
  // pga mode
  if(kt0803_get_pga_mod() == KT0803_PGA_MOD_1DB_STEP) {
    // PGA[2:0]
    pga <<= 2;
    pga |= kt0803_get_reg(0x1, 5, 3);
  } else {
    pga <<= 2;
    pga |= 0x3;
  }
  
  if((pga & 0x1C) == 0x10) {
    ret = 0;
  } else {
    if(pga & 0x10) {
      ret = (pga & 0x10) - 3;
    } else {
      ret = 0 - (int8_t)pga;
    }
  }
  
  return ret;
}

void kt0803_set_pga_gain(int8_t gain)
{
  uint8_t pga = 0x13;
  
  if(gain < -15) gain = -15;
  else if(gain > 12) gain = 12;
  
  if(gain == 0) {
    pga = 0x13; // 10011
  } else if(gain > 0) {
    pga = (gain + 3) | 0x10;
  } else {
    pga = ((uint8_t)(0 - gain) & 0xF);
  }
  
  // PGA_LSB[1:0]
  kt0803_set_reg(0x4, 5, 2, (pga & 0x3));
  
  // PGA[2:0]
  pga >>= 2;
  kt0803_set_reg(0x1, 5, 3, (pga & 0x7));
}

kt0803_pga_mod_t kt0803_get_pga_mod(void)
{
  return (kt0803_pga_mod_t)kt0803_get_reg(0x10, 0, 1);
}

void kt0803_set_pga_mod(kt0803_pga_mod_t mod)
{
  kt0803_set_reg(0x10, 0, 1, mod == KT0803_PGA_MOD_1DB_STEP ? 1 : 0);
}

/*
  Bass Boost Control
  00 : Disabled (default setting) 
  01 : 5dB
  10 : 11dB
  11 : 17dB
*/
kt0803_bass_t kt0803_get_bass(void)
{
  return (kt0803_bass_t)kt0803_get_reg(0x4, 1, 2);
}

void kt0803_set_bass(kt0803_bass_t bass)
{
  kt0803_set_reg(0x4, 1, 2, ((uint8_t)bass & 0x3));
}

bit kt0803_get_standby(void)
{
  return kt0803_get_reg(0xB, 7, 1) ? 1 : 0;
}

void kt0803_set_standby(bit enable)
{
  kt0803_set_reg(0xB, 7, 1, enable ? 1 : 0);
}

/*
  Power amplifier structure selection
    0 = Internal power supply, KT0803 compatible
    1 = External power supply via external inductor
    Note : When an external inductor is used, this bit
    must be set to 1 immediately after the Power OK
    indicator Reg 0x0F[4] is set to 1. Otherwise, the
    device may be destroyed!
*/
kt0803_pa_ctl_t kt0803_get_pa_ctl(void)
{
  return (kt0803_pa_ctl_t) kt0803_get_reg(0x13, 2, 1);
}

void kt0803_set_pa_ctl(kt0803_pa_ctl_t ctl)
{
  kt0803_set_reg(0x13, 2, 1, ctl == KT0803_PA_CTL_EXTERNAL ? 1 : 0);
}


/*
  Power Amplifier Power Down
  0 = Power amplifier power on (default setting) 
  1 = Power amplifier power down
*/
bit kt0803_get_pdpa(void)
{
  return kt0803_get_reg(0xB, 5, 1) ? 1 : 0;
}

void kt0803_set_pdpa(bit down)
{
  kt0803_set_reg(0xB, 5, 1, down ? 1 : 0);
}

/*
  Automatic Power Down Power Amplifier When
  Silence is Detected
  0 = Disable this feature (default setting) 
  1 = Enable this feature
*/
bit kt0803_get_padn(void)
{
  return kt0803_get_reg(0xB, 2, 1) ? 1 : 0;
}

void kt0803_set_padn(bit enable)
{
  kt0803_set_reg(0xB, 2, 1, enable ? 1 : 0);
}

/*
  PA Bias Current Enhancement.
  0 = Disable PA bias
  1 = Enable PA bias (default setting) 
*/
bit kt0803_get_pa_bias(void)
{
  return kt0803_get_reg(0xE, 1, 1) ? 1 : 0;
}

void kt0803_set_pa_bias(bit enable)
{
  kt0803_set_reg(0xE, 1, 1, enable ? 1 : 0);
}

bit kt0803_get_pw_ok(void)
{
  return kt0803_get_reg(0xF, 4, 1) ? 1 : 0;
}

bit kt0803_get_slncid(void)
{
  return kt0803_get_reg(0xF, 2, 1) ? 1 : 0;
}
/*
  Silence Detection Disable
  0 : Enable
  1 : Disable (default setting) 
*/
bit kt0803_get_slncdis(void)
{
  return kt0803_get_reg(0x12, 7, 1) ? 1 : 0;
}

void kt0803_set_slncdis(bit disable)
{
  kt0803_set_reg(0x12, 7, 1, disable ? 1 : 0);
}

/*
  Silence Detection Low Threshold
  000 : 0.25mv (default setting) 
  001 : 0.5mv
  010 : 1mv
  011 : 2mv
  100 : 4mv
  101 : 8mv
  110 : 16mv
  111 : 32mv
*/
kt0803_slncthl_t kt0803_get_slncthl(void)
{
  return (kt0803_slncthl_t)kt0803_get_reg(0x12, 6, 3);
}

void kt0803_set_slncthl(kt0803_slncthl_t th)
{
  kt0803_set_reg(0x12, 6, 3, ((uint8_t)th & 0x7));
}

/*
  Silence Detection High Threshold
  000 : 0.5mv (default setting)
  001 : 1mv
  010 : 2mv
  011 : 4mv
  100 : 8mv
  101 : 16mv
  110 : 32mv
  111 : 64mv
*/
kt0803_slncthh_t kt0803_get_slncthh(void)
{
  return (kt0803_slncthh_t)kt0803_get_reg(0x12, 3, 3);
}

void kt0803_set_slncthh(kt0803_slncthh_t th)
{
  kt0803_set_reg(0x12, 3, 3, ((uint8_t)th & 0x7));
}

/*
  Silence Detection High Level Counter Threshold
  000 : 15 (default setting)
  001 : 31
  010 : 63
  011 : 127
  100 : 255
  101 : 511
  110 : 1023
  111 : 2047
*/
kt0803_slnccnt_high_t kt0803_get_slnccnt_high(void)
{
  return (kt0803_slnccnt_high_t)kt0803_get_reg(0x14, 4, 3);
}

void kt0803_set_slnccnt_high(kt0803_slnccnt_high_t th)
{
  kt0803_set_reg(0x14, 4, 3, ((uint8_t)th & 0x7));
}

/*
  Silence Detection Low Level Counter Threshold
  000 : 1 (default setting)
  001 : 2
  010 : 4
  011 : 8
  100 : 16
  101 : 32
  110 : 64
  111 : 128
*/
kt0803_slnccnt_low_t kt0803_get_slnccnt_low(void)
{
  return (kt0803_slnccnt_low_t)kt0803_get_reg(0x16, 2, 3);
}

void kt0803_set_slnccnt_low(kt0803_slnccnt_low_t th)
{
  kt0803_set_reg(0x16, 2, 3, ((uint8_t)th & 0x7));
}

/*
  Multiple Reference Clock Selection Enable
  0 = Disable multiple reference clock feature and
  reference clock or crystal oscillator can only select
  through SW1/SW2 pins. (default setting)
  1 = Enable multiple reference clock and user can
  select different reference clock through
  REF_CLK[3:0]
*/
bit kt0803_get_dclk(void)
{
  return kt0803_get_reg(0x1E, 6, 1) ? 1 : 0;
}

void kt0803_set_dclk(bit enable)
{
  kt0803_set_reg(0x1E, 6, 1, enable ? 1 : 0);
}

/*
  Crystal Oscillator Disable Control
  0 = Enable crystal oscillator (default setting)
  1 = Disable crystal oscillator
*/
bit kt0803_get_xtald(void)
{
  return kt0803_get_reg(0x1E, 5, 1) ? 1 : 0;
}

void kt0803_set_xtald(bit enable)
{
  kt0803_set_reg(0x1E, 5, 1, enable ? 1 : 0);
}

/*
  Reference Clock Selection
  0000 = 32.768kHz (default setting)
  0001 = 6.5MHz
  0010 = 7.6MHz
  0011 = 12MHz
  0100 = 13MHz
  0101 = 15.2MHz
  0110 = 19.2MHz
  0111 = 24MHz
  1000 = 26MHz
*/
kt0803_ref_clk_t kt0803_get_ref_clk(void)
{
  return (kt0803_ref_clk_t)kt0803_get_reg(0x1E, 3, 4);
}

void kt0803_set_ref_clk(kt0803_ref_clk_t clk)
{
  kt0803_set_reg(0x1E, 3, 4, ((uint8_t)clk & 0xF));
}

/*
  Frequency Deviation Delection
  0 = 75kHz deviation (default setting)
  1 = 112.5kHz deviation
*/
kt0803_fedv_t kt0803_get_fedv(void)
{
  return (kt0803_fedv_t)kt0803_get_reg(0x17, 6, 1);
}

void kt0803_set_fedv(kt0803_fedv_t fedv)
{
  kt0803_set_reg(0x17, 6, 1, fedv == KT0803_FEDV_112P5KHZ ? 1 : 0);
}

/*
  Audio Frequency Response Enhancement Enable
  0 = Disable (default setting)
  1 = Enable
*/
bit kt0803_get_au_enhancement(void)
{
  return kt0803_get_reg(0x17, 5, 1) ? 1 : 0;
}

void kt0803_set_au_enhancement(bit enable)
{
  kt0803_set_reg(0x17, 5, 1, enable ? 1 : 0);
}

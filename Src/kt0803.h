#ifndef __FM_TRANS_KT0803_H__
#define __FM_TRANS_KT0803_H__

#include <stdint.h>
/*
  SM_TRANS_ALC,       // Automatic Level Control
  SM_TRANS_SD,        // Silence Detection
  SM_TRANS_RF_GAIN,   // RF Gain
  SM_TRANS_PTA,       // Pilot Tone Amplitude
  SM_TRANS_PTC,       // Pre-emphasis Time-Constant
  SM_TRANS_STEREO,    // Stereo or Mono
  SM_TRANS_BASS,      // Bass Boost Control
  SM_TRANS_FDD,       // Frequency Deviation Delection
  SM_TRANS_AFRE,      // Audio Frequency Response Enhancement
  SM_TRANS_SCM,       // Switching Channel Mode Selection
  SM_TRANS_PA         // Power Amplifier
*/

#define KT0803_CFG_FLAG_ALC_ENABLE  0x1
#define KT0803_CFG_FLAG_PDPA        0x2  // Power Amplifier
#define KT0803_CFG_FLAG_PADN        0x4
#define KT0803_CFG_FLAG_PA_BIAS     0x8
#define KT0803_CFG_FLAG_SLNCDIS     0x10
#define KT0803_CFG_FLAG_PLTADJ      0x20  // PTA
#define KT0803_CFG_FLAG_PHTCNST     0x40  // PTC
#define KT0803_CFG_FLAG_STEREO      0x80  // Stereo or Mono
#define KT0803_CFG_FLAG_FDD         0x100 // Frequency Deviation Delection
#define KT0803_CFG_FLAG_AFRE        0x200 // Audio Frequency Response Enhancement
#define KT0803_CFG_FLAG_SCM         0x300 // Switching Channel Mode Selection
#define KT0803_CFG_FLAG_MUTE        0x80  // Mute

typedef struct _kt0803_cfg_t
{
  uint16_t flag;
  uint16_t freq;        // Channel
  uint8_t  alc_delay;   // ALC
  uint8_t  alc_attack;
  uint8_t  alc_comp_gain;  
  uint8_t  alc_hold; 
  uint8_t  alc_high_th;
  uint8_t  alc_low_th; 
  uint8_t  slncthl;     // SD
  uint8_t  slncthh;
  uint8_t  slnccnt_high;
  uint8_t  slnccnt_low;
  uint8_t  rf_gain;     // RF Gain
  int8_t   pga_gain;    // PGA (Volume)
  uint8_t  bass;        // Bass Boost Control
}kt0803_cfg_t;

typedef enum _kt0803_rf_gain_t
{
  KT0803_RF_GAIN_955 = 0,
  KT0803_RF_GAIN_965,
  KT0803_RF_GAIN_975,
  KT0803_RF_GAIN_982,
  KT0803_RF_GAIN_989,  
  KT0803_RF_GAIN_1000, 
  KT0803_RF_GAIN_1015,
  KT0803_RF_GAIN_1028,
  KT0803_RF_GAIN_1051,
  KT0803_RF_GAIN_1056,
  KT0803_RF_GAIN_1062, 
  KT0803_RF_GAIN_1065,
  KT0803_RF_GAIN_1070,
  KT0803_RF_GAIN_1074,
  KT0803_RF_GAIN_1077,
  KT0803_RF_GAIN_1080
}kt0803_rf_gain_t;

typedef enum _kt0803_pltadj_t
{
  KT0803_PLTADJ_LOW = 0,
  KT0803_PLTADJ_HIGH
}kt0803_pltadj_t;

typedef enum _kt0803_phtcnst_t
{
  KT0803_PHTCNST_75US = 0,
  KT0803_PHTCNST_50US
}kt0803_phtcnst_t;

typedef enum _kt0803_bass_t
{
  KT0803_BASS_DISABLE = 0,
  KT0803_BASS_5DB,
  KT0803_BASS_11DB,
  KT0803_BASS_17DB, 
}kt0803_bass_t;

typedef enum _kt0803_pga_mod_t
{
  KT0803_PGA_MOD_4DB_STEP = 0,
  KT0803_PGA_MOD_1DB_STEP
}kt0803_pga_mod_t;

typedef enum _kt0803_alc_delay_t
{
  KT0803_ALC_DELAY_25US = 0,
  KT0803_ALC_DELAY_50US, 
  KT0803_ALC_DELAY_75US, 
  KT0803_ALC_DELAY_100US,
  KT0803_ALC_DELAY_125US,
  KT0803_ALC_DELAY_150US,
  KT0803_ALC_DELAY_175US,
  KT0803_ALC_DELAY_200US,
  KT0803_ALC_DELAY_50MS,
  KT0803_ALC_DELAY_100MS,
  KT0803_ALC_DELAY_150MS,
  KT0803_ALC_DELAY_200MS,
  KT0803_ALC_DELAY_250MS,
  KT0803_ALC_DELAY_300MS,
  KT0803_ALC_DELAY_350MS,
  KT0803_ALC_DELAY_400MS,  
}kt0803_alc_delay_t;

typedef enum _kt0803_alc_attack_t
{
  KT0803_ALC_ATTACK_25US = 0,
  KT0803_ALC_ATTACK_50US, 
  KT0803_ALC_ATTACK_75US, 
  KT0803_ALC_ATTACK_100US,
  KT0803_ALC_ATTACK_125US,
  KT0803_ALC_ATTACK_150US,
  KT0803_ALC_ATTACK_175US,
  KT0803_ALC_ATTACK_200US,
  KT0803_ALC_ATTACK_50MS,
  KT0803_ALC_ATTACK_100MS,
  KT0803_ALC_ATTACK_150MS,
  KT0803_ALC_ATTACK_200MS,
  KT0803_ALC_ATTACK_250MS,
  KT0803_ALC_ATTACK_300MS,
  KT0803_ALC_ATTACK_350MS,
  KT0803_ALC_ATTACK_400MS,  
}kt0803_alc_attack_t;

typedef enum _kt0803_alc_comp_gain_t
{
  KT0803_ALC_COMP_GAIN_N6DB = 0,
  KT0803_ALC_COMP_GAIN_N9DB,
  KT0803_ALC_COMP_GAIN_N12DB,
  KT0803_ALC_COMP_GAIN_N15DB, 
  KT0803_ALC_COMP_GAIN_6DB,
  KT0803_ALC_COMP_GAIN_3DB,
  KT0803_ALC_COMP_GAIN_0DB,
  KT0803_ALC_COMP_GAIN_N3DB, 
}kt0803_alc_comp_gain_t;

typedef enum _kt0803_alc_hold_t
{
  KT0803_ALC_HOLD_50MS = 0,
  KT0803_ALC_HOLD_100MS,
  KT0803_ALC_HOLD_150MS,
  KT0803_ALC_HOLD_200MS,
  KT0803_ALC_HOLD_1S,
  KT0803_ALC_HOLD_5S,
  KT0803_ALC_HOLD_10S,
  KT0803_ALC_HOLD_15S,  
}kt0803_alc_hold_t;

typedef enum _kt0803_alc_high_th_t
{
  KT0803_ALC_HIGH_TH_0P6 = 0,
  KT0803_ALC_HIGH_TH_0P5,
  KT0803_ALC_HIGH_TH_0P4,
  KT0803_ALC_HIGH_TH_0P3,
  KT0803_ALC_HIGH_TH_0P2,
  KT0803_ALC_HIGH_TH_0P1,
  KT0803_ALC_HIGH_TH_0P05,
  KT0803_ALC_HIGH_TH_0P01,  
}kt0803_alc_high_th_t;

typedef enum _kt0803_alc_low_th_t
{
  KT0803_ALC_LOW_TH_0P25 = 0, 
  KT0803_ALC_LOW_TH_0P2,
  KT0803_ALC_LOW_TH_0P15,
  KT0803_ALC_LOW_TH_0P1,
  KT0803_ALC_LOW_TH_0P05,
  KT0803_ALC_LOW_TH_0P03,
  KT0803_ALC_LOW_TH_0P02,
  KT0803_ALC_LOW_TH_0P01,
  KT0803_ALC_LOW_TH_0P005,
  KT0803_ALC_LOW_TH_0P001,
  KT0803_ALC_LOW_TH_0P0005,
  KT0803_ALC_LOW_TH_0P0001,  
}kt0803_alc_low_th_t;

typedef enum _kt0803_slncthl_t
{
  KT0803_SLNCTHL_025MV = 0,
  KT0803_SLNCTHL_050MV,
  KT0803_SLNCTHL_1MV, 
  KT0803_SLNCTHL_2MV,
  KT0803_SLNCTHL_4MV,
  KT0803_SLNCTHL_6MV,
  KT0803_SLNCTHL_8MV,
  KT0803_SLNCTHL_16MV,
  KT0803_SLNCTHL_32MV,  
}kt0803_slncthl_t;

typedef enum _kt0803_slncthh_t
{
  KT0803_SLNCTHH_050MV = 0,
  KT0803_SLNCTHH_1MV, 
  KT0803_SLNCTHH_2MV,
  KT0803_SLNCTHH_4MV,
  KT0803_SLNCTHH_6MV,
  KT0803_SLNCTHH_8MV,
  KT0803_SLNCTHH_16MV,
  KT0803_SLNCTHH_32MV, 
  KT0803_SLNCTHH_64MV,   
}kt0803_slncthh_t;

typedef enum _kt0803_slnccnt_high_t
{
  KT0803_SLNCCNT_HIGH_15 = 0,
  KT0803_SLNCCNT_HIGH_31,
  KT0803_SLNCCNT_HIGH_63,
  KT0803_SLNCCNT_HIGH_127,
  KT0803_SLNCCNT_HIGH_255, 
  KT0803_SLNCCNT_HIGH_511,
  KT0803_SLNCCNT_HIGH_1023,
  KT0803_SLNCCNT_HIGH_2047,  
}kt0803_slnccnt_high_t;

typedef enum _kt0803_slnccnt_low_t
{
  KT0803_SLNCCNT_LOW_1,
  KT0803_SLNCCNT_LOW_2,
  KT0803_SLNCCNT_LOW_4,
  KT0803_SLNCCNT_LOW_8, 
  KT0803_SLNCCNT_LOW_16,
  KT0803_SLNCCNT_LOW_32,
  KT0803_SLNCCNT_LOW_64,
  KT0803_SLNCCNT_LOW_128, 
}kt0803_slnccnt_low_t;

typedef enum _kt0803_swch_mod_t
{
  KT0803_SWCH_MOD_MUTE = 0,
  KT0803_SWCH_MOD_PA
}kt0803_swch_mod_t;

typedef enum _kt0803_pa_ctl_t
{
  KT0803_PA_CTL_INTERNAL = 0,
  KT0803_PA_CTL_EXTERNAL
}kt0803_pa_ctl_t;

typedef enum _kt0803_ref_clk_t
{
  KT0803_REF_CLK_32768K = 0,
  KT0803_REF_CLK_6P5M,
  KT0803_REF_CLK_7P6M,
  KT0803_REF_CLK_12M, 
  KT0803_REF_CLK_13M,
  KT0803_REF_CLK_15P2M,
  KT0803_REF_CLK_19P2M,
  KT0803_REF_CLK_24M,
  KT0803_REF_CLK_26M,  
}kt0803_ref_clk_t;

typedef enum _kt0803_fedv_t
{
  KT0803_FEDV_75KHZ = 0,
  KT0803_FEDV_112P5KHZ
}kt0803_fedv_t;

extern kt0803_cfg_t kt0803_cfg;

void kt0803_initialize(void);
void kt0803_factory_reset(void);


/* Channel selection */
uint16_t kt0803_get_ch(void);
void kt0803_set_ch(uint16_t ch);
kt0803_swch_mod_t kt0803_get_swch_mod(void);
void kt0803_set_swch_mod(kt0803_swch_mod_t mod);

/* RF Gain setting */
kt0803_rf_gain_t kt0803_get_rf_gain(void);
void kt0803_set_rf_gain(kt0803_rf_gain_t gain);

/* Software Mute */
bit kt0803_get_mute(void);
void kt0803_set_mute(bit enable);

/* Pilot Tone Amplitude Adjustment */
kt0803_pltadj_t kt0803_get_pltadj(void);
void kt0803_set_pltadj(kt0803_pltadj_t adj);

/* Pre-emphasis Time-Constant Set */
kt0803_phtcnst_t kt0803_get_phtcnst(void);
void kt0803_set_phtcnst(kt0803_phtcnst_t cnst);

/* Automatic Level Control */
bit kt0803_get_alc_enable(void);
void kt0803_set_alc_enable(bit enable);
kt0803_alc_delay_t kt0803_get_alc_delay(void);
void kt0803_set_alc_delay(kt0803_alc_delay_t delay);
kt0803_alc_attack_t kt0803_get_alc_attack(void);
void kt0803_set_alc_attack(kt0803_alc_attack_t attack);
kt0803_alc_comp_gain_t kt0803_get_alc_comp_gain(void);
void kt0803_set_alc_comp_gain(kt0803_alc_comp_gain_t gain);
kt0803_alc_hold_t kt0803_get_alc_hold(void);
void kt0803_set_alc_hold(kt0803_alc_hold_t hold);
kt0803_alc_high_th_t kt0803_get_alc_high_th(void);
void kt0803_set_alc_high_th(kt0803_alc_high_th_t th);
kt0803_alc_low_th_t kt0803_get_alc_low_th(void);
void kt0803_set_alc_low_th(kt0803_alc_low_th_t th);

/* Stereo or Mono */
bit kt0803_get_mono(void);
void kt0803_set_mono(bit enable);

/* PGA */
int8_t kt0803_get_pga_gain(void);
void kt0803_set_pga_gain(int8_t gain);
kt0803_pga_mod_t kt0803_get_pga_mod(void);
void kt0803_set_pga_mod(kt0803_pga_mod_t mod);

/* Bass Boost Control */
kt0803_bass_t kt0803_get_bass(void);
void kt0803_set_bass(kt0803_bass_t bass);

/* Stand by */
bit kt0803_get_standby(void);
void kt0803_set_standby(bit enable);

/* Power amplifier */
kt0803_pa_ctl_t kt0803_get_pa_ctl(void);
void kt0803_set_pa_ctl(kt0803_pa_ctl_t ctl);
bit kt0803_get_pdpa(void);
void kt0803_set_pdpa(bit down);
bit kt0803_get_padn(void);
void kt0803_set_padn(bit enable);
bit kt0803_get_pa_bias(void);
void kt0803_set_pa_bias(bit enable);

/* Status */
bit kt0803_get_pw_ok(void);
bit kt0803_get_slncid(void);

/* Silence Detection */
bit kt0803_get_slncdis(void);
void kt0803_set_slncdis(bit disable);
kt0803_slncthl_t kt0803_get_slncthl(void);
void kt0803_set_slncthl(kt0803_slncthl_t th);
kt0803_slncthh_t kt0803_get_slncthh(void);
void kt0803_set_slncthh(kt0803_slncthh_t th);
kt0803_slnccnt_high_t kt0803_get_slnccnt_high(void);
void kt0803_set_slnccnt_high(kt0803_slnccnt_high_t th);
kt0803_slnccnt_low_t kt0803_get_slnccnt_low(void);
void kt0803_set_slnccnt_low(kt0803_slnccnt_low_t th);

/* Reference Clock */
bit kt0803_get_dclk(void);
void kt0803_set_dclk(bit enable);
bit kt0803_get_xtald(void);
void kt0803_set_xtald(bit enable);
kt0803_ref_clk_t kt0803_get_ref_clk(void);
void kt0803_set_ref_clk(kt0803_ref_clk_t clk);

/* Frequency Deviation Delection */
kt0803_fedv_t kt0803_get_fedv(void);
void kt0803_set_fedv(kt0803_fedv_t fedv);

/* Audio Frequency Response Enhancement */
bit kt0803_get_au_enhancement(void);
void kt0803_set_au_enhancement(bit enable);

/* Switching Channel Mode Selection. */
kt0803_swch_mod_t kt0803_get_swch_mod(void);
void kt0803_set_swch_mod(kt0803_swch_mod_t mod);

//-----------------------------------------------
uint16_t kt0803_next_ch(bit coarse);
uint16_t kt0803_prev_ch(bit coarse);


uint8_t kt0803_get_vol(void);
uint8_t kt0803_next_vol(bit coarse);
uint8_t kt0803_prev_vol(bit coarse);

#endif
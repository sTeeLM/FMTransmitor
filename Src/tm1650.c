#include "tm1650.h"
#include "i2c.h"
#include "debug.h"

tm1650_cfg_t tm1650_cfg;
static uint8_t tm1650_data[4];

static uint8_t tm1650_display_mode;
/*        A
        F   B
          G
        E   C  
          D
*/

static unsigned char code tm1650_display_code[] =
{
        /* GFEDCBA */
   0x58, //1011000  o  degree Celsius
   0x40, //1000000  -  ascii: 0x2D
   0x00, //         .
   0x00, //         /
   0x3F, //0111111  0    
   0x06, //0000110  1
   0x5B, //1011011  2
   0x4F, //1001111  3
   0x66, //1100110  4
   0x6D, //1101101  5
   0x7D, //1111101  6
   0x07, //0000111  7
   0x7F, //1111111  8
   0x6F, //1101111  9
   0x00, //         :
   0x00, //         ;
   0x58, //1011000  <
   0x48, //1001000  =
   0x4C, //1001100  >
   0x00, //         ?
   0x00, //         @  
   0x77, //1110111  A
   0x7C, //1111100  B
   0x39, //0111001  C
   0x5E, //1011110  D
   0x79, //1111001  E
   0x71, //1110001  F
   0x6F, //1101111  G == 9
   0x76, //1110110  H
   0x30, //0110000  I
   0x0E, //0001110  J 
   0x00, //         K
   0x38, //0111000  L
   0x36, //0110110  M
   0x37, //0110111  N
   0x5C, //1011100  O
   0x73, //1110011  P
   0x00, //         Q
   0x50, //1010000  R
   0x6D, //1101101  S
   0x78, //1111000  T
   0x3E, //0111110  U
   0x00, //         V
   0x00, //         W
   0x00, //         X
   0x72, //1110010  Y
   0x00, //         Z ascii 0x5A
};

void tm1650_factory_reset(void)
{
  tm1650_cfg.brightness = 0;
}

void tm1650_initialize(void)
{
  tm1650_display_mode = 0;
  tm1650_set_mod(TM1650_MOD_8_DIG);
  tm1650_set_brightness(7);
  tm1650_enable_display(1);
  tm1650_clear();  
}

void tm1650_clear(void)
{
  tm1650_set_dig(0, 0, ' ');
  tm1650_set_dig(1, 0, ' '); 
  tm1650_set_dig(2, 0, ' ');
  tm1650_set_dig(3, 0, ' '); 
}

void tm1650_set_str(const char * str)
{
  tm1650_set_dig(3, 0, str[0]);
  tm1650_set_dig(2, 0, str[1]); 
  tm1650_set_dig(1, 0, str[2]);
  tm1650_set_dig(0, 0, str[3]); 
}

static void tm1650_set_data(uint8_t index, uint8_t dat);
static void tm1650_refresh(void);

void tm1650_clr_dp(void)
{
  tm1650_data[0] &= 0x7F;
  tm1650_data[1] &= 0x7F; 
  tm1650_data[2] &= 0x7F;
  tm1650_data[3] &= 0x7F; 
  tm1650_refresh(); 
}

static void tm1650_refresh(void)
{
  tm1650_set_data(0, tm1650_data[0]);
  tm1650_set_data(1, tm1650_data[1]); 
  tm1650_set_data(2, tm1650_data[2]);
  tm1650_set_data(3, tm1650_data[3]); 
}

void tm1650_set_dp(uint8_t dp)
{
  dp %= 4;
  tm1650_data[dp] |= 0x80; 
  
  tm1650_set_data(3 - dp, tm1650_data[dp]);
}

void tm1650_set_mod(tm1650_mod_t mod)
{
  tm1650_display_mode &= 0xF7;
  
  if(mod == TM1650_MOD_7_DIG) {
    tm1650_display_mode |= ~0xF7;
  }
}

tm1650_mod_t tm1650_get_mod(void)
{
  return (tm1650_display_mode & (~0xF7)) ? TM1650_MOD_7_DIG : TM1650_MOD_8_DIG;
}

void tm1650_set_brightness(uint8_t brightness)
{
  brightness &= 0x7;
  tm1650_display_mode &= 0x8F;
  tm1650_display_mode |= (brightness << 4);
  tm1650_refresh();
}

uint8_t tm1650_get_brightness(void)
{
  return ((tm1650_display_mode & (~0x8F)) >> 4);
}

uint8_t tm1650_next_brightness(void)
{
  uint8_t br = tm1650_get_brightness();
  br = ( ++ br) % 8;
  tm1650_set_brightness(br);
  return br;
  
}

uint8_t tm1650_prev_brightness(void)
{
  uint8_t br = tm1650_get_brightness();
  if(br == 0)
    br = 7;
  else
    br --;
  tm1650_set_brightness(br);
  return br;
}

void tm1650_enable_display(bit enable)
{
  tm1650_display_mode &= 0xFE;
  if(enable)
    tm1650_display_mode |= 1;
}

static void tm1650_set_data(uint8_t index, uint8_t dat)
{
  uint8_t addr = 0x68;
  addr += (3 - index) * 2;
  
  I2C_Init();
  I2C_Start();
  
  I2C_Write(0x48);
  if(I2C_GetAck()) {
    goto failed;
  }

  I2C_Write(tm1650_display_mode);
  if(I2C_GetAck()) {
    goto failed;
  }
  
  I2C_Start();
  
  I2C_Write(addr);
  if(I2C_GetAck()) {
    goto failed;
  }
  
  I2C_Write(dat);
  if(I2C_GetAck()) {
    goto failed;
  }
  
failed: 
  I2C_Stop();  
}

void tm1650_set_dig(uint8_t index, bit dp, uint8_t dat)
{
  uint8_t val = 0;
  index %= 4;
  

  if(dat == 0) {
    val = tm1650_display_code[0];
  } else if(dat == ' ') {
    dat = 0;
  } else if(dat >= '-' && dat <= 'Z' ) {
    val = tm1650_display_code[dat - '-' + 1];
  }
  
  if(dp) {
    val |= 0x80;
  }
  
  tm1650_data[index] = val;
  
  //index = 3 - index;
  /* write to chip! */
  tm1650_set_data(index, val);
}

uint8_t tm1650_get_scan_code(void)
{
  uint8_t ret = 0;
  
  I2C_Init();
  I2C_Start();
  
  I2C_Write(0x49);
  if(I2C_GetAck()) {
//    CDBG("tm1650_get_scan_code failed\n");
    goto failed;
  }  
  
  ret = I2C_Read();
  I2C_PutAck(0);
  
failed: 
  I2C_Stop();
  /* 没有按键按下的时候有奇怪的读出！ */
  if(ret < 0x44 || ret > 0x74)
    ret = 0;
  return ret;
}
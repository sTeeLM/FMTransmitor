#include "gpio.h"
#include "debug.h"

 
/*  
  SERIAL_RXD SERIAL_TXD 准双向口
  I2C_SDA I2C_SCL 准双向口 
  POWER_EN  推挽输出
  POWER_KEY 高阻输入
*/
void gpio_initialize(void)
{
  P0M0 = 0x00;
  P0M1 = 0xFF;
  P1M0 = 0x00;
  P1M1 = 0xFF;
  P2M0 = 0x00;
  P2M1 = 0xFF;
  P3M0 = 0x00;
  P3M1 = 0xF0;
  P4M0 = 0x00;
  P4M1 = 0xFF;
  P5M0 = 0x10;
  P5M1 = 0xEF;
  P_SW1 = 0x00;
  
  
  I2C_SDA     = 1;
  I2C_SCL     = 1;
  POWER_EN    = 0;
  POWER_KEY   = 1;  
}



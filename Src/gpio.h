#ifndef __FM_TRANS_GPIO_H__
#define __FM_TRANS_GPIO_H__

#include <STC8G.H>
/* com */
// P3.0 as RxD
// P3.1 as TxD
sbit SERIAL_RXD    = P3 ^ 0;
sbit SERIAL_TXD    = P3 ^ 1;

/* i2c */
sbit I2C_SDA       = P3 ^ 2;
sbit I2C_SCL       = P3 ^ 3;

/* power en */
sbit POWER_EN      = P5 ^ 4;

/* power key */
sbit POWER_KEY     = P5 ^ 5;

void gpio_initialize (void);

#endif

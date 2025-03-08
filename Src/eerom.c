#include <STC8G.H>
#include <intrins.h>
#include "eerom.h"
#include "debug.h"
#include "kt0803.h"
#include "tm1650.h"
#include "delay.h"
#include "button.h"

static void eerom_iap_idle()
{
  IAP_CONTR = 0;    //关闭IAP 功能
  IAP_CMD = 0;      //清除命令寄存器
  IAP_TRIG = 0;     //清除触发寄存器
  IAP_ADDRH = 0x80; //将地址设置到非IAP 区域
  IAP_ADDRL = 0;
}

static uint8_t eerom_iap_read(uint16_t addr)
{
  uint8_t dat;
  IAP_CONTR = 0x80;      //使能IAP
  IAP_TPS = 12;          //设置等待参数12MHz
  IAP_CMD = 1;           //设置IAP 读命令
  IAP_ADDRL = addr;      //设置IAP 低地址
  IAP_ADDRH = addr >> 8; //设置IAP 高地址
  IAP_TRIG = 0x5a;       //写触发命令(0x5a)
  IAP_TRIG = 0xa5;       //写触发命令(0xa5)
  _nop_();
  dat = IAP_DATA;        //读IAP 数据
  eerom_iap_idle();      //关闭IAP 功能
  return dat;
}

static void eerom_iap_write(uint16_t addr, uint8_t dat)
{
  IAP_CONTR = 0x80;      //使能IAP
  IAP_TPS = 12;          //设置等待参数12MHz
  IAP_CMD = 2;           //设置IAP 写命令
  IAP_ADDRL = addr;      //设置IAP 低地址
  IAP_ADDRH = addr >> 8; //设置IAP 高地址
  IAP_DATA = dat;        //写IAP 数据
  IAP_TRIG = 0x5a;       //写触发命令(0x5a)
  IAP_TRIG = 0xa5;       //写触发命令(0xa5)
  _nop_();
  eerom_iap_idle(); //关闭IAP 功能
}

static void eerom_iap_erase(uint16_t addr)
{
  IAP_CONTR = 0x80;      //使能IAP
  IAP_TPS = 12;          //设置等待参数12MHz
  IAP_CMD = 3;           //设置IAP 擦除命令
  IAP_ADDRL = addr;      //设置IAP 低地址
  IAP_ADDRH = addr >> 8; //设置IAP 高地址
  IAP_TRIG = 0x5a;       //写触发命令(0x5a)
  IAP_TRIG = 0xa5;       //写触发命令(0xa5)
  _nop_(); //
  eerom_iap_idle();      //关闭IAP 功能
}

static bit eerom_is_factory_reset(void)
{
  uint8_t key_code;
  key_code = tm1650_get_scan_code();
  return key_code == BUTTON_NEG_KEY_CODE; // -
}

void eerom_initialize (void)
{
  if(eerom_is_factory_reset()) {
    CDBG("eerom_initialize : FACTORY_RESET!!!\n");
    kt0803_factory_reset();
    tm1650_factory_reset();
    eerom_save_config();
  }
  
  eerom_load_config();
}

void eerom_load_config(void)
{
  uint16_t len;
  uint16_t addr = 0x0;
  uint8_t * p = NULL;
  
  p = (uint8_t *)&kt0803_cfg;
  for(len = 0 ; len < sizeof(kt0803_cfg); len ++) {
    *p = eerom_iap_read(addr);
    p++;
    addr ++;
  }
  
  p = (uint8_t *)&tm1650_cfg;
  for(len = 0 ; len < sizeof(tm1650_cfg); len ++) {
    *p = eerom_iap_read(addr);
    p++;
    addr ++;
  }  
}

void eerom_save_config(void)
{
  uint16_t len;
  uint16_t addr = 0x0;
  uint8_t * p = NULL;
  
  eerom_iap_erase(addr); // erase first!
  
  p = (uint8_t *)&kt0803_cfg;
  for(len = 0 ; len < sizeof(kt0803_cfg); len ++) {
    eerom_iap_write(addr, *p);
    p++;
    addr ++;
  }
  
  p = (uint8_t *)&tm1650_cfg;
  for(len = 0 ; len < sizeof(tm1650_cfg); len ++) {
    eerom_iap_write(addr, *p);
    p++;
    addr ++;
  }
}
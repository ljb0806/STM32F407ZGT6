#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f4xx.h"

void Delay_Init(void);				// 初始化延时函数
void Delay_ms(uint32_t nTime);	    // 延时ms函数
void Delay_us(uint32_t nTime);	    // 延时us函数
void Delay_s(uint32_t nTime);	    // 延时s函数

#endif


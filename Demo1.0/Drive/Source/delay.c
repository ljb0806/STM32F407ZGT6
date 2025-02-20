/**
*****************************************************************************
*
*  @file    delay.c
*  @brief   利用系统时钟中断实现延时函数
*			
*  @author  binbin
*  @date    2025/2/18
*  @version 1.0
*  
*****************************************************************************
**/

#include "delay.h"

static __IO uint32_t TimingDelay;  // 延时变量

// 初始化延时中断1s对应168M个时钟，那1us对应168个时钟
void Delay_Init(void)
{
	SysTick_Config(SystemCoreClock / 1000000);  //配置SysTick定时器，1us中断一次
}

// 在定时器中断中调用此函数，每1us计数值-1
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0)
	{ 
		TimingDelay--;
	}
}

// 延时us函数
void Delay_us(uint32_t nTime)
{ 
	TimingDelay = nTime;

	while(TimingDelay != 0);
}

// 延时ms函数
void Delay_ms(uint32_t nTime)
{
	TimingDelay = nTime * 1000;

	while(TimingDelay != 0);
}

// 延时s函数
void Delay_s(uint32_t nTime)
{
	TimingDelay = nTime * 1000000;

	while(TimingDelay != 0);
}

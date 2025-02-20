#include "stm32f4xx.h"

/* TIM2定时器，用于给ADC做外部触发 */
void TIM2_Init(uint32_t sampling_frequency)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;                           // 定时器初始化结构体

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);                     // 使能TIM2时钟

    TIM_TimeBaseStructure.TIM_Period = (84000000 / sampling_frequency) - 1;  // 计数器自动重装载值
    TIM_TimeBaseStructure.TIM_Prescaler = 0;                                 // 时钟预分频数
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;                             // 时钟分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;              // 向上计数模式
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);                          // 初始化TIM2

    TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);                    // 选择触发源

    TIM_Cmd(TIM2, ENABLE);                                                   // 使能TIM2
}

/* TIM4定时器，用于给DAC1做外部触发 */
void TIM4_Init(uint16_t arr, uint16_t psc){
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE); 			//TIM4 时钟使能
	                                                                
	TIM_InternalClockConfig(TIM4);	                                
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;	            //时基结构体
	TIM_TimeBaseInitStructure.TIM_Period = arr; 					//设置自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler =psc; 					//设置预分频值
	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0; 				//设置时钟分割
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
	                                                                
	TIM_SelectOutputTrigger(TIM4,TIM_TRGOSource_Update);			//更新溢出向外触发
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure); 			//时基初始化
	TIM_Cmd(TIM4, ENABLE);                                          //定时器使能
}

/* TIM5定时器，用于给DAC2做外部触发 */
void TIM5_Init(uint16_t arr, uint16_t psc){
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE); 			//TIM5 时钟使能
	                                                                
	TIM_InternalClockConfig(TIM5);                                  
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;				//时基结构体
	TIM_TimeBaseInitStructure.TIM_Period = arr;                     //设置自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler =psc;                   //设置预分频值
	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;                //设置时钟分割
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
	                                                                
	TIM_SelectOutputTrigger(TIM5,TIM_TRGOSource_Update);            //更新溢出向外触发
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStructure);             //时基初始化
	TIM_Cmd(TIM5, ENABLE);                                          //定时器使能
}


/**
*****************************************************************************
*
*  @file    ADC.c
*  @brief   初始化了ADC1的通道0(PA0)  可配置DMA
*			
*  @author  binbin
*  @date    2025/2/18
*  @version 1.0
*  
*****************************************************************************
**/

#include "stm32f4xx.h"
#include "ADC.h"

uint16_t ADC1_buffer[ADC_BUFFER_SIZE];  // DMA缓冲区
uint16_t DMA_completed;                 // DMA完成标志

/* 无DMA的ADC1初始化，以PA0为输入口 */
void ADC1_Init_WithoutDMA(void)
{
    ADC_InitTypeDef ADC_InitStructure;                      // ADC初始化结构体
    GPIO_InitTypeDef GPIO_InitStructure;                    // GPIO初始化结构体

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);   // 使能GPIOA时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);    // 使能ADC1时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;               // PA0是ADC1的通道0
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;            // 模拟输入模式
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;        // 不上下拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);                  // 初始化GPIOA

    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;                  // 12位模式
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;                           // 禁止扫描模式，因为是单通道
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                     // 禁止连续转换，使用手动转换，调用Get函数时才转换
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConvEdge_None; // 禁止外部触发转换
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                  // 数据右对齐
    ADC_InitStructure.ADC_NbrOfConversion = 1;                              // 转换通道数目为1                    
    ADC_Init(ADC1, &ADC_InitStructure);                                     // 初始化ADC1

    ADC_Cmd(ADC1, ENABLE);                                                       // 使能ADC1
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_84Cycles);   // 配置ADC1的通道0，采样时间为84个周期
}

/* 手动采样的数字信号，配合无DMA的初始化使用 */
uint16_t ADC1_GetDigitalValue(void)
{
    ADC_SoftwareStartConv(ADC1);                             // 开始转换
    while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);   // 等待转换结束
    return ADC_GetConversionValue(ADC1);                     // 返回转换结果
}

/* 手动采样的模拟信号，配合有DMA的初始化使用 */
float ADC1_GetAnalogValue(void)
{
	return ADC1_GetDigitalValue() * 3.3 / 4096;              // 返回模拟值
}

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

/* 有DMA的ADC1初始化，以PA0为输入口 */
void ADC1_Init_WithDMA(uint32_t sampling_frequency)
{
    /* 初始化TIM2作为外部触发源 */
    TIM2_Init(sampling_frequency);

    /* ADC1通道0 即PA0的配置 */
    ADC_InitTypeDef ADC_InitStructure;                      // ADC初始化结构体
    GPIO_InitTypeDef GPIO_InitStructure;                    // GPIO初始化结构体

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);   // 使能GPIOA时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);    // 使能ADC1时钟
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;               // PA0是ADC1的通道0
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;            // 模拟输入模式
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;        // 不上下拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);                  // 初始化GPIOA

    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;                  // 12位模式
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;                           // 禁止扫描模式，因为是单通道
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                     // 禁止连续转换，使用手动转换，调用Get函数时才转换
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_TRGO;  // 使用TIM2触发转换
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising; // 上升沿触发
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                  // 数据右对齐
    ADC_InitStructure.ADC_NbrOfConversion = 1;                              // 转换通道数目为1
    ADC_Init(ADC1, &ADC_InitStructure);                                     // 初始化ADC1

    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_84Cycles);   // 配置ADC1的通道0，采样时间为84个周期

    ADC_Cmd(ADC1, ENABLE);                                                       // 使能ADC1
    
    /* DMA的配置 */
    DMA_InitTypeDef DMA_InitStructure;                      // DMA初始化结构体

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);    // 使能DMA2时钟

    DMA_InitStructure.DMA_Channel = DMA_Channel_0;                   // DMA通道0
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;  // 外设地址
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)ADC1_buffer;   // 存储器地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;          // 外设到存储器模式
    DMA_InitStructure.DMA_BufferSize = ADC_BUFFER_SIZE;              // 缓冲区大小
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; // 外设地址不增加
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;          // 存储器地址增加
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // 外设数据大小为半字，16bit
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;         // 存储器数据大小为半字，16bit
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                               // 普通模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;                         // 高优先级
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;                      // 禁止FIFO
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;                 // 单次传输
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;         // 单次传输

    DMA_Init(DMA2_Stream0, &DMA_InitStructure);                                 // 初始化DMA2_Stream0
    DMA_Cmd(DMA2_Stream0, ENABLE);                                              // 使能DMA2_Stream0
    DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);                              // 使能DMA2_Stream0传输完成中断

    /* 配置DMA采样完成的中断 */
    NVIC_InitTypeDef NVIC_InitStructure;                                        // NVIC初始化结构体
    NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;                     // DMA2_Stream0中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;                   // 抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                          // 子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                             // 使能中断
    NVIC_Init(&NVIC_InitStructure);                                             // 初始化NVIC

    /* 配置ADC1使用DMA */
    ADC_DMACmd(ADC1, ENABLE);
}

/* DMA采样完成的中断 */
void DMA2_Stream0_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0) != RESET)
    {
        DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);

        ADC_Cmd(ADC1, DISABLE);
        DMA_Cmd(DMA2_Stream0, DISABLE);

        DMA_completed = 1;
    }
}

/* 手动开始一次DMA采样 */
void Start_ADC1_DMA(void)
{
    DMA_completed = 0;

    DMA_SetCurrDataCounter(DMA2_Stream0, ADC_BUFFER_SIZE);

    ADC_Cmd(ADC1, ENABLE);
    DMA_Cmd(DMA2_Stream0, ENABLE);
}




#include "stm32f4xx.h"
#include "Timer.h"
#include "DAC.h"

uint16_t DAC1_Value[DAC1_Value_Length] = {
2048,
2310,
2567,
2816,
3052,
3272,
3472,
3648,
3798,
3920,
4010,
4069,
4094,
4086,
4044,
3969,
3863,
3727,
3563,
3375,
3165,
2936,
2693,
2439,
2179,
1917,
1657,
1403,
1160,
931,
721,
533,
369,
233,
127,
52,
10,
2,
27,
86,
176,
298,
448,
624,
824,
1044,
1280,
1529,
1786,
2048,

    };

uint16_t DAC2_Value[DAC2_Value_Length] = {
	
4095,
4078,
4028,
3945,
3832,
3688,
3518,
3324,
3109,
2877,
2630,
2375,
2114,
1851,
1592,
1341,
1101,
877,
672,
489,
332,
204,
105,
39,
5,
5,
39,
105,
204,
332,
489,
672,
877,
1101,
1341,
1592,
1851,
2114,
2375,
2630,
2877,
3109,
3324,
3518,
3688,
3832,
3945,
4028,
4078,
4095,

    
    };
	
void DAC1_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    DAC_InitTypeDef DAC_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    DAC_InitStructure.DAC_Trigger = DAC_Trigger_T4_TRGO;
    DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
    DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
    DAC_Init(DAC_Channel_1, &DAC_InitStructure);

    DAC_SetChannel1Data(DAC_Align_12b_R, 0);

    DMA_InitStructure.DMA_Channel = DMA_Channel_7;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_BufferSize = DAC1_Value_Length;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;

    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;

    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)DAC1_Value;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;

    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&DAC->DHR12R1;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

    DMA_Init(DMA1_Stream5, &DMA_InitStructure);

    DMA_Cmd(DMA1_Stream5, ENABLE);
    DAC_Cmd(DAC_Channel_1, ENABLE);
    DAC_DMACmd(DAC_Channel_1, ENABLE);
}

void DAC2_Init(){
	//结构体定义
	GPIO_InitTypeDef  GPIO_InitStructure;	//GPIO结构体
	DAC_InitTypeDef DAC_InitStructure;		//DAC结构体
	DMA_InitTypeDef DMA_InitStructure;      //DMA结构体

	//时钟使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	//使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);     //使能DAC时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);     //使能DMA1时钟
	
	//GPIO配置初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				//PIN5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;            //模拟模式		    
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;        //浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);                  //GPIO结构体初始化
	
	//DAC配置初始化
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_T5_TRGO;					//！！！定时器触发转换 一次转移数组里的一个数据 TIM5
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;         
	DAC_Init(DAC_Channel_2, &DAC_InitStructure);                            //DAC2初始化
		//DAC设置数据对齐
	DAC_SetChannel2Data(DAC_Align_12b_R, 0);								//右对齐
	
	
	//DMA配置初始化
	DMA_InitStructure.DMA_Channel = DMA_Channel_7;							//DMA_CH7
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                         //循环模式
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;                 //内存到外设
	DMA_InitStructure.DMA_BufferSize = DAC2_Value_Length;                    //传输次数:数组长度
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                   //优先级
		//DMA_FIFO
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)DAC2_Value;			//内存地址:输出DA值数组
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;             
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;     //16 bit
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 //内存自增

	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(DAC->DHR12R2);			//外设地址:DAC2地址
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;             
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;     //16 bit
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;                //外设不自增
	DMA_Init(DMA1_Stream6,&DMA_InitStructure);			                            //DMA1_Stream6 初始化 
	
	DMA_Cmd(DMA1_Stream6,ENABLE);				//DMA1_Stream6 使能
	DAC_DMACmd(DAC_Channel_2, ENABLE);          //DAC2_DMA 使能  	
	DAC_Cmd(DAC_Channel_2,ENABLE);              //DAC2 使能
}


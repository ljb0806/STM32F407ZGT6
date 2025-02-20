/**
*****************************************************************************
*
*  @file    FFT.c
*  @brief   使用官方汇编库实现FFT
*			
*  @author  binbin
*  @date    2025/2/18
*  @version 1.0
*  
*****************************************************************************
**/

#include "stm32f4xx.h"
#include "FFT.h"
#include "arm_math.h"

long In_Arr[FFT_LENGTH];
long Out_Arr[FFT_LENGTH / 2];
long Amp_Arr[FFT_LENGTH / 2];
float Analog_Arr[FFT_LENGTH / 2];

void ADC_Buff_to_In_Arr(uint16_t * ADC_Buff)
{
	for(int i = 0; i < FFT_LENGTH; i++)
	{
		In_Arr[i] = ADC_Buff[i] << 16;
	}
}

void In_Arr_to_Out_Arr(void)
{
	cr4_fft_1024_stm32(Out_Arr, In_Arr, FFT_LENGTH);
}

void Get_Amp_Arr(void)
{
	signed short lx, ly;
	float x, y, Amp;

	for(int i = 0; i < FFT_LENGTH / 2; i++)
	{
		lx = (Out_Arr[i] << 16) >> 16;
		ly = Out_Arr[i] >> 16;
		x = (float)lx * FFT_LENGTH / 32768;
		y = (float)ly * FFT_LENGTH / 32768;
		Amp = sqrt(x * x + y * y) / FFT_LENGTH;
		if(i == 0)
		{
			Amp_Arr[i] = (unsigned long)(Amp * 32768);
		}
		else
		{
			Amp_Arr[i] = (unsigned long)(Amp * 32768 * 2);
		}
	}
}

void Get_Analog_Arr(void)
{
	for(int i = 0; i < FFT_LENGTH / 2; i++)
	{
		Analog_Arr[i] = Amp_Arr[i] * 3.3 / 4096;
	}
}

void FFT_BEGIN(uint16_t * ADC_Buff)
{
	ADC_Buff_to_In_Arr(ADC_Buff);
	In_Arr_to_Out_Arr();
	Get_Amp_Arr();
	Get_Analog_Arr();
}

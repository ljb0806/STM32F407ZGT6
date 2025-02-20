#ifndef __AD9833_H
#define __AD9833_H
#include "stm32f4xx.h"

#define TRI_WAVE 0
#define SINE_WAVE 1
#define SQUARE_WAVE 2

#define FSYNC_1() GPIO_SetBits(GPIOC, GPIO_Pin_13)
#define FSYNC_0() GPIO_ResetBits(GPIOC, GPIO_Pin_13)
#define SCK_1()   GPIO_SetBits(GPIOF, GPIO_Pin_1)
#define SCK_0()   GPIO_ResetBits(GPIOF, GPIO_Pin_1)
#define DAT_1()   GPIO_SetBits(GPIOF, GPIO_Pin_3)
#define DAT_0()   GPIO_ResetBits(GPIOF, GPIO_Pin_3)
#define CS_1()    GPIO_SetBits(GPIOF, GPIO_Pin_5)
#define CS_0()    GPIO_ResetBits(GPIOF, GPIO_Pin_5)

void AD9833_Init(void);
void AD9833_WaveSet(double Freq,unsigned int Freq_SFR,unsigned int WaveMode,unsigned int Phase);
void AD9833_AmpSet(unsigned char Amp);

#endif

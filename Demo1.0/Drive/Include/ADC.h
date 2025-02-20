#ifndef __ADC_H
#define __ADC_H

#define ADC_BUFFER_SIZE 1024  // DMA缓冲区

extern uint16_t ADC1_buffer[ADC_BUFFER_SIZE];  // DMA缓冲区
extern uint16_t DMA_completed;                 // DMA完成标志

void ADC1_Init_WithoutDMA(void);
void ADC1_Init_WithDMA(uint32_t sampling_frequency);
uint16_t ADC1_GetDigitalValue(void);
float ADC1_GetAnalogValue(void);



#endif

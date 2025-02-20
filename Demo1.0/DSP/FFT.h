#ifndef __FFT_H
#define __FFT_H

#define FFT_LENGTH 1024

extern long In_Arr[FFT_LENGTH];
extern long Out_Arr[FFT_LENGTH / 2];
extern long Amp_Arr[FFT_LENGTH / 2];
extern float Analog_Arr[FFT_LENGTH / 2];

void ADC_Buff_to_In_Arr(uint16_t * ADC_Buff);
void In_Arr_to_Out_Arr(void);
void Get_Amp_Arr(void);
void FFT_BEGIN(uint16_t * ADC_Buff);

#endif

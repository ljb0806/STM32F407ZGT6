#include "stm32f4xx.h"
#include "delay.h"
#include "DAC.h"
#include "Timer.h"
#include "AD9833.h"

int main(void)
{
//*******************//	
	Delay_Init();    // 不能删Delay_Init(),否则很多带有Delay延时函数的库函数将无法运行卡死
//*******************//

	AD9833_Init();
	AD9833_WaveSet(10000, 0, SINE_WAVE, 0);
	AD9833_AmpSet(80);
	
	
	while (1)
	{		
		
	}	
}








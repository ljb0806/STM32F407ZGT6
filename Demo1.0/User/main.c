#include "stm32f4xx.h"
#include "delay.h"
#include "OLED.h"

int main(void)
{
//*******************//	
	Delay_Init();    // 不能删Delay_Init(),否则很多带有Delay延时函数的库函数将无法运行卡死
//*******************//

	OLED_Init();
	OLED_Clear();
	
	OLED_ShowChar(1, 1, 'A');
	
	while (1)
	{		
		
	}	
}








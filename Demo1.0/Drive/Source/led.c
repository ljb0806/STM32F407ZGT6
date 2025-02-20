/**
*****************************************************************************
*
*  @file    LED.c
*  @brief   对开发板上的指示灯进行初始化
*			可以用于做一些程序测试
*          
*  @author  binbin
*  @date    2025/2/18
*  @version 1.0
*  
*****************************************************************************
**/

#include "led.h"  



void LED_Init(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_AHB1PeriphClockCmd ( LED1_CLK, ENABLE); 	
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
	
	GPIO_InitStructure.GPIO_Pin = LED1_PIN;	 
	GPIO_Init(LED1_PORT, &GPIO_InitStructure);	
	
	GPIO_ResetBits(LED1_PORT,LED1_PIN);  
}





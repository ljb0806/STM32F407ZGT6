#ifndef __USART_H
#define __USART_H

void USART1_Init(u32 bound);
void USART2_Init(u32 bound);
void USART3_Init(u32 bound);
void USART6_Init(u32 bound);
void USART_Sendbyte(USART_TypeDef* usrUSARTx, uint8_t data);
	
//extern int USART_RxData;
//extern int USART_RxFlag;

#endif


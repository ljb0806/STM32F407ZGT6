#ifndef __AD9954_H
#define __AD9954_H
#include "stm32f4xx.h"

#define UPD_1()     GPIO_SetBits(GPIOC, GPIO_Pin_2)
#define UPD_0()     GPIO_ResetBits(GPIOC, GPIO_Pin_2)
#define PS1_1()     GPIO_SetBits(GPIOC, GPIO_Pin_0)
#define PS1_0()     GPIO_ResetBits(GPIOC, GPIO_Pin_0)
#define PS0_1()     GPIO_SetBits(GPIOF, GPIO_Pin_9)
#define PS0_0()     GPIO_ResetBits(GPIOF, GPIO_Pin_9)
#define OSK_1()     GPIO_SetBits(GPIOF, GPIO_Pin_7)
#define OSK_0()     GPIO_ResetBits(GPIOF, GPIO_Pin_7)
#define SDIO_1()    GPIO_SetBits(GPIOA, GPIO_Pin_1)
#define SDIO_0()    GPIO_ResetBits(GPIOA, GPIO_Pin_1)
#define SCK_1()     GPIO_SetBits(GPIOF, GPIO_Pin_14)
#define SCK_0()     GPIO_ResetBits(GPIOF, GPIO_Pin_14)
#define CS_1()      GPIO_SetBits(GPIOB, GPIO_Pin_1)
#define CS_0()      GPIO_ResetBits(GPIOB, GPIO_Pin_1)
#define PWR_1()     GPIO_SetBits(GPIOF, GPIO_Pin_8)
#define PWR_0()     GPIO_ResetBits(GPIOF, GPIO_Pin_8)
#define RST_1()     GPIO_SetBits(GPIOA, GPIO_Pin_0)
#define RST_0()     GPIO_ResetBits(GPIOA, GPIO_Pin_0)
#define IOSY_1()    GPIO_SetBits(GPIOA, GPIO_Pin_2)
#define IOSY_0()    GPIO_ResetBits(GPIOA, GPIO_Pin_2)

#define CFR1	0X00	// 控制功能寄存器1
#define CFR2    0X01	// 控制功能寄存器2
#define ASF     0X02	// 振幅比例因子寄存器
#define ARR     0X03	// 振幅斜坡速率寄存器

#define FTW0    0X04	// 频率调谐字寄存器0
#define POW0    0X05	// 相位偏移字寄存器
#define FTW1    0X06	// 频率调谐字寄存器1

#define NLSCW   0X07	// 下降扫描控制字寄存器
#define PLSCW   0X08	// 上升扫描控制字寄存器

#define RSCW0   0X07	
#define RSCW1   0X08	
#define RSCW2   0X09	
#define RSCW3   0X0A	// RAM段控制字寄存器
#define RAM     0X0B	// 读取指令写入RAM签名寄存器数据

#define No_Dwell	0x04	// No_Dwell不停留，输出扫频到终止频率回到起始频率。
#define Dwell 		0x00	// Dwell停留，输出扫频到终止频率后保持在终止频率。

void AD9954_GPIO_Init(void);
void AD9954_Init(void);
void AD9954_Set_Fre(double fre);
void AD9954_Set_Amp(uint16_t amp);
void AD9954_Set_Phase(uint16_t phase);

#endif

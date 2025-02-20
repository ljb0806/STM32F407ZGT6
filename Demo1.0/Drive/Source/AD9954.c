/**
*****************************************************************************
*
*  @file    AD9954.c
*  @brief   驱动AD9954
* 			连线: PC2 - UPD
* 				  PC0 - PS1
* 				  PF9 - PS0
* 				  PF7 - OSK
* 				  PA1 - SDIO
* 				  PF14 - SCK
* 				  PB1 - CS
* 				  PF8 - PWR
* 				  PA0 - RST
* 				  PA2 - IOSY
* 				  PF13 - SDO
*
*  @author  binbin
*  @date    2025/2/18
*  @version 1.0
*  
*****************************************************************************
**/

#include "stm32f4xx.h"
#include "delay.h"
#include "AD9954.h"
#include "GPIO.h"

#define fosc  20                        //晶振频率 20Mhz
#define PLL_MULTIPLIER  20              //PLL倍频数（4--20）
#define fs (fosc*PLL_MULTIPLIER)        //系统时钟频率

double fH_Num=10.73741824;

void AD9954_GPIO_Init(void)
{
    Config_GPIO(GPIOC, GPIO_Pin_2, GPIO_MODE_OUTPUT);            // PC2 - UPD
    Config_GPIO(GPIOC, GPIO_Pin_0, GPIO_MODE_OUTPUT);            // PC0 - PS1
    Config_GPIO(GPIOF, GPIO_Pin_9, GPIO_MODE_OUTPUT);            // PF9 - PS0
    Config_GPIO(GPIOF, GPIO_Pin_7, GPIO_MODE_OUTPUT);            // PF7 - OSK
    Config_GPIO(GPIOA, GPIO_Pin_1, GPIO_MODE_OUTPUT);            // PA1 - SDIO
    Config_GPIO(GPIOF, GPIO_Pin_14, GPIO_MODE_OUTPUT);           // PF14 - SCK
    Config_GPIO(GPIOB, GPIO_Pin_1, GPIO_MODE_OUTPUT);            // PB1 - CS
    Config_GPIO(GPIOF, GPIO_Pin_8, GPIO_MODE_OUTPUT);            // PF8 - PWR
    Config_GPIO(GPIOA, GPIO_Pin_0, GPIO_MODE_OUTPUT);            // PA0 - RST
    Config_GPIO(GPIOA, GPIO_Pin_2, GPIO_MODE_OUTPUT);            // PA2 - IOSY
    Config_GPIO(GPIOF, GPIO_Pin_13, GPIO_MODE_INPUT);            // PF13 - SDO

    RST_0();
    CS_0();
    SCK_0();
    SDIO_0();    
    OSK_0();
    PS0_0();
    PS1_0();
    UPD_0();
    IOSY_0();
    PWR_0();

    Delay_ms(10);
}

void AD9954_RST(void)
{
    RST_1();
    Delay_ms(10);
    RST_0();
    CS_0();
    SCK_0();
    CS_1();
}

void AD9954_UPDATE(void)
{
    UPD_1();
    Delay_ms(10);
    UPD_0();
}

void AD9954_SendByte(uint8_t data)
{
    for(uint8_t i = 0; i < 8; i++)
    {
        SCK_0();
        Delay_us(10);

        if(data & 0x80) SDIO_1();
        else SDIO_0();

        SCK_1();
        Delay_us(10);
        
        data <<= 1;
    }
}

void AD9954_SendBytes(uint8_t RegAddr, uint8_t *Data, uint8_t Len)
{
    AD9954_SendByte(RegAddr);

    for(uint8_t i = 0; i < Len; i++)
    {
        AD9954_SendByte(Data[i]);
    }
}

uint32_t Get_FTW(double Real_fH)
{
    return (uint32_t)(fH_Num * Real_fH);
}

void AD9954_Init(void)
{
    uint8_t CFR1_data[4] = {0, 0, 0, 0};
    uint8_t CFR2_data[3] = {0, 0, 0};

    AD9954_GPIO_Init();
    AD9954_RST();
    Delay_ms(300);
    CS_0();

    CFR1_data[0]=0X02; // 此处：0x02->OSK使能；0X00->OSK关闭。在OSK模式使能的前提下，幅度寄存器(0X02)生效
    CFR1_data[1]=0X00;
	CFR1_data[2]=0X00;
	CFR1_data[3]=0x00; // 比较器启用,方波输出;0x40,比较器禁用方波无输出
    AD9954_SendBytes(CFR1, CFR1_data, 4);

    CFR2_data[0]=0X00;
    CFR2_data[1]=0X00;
    if(fs > 400)                                                       ;                               
    else if(fs >= 250) CFR2_data[2] = PLL_MULTIPLIER << 3 | 0x04 | 0x03;
    else               CFR2_data[2] = PLL_MULTIPLIER << 3;
    AD9954_SendBytes(CFR2, CFR2_data, 3);

    CS_1();
}

void AD9954_Set_Fre(double fre)
{
    uint8_t data[4] = {0x00, 0x00, 0x00, 0x00};
    uint32_t temp = Get_FTW(fre);

    data[0] = (uint8_t)(temp >> 24);
    data[1] = (uint8_t)(temp >> 16);
    data[2] = (uint8_t)(temp >> 8);
    data[3] = (uint8_t)(temp);

    CS_0();
    AD9954_SendBytes(FTW0, data, 4);
    CS_1();
    AD9954_UPDATE();
}

void AD9954_Set_Amp(uint16_t amp)
{
    uint8_t data[2] = {0x00, 0x00};
    CS_0();

    data[0] = (uint8_t)(amp >> 8);
    data[1] = (uint8_t)(amp);

    AD9954_SendBytes(ASF, data, 2);
    CS_1();

    AD9954_UPDATE();
}

void AD9954_Set_Phase(uint16_t phase)
{
    uint8_t data[2] = {0x00, 0x00};
    CS_0();

    data[0] = (uint8_t)(phase >> 8);
    data[1] = (uint8_t)(phase);

    AD9954_SendBytes(POW0, data, 2);
    CS_1();

    AD9954_UPDATE();
}























/**
*****************************************************************************
*
*  @file    AD9833.c
*  @brief   驱动AD9833
*			连线: FSYNC - PC13
*                 SCK - PF1  
*          		  DAT - PF3
*			      CS  - PF5
*
*  @author  binbin
*  @date    2025/2/18
*  @version 1.0
*  
*****************************************************************************
**/

#include "stm32f4xx.h"
#include "delay.h"
#include "GPIO.h"
#include "AD9833.h"

void AD9833_Init(void)
{
    Config_GPIO(GPIOC, GPIO_Pin_13, GPIO_MODE_OUTPUT);
    Config_GPIO(GPIOF, GPIO_Pin_1, GPIO_MODE_OUTPUT);
    Config_GPIO(GPIOF, GPIO_Pin_3, GPIO_MODE_OUTPUT);
    Config_GPIO(GPIOF, GPIO_Pin_5, GPIO_MODE_OUTPUT);

    FSYNC_1();
    CS_1();
    SCK_0();
    DAT_0();
}

void AD9833_WriteReg(uint16_t Data)
{
    SCK_1();
    FSYNC_0();

    for(uint8_t i = 0; i < 16; i++)
    {
        if(Data & 0x8000) DAT_1();
        else DAT_0();
        
        SCK_0();
        SCK_1();

        Data <<= 1;
    }

    FSYNC_1();
}

void AD9833_AmpSet(unsigned char Amp)
{
    CS_0();

    unsigned int temp;
    temp = 0x1100 | Amp;

    for(uint8_t i = 0; i < 16; i++)
    {
        SCK_0();

        if(temp & 0x8000) DAT_1();
        else DAT_0();

        SCK_1();

        temp <<= 1;

        //Delay_ms(2);
    }

    CS_1();
}

void AD9833_WaveSet(double Freq,unsigned int Freq_SFR,unsigned int WaveMode,unsigned int Phase )
{

		int frequence_LSB,frequence_MSB,Phs_data;
		double   frequence_mid,frequence_DATA;
		long int frequence_hex;

		/*********************************计算频率的16进制值***********************************/
		frequence_mid=268435456/25;						//适合25M晶振
		//如果时钟频率不为25MHZ，修改该处的频率值，单位MHz ，AD9833最大支持25MHz
		frequence_DATA=Freq;
		frequence_DATA=frequence_DATA/1000000;
		frequence_DATA=frequence_DATA*frequence_mid;
		frequence_hex=frequence_DATA;  					//这个frequence_hex的值是32位的一个很大的数字，需要拆分成两个14位进行处理；
		frequence_LSB=frequence_hex; 					//frequence_hex低16位送给frequence_LSB
		frequence_LSB=frequence_LSB&0x3fff;				//去除最高两位，16位数换去掉高位后变成了14位
		frequence_MSB=frequence_hex>>14;    			//frequence_hex高16位送给frequence_HSB
		frequence_MSB=frequence_MSB&0x3fff;				//去除最高两位，16位数换去掉高位后变成了14位

		Phs_data=Phase|0xC000;							//相位值
		AD9833_WriteReg(0x0100); 						//复位AD9833,即RESET位为1
		AD9833_WriteReg(0x2100); 						//选择数据一次写入，B28位和RESET位为1

		if(Freq_SFR==0)				  					//把数据设置到设置频率寄存器0
		{
		 	frequence_LSB=frequence_LSB|0x4000;
		 	frequence_MSB=frequence_MSB|0x4000;
			//使用频率寄存器0输出波形
			AD9833_WriteReg(frequence_LSB); 			//L14，选择频率寄存器0的低14位数据输入
			AD9833_WriteReg(frequence_MSB); 			//H14 频率寄存器的高14位数据输入
			AD9833_WriteReg(Phs_data);					//设置相位
			//AD9833_Write(0x2000); /**设置FSELECT位为0，芯片进入工作状态,频率寄存器0输出波形**/
	    }
		if(Freq_SFR==1)									//把数据设置到设置频率寄存器1
		{
			frequence_LSB=frequence_LSB|0x8000;
			frequence_MSB=frequence_MSB|0x8000;
			//使用频率寄存器1输出波形
			AD9833_WriteReg(frequence_LSB); 			//L14，选择频率寄存器1的低14位输入
			AD9833_WriteReg(frequence_MSB); 			//H14 频率寄存器1为
			AD9833_WriteReg(Phs_data);					//设置相位
			//AD9833_Write(0x2800); /**设置FSELECT位为0，设置FSELECT位为1，即使用频率寄存器1的值，芯片进入工作状态,频率寄存器1输出波形**/
		}

		if(WaveMode==TRI_WAVE) 							//输出三角波波形
		 	AD9833_WriteReg(0x2002); 
		if(WaveMode==SQUARE_WAVE)							//输出方波波形
			AD9833_WriteReg(0x2028); 
		if(WaveMode==SINE_WAVE)							//输出正弦波形
			AD9833_WriteReg(0x2000); 
}






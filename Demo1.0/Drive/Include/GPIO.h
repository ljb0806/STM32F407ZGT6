#ifndef __GPIO_H
#define __GPIO_H

typedef enum {
    GPIO_MODE_INPUT,
    GPIO_MODE_OUTPUT
} GPIO_Mode_TypeDef;

void Config_GPIO(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_Mode_TypeDef Mode);
void Set_GPIO_High(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void Set_GPIO_Low(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

#endif

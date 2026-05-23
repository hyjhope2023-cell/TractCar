#ifndef __BUTTON_H
#define __BUTTON_H

#include "stm32f1xx_hal.h"
#include <stdbool.h>
// ????????
#define Button_PORT Button_GPIO_Port
#define Button_PIN Button_Pin

// ????????

bool Button_Down(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
//GPIO_PinState Button_Up(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

/**
 * @brief 读取旋转传感器 (IN8) 
 */
uint32_t Get_Knob_Value(ADC_HandleTypeDef *hadc);


#endif



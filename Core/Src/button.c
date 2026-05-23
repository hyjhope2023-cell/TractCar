#include "button.h"




bool Button_Down(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	return	HAL_GPIO_ReadPin(GPIOx,GPIO_Pin)==GPIO_PIN_RESET;
}

uint32_t Get_Knob_Value(ADC_HandleTypeDef *hadc) {
    uint32_t value = 0;

    // 1. ����������ʼת��
    HAL_ADC_Start(hadc); 

    // 2. �ȴ�ת����� (��ʱʱ����Ϊ 10ms)
    if (HAL_ADC_PollForConversion(hadc, 10) == HAL_OK) {
        // 3. ��ȡת�����
        value = HAL_ADC_GetValue(hadc); 
    }

    // 4. ֹͣ ADC����Ϊ��û��������ת�������Ե��ζ�ȡ����ֹͣ��
    HAL_ADC_Stop(hadc);

    return value;
}


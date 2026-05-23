#include "motor.h"

/**
 * @brief �����ʼ����������ʱ��PWM���
 */
void Motor_Init(void)
{
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
}


/**
 * @brief ���õ���ٶȺͷ���
 * @param speed ��Χ -100 �� 100 (������ת��������ת��0ֹͣ)
 */
void Motor_STOP(TIM_HandleTypeDef*him,uint32_t Channel1,uint32_t Channel2)
{
	__HAL_TIM_SET_COMPARE(him,Channel1,0);	// ��������д��CCR
	__HAL_TIM_SET_COMPARE(him,Channel2,0);	// ��������д��CCR
}
void Motor_FORWARD(TIM_HandleTypeDef*him,uint32_t Channel1,uint32_t Channel2,uint16_t ccr)
{
	__HAL_TIM_SET_COMPARE(him,Channel1,ccr);	// ��������д��CCR
	__HAL_TIM_SET_COMPARE(him,Channel2,0);	// ��������д��CCR
}
void Motor_BACK(TIM_HandleTypeDef*him,uint32_t Channel1,uint32_t Channel2,uint16_t ccr)
{
	__HAL_TIM_SET_COMPARE(him,Channel1,0);	// ��������д��CCR
	__HAL_TIM_SET_COMPARE(him,Channel2,ccr);	// ��������д��CCR
}



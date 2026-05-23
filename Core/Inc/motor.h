#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f1xx_hal.h"

// ��ʹ�õĶ�ʱ�����
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim4;
// ���巽������������ڵĵ�ƽ
#define MOTOR_M1A_PIN   TIM_CHANNEL_1
#define MOTOR_M1B_PIN   TIM_CHANNEL_2
#define MOTOR_M2A_PIN   TIM_CHANNEL_3
#define MOTOR_M2B_PIN   TIM_CHANNEL_4
#define MOTOR_M3A_PIN   TIM_CHANNEL_1
#define MOTOR_M3B_PIN   TIM_CHANNEL_2
#define MOTOR_M4A_PIN   TIM_CHANNEL_3
#define MOTOR_M4B_PIN   TIM_CHANNEL_4



// ��������
void Motor_Init(void);
void Motor_STOP(TIM_HandleTypeDef*him,uint32_t Channel1,uint32_t Channel2);
void Motor_FORWARD(TIM_HandleTypeDef*him,uint32_t Channel1,uint32_t Channel2,uint16_t ccr);
void Motor_BACK(TIM_HandleTypeDef*him,uint32_t Channel1,uint32_t Channel2,uint16_t ccr);
#endif

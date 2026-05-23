#ifndef __TRACKING_H
#define __TRACKING_H

#include "stm32f1xx_hal.h"

// ��������
#define TRACKING_PORT GPIOA
#define L2_PIN GPIO_PIN_4  // �����
#define L1_PIN GPIO_PIN_5  // �м���
#define R1_PIN GPIO_PIN_6  // �м���
#define R2_PIN GPIO_PIN_7  // ���ұ�

// ��������
void read_track(int* state);

#endif


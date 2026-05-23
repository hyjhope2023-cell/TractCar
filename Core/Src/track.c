#include "track.h"

/**
 * @brief ��ȡ��·ѭ��״̬
 * @return һ��4λ�����������Ӹߵ��ͷֱ���� L2 L1 R1 R2
 *         ���磺0110 ��ʾ�м���·��Ӧ������
 */
 
// int L1;
// int L2;
// int R1;
// int R2;

void read_track(int* state)
{
  state[0] = (int)!HAL_GPIO_ReadPin(TRACKING_PORT, L2_PIN);
  state[1] = (int)!HAL_GPIO_ReadPin(TRACKING_PORT, L1_PIN);
  state[2] = (int)!HAL_GPIO_ReadPin(TRACKING_PORT, R1_PIN);
  state[3] = (int)!HAL_GPIO_ReadPin(TRACKING_PORT, R2_PIN);
}



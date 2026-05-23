#include "car.h"
#include "motor.h"
#include <stdlib.h>
#include <stdio.h>
#include "track.h"
#include <math.h>
#include "main.h"
// #include "PID.h"

/* 
 * @brief 传感器状态误差
 */
float Get_Track_Error(int state[])
{
	if(state[0]==1&&state[3]==0){
		if(state[1]==1&&state[2]==1){	//左弯道【1，1，1，0】
			return -5.0f;
		}
		else if(state[1]==1&&state[2]==0){	//【1，1，0，0】
			return -2.5f;
		}
		else if(state[1]==0&&state[2]==0){	//【1，0，0，0】
			return -4.0f;
		}
	}
	else if(state[0]==0&&state[3]==1){
		if(state[1]==1&&state[2]==1){	//右弯道【0，1，1，1】
			return 5.0f;
		}
		else if(state[1]==0&&state[2]==1){	//【0，0，1，1】
			return 2.5f;
		}
		else if(state[1]==0&&state[2]==0){	//【0，0，0，1】
			return 4.0f;
		}
	}
	else if(state[0]==1&&state[3]==1){
		if(state[1]==1&&state[2]==1){	//双侧弯道【1，1，1，1】
			return 5.0f;
		}
	}
	else if(state[0]==0&&state[3]==0){
		if(state[1]==1&&state[2]==1){	//直线道【0，1，1，0】
			return 0.0f;
		}
		else if(state[1]==0&&state[2]==1){	//【0，0，1，0】
			return 1.0f;
		}
		else if(state[1]==1&&state[2]==0){	//【0，1，0，0】
			return -1.0f;
		}
		else if(state[1]==0&&state[2]==0){	//【0，0，0，0】
			return 100.0f;
		}
	}
	return 0.0f;
}

uint16_t Calculate_CCR(TIM_HandleTypeDef *htim, uint16_t speed)
{
    // 1. 安全限幅：确保速度不会超过 100%
    if (speed > 100) speed = 100;
    // 2. 获取当前定时器的 ARR 值
    // 使用 __HAL_TIM_GET_AUTORELOAD 宏可以实时获取，防止你在CubeMX改了值后代码失效
    uint32_t arr = __HAL_TIM_GET_AUTORELOAD(htim);

    // 3. 计算 CCR
    // 注意：先乘以 arr，再除以 100。
    // 使用 (uint32_t) 强制转换是为了防止 speed * arr 的中间结果超过 65535
    uint32_t ccr = (uint32_t)speed * (arr + 1) / 100;

    return (uint16_t)ccr;
}

/**
 * @brief 差速驱动底层
 */
void Drive_Differential(int left_speed, int right_speed) {
    // 限制范围在 -100 到 100
    int l = (left_speed > 100) ? 100 : (left_speed < -100 ? -100 : left_speed);
    int r = (right_speed > 100) ? 100 : (right_speed < -100 ? -100 : right_speed);

    // 左侧电机 (M1, M2)
    if (l >= 0) {
        Motor_FORWARD(&htim4, MOTOR_M1A_PIN, MOTOR_M1B_PIN, Calculate_CCR(&htim4, l));
        Motor_FORWARD(&htim4, MOTOR_M2A_PIN, MOTOR_M2B_PIN, Calculate_CCR(&htim4, l));
    } else {
        Motor_BACK(&htim4, MOTOR_M1A_PIN, MOTOR_M1B_PIN, Calculate_CCR(&htim4, -l));
        Motor_BACK(&htim4, MOTOR_M2A_PIN, MOTOR_M2B_PIN, Calculate_CCR(&htim4, -l));
    }

    // 右侧电机 (M3, M4)
    if (r >= 0) {
        Motor_FORWARD(&htim2, MOTOR_M3A_PIN, MOTOR_M3B_PIN, Calculate_CCR(&htim2, r));
        Motor_FORWARD(&htim2, MOTOR_M4A_PIN, MOTOR_M4B_PIN, Calculate_CCR(&htim2, r));
    } else {
        Motor_BACK(&htim2, MOTOR_M3A_PIN, MOTOR_M3B_PIN, Calculate_CCR(&htim2, -r));
        Motor_BACK(&htim2, MOTOR_M4A_PIN, MOTOR_M4B_PIN, Calculate_CCR(&htim2, -r));
    }
}


void STOP()
{
	Motor_STOP(&htim4,MOTOR_M1A_PIN,MOTOR_M1B_PIN);
	Motor_STOP(&htim4,MOTOR_M2A_PIN,MOTOR_M2B_PIN);
	Motor_STOP(&htim2,MOTOR_M3A_PIN,MOTOR_M3B_PIN);
	Motor_STOP(&htim2,MOTOR_M4A_PIN,MOTOR_M4B_PIN);
}
void FORWARD(uint16_t speed)
{
	uint16_t ccr = Calculate_CCR(&htim2, speed);
	Motor_FORWARD(&htim4,MOTOR_M1A_PIN,MOTOR_M1B_PIN,ccr);
	Motor_FORWARD(&htim4,MOTOR_M2A_PIN,MOTOR_M2B_PIN,ccr);
	Motor_FORWARD(&htim2,MOTOR_M3A_PIN,MOTOR_M3B_PIN,ccr);
	Motor_FORWARD(&htim2,MOTOR_M4A_PIN,MOTOR_M4B_PIN,ccr);
}

void BACK(uint16_t speed)
{
	uint16_t ccr = Calculate_CCR(&htim2, speed);
	Motor_BACK(&htim4,MOTOR_M1A_PIN,MOTOR_M1B_PIN,ccr);
	Motor_BACK(&htim4,MOTOR_M2A_PIN,MOTOR_M2B_PIN,ccr);
	Motor_BACK(&htim2,MOTOR_M3A_PIN,MOTOR_M3B_PIN,ccr);
	Motor_BACK(&htim2,MOTOR_M4A_PIN,MOTOR_M4B_PIN,ccr);
};
void LEFT(uint16_t speed)
{
	uint16_t ccr = Calculate_CCR(&htim2, speed);
	Motor_BACK(&htim4,MOTOR_M1A_PIN,MOTOR_M1B_PIN,ccr);
	Motor_BACK(&htim4,MOTOR_M2A_PIN,MOTOR_M2B_PIN,ccr);
	// Motor_FORWARD(&htim4,MOTOR_M1A_PIN,MOTOR_M1B_PIN,ccr/2);
	// Motor_FORWARD(&htim4,MOTOR_M2A_PIN,MOTOR_M2B_PIN,ccr/2);
	Motor_FORWARD(&htim2,MOTOR_M3A_PIN,MOTOR_M3B_PIN,ccr);
	Motor_FORWARD(&htim2,MOTOR_M4A_PIN,MOTOR_M4B_PIN,ccr);
}
void RIGHT(uint16_t speed)
{
	uint16_t ccr = Calculate_CCR(&htim2, speed);
	Motor_FORWARD(&htim4,MOTOR_M1A_PIN,MOTOR_M1B_PIN,ccr);
	Motor_FORWARD(&htim4,MOTOR_M2A_PIN,MOTOR_M2B_PIN,ccr);
	// Motor_FORWARD(&htim2,MOTOR_M3A_PIN,MOTOR_M3B_PIN,ccr/2);
	// Motor_FORWARD(&htim2,MOTOR_M4A_PIN,MOTOR_M4B_PIN,ccr/2);
	Motor_BACK(&htim2,MOTOR_M3A_PIN,MOTOR_M3B_PIN,ccr);
	Motor_BACK(&htim2,MOTOR_M4A_PIN,MOTOR_M4B_PIN,ccr);
}
void LEFT_MOVE(uint16_t speed)
{
	uint16_t ccr = Calculate_CCR(&htim2, speed);
	Motor_BACK(&htim4,MOTOR_M1A_PIN,MOTOR_M1B_PIN,ccr);
	Motor_FORWARD(&htim4,MOTOR_M2A_PIN,MOTOR_M2B_PIN,ccr);
	Motor_FORWARD(&htim2,MOTOR_M3A_PIN,MOTOR_M3B_PIN,ccr);
	Motor_BACK(&htim2,MOTOR_M4A_PIN,MOTOR_M4B_PIN,ccr);
}
void RIGHT_MOVE(uint16_t speed)
{
	uint16_t ccr = Calculate_CCR(&htim2, speed);
	Motor_FORWARD(&htim4,MOTOR_M1A_PIN,MOTOR_M1B_PIN,ccr);
	Motor_BACK(&htim4,MOTOR_M2A_PIN,MOTOR_M2B_PIN,ccr);
	Motor_BACK(&htim2,MOTOR_M3A_PIN,MOTOR_M3B_PIN,ccr);
	Motor_FORWARD(&htim2,MOTOR_M4A_PIN,MOTOR_M4B_PIN,ccr);
}
void LEFT_FORWARD(uint16_t speed)
{
	uint16_t ccr = Calculate_CCR(&htim2, speed);
	Motor_STOP(&htim4,MOTOR_M1A_PIN,MOTOR_M1B_PIN);
	Motor_FORWARD(&htim4,MOTOR_M2A_PIN,MOTOR_M2B_PIN,ccr);
	Motor_FORWARD(&htim2,MOTOR_M3A_PIN,MOTOR_M3B_PIN,ccr);
	Motor_STOP(&htim2,MOTOR_M4A_PIN,MOTOR_M4B_PIN);
}
void LEFT_BACK(uint16_t speed)
{
	uint16_t ccr = Calculate_CCR(&htim2, speed);
	Motor_BACK(&htim4,MOTOR_M1A_PIN,MOTOR_M1B_PIN,ccr);
	Motor_STOP(&htim4,MOTOR_M2A_PIN,MOTOR_M2B_PIN);
	Motor_STOP(&htim2,MOTOR_M3A_PIN,MOTOR_M3B_PIN);
	Motor_BACK(&htim2,MOTOR_M4A_PIN,MOTOR_M4B_PIN,ccr);
}
void RIGHT_FORWARD(uint16_t speed)
{
	uint16_t ccr = Calculate_CCR(&htim2, speed);
	Motor_FORWARD(&htim4,MOTOR_M1A_PIN,MOTOR_M1B_PIN,ccr);
	Motor_STOP(&htim4,MOTOR_M2A_PIN,MOTOR_M2B_PIN);
	Motor_STOP(&htim2,MOTOR_M3A_PIN,MOTOR_M3B_PIN);
	Motor_FORWARD(&htim2,MOTOR_M4A_PIN,MOTOR_M4B_PIN,ccr);
}
void RIGHT_BACK(uint16_t speed)
{
	uint16_t ccr = Calculate_CCR(&htim2, speed);
	Motor_STOP(&htim4,MOTOR_M1A_PIN,MOTOR_M1B_PIN);
	Motor_BACK(&htim4,MOTOR_M2A_PIN,MOTOR_M2B_PIN,ccr);
	Motor_BACK(&htim2,MOTOR_M3A_PIN,MOTOR_M3B_PIN,ccr);
	Motor_STOP(&htim2,MOTOR_M4A_PIN,MOTOR_M4B_PIN);
}

void MOVE(enum Direction dir, uint16_t speed)
{
    switch (dir)
    {
        case stop:    STOP();           break;
        case forward: FORWARD(speed);   break;
        case back:    BACK(speed);      break;
        case left:    LEFT(speed);      break;
        case right:   RIGHT(speed);     break;
        case left_move:  LEFT_MOVE(speed);  break;
        case right_move: RIGHT_MOVE(speed); break;
		case left_forward: LEFT_FORWARD(speed); break;
		case left_back: LEFT_BACK(speed); break;
		case right_forward: RIGHT_FORWARD(speed); break;
		case right_back: RIGHT_BACK(speed); break;
        default:      STOP();           break;
    }
}



/**
 * @brief PID 循迹主函数
 */

void TRACK_PID(CarPosition p) {
    int state[4];
    read_track(state);
    float error = Get_Track_Error(state);
    // 1. 处理丢线
    if (error == 100.0f) {
        if (p->pid->last_error < 0) Drive_Differential(-35, 35);
        else if (p->pid->last_error > 0) Drive_Differential(35, -35);
        else Drive_Differential(0, 0);
        return;
    }

    // 2. 调用分文件后的 PID 计算函数
    float output = p->pid->PID_Compute(p->pid, error);
    // 3. 基础速度策略
    int base_speed = 40;
    if (fabs(error) >= 4.0f) base_speed = 25; // 大弯减速
    // 4. 合成速度并执行
    Drive_Differential(base_speed + (int)output, base_speed - (int)output);
}


void TRACK(CarPosition p)
{
	int state[4];
	read_track(state);

	if(state[0]==1&&state[3]==0){
		if(state[1]==1&&state[2]==1){	//左弯道【1，1，1，0】
			HAL_Delay(1200);
			p->MOVE(left,40);
			printf("left");
			read_track(state);
			while (state[2]==0)
			{
				printf("b");
				read_track(state);
				HAL_Delay(100);
			}
			printf("stop");
		}
		else if(state[1]==1&&state[2]==0){	//【1，1，0，0】
			p->MOVE(left,40);
			while (state[2]==0)
			{
				read_track(state);
			}
		}
		else if(state[1]==0&&state[2]==0){	//【1，0，0，0】
			p->MOVE(left,30);
			while (state[2]==0)
			{
				read_track(state);
			}
		}
	}
	else if(state[0]==0&&state[3]==1){
		if(state[1]==1&&state[2]==1){	//右弯道【0，1，1，1】
			HAL_Delay(1200);
			p->MOVE(right,40);
			read_track(state);
			while (state[1]==0)
			{
				read_track(state);
			}
		}
		else if(state[1]==0&&state[2]==1){	//【0，0，1，1】
			p->MOVE(right,40);
			while (state[1]==0)
			{
				read_track(state);
			}
		}
		else if(state[1]==0&&state[2]==0){	//【0，0，0，1】
			p->MOVE(right,30);
			while (state[1]==0)
			{
				read_track(state);
			}
		}
	}
	else if(state[0]==1&&state[3]==1){
		if(state[1]==1&&state[2]==1){	//双侧弯道【1，1，1，1】
			HAL_Delay(1200);
			p->MOVE(right,40);
			read_track(state);
			while (state[1]==0)
			{
				read_track(state);
			}
		}
	}
	else if(state[0]==0&&state[3]==0){
		if(state[1]==1&&state[2]==1){	//直线道【0，1，1，0】
			p->MOVE(forward,40);
		}
		else if(state[1]==0&&state[2]==1){	//【0，0，1，0】
			p->MOVE(right,30);
			while (state[1]==0)
			{
				read_track(state);
			}
		}
		else if(state[1]==1&&state[2]==0){	//【0，1，0，0】
			p->MOVE(left,30);
			while (state[2]==0)
			{
				read_track(state);
			}
		}
		else if(state[1]==0&&state[2]==0){	//【0，0，0，0】
			p->MOVE(stop,30);
		}
	}
}


CarPosition createCar()
{
	CarPosition p = (struct Car*)malloc(sizeof(struct Car));
	if(p==NULL){
		return NULL;
	}
	p->speed=100;
	p->direction=stop;
	p->pid = createPID();
	p->createCar=createCar;
	p->STOP=STOP;
	p->FORWARD=FORWARD;
	p->BACK=BACK;
	p->LEFT=LEFT;
	p->RIGHT=RIGHT;
	p->LEFT_MOVE=LEFT_MOVE;
	p->RIGHT_MOVE=RIGHT_MOVE;
	p->LEFT_FORWARD=LEFT_FORWARD;
	p->LEFT_BACK=LEFT_BACK;
	p->RIGHT_FORWARD=RIGHT_FORWARD;
	p->RIGHT_BACK=RIGHT_BACK;
	p->MOVE=MOVE;
	p->TRACK=TRACK;
	p->TRACK_PID=TRACK_PID;

	return p;
}

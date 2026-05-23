#include "PID.h"
#include <stdlib.h> 


/**
 * @brief PID 运算
 * @param measure 当前测量值
 * @return 修正后的输出量
 */
float PID_Compute(PID_Controller pid, float measure) {
    float error = measure - pid->target;
    
    // 积分项
    pid->integral += error;
    // 积分限幅 (Anti-Windup)
    if (pid->integral > pid->i_limit) pid->integral = pid->i_limit;
    if (pid->integral < -pid->i_limit) pid->integral = -pid->i_limit;
    
    // 微分项
    float derivative = error - pid->last_error;
    
    // 总输出
    float output = (pid->Kp * error) + (pid->Ki * pid->integral) + (pid->Kd * derivative);
    
    // 输出限幅
    if (output > pid->out_limit) output = pid->out_limit;
    if (output < -pid->out_limit) output = -pid->out_limit;
    
    pid->last_error = error;
    
    return output;
}

void PID_Reset(PID_Controller pid) {
    pid->integral = 0;
    pid->last_error = 0;
}

/**
 * @brief PID 初始化
 */
PID_Controller createPID(void)
{
    PID_Controller pid = (struct PID*)malloc(sizeof(struct PID));
	if(pid==NULL){
		return NULL;
	}
    pid->Kp = 60.5f;
    pid->Ki = 0.0f;
    pid->Kd = 17.0f;
    pid->i_limit = 50.0f;
    pid->out_limit = 80.0f;
    pid->target = 0.0f;
    pid->last_error = 0.0f;
    pid->integral = 0.0f;
    pid->createPID = createPID;
    pid->PID_Compute = PID_Compute;
    pid->PID_Reset = PID_Reset;

    return pid;
}


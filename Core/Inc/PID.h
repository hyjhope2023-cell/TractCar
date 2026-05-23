#ifndef __PID_H
#define __PID_H


typedef struct PID* PID_Controller;
// PID 控制器结构体
struct PID{
     // === PID 增益参数 (Gains) ===
    float Kp;               // 比例系数 (Proportional): 决定修正灵敏度，Kp越大响应越快，但易震荡
    float Ki;               // 积分系数 (Integral): 用于消除系统静态误差，但在快速循迹中需谨慎使用
    float Kd;               // 微分系数 (Derivative): 提供阻尼作用，抑制 P 项引起的震荡，预测误差趋势

    // === 运行状态变量 (State Variables) ===
    float target;           // 目标设定值 (Setpoint): 期望系统达到的数值（如：误差为0）
    float last_error;       // 上一次误差 (Last Error): 用于计算微分项 (D = Error - Last_Error)
    float integral;         // 积分累加值 (Cumulative Error): 存储过去误差的累积，用于计算 I 项

    // === 算法限制 (Constraints/Anti-Windup) ===
    float i_limit;          // 积分限幅: 限制 integral 值的最大范围，防止“积分饱和”导致系统失控
    float out_limit;        // 输出限幅: 限制 PID 计算结果的最大绝对值（如：最大速度差不能超过80）

    // === PID操作指针 ===
    PID_Controller (*createPID)(void);
    float (*PID_Compute)(PID_Controller pid, float measure);
    void (*PID_Reset)(PID_Controller pid);
};

// 函数声明
PID_Controller createPID(void);
// float PID_Compute(PID_Controller pid, float measure);
// void PID_Reset(PID_Controller pid);

#endif



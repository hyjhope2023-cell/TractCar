#ifndef __OLED_H
#define __OLED_H

// #include "stm32f1xx_hal.h"
#include "i2c.h"


typedef struct OLED *OLED_Position;
// 定义 OLED 结构体，参数化所有硬件关联
struct OLED{
    I2C_HandleTypeDef *hi2c;  // I2C 句柄 (如 &hi2c1)
    uint16_t address;         // I2C 地址 (0x78 或 0x7A)
    uint8_t width;            // 屏幕宽度
    uint8_t height;           // 屏幕高度
    uint8_t buffer[1024];     // 128x64 显存缓冲区

    // 函数方法指针 (Methods)
    void (*OLED_Init)(OLED_Position dev, I2C_HandleTypeDef *hi2c, uint16_t addr);
    void (*OLED_Clear)(OLED_Position dev);
    void (*OLED_Update)(OLED_Position dev);
    void (*OLED_DrawPixel)(OLED_Position dev, uint8_t x, uint8_t y, uint8_t color);
    void (*OLED_ShowChar)(OLED_Position dev, uint8_t x, uint8_t y, char chr, uint8_t size);
    void (*OLED_ShowString)(OLED_Position dev, uint8_t x, uint8_t y, char *str, uint8_t size);
};

// 函数声明，第一个参数均为 OLED_Position 指针
OLED_Position createOLED(void);

#endif


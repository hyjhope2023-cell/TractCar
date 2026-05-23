#include "OLED.h"
#include "CharLibrary.h"
#include <stdlib.h>
#include <string.h>

// 引入 ASCII 8x16 点阵表 (需在外部定义或在此处添加)
extern const uint8_t OLED_Font8x16[];

// --- 底层私有函数：写命令 ---
void OLED_WriteCmd(OLED_Position dev, uint8_t cmd) {
    HAL_I2C_Mem_Write(dev->hi2c, dev->address, 0x00, I2C_MEMADD_SIZE_8BIT, &cmd, 1, 10);
}

// --- 方法实现 ---

void OLED_Init(OLED_Position dev, I2C_HandleTypeDef *hi2c, uint16_t addr) {
    dev->hi2c = hi2c;
    dev->address = addr;
    dev->width = 128;
    dev->height = 64;

    HAL_Delay(100);
    OLED_WriteCmd(dev, 0xAE); // 关闭显示
    OLED_WriteCmd(dev, 0x20); // 地址模式
    OLED_WriteCmd(dev, 0x10); 
    OLED_WriteCmd(dev, 0xB0); 
    OLED_WriteCmd(dev, 0xC8); 
    OLED_WriteCmd(dev, 0x00); 
    OLED_WriteCmd(dev, 0x10); 
    OLED_WriteCmd(dev, 0x40); 
    OLED_WriteCmd(dev, 0x81); 
    OLED_WriteCmd(dev, 0xFF);
    OLED_WriteCmd(dev, 0xA1); 
    OLED_WriteCmd(dev, 0xA6); 
    OLED_WriteCmd(dev, 0xA8); 
    OLED_WriteCmd(dev, 0x3F);
    OLED_WriteCmd(dev, 0xD3); 
    OLED_WriteCmd(dev, 0x00);
    OLED_WriteCmd(dev, 0xD5); 
    OLED_WriteCmd(dev, 0x80);
    OLED_WriteCmd(dev, 0xD9); 
    OLED_WriteCmd(dev, 0x22);
    OLED_WriteCmd(dev, 0xDA); 
    OLED_WriteCmd(dev, 0x12);
    OLED_WriteCmd(dev, 0xDB); 
    OLED_WriteCmd(dev, 0x20);
    OLED_WriteCmd(dev, 0x8D); // 电荷泵
    OLED_WriteCmd(dev, 0x14);
    OLED_WriteCmd(dev, 0xAF); // 开启显示

    dev->OLED_Clear(dev);
    dev->OLED_Update(dev);
}

void OLED_Clear(OLED_Position dev) {
    memset(dev->buffer, 0, 1024);
}

void OLED_Update(OLED_Position dev) {
    for (uint8_t i = 0; i < 8; i++) {
        OLED_WriteCmd(dev, 0xB0 + i);
        OLED_WriteCmd(dev, 0x00);
        OLED_WriteCmd(dev, 0x10);
        HAL_I2C_Mem_Write(dev->hi2c, dev->address, 0x40, I2C_MEMADD_SIZE_8BIT, &dev->buffer[128 * i], 128, 100);
    }
}

void OLED_DrawPixel(OLED_Position dev, uint8_t x, uint8_t y, uint8_t color) {
    if (x >= 128 || y >= 64) return;
    if (color){
			dev->buffer[x + (y / 8) * 128] |= (1 << (y % 8));
		}
    else      { dev->buffer[x + (y / 8) * 128] &= ~(1 << (y % 8));
		}
}

void OLED_ShowChar(OLED_Position dev, uint8_t x, uint8_t y, char chr, uint8_t size) {
    // 基础检查：只处理可见 ASCII 字符 (空格 32 到 z 122)
    if (chr < ' ' || chr > 'z') return;
    // 边界检查：防止超出屏幕
    if (x > 128 - 8 || y > 64 - 16) return;

    // 计算字符在 OLED_Font8x16 数组中的起始位置
    uint8_t c = (uint8_t)chr - ' '; 
    uint16_t offset = c * 16; 

    // 绘制字符
    for (uint8_t i = 0; i < 16; i++) {
        uint8_t line_data = OLED_Font8x16[offset + i];
        for (uint8_t j = 0; j < 8; j++) {
            if (line_data & (0x80 >> j)) {
                dev->OLED_DrawPixel(dev, x + j, y + i, 1);
            } else {
                dev->OLED_DrawPixel(dev, x + j, y + i, 0);
            }
        }
    }
}

void OLED_ShowString(OLED_Position dev, uint8_t x, uint8_t y, char *str, uint8_t size) {
    while (*str) {
        dev->OLED_ShowChar(dev, x, y, *str, size);
        x += 8;
        str++;
    }
}

// --- 工厂函数实现 ---
static struct OLED global_oled_instance;
OLED_Position createOLED(void) {
    OLED_Position p = &global_oled_instance;
    // 绑定方法指针
    p->OLED_Init = OLED_Init;
    p->OLED_Clear = OLED_Clear;
    p->OLED_Update = OLED_Update;
    p->OLED_DrawPixel = OLED_DrawPixel;
    p->OLED_ShowChar = OLED_ShowChar;
    p->OLED_ShowString = OLED_ShowString;

    return p;
}



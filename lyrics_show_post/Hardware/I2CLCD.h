#ifndef __I2C_LCD_H
#define __I2C_LCD_H
#include "stm32f10x.h"

void LCD_Init(void);
void LCD_ShowString(u8 x, u8 y, char *str);
void LCD_Clear(void);

#endif
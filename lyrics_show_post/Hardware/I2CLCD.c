#include "I2CLCD.h"
#include "delay.h"

#define LCD_ADDR 0x4E 

// 提速宏定义：利用少量 __nop() 替代耗时的 Delay_us(5)
// STM32在72MHz下，1个nop大概是14纳秒，足够I2C电平翻转了
#define I2C_DELAY() {__NOP(); __NOP(); __NOP(); __NOP(); __NOP();}

#define SDA_H GPIO_SetBits(GPIOB, GPIO_Pin_7)
#define SDA_L GPIO_ResetBits(GPIOB, GPIO_Pin_7)
#define SCL_H GPIO_SetBits(GPIOB, GPIO_Pin_6)
#define SCL_L GPIO_ResetBits(GPIOB, GPIO_Pin_6)

static void I2C_Start(void) {
    SDA_H; SCL_H; I2C_DELAY();
    SDA_L; I2C_DELAY();
    SCL_L; I2C_DELAY();
}

static void I2C_Stop(void) {
    SDA_L; SCL_H; I2C_DELAY();
    SDA_H; I2C_DELAY();
}

static void I2C_SendByte(u8 byte) {
    u8 i;
    for(i=0; i<8; i++) {
        if(byte & 0x80) SDA_H;
        else SDA_L;
        byte <<= 1;
        SCL_H; I2C_DELAY();
        SCL_L; I2C_DELAY();
    }
    SDA_H; SCL_H; I2C_DELAY(); SCL_L; I2C_DELAY(); 
}

static void LCD_Write(u8 data, u8 rs) {
    u8 high_nibble = data & 0xF0;
    u8 low_nibble = (data << 4) & 0xF0;
    u8 bl = 0x08; 

    I2C_Start();
    I2C_SendByte(LCD_ADDR);
    
    // 发送高4位 (EN脉冲大幅缩短)
    I2C_SendByte(high_nibble | bl | 0x04 | rs); 
    I2C_DELAY(); // 极短使能脉冲
    I2C_SendByte(high_nibble | bl | 0x00 | rs); 

    // 发送低4位
    I2C_SendByte(low_nibble | bl | 0x04 | rs); 
    I2C_DELAY(); // 极短使能脉冲
    I2C_SendByte(low_nibble | bl | 0x00 | rs); 
    
    I2C_Stop();
}

// 缩短 LCD 执行指令的死区等待时间
static void LCD_WriteCmd(u8 cmd) { LCD_Write(cmd, 0); Delay_ms(1); }
static void LCD_WriteData(u8 data) { LCD_Write(data, 1); Delay_us(30); } // 从 50us 压榨到 30us

void LCD_Clear(void) { LCD_WriteCmd(0x01); Delay_ms(2); }

void LCD_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    // 设为推挽输出可以提高I2C边沿速度（因为你没外接上拉电阻）
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    SDA_H; SCL_H;
    Delay_ms(20);
    
    LCD_WriteCmd(0x33);
    LCD_WriteCmd(0x32);
    LCD_WriteCmd(0x28); 
    LCD_WriteCmd(0x0C); 
    LCD_WriteCmd(0x06); 
    LCD_WriteCmd(0x01); 
    Delay_ms(2);
}

void LCD_ShowString(u8 x, u8 y, char *str) {
    u8 addr = (y == 0) ? (0x00 + x) : (0x40 + x);
    LCD_WriteCmd(addr | 0x80);
    while (*str != '\0') {
        LCD_WriteData(*str);
        str++;
    }
}
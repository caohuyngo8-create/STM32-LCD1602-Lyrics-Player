#include "stm32f10x.h"
#include "delay.h"
#include "I2CLCD.h"
#include "string.h"

// 定义歌词结构体：把文本和时间绑定在一起，方便管理
typedef struct {
    char text[17];     // 歌词文本（最大16字符，千万不能超）
    uint16_t time_ms;  // 这句歌词停留的时间（毫秒，即唱这句需要多久）
} Lyric_t;

// 填入 Alex Warren - Carry You Home 的歌词与大概节拍
Lyric_t carry_you_home[] = {
    // ---- 副歌部分 (Chorus) ----
    {"Oh I hope u know", 1450}, // Oh, I hope you know,
    {"I will carry you", 1100}, // I will carry you
    {"home            ", 1100}, // home.
    {"Whether tonight ", 1600}, // Whether it's tonight
    {"or 55 years down", 1400}, // or fifty-five years down
    {"the road        ", 1200}, // the road.
    {"Oh I know there's", 1400}, // Oh, I know there's
    {"so many ways    ", 1100}, // so many ways that
    {"this could go   ", 2200}, // this could go.
    {"Dont want u to  ", 900}, // Don't want you to
    {"wonder darling  ", 1800}, // wonder, darling,
    {"I need u to know", 2100}, // I need you to know.
    
    // ---- 主歌部分 (Verse 1) ----
    {"We were talking ", 1500}, // We were talking
    {"to the sunset   ", 1800}, // to the sunset.
    {"Throwing dreams ", 1500}, // Throwing dreams
    {"against the wall", 3000}, // against the wall
    {"I know none of  ", 1300}, // I know none of
    {"them stuck yet  ", 2200}, // them have stuck yet.
    {"But I bet it on ", 950}, // But I bet it on
    {"you honey       ", 1000}, // you, honey.
    {"Oh I would risk ", 700}, // Oh, I would risk
    {"it all          ", 1100}, // it all.
    

};

// 记录屏幕上两行当前的内容（初始化为16个空格）
char line1_buf[17] = "                "; 
char line2_buf[17] = "                ";

// 真正的 KTV 双行滚动刷新核心逻辑
void Play_Next_Lyric(char* next_lyric, uint16_t delay_ms) {
    // 1. 把第二行的歌词直接顶到第一行
    strcpy(line1_buf, line2_buf);
    
    // 2. 把新歌词放入第二行（不足16个字符用空格补齐，直接覆盖掉上一句的残留字母）
    uint8_t len = strlen(next_lyric);
    for(int i = 0; i < 16; i++) {
        if(i < len) {
            line2_buf[i] = next_lyric[i];
        } else {
            line2_buf[i] = ' '; // 用空格强制覆盖
        }
    }
    line2_buf[16] = '\0'; // 补充字符串结束符
    
    // 3. 刷新屏幕
    // 注意：这里不用 LCD_Clear()，直接用两行完整的16个字符硬覆盖，视觉效果最丝滑
    LCD_ShowString(0, 0, line1_buf);
    LCD_ShowString(0, 1, line2_buf);
    
    // 4. 等待这句歌词唱完
    Delay_ms(delay_ms);
}

int main(void) {
    SystemInit(); // 初始化系统时钟为 72MHz
    
    LCD_Init();   // 初始化 I2C LCD1602
    LCD_Clear();
    
    
    
    // 自动计算结构体数组里的歌词总行数
    uint16_t total_lines = sizeof(carry_you_home) / sizeof(Lyric_t);
    
    while(1) {
        // 遍历播放每一句歌词
        for(uint16_t i = 0; i < total_lines; i++) {
            Play_Next_Lyric(carry_you_home[i].text, carry_you_home[i].time_ms);
        }
        
        // 显示结束信息
        LCD_ShowString(0, 0, " Thanks for     ");
        LCD_ShowString(0, 1, "      Listening ");
        Delay_ms(3000); // 展示3秒
        
        // 也可以加上你的署名
        LCD_Clear();
        LCD_ShowString(0, 0, "Carry You Home");
        LCD_ShowString(1, 1, "By Alex Warren"); 
        Delay_ms(3000);

        // 4. 【重置状态】 为下一轮循环做准备
        strcpy(line1_buf, "                ");
        strcpy(line2_buf, "                ");
        LCD_Clear();
        Delay_ms(1000); 
    }
}
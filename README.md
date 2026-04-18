# STM32 LCD1602 Lyrics Player 🎵

Welcome to my first GitHub project! This is an embedded system project based on the STM32F103C8T6 microcontroller, which drives a 3.3V LCD1602A display (via an I2C adapter) to show dual-line KTV-style scrolling lyrics. 

Currently playing: **Alex Warren - Carry You Home** 🎤

## ✨ Features
- **I2C Communication:** Custom, highly optimized software I2C driver for PCF8574, saving GPIO pins.
- **KTV-Style Dual Line Scroll:** Smoothly transitions lyrics from the bottom line to the top line.
- **Space-Padding Overwrite:** Achieves seamless refreshing without using `LCD_Clear()`, eliminating screen flicker.
- **Time Sync:** Lyrics are synchronized with millisecond precision to match the original song's beat.

## 🛠 Hardware Required
- **MCU:** STM32F103C8T6 Minimum System Board
- **Display:** 3.3V LCD1602A with HW-61 I2C Adapter (PCF8574)
- **Debugger:** ST-Link V2
- **Wires:** 4 Jumper Wires (Female-to-Female)

## 🔌 Wiring Guide
| I2C Adapter | STM32F103C8T6 | Note |
| :---: | :---: | :--- |
| VCC | 3.3V | *Do not connect to 5V!* |
| GND | GND | Common ground |
| SDA | PB7 | Data Line |
| SCL | PB6 | Clock Line |

*Note: Please adjust the blue potentiometer on the back of the I2C adapter if the screen lights up but shows no text.*

## 🚀 How to Use
1. Clone or download this repository.
2. Open the project using **Keil uVision 5**.
3. Compile the code (F7) ensuring `0 Errors`.
4. Download to your STM32 via ST-Link.
5. Enjoy the lyrics! 

---
*Created by Zankley - 2026*

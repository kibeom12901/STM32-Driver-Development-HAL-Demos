#ifndef __CLCD_H
#define __CLCD_H

#include "stm32f4xx_hal.h"

/* ===== Pin mapping (STM32F407, Port E) ===== */
#define GPIO_PORT_LCD      GPIOE

#define GPIO_PIN_RS        GPIO_PIN_0
#define GPIO_PIN_EN        GPIO_PIN_1
#define GPIO_PIN_RW        GPIO_PIN_2   /* tie RW to GND if you prefer write-only */

#define GPIO_PIN_D4        GPIO_PIN_4
#define GPIO_PIN_D5        GPIO_PIN_5
#define GPIO_PIN_D6        GPIO_PIN_6
#define GPIO_PIN_D7        GPIO_PIN_7

/* ===== API (same naming style as the video) ===== */
void CLCD_GPIO_Init(void);                       // MCU GPIO init
void CLCD_Init(void);                            // LCD init (4-bit mode)

void CLCD_Write_Instruction(unsigned char b);    // RS=0
void CLCD_Write_Display(unsigned char b);        // RS=1

void CLCD_Clear(void);
void CLCD_GotoXY(unsigned char x, unsigned char y);
void CLCD_Puts(unsigned char x, unsigned char y, unsigned char *str);
void CLCD_Print(unsigned char *str);             // prints at current cursor

#endif /* __CLCD_H */

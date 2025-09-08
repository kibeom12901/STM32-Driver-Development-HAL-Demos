#include "clcd.h"

/* --- tiny helpers --- */
static inline void _delay_ms(uint32_t ms) { HAL_Delay(ms); }

static void _pulse_en(void) {
    HAL_GPIO_WritePin(GPIO_PORT_LCD, GPIO_PIN_EN, GPIO_PIN_SET);
    _delay_ms(1);
    HAL_GPIO_WritePin(GPIO_PORT_LCD, GPIO_PIN_EN, GPIO_PIN_RESET);
    _delay_ms(1);
}

/* D4..D7 <= low nibble (bit0..bit3 of 'nib') */
static void _write4(uint8_t nib) {
    HAL_GPIO_WritePin(GPIO_PORT_LCD, GPIO_PIN_D4, (nib & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIO_PORT_LCD, GPIO_PIN_D5, (nib & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIO_PORT_LCD, GPIO_PIN_D6, (nib & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIO_PORT_LCD, GPIO_PIN_D7, (nib & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    _pulse_en();
}

/* ===== GPIO init (like the tutorial) ===== */
void CLCD_GPIO_Init(void) {
    __HAL_RCC_GPIOE_CLK_ENABLE();

    GPIO_InitTypeDef gi = {0};
    gi.Pin   = GPIO_PIN_RS | GPIO_PIN_EN | GPIO_PIN_RW |
               GPIO_PIN_D4 | GPIO_PIN_D5 | GPIO_PIN_D6 | GPIO_PIN_D7;
    gi.Mode  = GPIO_MODE_OUTPUT_PP;
    gi.Pull  = GPIO_NOPULL;
    gi.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIO_PORT_LCD, &gi);

    /* default levels */
    HAL_GPIO_WritePin(GPIO_PORT_LCD, GPIO_PIN_RS | GPIO_PIN_EN | GPIO_PIN_RW |
                                     GPIO_PIN_D4 | GPIO_PIN_D5 | GPIO_PIN_D6 | GPIO_PIN_D7,
                                     GPIO_PIN_RESET);
}

/* ===== LCD-side init (HD44780, 4-bit, 2-line) ===== */
void CLCD_Init(void) {
    _delay_ms(40);          // power-up wait

    /* force 8-bit init sequence (while we're still in unknown mode) */
    _write4(0x03); _delay_ms(5);
    _write4(0x03); _delay_ms(1);
    _write4(0x03); _delay_ms(1);

    /* switch to 4-bit interface */
    _write4(0x02);          // 4-bit

    /* now normal commands in 4-bit mode */
    CLCD_Write_Instruction(0x28);   // Function: 4-bit, 2 lines, 5x8
    CLCD_Write_Instruction(0x0C);   // Display ON, Cursor OFF, Blink OFF
    CLCD_Write_Instruction(0x06);   // Entry: increment, no shift
    CLCD_Clear();
}

/* ===== low-level send ===== */
void CLCD_Write_Instruction(unsigned char b) {
    HAL_GPIO_WritePin(GPIO_PORT_LCD, GPIO_PIN_RS, GPIO_PIN_RESET);   // RS=0
    HAL_GPIO_WritePin(GPIO_PORT_LCD, GPIO_PIN_RW, GPIO_PIN_RESET);   // RW=0 (write)

    _write4(b >> 4);     // high nibble
    _write4(b & 0x0F);   // low nibble
    _delay_ms(2);
}

void CLCD_Write_Display(unsigned char b) {
    HAL_GPIO_WritePin(GPIO_PORT_LCD, GPIO_PIN_RS, GPIO_PIN_SET);     // RS=1
    HAL_GPIO_WritePin(GPIO_PORT_LCD, GPIO_PIN_RW, GPIO_PIN_RESET);   // RW=0 (write)

    _write4(b >> 4);
    _write4(b & 0x0F);
    _delay_ms(2);
}

/* ===== high-level helpers ===== */
void CLCD_Clear(void) {
    CLCD_Write_Instruction(0x01);   // clear
    _delay_ms(2);
}

void CLCD_GotoXY(unsigned char x, unsigned char y) {
    uint8_t addr = (y == 0) ? 0x00 : 0x40;  // line 0 or 1
    addr += x;
    CLCD_Write_Instruction(0x80 | addr);
}

void CLCD_Puts(unsigned char x, unsigned char y, unsigned char *str) {
    CLCD_GotoXY(x, y);
    while (*str) CLCD_Write_Display(*str++);
}

void CLCD_Print(unsigned char *str) {
    while (*str) CLCD_Write_Display(*str++);
}

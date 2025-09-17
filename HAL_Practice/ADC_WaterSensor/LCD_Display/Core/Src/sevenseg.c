#include "sevenseg.h"
#include <string.h>

/* =============== Internal config/derivatives =============== */

#if SEVENSEG_COMMON_ANODE
  #define SEG_ON_LEVEL   GPIO_PIN_RESET   /* segments sink current */
  #define SEG_OFF_LEVEL  GPIO_PIN_SET
  #define DIG_ON_LEVEL   GPIO_PIN_SET     /* digit anodes high */
  #define DIG_OFF_LEVEL  GPIO_PIN_RESET
#else
  #define SEG_ON_LEVEL   GPIO_PIN_SET
  #define SEG_OFF_LEVEL  GPIO_PIN_RESET
  #define DIG_ON_LEVEL   GPIO_PIN_RESET   /* digit cathodes low */
  #define DIG_OFF_LEVEL  GPIO_PIN_SET
#endif

/* Segment order: bit0=a, bit1=b, bit2=c, bit3=d, bit4=e, bit5=f, bit6=g, bit7=dp
   These patterns are the "logical ON" bits. We invert as needed at drive time
   depending on COMMON_ANODE/COMMON_CATHODE. */
static const uint8_t glyphs_basic[16] = {
/* 0 */ 0b00111111,
/* 1 */ 0b00000110,
/* 2 */ 0b01011011,
/* 3 */ 0b01001111,
/* 4 */ 0b01100110,
/* 5 */ 0b01101101,
/* 6 */ 0b01111101,
/* 7 */ 0b00000111,
/* 8 */ 0b01111111,
/* 9 */ 0b01101111,
/* A */ 0b01110111,
/* b */ 0b01111100,
/* C */ 0b00111001,
/* d */ 0b01011110,
/* E */ 0b01111001,
/* F */ 0b01110001
};

static inline uint8_t ascii_to_seg(char c)
{
    switch (c) {
        case '0'...'9': return glyphs_basic[c - '0'];
        case 'A'...'F': return glyphs_basic[10 + (c - 'A')];
        case 'a'...'f': return glyphs_basic[10 + (c - 'a')];
        case '-':       return 0b01000000;  /* g */
        case '_':       return 0b00001000;  /* d */
        case ' ':       return 0b00000000;  /* blank */
        case 'H':       return 0b01110110;
        case 'h':       return 0b01110100;
        case 'L':       return 0b00111000;
        case 'n':       return 0b01010100;
        case 'o':       return 0b01011100;
        case 'P':       return 0b01110011;
        case 'r':       return 0b01010000;
        case 'u':       return 0b00011100;
        default:        return 0b00000000;  /* unsupported → blank */
    }
}

/* Digit pin array, left→right (DIG1..DIG4) */
static const uint16_t DIG_PIN[4] = {
    SEVENSEG_DIG1_PIN, SEVENSEG_DIG2_PIN, SEVENSEG_DIG3_PIN, SEVENSEG_DIG4_PIN
};

/* Current 4 bytes to display (logical ON bits). DP handled separately via mask. */
static volatile uint8_t s_buf[4] = {0};
static volatile uint8_t s_dp_mask = 0;
static volatile uint8_t s_brightness = 10; /* 0..10 */

/* Scan state */
static volatile uint8_t s_cur_digit = 0;
static volatile uint8_t s_frame_div = 0;

/* =============== Low-level drive helpers =============== */

static inline void all_digits_off(void)
{
    for (int i = 0; i < 4; ++i) {
        HAL_GPIO_WritePin(SEVENSEG_DIG_GPIO, DIG_PIN[i], DIG_OFF_LEVEL);
    }
}

static inline void write_segments(uint8_t seg_byte_with_dp)
{
    /* We map bit0..bit7 of seg_byte to PE0..PE7 */
    for (uint8_t bit = 0; bit < 8; ++bit) {
        GPIO_PinState level = (seg_byte_with_dp & (1u << bit)) ? SEG_ON_LEVEL : SEG_OFF_LEVEL;
        HAL_GPIO_WritePin(SEVENSEG_SEG_GPIO, (uint16_t)(1u << bit), level);
    }
}

/* =============== Public API =============== */

void SevenSeg_Init(void)
{
    /* Assume GPIO clocks and modes already set in MX_GPIO_Init() as outputs.
       Just blank everything. */
    SevenSeg_Clear();
    s_brightness = 10;
    s_cur_digit = 0;
    s_frame_div = 0;
}

void SevenSeg_Clear(void)
{
    /* Turn off all digits first, then clear segments. */
    all_digits_off();
    for (uint8_t bit = 0; bit < 8; ++bit) {
        HAL_GPIO_WritePin(SEVENSEG_SEG_GPIO, (uint16_t)(1u << bit), SEG_OFF_LEVEL);
    }
    memset((void*)s_buf, 0, sizeof(s_buf));
    s_dp_mask = 0;
}

void SevenSeg_SetBrightness(uint8_t level)
{
    if (level > 10) level = 10;
    s_brightness = level;
}

/* Copy 4 ASCII chars and dp mask */
void SevenSeg_SetText4(const char text4[4], uint8_t dp_mask)
{
    uint8_t tmp[4];
    for (int i = 0; i < 4; ++i) tmp[i] = ascii_to_seg(text4[i]);
    __disable_irq();
    for (int i = 0; i < 4; ++i) s_buf[i] = tmp[i];
    s_dp_mask = dp_mask;
    __enable_irq();
}

void SevenSeg_SetRaw(const uint8_t seg_bytes[4])
{
    __disable_irq();
    for (int i = 0; i < 4; ++i) s_buf[i] = seg_bytes[i];
    /* Keep existing DP mask (caller can bake DP into seg_bytes if desired) */
    __enable_irq();
}

void SevenSeg_ShowInt(int value, uint8_t zero_pad, uint8_t dp_mask)
{
    char out[4] = {' ', ' ', ' ', ' '};

    if (value < -999 || value > 9999) {
        out[0] = out[1] = out[2] = out[3] = '-';
        SevenSeg_SetText4(out, 0);
        return;
    }

    if (value < 0) {
        int v = -value; /* 0..999 */
        /* print -### into 4 chars */
        out[0] = '-';
        out[3] = (char)('0' + (v % 10)); v /= 10;
        out[2] = (v > 0) ? (char)('0' + (v % 10)) : ((zero_pad >= 3) ? '0' : ' ');
        v /= 10;
        out[1] = (v > 0) ? (char)('0' + (v % 10)) : ((zero_pad >= 2) ? '0' : ' ');
    } else {
        /* value 0..9999 */
        int v = value;
        out[3] = (char)('0' + (v % 10)); v /= 10;
        out[2] = (v > 0) ? (char)('0' + (v % 10)) : ((zero_pad >= 2) ? '0' : ' ');
        v /= 10;
        out[1] = (v > 0) ? (char)('0' + (v % 10)) : ((zero_pad >= 3) ? '0' : ' ');
        v /= 10;
        out[0] = (v > 0) ? (char)('0' + (v % 10)) : ((zero_pad >= 4) ? '0' : ' ');
    }

    SevenSeg_SetText4(out, dp_mask);
}

/* =============== Multiplex ISR tick =============== */

void SevenSeg_Tick(void)
{
    /* Simple brightness via frame skipping:
       s_brightness = 0..10 → draw this many ticks out of 10 */
    static uint8_t duty_ctr = 0;
    duty_ctr++;
    if (duty_ctr >= 10) duty_ctr = 0;

    /* Turn off current digit before changing segments */
    all_digits_off();

    /* Advance digit 0..3 at 1 kHz → 250 Hz per digit */
    s_cur_digit = (uint8_t)((s_cur_digit + 1) & 0x03);

    /* Decide whether to light this tick based on brightness */
    bool on_this_tick = (s_brightness > duty_ctr) && (s_brightness > 0);

    if (!on_this_tick) {
        /* Keep segments off this tick (digit remains off) */
        return;
    }

    /* Build segment byte + dp for the current digit */
    uint8_t seg = s_buf[s_cur_digit];
    if (s_dp_mask & (1u << s_cur_digit)) seg |= 0x80; /* dp bit7 on */

    /* Drive segments */
    write_segments(seg);

    /* Enable the active digit */
    HAL_GPIO_WritePin(SEVENSEG_DIG_GPIO, DIG_PIN[s_cur_digit], DIG_ON_LEVEL);
}

/* =============== Example: hook into HAL timer ISR =============== */
/*
In your stm32xx_it.c:

extern TIM_HandleTypeDef htimX; // the 1 kHz base timer you choose

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == htimX.Instance) {
        SevenSeg_Tick();
    }
}

And in your init code:
- Configure TIMX to fire update interrupt at SEVENSEG_TICK_HZ (1 kHz)
- HAL_TIM_Base_Start_IT(&htimX);
- SevenSeg_Init();
*/


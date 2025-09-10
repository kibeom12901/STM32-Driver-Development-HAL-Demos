#ifndef SEVENSEG_H
#define SEVENSEG_H

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

/* ===================== USER CONFIG ===================== */

/* Choose your module type */
#define SEVENSEG_COMMON_ANODE   1   /* set to 0 for common-cathode */

/* Segment wiring: a..g,dp mapped to PE0..PE7 by default */
#define SEVENSEG_SEG_GPIO       GPIOE
#define SEVENSEG_SEG_PINS_MASK  (GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3| \
                                 GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7)

/* Digit selects: default PC0..PC3 (DIG1..DIG4, left→right) */
#define SEVENSEG_DIG_GPIO       GPIOC
#define SEVENSEG_DIG1_PIN       GPIO_PIN_0
#define SEVENSEG_DIG2_PIN       GPIO_PIN_1
#define SEVENSEG_DIG3_PIN       GPIO_PIN_2
#define SEVENSEG_DIG4_PIN       GPIO_PIN_3

/* If you prefer PD12..PD15, comment the 4 lines above and use:
#define SEVENSEG_DIG_GPIO       GPIOD
#define SEVENSEG_DIG1_PIN       GPIO_PIN_12
#define SEVENSEG_DIG2_PIN       GPIO_PIN_13
#define SEVENSEG_DIG3_PIN       GPIO_PIN_14
#define SEVENSEG_DIG4_PIN       GPIO_PIN_15
*/

/* Tick rate in Hz for SevenSeg_Tick(). 1000 by default. */
#define SEVENSEG_TICK_HZ        1000u

/* ======================================================= */

/* Public API */
void SevenSeg_Init(void);

/* Set 4 ASCII characters: '0'..'9', ' ', '-', '_', 'A'..'F', etc.
   dp_mask: bit0→digit1, bit1→digit2, ... set bit to turn DP on for that digit */
void SevenSeg_SetText4(const char text4[4], uint8_t dp_mask);

/* Show a signed integer in range [-999..9999], with optional zero pad (2..4).
   dp_mask same as above. Out-of-range shows "----". */
void SevenSeg_ShowInt(int value, uint8_t zero_pad, uint8_t dp_mask);

/* Set raw segment patterns per digit (bit0=a ... bit6=g, bit7=dp).
   Bits represent the SEG line logic (before module inversion). */
void SevenSeg_SetRaw(const uint8_t seg_bytes[4]);

/* Brightness 0..10 (0=off, 10=max). Implemented by simple frame skipping. */
void SevenSeg_SetBrightness(uint8_t level);

/* Multiplex ISR tick. Call from a 1 kHz (SEVENSEG_TICK_HZ) timer IRQ. */
void SevenSeg_Tick(void);

/* Optional helper to blank the display immediately. */
void SevenSeg_Clear(void);

#endif /* SEVENSEG_H */

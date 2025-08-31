#ifndef STM32F407XX_GPIO_DRIVER_H
#define STM32F407XX_GPIO_DRIVER_H

#include "stm32f407xx.h"

#define GPIO_PIN_0  0U
#define GPIO_PIN_1  1U
#define GPIO_PIN_2  2U
#define GPIO_PIN_3  3U
#define GPIO_PIN_4  4U
#define GPIO_PIN_5  5U
#define GPIO_PIN_6  6U
#define GPIO_PIN_7  7U
#define GPIO_PIN_8  8U
#define GPIO_PIN_9  9U
#define GPIO_PIN_10 10U
#define GPIO_PIN_11 11U
#define GPIO_PIN_12 12U
#define GPIO_PIN_13 13U
#define GPIO_PIN_14 14U
#define GPIO_PIN_15 15U

#define GPIO_MODE_IN      0U
#define GPIO_MODE_OUT     1U
#define GPIO_MODE_ALTFN   2U
#define GPIO_MODE_ANALOG  3U

#define GPIO_OP_TYPE_PP   0U
#define GPIO_OP_TYPE_OD   1U

#define GPIO_SPEED_LOW    0U
#define GPIO_SPEED_MED    1U
#define GPIO_SPEED_HIGH   2U
#define GPIO_SPEED_VHIGH  3U

#define GPIO_NO_PUPD      0U
#define GPIO_PIN_PU       1U
#define GPIO_PIN_PD       2U

typedef struct {
  uint8_t GPIO_PinNumber;
  uint8_t GPIO_PinMode;
  uint8_t GPIO_PinSpeed;
  uint8_t GPIO_PinPuPdControl;
  uint8_t GPIO_PinOPType;
  uint8_t GPIO_PinAltFunMode;
} GPIO_PinConfig_t;

typedef struct {
  GPIO_RegDef_t *pGPIOx;
  GPIO_PinConfig_t GPIO_PinConfig;
} GPIO_Handle_t;

void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t En);
void GPIO_Init(GPIO_Handle_t *pGPIOHandle);

#endif

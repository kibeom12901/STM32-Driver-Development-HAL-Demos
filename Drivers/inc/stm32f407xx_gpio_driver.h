#ifndef STM32F407XX_GPIO_DRIVER_H
#define STM32F407XX_GPIO_DRIVER_H
#include "stm32f407xx.h"
#include <stdint.h>

#define GPIO_MODE_IN      0
#define GPIO_MODE_OUT     1
#define GPIO_MODE_ALTFN   2
#define GPIO_MODE_ANALOG  3

#define GPIO_OP_TYPE_PP   0
#define GPIO_OP_TYPE_OD   1

#define GPIO_SPEED_LOW    0
#define GPIO_SPEED_MEDIUM 1
#define GPIO_SPEED_FAST   2
#define GPIO_SPEED_HIGH   3

#define GPIO_NO_PUPD 0
#define GPIO_PIN_PU  1
#define GPIO_PIN_PD  2

#define GPIO_PIN_NO_0   0
#define GPIO_PIN_NO_6   6
#define GPIO_PIN_NO_9   9

typedef struct{
  uint8_t GPIO_PinNumber;
  uint8_t GPIO_PinMode;
  uint8_t GPIO_PinSpeed;
  uint8_t GPIO_PinPuPdControl;
  uint8_t GPIO_PinOPType;
  uint8_t GPIO_PinAltFunMode;
} GPIO_PinConfig_t;

typedef struct{
  GPIO_RegDef_t *pGPIOx;
  GPIO_PinConfig_t GPIO_PinConfig;
} GPIO_Handle_t;

void    GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnOrDi);
void    GPIO_Init(GPIO_Handle_t *pGPIOHandle);
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);
#endif

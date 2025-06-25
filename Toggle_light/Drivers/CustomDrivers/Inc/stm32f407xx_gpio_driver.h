#ifndef INC_GPIO_DRIVER_H
#define INC_GPIO_DRIVER_H

#include "stm32f407xx.h"

#define GPIO_PIN_NO_12        12
#define GPIO_MODE_OUT         1
#define GPIO_SPEED_FAST       2
#define GPIO_OP_TYPE_PP       0
#define GPIO_NO_PUPD          0
#define ENABLE                1
#define DISABLE               0

typedef struct
{
    uint8_t GPIO_PinNumber;
    uint8_t GPIO_PinMode;
    uint8_t GPIO_PinSpeed;
    uint8_t GPIO_PinPuPdControl;
    uint8_t GPIO_PinOPType;
} GPIO_PinConfig_t;

typedef struct
{
    GPIO_RegDef_t *pGPIOx;
    GPIO_PinConfig_t GPIO_PinConfig;
} GPIO_Handle_t;

void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnOrDi);
void GPIO_Init(GPIO_Handle_t *pGPIOHandle);
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);

#endif

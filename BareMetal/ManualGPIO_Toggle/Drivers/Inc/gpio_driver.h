#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#include "stm32f4xx_custom.h"

typedef struct {
    uint8_t GPIO_PinNumber;
    uint8_t GPIO_PinMode;
    uint8_t GPIO_PinPuPdControl;
} GPIO_PinConfig_t;

typedef struct {
    GPIO_RegDef_t *pGPIOx;
    GPIO_PinConfig_t GPIO_PinConfig;
} GPIO_Handle_t;

#define GPIO_MODE_IN   0
#define GPIO_MODE_OUT  1
#define GPIO_NO_PUPD   0
#define GPIO_PIN_NO_0  0
#define GPIO_PIN_NO_12 12

void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnOrDi);
void GPIO_Init(GPIO_Handle_t *pGPIOHandle);
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);

#endif /* GPIO_DRIVER_H */

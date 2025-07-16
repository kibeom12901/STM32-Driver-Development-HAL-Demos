#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#include "stm32f4xx_custom.h"
#include <stdint.h>

/* Pin modes */
#define GPIO_MODE_IN      0
#define GPIO_MODE_OUT     1
#define GPIO_MODE_ALTFN   2
#define GPIO_MODE_ANALOG  3
/* EXTI modes */
#define GPIO_MODE_IT_FT   4
#define GPIO_MODE_IT_RT   5
#define GPIO_MODE_IT_RFT  6

/* Output type */
#define GPIO_OP_TYPE_PP   0
#define GPIO_OP_TYPE_OD   1

/* Speed */
#define GPIO_SPEED_LOW    0
#define GPIO_SPEED_MEDIUM 1
#define GPIO_SPEED_FAST   2
#define GPIO_SPEED_HIGH   3

/* Pull-up/down */
#define GPIO_NO_PUPD      0
#define GPIO_PIN_PU       1
#define GPIO_PIN_PD       2

/* IRQ numbers for EXTI */
#define IRQ_NO_EXTI0      6
#define IRQ_NO_EXTI1      7
#define IRQ_NO_EXTI2      8
#define IRQ_NO_EXTI3      9
#define IRQ_NO_EXTI4      10
#define IRQ_NO_EXTI9_5    23
#define IRQ_NO_EXTI15_10  40

/* Pin-config struct */
typedef struct {
    uint8_t GPIO_PinNumber;
    uint8_t GPIO_PinMode;
    uint8_t GPIO_PinSpeed;
    uint8_t GPIO_PinPuPdControl;
    uint8_t GPIO_PinOPType;
    uint8_t GPIO_PinAltFunMode;
} GPIO_PinConfig_t;

/* Handle */
typedef struct {
    GPIO_RegDef_t *pGPIOx;
    GPIO_PinConfig_t GPIO_PinConfig;
} GPIO_Handle_t;

/* APIs */
void GPIO_PeriClockControl(GPIO_RegDef_t*, uint8_t);
void GPIO_Init(GPIO_Handle_t*);
void GPIO_DeInit(GPIO_RegDef_t*);
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t*, uint8_t);
void    GPIO_ToggleOutputPin(GPIO_RegDef_t*, uint8_t);
void    GPIO_IRQConfig(uint8_t, uint8_t);
void    GPIO_IRQPriorityConfig(uint8_t, uint8_t);
void    GPIO_IRQHandling(uint8_t);

#endif /* GPIO_DRIVER_H */

#include <string.h>
#include "gpio_driver.h"

static void delay(void) {
    for (volatile uint32_t i = 0; i < 500000; ++i) { }
}

/* EXTI line 0 interrupt handler (PA0 → IRQ 6) */
void EXTI0_IRQHandler(void) {
    delay();                             // debounce
    GPIO_IRQHandling(GPIO_PIN_NO_0);     // clear EXTI0 pending
    GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_12);
}

int main(void) {
    GPIO_Handle_t GpioLed, GPIOBtn;

    /* zero out handles */
    memset(&GpioLed, 0, sizeof(GpioLed));
    memset(&GPIOBtn,  0, sizeof(GPIOBtn));

    /* PD12 output (LED) */
    GpioLed.pGPIOx = GPIOD;
    GpioLed.GPIO_PinConfig.GPIO_PinNumber      = GPIO_PIN_NO_12;
    GpioLed.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_OUT;
    GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
    GPIO_Init(&GpioLed);

    /* PA0 input w/ EXTI rising edge (user button) */
    GPIOBtn.pGPIOx = GPIOA;
    GPIOBtn.GPIO_PinConfig.GPIO_PinNumber      = GPIO_PIN_NO_0;
    GPIOBtn.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_IT_RT;
    GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
    GPIO_Init(&GPIOBtn);

    /* IRQ setup: EXTI0 is IRQ number 6 */
    GPIO_IRQPriorityConfig(IRQ_NO_EXTI0, NVIC_IRQ_PRI15);
    GPIO_IRQConfig        (IRQ_NO_EXTI0, ENABLE);

    while (1) {
        /* just wait for interrupts */
    }
}

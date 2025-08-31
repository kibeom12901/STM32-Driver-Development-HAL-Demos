#include "gpio_driver.h"

int main(void)
{
    GPIO_Handle_t GpioLed, GPIOBtn;

    /* LED setup (PD12) */
    GpioLed.pGPIOx = GPIOD;
    GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
    GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
    GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

    GPIO_Init(&GpioLed);

    /* Button setup (PA0) */
    GPIOBtn.pGPIOx = GPIOA;
    GPIOBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
    GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
    GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

    GPIO_Init(&GPIOBtn);

    while (1) {
        if (GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0)) {
            for (int i = 0; i < 500000; i++); /* crude delay before toggle (debounce) */
            GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_12);
        }
    }

    return 0;
}

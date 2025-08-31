#include "stm32f407xx_gpio_driver.h"

void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t En)
{
    if (En == ENABLE) {
        if (pGPIOx == GPIOA) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
        else if (pGPIOx == GPIOB) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
        else if (pGPIOx == GPIOC) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    } else {
        if (pGPIOx == GPIOA) RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOAEN;
        else if (pGPIOx == GPIOB) RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOBEN;
        else if (pGPIOx == GPIOC) RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOCEN;
    }
}

void GPIO_Init(GPIO_Handle_t *h)
{
    GPIO_PeriClockControl(h->pGPIOx, ENABLE);

    uint32_t pin = h->GPIO_PinConfig.GPIO_PinNumber;

    /* Mode */
    h->pGPIOx->MODER &= ~(0x3U << (2U*pin));
    h->pGPIOx->MODER |=  ((uint32_t)h->GPIO_PinConfig.GPIO_PinMode << (2U*pin));

    /* Output type */
    h->pGPIOx->OTYPER &= ~(1U << pin);
    h->pGPIOx->OTYPER |=  ((uint32_t)h->GPIO_PinConfig.GPIO_PinOPType << pin);

    /* Speed */
    h->pGPIOx->OSPEEDR &= ~(0x3U << (2U*pin));
    h->pGPIOx->OSPEEDR |=  ((uint32_t)h->GPIO_PinConfig.GPIO_PinSpeed << (2U*pin));

    /* Pull-up/down */
    h->pGPIOx->PUPDR &= ~(0x3U << (2U*pin));
    h->pGPIOx->PUPDR |=  ((uint32_t)h->GPIO_PinConfig.GPIO_PinPuPdControl << (2U*pin));

    /* Alt function (if needed) */
    if (h->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN) {
        uint8_t idx = pin / 8U;
        uint8_t pos = (pin % 8U) * 4U;
        h->pGPIOx->AFR[idx] &= ~(0xFU << pos);
        h->pGPIOx->AFR[idx] |=  ((uint32_t)(h->GPIO_PinConfig.GPIO_PinAltFunMode & 0xF) << pos);
    }
}

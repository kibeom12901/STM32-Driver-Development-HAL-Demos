#include "gpio_driver.h"

void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnOrDi){
    if (pGPIOx == GPIOA) { 
        EnOrDi ? (RCC->AHB1ENR |= (1<<0)) 
               : (RCC->AHB1ENR &= ~(1<<0)); 
    } else if (pGPIOx == GPIOB){ 
        EnOrDi ? (RCC->AHB1ENR |= (1<<1)) 
               : (RCC->AHB1ENR &= ~(1<<1)); 
    } else if (pGPIOx == GPIOC){ 
        EnOrDi ? (RCC->AHB1ENR |= (1<<2)) 
               : (RCC->AHB1ENR &= ~(1<<2)); 
    }
}

void GPIO_Init(GPIO_Handle_t *pH){
    uint8_t pin  = pH->GPIO_PinConfig.GPIO_PinNumber;
    uint8_t mode = pH->GPIO_PinConfig.GPIO_PinMode;

    GPIO_PeriClockControl(pH->pGPIOx, ENABLE);

    pH->pGPIOx->MODER   &= ~(0x3U << (2*pin));
    pH->pGPIOx->MODER   |=  (mode  << (2*pin));

    pH->pGPIOx->OTYPER  &= ~(1U << pin);
    pH->pGPIOx->OTYPER  |=  (pH->GPIO_PinConfig.GPIO_PinOPType << pin);

    pH->pGPIOx->OSPEEDR &= ~(0x3U << (2*pin));
    pH->pGPIOx->OSPEEDR |=  (pH->GPIO_PinConfig.GPIO_PinSpeed << (2*pin));

    pH->pGPIOx->PUPDR   &= ~(0x3U << (2*pin));
    pH->pGPIOx->PUPDR   |=  (pH->GPIO_PinConfig.GPIO_PinPuPdControl << (2*pin));

    if (mode == GPIO_MODE_ALTFN) {
        uint8_t idx = pin/8, pos = (pin%8)*4;
        pH->pGPIOx->AFR[idx] &= ~(0xFU << pos);
        pH->pGPIOx->AFR[idx] |=  (pH->GPIO_PinConfig.GPIO_PinAltFunMode << pos);
    }
}

void GPIO_DeInit(GPIO_RegDef_t *pGPIOx){
    if(pGPIOx==GPIOA){ RCC->AHB1RSTR |= (1<<0); RCC->AHB1RSTR &= ~(1<<0); }
    else if(pGPIOx==GPIOB){ RCC->AHB1RSTR |= (1<<1); RCC->AHB1RSTR &= ~(1<<1); }
    else if(pGPIOx==GPIOC){ RCC->AHB1RSTR |= (1<<2); RCC->AHB1RSTR &= ~(1<<2); }
}

uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber){
    return (uint8_t)((pGPIOx->IDR >> PinNumber) & 0x1);
}

void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber){
    pGPIOx->ODR ^= (1 << PinNumber);
}

#include "gpio_driver.h"

/* clock enable/disable */
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnOrDi){
    if (pGPIOx==GPIOA) { if(EnOrDi) RCC->AHB1ENR |= 1<<0; else RCC->AHB1ENR &= ~(1<<0);} 
    if (pGPIOx==GPIOB) { if(EnOrDi) RCC->AHB1ENR |= 1<<1; else RCC->AHB1ENR &= ~(1<<1);} 
    if (pGPIOx==GPIOC) { if(EnOrDi) RCC->AHB1ENR |= 1<<2; else RCC->AHB1ENR &= ~(1<<2);} 
    if (pGPIOx==GPIOD) { if(EnOrDi) RCC->AHB1ENR |= 1<<3; else RCC->AHB1ENR &= ~(1<<3);} 
}

/* init or exti */
void GPIO_Init(GPIO_Handle_t *pH){
    uint32_t pin = pH->GPIO_PinConfig.GPIO_PinNumber;
    uint32_t mode= pH->GPIO_PinConfig.GPIO_PinMode;
    /* 1. clk */
    GPIO_PeriClockControl(pH->pGPIOx, ENABLE);
    /* 2. MODE */
    pH->pGPIOx->MODER &= ~(0x3U << (2*pin));
    pH->pGPIOx->MODER |=  (mode   << (2*pin));
    /* 3. TYPE */
    pH->pGPIOx->OTYPER &= ~(1U<<pin);
    pH->pGPIOx->OTYPER |=  (pH->GPIO_PinConfig.GPIO_PinOPType << pin);
    /* 4. SPEED */
    pH->pGPIOx->OSPEEDR &= ~(0x3U << (2*pin));
    pH->pGPIOx->OSPEEDR |=  (pH->GPIO_PinConfig.GPIO_PinSpeed  << (2*pin));
    /* 5. PUPD */
    pH->pGPIOx->PUPDR   &= ~(0x3U << (2*pin));
    pH->pGPIOx->PUPDR   |=  (pH->GPIO_PinConfig.GPIO_PinPuPdControl << (2*pin));
    /* 6. ALT FN */
    if (mode == GPIO_MODE_ALTFN){
        uint8_t idx = pin/8, pos=(pin%8)*4;
        pH->pGPIOx->AFR[idx] &= ~(0xFU<<pos);
        pH->pGPIOx->AFR[idx] |=  (pH->GPIO_PinConfig.GPIO_PinAltFunMode<<pos);
    }
    /* 7. EXTI */
    if (mode>=GPIO_MODE_IT_FT && mode<=GPIO_MODE_IT_RFT){
        /* edge */
        if (mode==GPIO_MODE_IT_FT){ EXTI->FTSR |= 1<<pin; EXTI->RTSR &= ~(1<<pin);} 
        else if(mode==GPIO_MODE_IT_RT){EXTI->RTSR|=1<<pin; EXTI->FTSR&=~(1<<pin);} 
        else {EXTI->FTSR|=1<<pin; EXTI->RTSR|=1<<pin;} 
        /* SYSCFG map */
        uint8_t idx2=pin/4, pos2=(pin%4)*4;
        uint8_t code = (pH->pGPIOx==GPIOA?0:(pH->pGPIOx==GPIOB?1:(pH->pGPIOx==GPIOC?2:3)));
        RCC->APB2ENR |= 1<<14;
        SYSCFG->EXTICR[idx2] &= ~(0xFU<<pos2);
        SYSCFG->EXTICR[idx2] |=  code<<pos2;
        EXTI->IMR |= 1<<pin;
    }
}

/* de-init */
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx){
    if(pGPIOx==GPIOA){ RCC->AHB1RSTR|=1<<0; RCC->AHB1RSTR&=~(1<<0);} 
    if(pGPIOx==GPIOB){ RCC->AHB1RSTR|=1<<1; RCC->AHB1RSTR&=~(1<<1);} 
    if(pGPIOx==GPIOC){ RCC->AHB1RSTR|=1<<2; RCC->AHB1RSTR&=~(1<<2);} 
    if(pGPIOx==GPIOD){ RCC->AHB1RSTR|=1<<3; RCC->AHB1RSTR&=~(1<<3);} 
}

/* read pin */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber){
    return (pGPIOx->IDR >> PinNumber)&0x1;
}

/* toggle */
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber){
    pGPIOx->ODR ^= 1<<PinNumber;
}

/* IRQ config */
void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t EnOrDi){
    if (EnOrDi){
        if(IRQNumber<=31) *NVIC_ISER0 |= 1<<IRQNumber;
        else              *NVIC_ISER1 |= 1<<(IRQNumber%32);
    } else {
        if(IRQNumber<=31) *NVIC_ICER0 |= 1<<IRQNumber;
        else              *NVIC_ICER1 |= 1<<(IRQNumber%32);
    }
}

/* IRQ priority */
void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority){
    uint8_t iprx=IRQNumber/4, section=IRQNumber%4;
    uint8_t shift=(8*section)+(8-NO_PR_BITS_IMPLEMENTED);
    *(NVIC_PR_BASE+iprx) |= (IRQPriority<<shift);
}

/* clear pending */
void GPIO_IRQHandling(uint8_t PinNumber){
    if (EXTI->PR & (1<<PinNumber)) EXTI->PR |= 1<<PinNumber;
}

#include "gpio_driver.h"

void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnOrDi) {
    if (EnOrDi == ENABLE) {
        if      (pGPIOx == GPIOA) GPIOA_PCLK_EN();
        else if (pGPIOx == GPIOD) GPIOD_PCLK_EN();
    }
}

void GPIO_Init(GPIO_Handle_t *pGPIOHandle) {
    uint32_t temp;

    /* 1. Enable GPIO clock */
    GPIO_PeriClockControl(pGPIOHandle->pGPIOx, ENABLE);

    /* 2. Configure mode */
    if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_OUT) {
        temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <<
               (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
        pGPIOHandle->pGPIOx->MODER &=
           ~(0x3 << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
        pGPIOHandle->pGPIOx->MODER |= temp;
    } else {
        /* 3. EXTI trigger setup */
        if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FT) {
            EXTI->FTSR |=  (1U << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
            EXTI->RTSR &= ~(1U << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
        } else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RT) {
            EXTI->RTSR |=  (1U << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
            EXTI->FTSR &= ~(1U << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
        } else { /* both edges */
            EXTI->RTSR |= (1U << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
            EXTI->FTSR |= (1U << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
        }

        /* 4. SYSCFG mapping */
        uint8_t idx  = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 4;
        uint8_t off  = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 4;
        uint8_t code = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx);

        SYSCFG_PCLK_EN();
        SYSCFG->EXTICR[idx] &= ~(0xF << (off * 4));
        SYSCFG->EXTICR[idx] |=  (code << (off * 4));

        /* 5. Unmask */
        EXTI->IMR |= (1U << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

        /* 6. NVIC enable for all EXTI lines 0–4 */
        GPIO_IRQConfig(IRQ_NO_EXTI0 + pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber, ENABLE);
    }

    /* 7. Pull-up/down */
    temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl <<
           (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
    pGPIOHandle->pGPIOx->PUPDR &=
       ~(0x3 << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
    pGPIOHandle->pGPIOx->PUPDR |= temp;
}

uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber) {
    return (uint8_t)((pGPIOx->IDR >> PinNumber) & 0x1U);
}

void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber) {
    pGPIOx->ODR ^= (1U << PinNumber);
}

void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t EnOrDi) {
    if (EnOrDi == ENABLE) {
        if      (IRQNumber <= 31) *NVIC_ISER0 |=  (1U << IRQNumber);
        else if (IRQNumber <= 63) *NVIC_ISER1 |=  (1U << (IRQNumber % 32));
        else if (IRQNumber <= 95) *NVIC_ISER2 |=  (1U << (IRQNumber % 64));
        else                      *NVIC_ISER3 |=  (1U << (IRQNumber % 96));
    } else {
        if      (IRQNumber <= 31) *NVIC_ICER0 |=  (1U << IRQNumber);
        else if (IRQNumber <= 63) *NVIC_ICER1 |=  (1U << (IRQNumber % 32));
        else if (IRQNumber <= 95) *NVIC_ICER2 |=  (1U << (IRQNumber % 64));
        else                      *NVIC_ICER3 |=  (1U << (IRQNumber % 96));
    }
}

void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority) {
    uint8_t iprx         = IRQNumber / 4;
    uint8_t iprx_section = IRQNumber % 4;
    uint8_t shift_amt    = (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);

    *(NVIC_PR_BASE_ADDR + iprx) |= ((uint32_t)IRQPriority << shift_amt);
}

void GPIO_IRQHandling(uint8_t PinNumber) {
    if (EXTI->PR & (1U << PinNumber)) {
        EXTI->PR |= (1U << PinNumber);  // clear pending
    }
}

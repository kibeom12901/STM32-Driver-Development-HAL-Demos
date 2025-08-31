#ifndef STM32F407XX_H
#define STM32F407XX_H

#include <stdint.h>

/* =================== BASE ADDRESSES =================== */
#define PERIPH_BASE        0x40000000U
#define AHB1PERIPH_BASE    (PERIPH_BASE + 0x00020000U)
#define APB1PERIPH_BASE    (PERIPH_BASE + 0x00000000U)
#define APB2PERIPH_BASE    (PERIPH_BASE + 0x00010000U)

/* ======================= GPIO ========================= */
typedef struct {
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFR[2];
} GPIO_RegDef_t;

#define GPIOA ((GPIO_RegDef_t*)(AHB1PERIPH_BASE + 0x0000))
#define GPIOB ((GPIO_RegDef_t*)(AHB1PERIPH_BASE + 0x0400))
#define GPIOC ((GPIO_RegDef_t*)(AHB1PERIPH_BASE + 0x0800))
#define GPIOD ((GPIO_RegDef_t*)(AHB1PERIPH_BASE + 0x0C00))

/* ======================== RCC ========================= */
typedef struct {
    volatile uint32_t CR;
    volatile uint32_t PLLCFGR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t AHB1RSTR;
    volatile uint32_t AHB2RSTR;
    volatile uint32_t AHB3RSTR;
    volatile uint32_t __RES0;
    volatile uint32_t APB1RSTR;
    volatile uint32_t APB2RSTR;
    volatile uint32_t __RES1[2];
    volatile uint32_t AHB1ENR;
    volatile uint32_t AHB2ENR;
    volatile uint32_t AHB3ENR;
    volatile uint32_t __RES2;
    volatile uint32_t APB1ENR;
    volatile uint32_t APB2ENR;
    volatile uint32_t __RES3[2];
} RCC_RegDef_t;

#define RCC ((RCC_RegDef_t*)(AHB1PERIPH_BASE + 0x3800))

/* ======================== I2C ========================= */
typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t OAR1;
    volatile uint32_t OAR2;
    volatile uint32_t DR;
    volatile uint32_t SR1;
    volatile uint32_t SR2;
    volatile uint32_t CCR;
    volatile uint32_t TRISE;
    volatile uint32_t FLTR;
} I2C_RegDef_t;

#define I2C1 ((I2C_RegDef_t*)(APB1PERIPH_BASE + 0x5400))

/* ====================== GENERIC ======================= */
#define ENABLE  1
#define DISABLE 0

#endif // STM32F407XX_H

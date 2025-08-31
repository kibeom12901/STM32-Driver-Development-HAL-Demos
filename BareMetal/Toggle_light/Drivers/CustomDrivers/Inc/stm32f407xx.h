#ifndef INC_STM32F407XX_H
#define INC_STM32F407XX_H

#include <stdint.h>

#define PERIPH_BASE           0x40000000U
#define AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000U)
#define GPIOD_BASE            (AHB1PERIPH_BASE + 0x0C00U)
#define RCC_BASE              (AHB1PERIPH_BASE + 0x3800U)

typedef struct
{
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

typedef struct
{
    volatile uint32_t CR;
    volatile uint32_t PLLCFGR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t AHB1RSTR;
    volatile uint32_t RESERVED0[3];
    volatile uint32_t AHB1ENR;
} RCC_RegDef_t;

#define GPIOD   ((GPIO_RegDef_t *)GPIOD_BASE)
#define RCC     ((RCC_RegDef_t *)RCC_BASE)

#endif

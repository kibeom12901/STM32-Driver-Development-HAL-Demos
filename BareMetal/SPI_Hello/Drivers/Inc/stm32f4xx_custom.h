#ifndef STM32F4XX_CUSTOM_H
#define STM32F4XX_CUSTOM_H

#include <stdint.h>

/* ==== BUS BASES ==== */
#define PERIPH_BASE        0x40000000UL
#define AHB1PERIPH_BASE    (PERIPH_BASE + 0x00020000UL)
#define APB1PERIPH_BASE    (PERIPH_BASE + 0x00000000UL)
#define APB2PERIPH_BASE    (PERIPH_BASE + 0x00010000UL)

/* ==== GPIO ==== */
typedef struct {
    volatile uint32_t MODER, OTYPER, OSPEEDR, PUPDR;
    volatile uint32_t IDR,   ODR,    BSRR,   LCKR, AFR[2];
} GPIO_RegDef_t;
#define GPIOA   ((GPIO_RegDef_t*)(AHB1PERIPH_BASE + 0x0000))
#define GPIOB   ((GPIO_RegDef_t*)(AHB1PERIPH_BASE + 0x0400))
#define GPIOC   ((GPIO_RegDef_t*)(AHB1PERIPH_BASE + 0x0800))
#define GPIOD   ((GPIO_RegDef_t*)(AHB1PERIPH_BASE + 0x0C00))

/* ==== RCC ==== */
typedef struct {
    volatile uint32_t CR, PLLCFGR, CFGR, CIR;
    volatile uint32_t AHB1RSTR, AHB2RSTR, AHB3RSTR, __RES0;
    volatile uint32_t APB1RSTR, APB2RSTR, __RES1[2];
    volatile uint32_t AHB1ENR,  AHB2ENR,  AHB3ENR,  __RES2;
    volatile uint32_t APB1ENR,  APB2ENR,  __RES3[2];
} RCC_RegDef_t;
#define RCC     ((RCC_RegDef_t*)(AHB1PERIPH_BASE + 0x3800))

/* ==== SYSCFG & EXTI ==== */
typedef struct { volatile uint32_t MEMRMP, PMC, EXTICR[4]; } SYSCFG_RegDef_t;
#define SYSCFG  ((SYSCFG_RegDef_t*)(APB2PERIPH_BASE + 0x3800))
typedef struct { volatile uint32_t IMR, EMR, RTSR, FTSR, SWIER, PR; } EXTI_RegDef_t;
#define EXTI    ((EXTI_RegDef_t*)(APB2PERIPH_BASE + 0x3C00))

/* ==== NVIC ==== */
#define NVIC_ISER0   ((volatile uint32_t*)0xE000E100)
#define NVIC_ISER1   ((volatile uint32_t*)0xE000E104)
#define NVIC_ICER0   ((volatile uint32_t*)0xE000E180)
#define NVIC_ICER1   ((volatile uint32_t*)0xE000E184)
#define NVIC_PR_BASE ((volatile uint32_t*)0xE000E400)
#define NO_PR_BITS_IMPLEMENTED 4

/* ==== SPI ==== */
typedef struct {
    volatile uint32_t CR1, CR2, SR, DR, CRCPR, RXCRCR, TXCRCR, I2SCFGR, I2SPR;
} SPI_RegDef_t;
#define SPI1    ((SPI_RegDef_t*)(APB2PERIPH_BASE + 0x3000))
#define SPI2    ((SPI_RegDef_t*)(APB1PERIPH_BASE + 0x3800))
#define SPI3    ((SPI_RegDef_t*)(APB1PERIPH_BASE + 0x3C00))

/* SPI_CR2 bits */
#define SPI_CR2_SSOE 2U

/* ==== GENERIC ==== */
#define ENABLE   1
#define DISABLE  0

#endif /* STM32F4XX_CUSTOM_H */

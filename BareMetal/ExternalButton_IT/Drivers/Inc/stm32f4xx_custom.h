#ifndef STM32F4XX_CUSTOM_H
#define STM32F4XX_CUSTOM_H

#include <stdint.h>

/* Peripheral base addresses */
#define PERIPH_BASE         0x40000000UL
#define AHB1PERIPH_BASE     (PERIPH_BASE + 0x00020000UL)
#define APB2PERIPH_BASE     (PERIPH_BASE + 0x00010000UL)

#define GPIOA_BASE          (AHB1PERIPH_BASE + 0x0000UL)
#define GPIOB_BASE          (AHB1PERIPH_BASE + 0x0400UL)
#define GPIOC_BASE          (AHB1PERIPH_BASE + 0x0800UL)
#define GPIOD_BASE          (AHB1PERIPH_BASE + 0x0C00UL)

#define RCC_BASE            (AHB1PERIPH_BASE + 0x3800UL)
#define SYSCFG_BASE         (APB2PERIPH_BASE + 0x3800UL)
#define EXTI_BASE           (APB2PERIPH_BASE + 0x3C00UL)

/* GPIO register map */
typedef struct {
    volatile uint32_t MODER, OTYPER, OSPEEDR, PUPDR, IDR, ODR, BSRR, LCKR, AFR[2];
} GPIO_RegDef_t;

/* RCC register map */
typedef struct {
    volatile uint32_t CR, PLLCFGR, CFGR, CIR, AHB1RSTR, AHB2RSTR, AHB3RSTR;
    uint32_t RESERVED0;
    volatile uint32_t APB1RSTR, APB2RSTR;
    uint32_t RESERVED1[2];
    volatile uint32_t AHB1ENR, AHB2ENR, AHB3ENR;
    uint32_t RESERVED2;
    volatile uint32_t APB1ENR, APB2ENR;
} RCC_RegDef_t;

/* SYSCFG register map */
typedef struct {
    volatile uint32_t MEMRMP, PMC, EXTICR[4];
} SYSCFG_RegDef_t;

/* EXTI register map */
typedef struct {
    volatile uint32_t IMR, EMR, RTSR, FTSR, SWIER, PR;
} EXTI_RegDef_t;

/* Peripheral instances */
#define GPIOA   ((GPIO_RegDef_t*)GPIOA_BASE)
#define GPIOB   ((GPIO_RegDef_t*)GPIOB_BASE)
#define GPIOC   ((GPIO_RegDef_t*)GPIOC_BASE)
#define GPIOD   ((GPIO_RegDef_t*)GPIOD_BASE)
#define RCC     ((RCC_RegDef_t*)RCC_BASE)
#define SYSCFG  ((SYSCFG_RegDef_t*)SYSCFG_BASE)
#define EXTI    ((EXTI_RegDef_t*)EXTI_BASE)

/* NVIC ISER/ICER addresses */
#define NVIC_ISER0    ((volatile uint32_t*)0xE000E100)
#define NVIC_ISER1    ((volatile uint32_t*)0xE000E104)
#define NVIC_ISER2    ((volatile uint32_t*)0xE000E108)
#define NVIC_ISER3    ((volatile uint32_t*)0xE000E10C)
#define NVIC_ICER0    ((volatile uint32_t*)0xE000E180)
#define NVIC_ICER1    ((volatile uint32_t*)0xE000E184)
#define NVIC_ICER2    ((volatile uint32_t*)0xE000E188)
#define NVIC_ICER3    ((volatile uint32_t*)0xE000E18C)

/* NVIC priority config */
#define NO_PR_BITS_IMPLEMENTED   4
#define NVIC_PR_BASE_ADDR        ((volatile uint32_t*)0xE000E400)

/* IRQ numbers for EXTI lines */
#define IRQ_NO_EXTI0      6
#define IRQ_NO_EXTI1      7
#define IRQ_NO_EXTI2      8
#define IRQ_NO_EXTI3      9
#define IRQ_NO_EXTI4      10
#define IRQ_NO_EXTI9_5    23
#define IRQ_NO_EXTI15_10  40

/* Named NVIC priority values */
#define NVIC_IRQ_PRI0   0
#define NVIC_IRQ_PRI1   1
#define NVIC_IRQ_PRI2   2
#define NVIC_IRQ_PRI3   3
#define NVIC_IRQ_PRI4   4
#define NVIC_IRQ_PRI5   5
#define NVIC_IRQ_PRI6   6
#define NVIC_IRQ_PRI7   7
#define NVIC_IRQ_PRI8   8
#define NVIC_IRQ_PRI9   9
#define NVIC_IRQ_PRI10  10
#define NVIC_IRQ_PRI11  11
#define NVIC_IRQ_PRI12  12
#define NVIC_IRQ_PRI13  13
#define NVIC_IRQ_PRI14  14
#define NVIC_IRQ_PRI15  15

/* Convenience ENABLE/DISABLE */
#define ENABLE   1
#define DISABLE  0

/* Clock enable macros */
#define GPIOA_PCLK_EN()    (RCC->AHB1ENR |= (1 << 0))
#define GPIOD_PCLK_EN()    (RCC->AHB1ENR |= (1 << 3))
#define SYSCFG_PCLK_EN()   (RCC->APB2ENR |= (1 << 14))

/* Port-to-code for SYSCFG EXTICR */
#define GPIO_BASEADDR_TO_CODE(x)   ( (x == GPIOA) ? 0 : \
                                     (x == GPIOB) ? 1 : \
                                     (x == GPIOC) ? 2 : \
                                     (x == GPIOD) ? 3 : 0 )

#endif /* STM32F4XX_CUSTOM_H */

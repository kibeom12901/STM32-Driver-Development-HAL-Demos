#ifndef STM32F4XX_CUSTOM_H
#define STM32F4XX_CUSTOM_H

#include <stdint.h>

/* Peripheral Base Addresses */
#define PERIPH_BASE        0x40000000UL
#define AHB1PERIPH_BASE    (PERIPH_BASE + 0x00020000UL)

/* GPIO Base Addresses */
#define GPIOA_BASE         (AHB1PERIPH_BASE + 0x0000UL)
#define GPIOD_BASE         (AHB1PERIPH_BASE + 0x0C00UL)

/* RCC Base Address */
#define RCC_BASE           (AHB1PERIPH_BASE + 0x3800UL)

/* GPIO Register Definition */
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

/* RCC Register Definition */
typedef struct {
  volatile uint32_t CR;
  volatile uint32_t PLLCFGR;
  volatile uint32_t CFGR;
  volatile uint32_t CIR;
  volatile uint32_t AHB1RSTR;
  volatile uint32_t AHB2RSTR;
  volatile uint32_t AHB3RSTR;
  uint32_t RESERVED0;
  volatile uint32_t APB1RSTR;
  volatile uint32_t APB2RSTR;
  uint32_t RESERVED1[2];
  volatile uint32_t AHB1ENR;
  volatile uint32_t AHB2ENR;
  volatile uint32_t AHB3ENR;
  uint32_t RESERVED2;
  volatile uint32_t APB1ENR;
  volatile uint32_t APB2ENR;
} RCC_RegDef_t;

/* Peripheral Instances */
#define GPIOA ((GPIO_RegDef_t *)GPIOA_BASE)
#define GPIOD ((GPIO_RegDef_t *)GPIOD_BASE)
#define RCC   ((RCC_RegDef_t *)RCC_BASE)

#endif /* STM32F4XX_CUSTOM_H */

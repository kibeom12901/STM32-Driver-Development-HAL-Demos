#ifndef STM32F407XX_H
#define STM32F407XX_H

#include <stdint.h>

#define __IO volatile

/* ================= Cortex-M4 NVIC ================= */
typedef struct {
  __IO uint32_t ISER[8];      uint32_t RESERVED0[24];
  __IO uint32_t ICER[8];      uint32_t RESERVED1[24];
  __IO uint32_t ISPR[8];      uint32_t RESERVED2[24];
  __IO uint32_t ICPR[8];      uint32_t RESERVED3[24];
  __IO uint32_t IABR[8];      uint32_t RESERVED4[56];
  __IO uint8_t  IPR[240];
} NVIC_Type;
#define NVIC ((NVIC_Type*)0xE000E100UL)

/* ================== Base addresses ================= */
#define PERIPH_BASE        0x40000000UL
#define APB1PERIPH_BASE    (PERIPH_BASE + 0x00000000UL)
#define APB2PERIPH_BASE    (PERIPH_BASE + 0x00010000UL)
#define AHB1PERIPH_BASE    (PERIPH_BASE + 0x00020000UL)

/* ================== RCC ================== */
typedef struct {
  __IO uint32_t CR;       __IO uint32_t PLLCFGR; __IO uint32_t CFGR;   __IO uint32_t CIR;
  __IO uint32_t AHB1RSTR; __IO uint32_t AHB2RSTR;__IO uint32_t AHB3RSTR; uint32_t RESERVED0;
  __IO uint32_t APB1RSTR; __IO uint32_t APB2RSTR; uint32_t RESERVED1[2];
  __IO uint32_t AHB1ENR;  __IO uint32_t AHB2ENR; __IO uint32_t AHB3ENR;  uint32_t RESERVED2;
  __IO uint32_t APB1ENR;  __IO uint32_t APB2ENR; uint32_t RESERVED3[2];
  __IO uint32_t AHB1LPENR;__IO uint32_t AHB2LPENR;__IO uint32_t AHB3LPENR;uint32_t RESERVED4;
  __IO uint32_t APB1LPENR;__IO uint32_t APB2LPENR;uint32_t RESERVED5[2];
  __IO uint32_t BDCR;     __IO uint32_t CSR;      uint32_t RESERVED6[2];
  __IO uint32_t SSCGR;    __IO uint32_t PLLI2SCFGR; __IO uint32_t PLLSAICFGR;
  __IO uint32_t DCKCFGR;  __IO uint32_t CKGATENR;   __IO uint32_t DCKCFGR2;
} RCC_RegDef_t;
#define RCC ((RCC_RegDef_t*)(AHB1PERIPH_BASE + 0x3800UL))

/* ================== GPIO ================== */
typedef struct {
  __IO uint32_t MODER;   __IO uint32_t OTYPER; __IO uint32_t OSPEEDR; __IO uint32_t PUPDR;
  __IO uint32_t IDR;     __IO uint32_t ODR;    __IO uint32_t BSRR;    __IO uint32_t LCKR;
  __IO uint32_t AFR[2];
} GPIO_RegDef_t;
#define GPIOA ((GPIO_RegDef_t*)(AHB1PERIPH_BASE + 0x0000UL))
#define GPIOB ((GPIO_RegDef_t*)(AHB1PERIPH_BASE + 0x0400UL))
#define GPIOC ((GPIO_RegDef_t*)(AHB1PERIPH_BASE + 0x0800UL))

/* ================== I2C ================== */
typedef struct {
  __IO uint32_t CR1;   __IO uint32_t CR2;   __IO uint32_t OAR1;  __IO uint32_t OAR2;
  __IO uint32_t DR;    __IO uint32_t SR1;   __IO uint32_t SR2;   __IO uint32_t CCR;
  __IO uint32_t TRISE; __IO uint32_t FLTR;
} I2C_RegDef_t;
#define I2C1 ((I2C_RegDef_t*)(APB1PERIPH_BASE + 0x5400UL))
#define I2C2 ((I2C_RegDef_t*)(APB1PERIPH_BASE + 0x5800UL))
#define I2C3 ((I2C_RegDef_t*)(APB1PERIPH_BASE + 0x5C00UL))

/* ================== RCC clock enable bits ================== */
#define RCC_AHB1ENR_GPIOAEN   (1U<<0)
#define RCC_AHB1ENR_GPIOBEN   (1U<<1)
#define RCC_AHB1ENR_GPIOCEN   (1U<<2)

#define RCC_APB1ENR_I2C1EN    (1U<<21)
#define RCC_APB1ENR_I2C2EN    (1U<<22)
#define RCC_APB1ENR_I2C3EN    (1U<<23)

#define ENABLE  1U
#define DISABLE 0U

#endif /* STM32F407XX_H */

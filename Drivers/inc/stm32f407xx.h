#ifndef STM32F407XX_H
#define STM32F407XX_H
#include <stdint.h>
#define __vo volatile

/* Base addresses */
#define PERIPH_BASE        0x40000000U
#define APB1PERIPH_BASE    PERIPH_BASE
#define APB2PERIPH_BASE    (PERIPH_BASE + 0x00010000U)
#define AHB1PERIPH_BASE    (PERIPH_BASE + 0x00020000U)

/* GPIO base */
#define GPIOA_BASE (AHB1PERIPH_BASE + 0x0000)
#define GPIOB_BASE (AHB1PERIPH_BASE + 0x0400)

/* RCC & I2C base */
#define RCC_BASE   (AHB1PERIPH_BASE + 0x3800)
#define I2C1_BASE  (APB1PERIPH_BASE + 0x5400)
#define I2C2_BASE  (APB1PERIPH_BASE + 0x5800)
#define I2C3_BASE  (APB1PERIPH_BASE + 0x5C00)

/* Reg defs */
typedef struct{ __vo uint32_t MODER,OTYPER,OSPEEDR,PUPDR,IDR,ODR,BSRR,LCKR,AFR[2]; } GPIO_RegDef_t;
typedef struct{ __vo uint32_t CR1,CR2,OAR1,OAR2,DR,SR1,SR2,CCR,TRISE,FLTR; } I2C_RegDef_t;
typedef struct{
  __vo uint32_t CR,PLLCFGR,CFGR,CIR,AHB1RSTR,AHB2RSTR,AHB3RSTR,RES0,APB1RSTR,APB2RSTR,RES1[2],
                AHB1ENR,AHB2ENR,AHB3ENR,RES2,APB1ENR,APB2ENR,RES3[2];
} RCC_RegDef_t;

/* Instances */
#define GPIOA ((GPIO_RegDef_t*)GPIOA_BASE)
#define GPIOB ((GPIO_RegDef_t*)GPIOB_BASE)
#define RCC   ((RCC_RegDef_t*)RCC_BASE)
#define I2C1  ((I2C_RegDef_t*)I2C1_BASE)
#define I2C2  ((I2C_RegDef_t*)I2C2_BASE)
#define I2C3  ((I2C_RegDef_t*)I2C3_BASE)

/* Handy */
#define ENABLE 1
#define DISABLE 0
#endif

#ifndef STM32F407XX_I2C_DRIVER_H
#define STM32F407XX_I2C_DRIVER_H

#include "stm32f407xx.h"
#include <stdint.h>

/* ==== Bit positions ==== */
/* CR1 */
#define I2C_CR1_PE           0
#define I2C_CR1_START        8
#define I2C_CR1_STOP         9
#define I2C_CR1_ACK          10
#define I2C_CR1_SWRST        15
/* CR2 */
#define I2C_CR2_FREQ         0
#define I2C_CR2_ITERREN      8
#define I2C_CR2_ITEVTEN      9
#define I2C_CR2_ITBUFEN      10
/* SR1 */
#define I2C_SR1_SB           0
#define I2C_SR1_ADDR         1
#define I2C_SR1_BTF          2
#define I2C_SR1_STOPF        4
#define I2C_SR1_RXNE         6
#define I2C_SR1_TXE          7
#define I2C_SR1_BERR         8
#define I2C_SR1_ARLO         9
#define I2C_SR1_AF           10
#define I2C_SR1_OVR          11
#define I2C_SR1_TIMEOUT      14
/* SR2 */
#define I2C_SR2_MSL          0
#define I2C_SR2_BUSY         1
#define I2C_SR2_TRA          2

/* ==== Flags ==== */
#define I2C_FLAG_SB      (1U << I2C_SR1_SB)
#define I2C_FLAG_ADDR    (1U << I2C_SR1_ADDR)
#define I2C_FLAG_BTF     (1U << I2C_SR1_BTF)
#define I2C_FLAG_RXNE    (1U << I2C_SR1_RXNE)
#define I2C_FLAG_TXE     (1U << I2C_SR1_TXE)
#define I2C_FLAG_MSL     (1U << I2C_SR2_MSL)

/* ==== Configs ==== */
#define I2C_SCL_SPEED_SM     100000U
#define I2C_SCL_SPEED_FM4K   400000U

#define I2C_ACK_ENABLE       1
#define I2C_ACK_DISABLE      0

#define I2C_FM_DUTY_2        0
#define I2C_FM_DUTY_16_9     1

#define I2C_ENABLE_SR        1
#define I2C_DISABLE_SR       0

/* States (for IT stubs) */
#define I2C_READY            0
#define I2C_BUSY_IN_RX       1
#define I2C_BUSY_IN_TX       2

typedef struct {
    uint32_t I2C_SCLSpeed;
    uint8_t  I2C_DeviceAddress;   /* 7-bit own address */
    uint8_t  I2C_ACKControl;
    uint8_t  I2C_FMDutyCycle;
} I2C_Config_t;

typedef struct {
    I2C_RegDef_t *pI2Cx;
    I2C_Config_t  I2CConfig;

    uint8_t  *pTxBuffer;
    uint8_t  *pRxBuffer;
    uint32_t  TxLen;
    uint32_t  RxLen;
    uint8_t   TxRxState;
    uint8_t   DevAddr;
    uint32_t  RxSize;
    uint8_t   Sr;
} I2C_Handle_t;

/* RCC/peripheral clocks */
void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnOrDi);

/* Init/deinit */
void I2C_Init(I2C_Handle_t *pI2CHandle);
void I2C_DeInit(I2C_RegDef_t *pI2Cx);

/* Controls */
void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t EnOrDi);
void I2C_ManageAcking(I2C_RegDef_t *pI2Cx, uint8_t EnOrDi);
void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx);
void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx);

/* Blocking master APIs */
void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr);
void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr);

/* Simple slave helpers */
void    I2C_SlaveSendData(I2C_RegDef_t *pI2Cx, uint8_t data);
uint8_t I2C_SlaveReceiveData(I2C_RegDef_t *pI2Cx);

/* IRQ stubs */
void I2C_EV_IRQHandling(I2C_Handle_t *pI2CHandle);
void I2C_ER_IRQHandling(I2C_Handle_t *pI2CHandle);

/* Weak app callback (for IT mode, unused here) */
__attribute__((weak)) void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle, uint8_t AppEv);

#endif

#ifndef STM32F407XX_I2C_DRIVER_H
#define STM32F407XX_I2C_DRIVER_H
#include "stm32f407xx.h"
#include <stdint.h>

/* Bit positions (CR1/CR2/SR1/SR2) */
#define I2C_CR1_PE 0
#define I2C_CR1_START 8
#define I2C_CR1_STOP  9
#define I2C_CR1_ACK   10
#define I2C_CR1_SWRST 15

#define I2C_SR1_SB    0
#define I2C_SR1_ADDR  1
#define I2C_SR1_BTF   2
#define I2C_SR1_RXNE  6
#define I2C_SR1_TXE   7
#define I2C_SR2_MSL   0

#define I2C_FLAG_SB   (1U<<I2C_SR1_SB)
#define I2C_FLAG_ADDR (1U<<I2C_SR1_ADDR)
#define I2C_FLAG_BTF  (1U<<I2C_SR1_BTF)
#define I2C_FLAG_TXE  (1U<<I2C_SR1_TXE)

#define I2C_SCL_SPEED_SM   100000U
#define I2C_SCL_SPEED_FM4K 400000U
#define I2C_ACK_ENABLE     1
#define I2C_ACK_DISABLE    0
#define I2C_FM_DUTY_2      0
#define I2C_FM_DUTY_16_9   1
#define I2C_DISABLE_SR     0
#define I2C_ENABLE_SR      1

typedef struct{
  uint32_t I2C_SCLSpeed;
  uint8_t  I2C_DeviceAddress;
  uint8_t  I2C_ACKControl;
  uint8_t  I2C_FMDutyCycle;
} I2C_Config_t;

typedef struct{
  I2C_RegDef_t *pI2Cx;
  I2C_Config_t  I2CConfig;
} I2C_Handle_t;

/* API */
void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnOrDi);
void I2C_Init(I2C_Handle_t *pI2CHandle);
void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx);
void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx);
void I2C_ManageAcking(I2C_RegDef_t *pI2Cx, uint8_t EnOrDi);

/* TX only */
void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr);
#endif

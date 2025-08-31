#ifndef STM32F407XX_I2C_DRIVER_H
#define STM32F407XX_I2C_DRIVER_H

#include "stm32f407xx.h"
#include <stdint.h>

#define SET   1U
#define RESET 0U

#define I2C_SCL_SPEED_SM     100000U
#define I2C_SCL_SPEED_FM2K   200000U
#define I2C_SCL_SPEED_FM4K   400000U

#define I2C_ACK_DISABLE      0U
#define I2C_ACK_ENABLE       1U
#define I2C_FM_DUTY_2        0U
#define I2C_FM_DUTY_16_9     1U

#define I2C_READY            0U
#define I2C_BUSY_IN_RX       1U
#define I2C_BUSY_IN_TX       2U

#define I2C_EV_TX_CMPLT      1U
#define I2C_EV_RX_CMPLT      2U
#define I2C_EV_STOP          3U
#define I2C_ERROR_BERR       4U
#define I2C_ERROR_ARLO       5U
#define I2C_ERROR_AF         6U
#define I2C_ERROR_OVR        7U
#define I2C_ERROR_TIMEOUT    8U

#define I2C_FLAG_SB          (1U<<0)
#define I2C_FLAG_ADDR        (1U<<1)
#define I2C_FLAG_BTF         (1U<<2)
#define I2C_FLAG_STOPF       (1U<<4)
#define I2C_FLAG_RXNE        (1U<<6)
#define I2C_FLAG_TXE         (1U<<7)

#define I2C_ERR_BERR         (1U<<8)
#define I2C_ERR_ARLO         (1U<<9)
#define I2C_ERR_AF           (1U<<10)
#define I2C_ERR_OVR          (1U<<11)
#define I2C_ERR_TIMEOUT      (1U<<14)

#define IRQ_NO_I2C1_EV       31U
#define IRQ_NO_I2C1_ER       32U

typedef struct {
  uint32_t I2C_SCLSpeed;
  uint8_t  I2C_DeviceAddress;
  uint8_t  I2C_ACKControl;
  uint8_t  I2C_FMDutyCycle;
} I2C_Config_t;

typedef struct {
  I2C_RegDef_t *pI2Cx;
  I2C_Config_t  I2C_Config;

  uint8_t  *pTxBuffer;  uint32_t TxLen;
  uint8_t  *pRxBuffer;  uint32_t RxLen; uint32_t RxSize;
  uint8_t   Sr;         uint8_t  TxRxState;
  uint8_t   DevAddr;
} I2C_Handle_t;

void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t En);
void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t En);
void I2C_Init(I2C_Handle_t *pHandle);
void I2C_DeInit(I2C_RegDef_t *pI2Cx);
void I2C_ManageAcking(I2C_RegDef_t *pI2Cx, uint8_t En);

uint8_t I2C_MasterSendDataIT(I2C_Handle_t *pH, uint8_t *pTx, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr);
uint8_t I2C_MasterReceiveDataIT(I2C_Handle_t *pH, uint8_t *pRx, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr);

void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint8_t En);
void I2C_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);

void I2C_EV_IRQHandling(I2C_Handle_t *pH);
void I2C_ER_IRQHandling(I2C_Handle_t *pH);

uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint32_t FlagMask);

void I2C_ApplicationEventCallback(I2C_Handle_t *pHandle, uint8_t AppEvent) __attribute__((weak));

#endif

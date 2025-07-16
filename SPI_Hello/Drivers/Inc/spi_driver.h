#ifndef SPI_DRIVER_H
#define SPI_DRIVER_H

#include "stm32f4xx_custom.h"
#include <stdint.h>

/* flag status */
#define FLAG_RESET    0
#define FLAG_SET      1

/* SPI_SR bits */
#define SPI_SR_RXNE   0
#define SPI_SR_TXE    1
#define SPI_SR_BSY    7

/* masks */
#define SPI_RXNE_FLAG (1U<<SPI_SR_RXNE)
#define SPI_TXE_FLAG  (1U<<SPI_SR_TXE)
#define SPI_BUSY_FLAG (1U<<SPI_SR_BSY)

/* device mode */
#define SPI_DEVICE_MODE_SLAVE   0
#define SPI_DEVICE_MODE_MASTER  1
/* bus config */
#define SPI_BUSCONFIG_FD        1
#define SPI_BUSCONFIG_HD        2
#define SPI_BUSCONFIG_RXONLY    3
/* SCLK speed */
#define SPI_SCLK_SPEED_DIV2     0
#define SPI_SCLK_SPEED_DIV4     1
#define SPI_SCLK_SPEED_DIV8     2
#define SPI_SCLK_SPEED_DIV16    3
#define SPI_SCLK_SPEED_DIV32    4
#define SPI_SCLK_SPEED_DIV64    5
#define SPI_SCLK_SPEED_DIV128   6
#define SPI_SCLK_SPEED_DIV256   7
/* DFF */
#define SPI_DFF_8BIT            0
#define SPI_DFF_16BIT           1
/* CPOL/CPHA */
#define SPI_CPOL_LOW            0
#define SPI_CPOL_HIGH           1
#define SPI_CPHA_LOW            0
#define SPI_CPHA_HIGH           1
/* SSM */
#define SPI_SSM_DISABLE         0
#define SPI_SSM_ENABLE          1

/* SPI config struct */
typedef struct {
    uint8_t SPI_DeviceMode;
    uint8_t SPI_BusConfig;
    uint8_t SPI_SclkSpeed;
    uint8_t SPI_DFF;
    uint8_t SPI_CPOL;
    uint8_t SPI_CPHA;
    uint8_t SPI_SSM;
} SPI_Config_t;

/* SPI handle */
typedef struct {
    SPI_RegDef_t *pSPIx;
    SPI_Config_t  SPIConfig;
} SPI_Handle_t;

/* APIs */
uint8_t SPI_GetFlagStatus(SPI_RegDef_t*, uint32_t);
void    SPI_PeriClockControl(SPI_RegDef_t*, uint8_t);
void    SPI_Init(SPI_Handle_t*);
void    SPI_DeInit(SPI_RegDef_t*);
void    SPI_SendData(SPI_RegDef_t*, uint8_t*, uint32_t);
void    SPI_ReceiveData(SPI_RegDef_t*, uint8_t*, uint32_t);
void    SPI_PeripheralControl(SPI_RegDef_t*, uint8_t);
void    SPI_SSIConfig(SPI_RegDef_t*, uint8_t);
void    SPI_IRQConfig(uint8_t, uint8_t);
void    SPI_IRQPriorityConfig(uint8_t, uint8_t);
void    SPI_IRQHandling(SPI_Handle_t*);

#endif /* SPI_DRIVER_H */

#ifndef SPI_DRIVER_H
#define SPI_DRIVER_H

#include "stm32f4xx_custom.h"
#include <stdint.h>

/* flags */
#define FLAG_RESET    0
#define FLAG_SET      1
#define SPI_RXNE_FLAG (1U<<0)
#define SPI_TXE_FLAG  (1U<<1)
#define SPI_BUSY_FLAG (1U<<7)

/* configs */
#define SPI_DEVICE_MODE_SLAVE   0
#define SPI_DEVICE_MODE_MASTER  1

#define SPI_BUSCONFIG_FD        1

#define SPI_SCLK_SPEED_DIV2     0
#define SPI_SCLK_SPEED_DIV4     1
#define SPI_SCLK_SPEED_DIV8     2
#define SPI_SCLK_SPEED_DIV16    3

#define SPI_DFF_8BIT            0
#define SPI_DFF_16BIT           1

#define SPI_CPOL_LOW            0
#define SPI_CPOL_HIGH           1

#define SPI_CPHA_LOW            0
#define SPI_CPHA_HIGH           1

#define SPI_SSM_DISABLE         0
#define SPI_SSM_ENABLE          1

typedef struct {
    uint8_t SPI_DeviceMode;
    uint8_t SPI_BusConfig;
    uint8_t SPI_SclkSpeed;
    uint8_t SPI_DFF;
    uint8_t SPI_CPOL;
    uint8_t SPI_CPHA;
    uint8_t SPI_SSM;
} SPI_Config_t;

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
void    SPI_SSOEConfig(SPI_RegDef_t*, uint8_t);

#endif // SPI_DRIVER_H

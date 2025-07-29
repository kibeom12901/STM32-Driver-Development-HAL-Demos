#ifndef SPI_DRIVER_H
#define SPI_DRIVER_H

#include "stm32f4xx_custom.h"
#include <stdint.h>

/* flags & configs */
#define FLAG_RESET       0
#define FLAG_SET         1
#define SPI_RXNE_FLAG    (1U<<0)
#define SPI_TXE_FLAG     (1U<<1)
#define SPI_BUSY_FLAG    (1U<<7)

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

/* IRQ-state definitions */
#define SPI_READY        0
#define SPI_BUSY_IN_TX   1
#define SPI_BUSY_IN_RX   2

/* Application event macros */
#define SPI_EVENT_TX_CMPLT   1
#define SPI_EVENT_RX_CMPLT   2
#define SPI_EVENT_OVR_ERR    3

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

    /* IT-mode buffers & state */
    uint8_t  *pTxBufferIT;
    uint32_t  TxLenIT;
    uint8_t   TxState;

    uint8_t  *pRxBufferIT;
    uint32_t  RxLenIT;
    uint8_t   RxState;
} SPI_Handle_t;

/* blocking APIs */
uint8_t SPI_GetFlagStatus(SPI_RegDef_t*, uint32_t);
void    SPI_PeriClockControl(SPI_RegDef_t*, uint8_t);
void    SPI_Init(SPI_Handle_t*);
void    SPI_DeInit(SPI_RegDef_t*);
void    SPI_SendData(SPI_RegDef_t*, uint8_t*, uint32_t);
void    SPI_ReceiveData(SPI_RegDef_t*, uint8_t*, uint32_t);
void    SPI_PeripheralControl(SPI_RegDef_t*, uint8_t);
void    SPI_SSIConfig(SPI_RegDef_t*, uint8_t);
void    SPI_SSOEConfig(SPI_RegDef_t*, uint8_t);

/* non-blocking IT APIs */
uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t Len);
uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t Len);
void    SPI_IRQHandling(SPI_Handle_t *pSPIHandle);

/* Application callback (weak—override in your app) */
void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle, uint8_t AppEv);

#endif // SPI_DRIVER_H

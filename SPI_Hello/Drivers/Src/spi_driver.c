#include "spi_driver.h"

/* flag helper */
uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName){
    return ( (pSPIx->SR & FlagName) ? FLAG_SET : FLAG_RESET );
}

/* clock */
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi){
    if (EnOrDi){
        if (pSPIx==SPI1) RCC->APB2ENR |= 1<<12;
        if (pSPIx==SPI2) RCC->APB1ENR |= 1<<14;
        if (pSPIx==SPI3) RCC->APB1ENR |= 1<<15;
    } else {
        if (pSPIx==SPI1) RCC->APB2ENR &= ~(1<<12);
        if (pSPIx==SPI2) RCC->APB1ENR &= ~(1<<14);
        if (pSPIx==SPI3) RCC->APB1ENR &= ~(1<<15);
    }
}

/* init */
void SPI_Init(SPI_Handle_t *pH){
    uint32_t temp=0;
    SPI_PeriClockControl(pH->pSPIx, ENABLE);
    /* CR1 */
    temp |= pH->SPIConfig.SPI_DeviceMode << 2;  /* MSTR */
    temp |= pH->SPIConfig.SPI_BusConfig    == SPI_BUSCONFIG_HD ? (1U<<15) : 0;
    temp |= pH->SPIConfig.SPI_BusConfig    == SPI_BUSCONFIG_RXONLY ? (1U<<10) : 0;
    temp |= pH->SPIConfig.SPI_SclkSpeed    << 3;
    temp |= pH->SPIConfig.SPI_DFF          << 11;
    temp |= pH->SPIConfig.SPI_CPOL         << 1;
    temp |= pH->SPIConfig.SPI_CPHA         << 0;
    temp |= pH->SPIConfig.SPI_SSM          << 9;
    pH->pSPIx->CR1 = temp;
    if (pH->SPIConfig.SPI_SSM) SPI_SSIConfig(pH->pSPIx, ENABLE);
}

/* de-init */
void SPI_DeInit(SPI_RegDef_t *pSPIx){
    if(pSPIx==SPI1){ RCC->APB2RSTR|=1<<12; RCC->APB2RSTR&=~(1<<12);} 
    if(pSPIx==SPI2){ RCC->APB1RSTR|=1<<14; RCC->APB1RSTR&=~(1<<14);} 
    if(pSPIx==SPI3){ RCC->APB1RSTR|=1<<15; RCC->APB1RSTR&=~(1<<15);} 
}

/* send */
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *buf, uint32_t len){
    while(len--){
        while(SPI_GetFlagStatus(pSPIx, SPI_TXE_FLAG)==FLAG_RESET);
        if(pSPIx->CR1 & (1U<<11)){
            pSPIx->DR = *((uint16_t*)buf);
            buf+=2;
        } else {
            pSPIx->DR = *buf;
            buf++;
        }
    }
    while(SPI_GetFlagStatus(pSPIx, SPI_BUSY_FLAG)==FLAG_SET);
}

/* recv */
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *buf, uint32_t len){
    while(len--){
        while(SPI_GetFlagStatus(pSPIx, SPI_RXNE_FLAG)==FLAG_RESET);
        if(pSPIx->CR1 & (1U<<11)){
            *((uint16_t*)buf)=pSPIx->DR;
            buf+=2;
        } else {
            *buf = pSPIx->DR;
            buf++;
        }
    }
}

/* enable/disable SPI */
void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi){
    if(EnOrDi) pSPIx->CR1 |= 1<<6;
    else       pSPIx->CR1 &= ~(1<<6);
}

/* SSI for SSM */
void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi){
    if(EnOrDi) pSPIx->CR1 |= 1<<8;
    else       pSPIx->CR1 &= ~(1<<8);
}

/* IRQ stubs */
void SPI_IRQConfig(uint8_t IRQ, uint8_t EnOrDi){}
void SPI_IRQPriorityConfig(uint8_t IRQ, uint8_t Prio){}
void SPI_IRQHandling(SPI_Handle_t *pH){}

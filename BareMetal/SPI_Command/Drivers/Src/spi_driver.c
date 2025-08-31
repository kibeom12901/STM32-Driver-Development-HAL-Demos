#include "spi_driver.h"

uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName){
    return ((pSPIx->SR & FlagName) ? FLAG_SET : FLAG_RESET);
}

void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi){
    if (EnOrDi) {
        if      (pSPIx==SPI1) RCC->APB2ENR |= (1<<12);
        else if (pSPIx==SPI2) RCC->APB1ENR |= (1<<14);
        else if (pSPIx==SPI3) RCC->APB1ENR |= (1<<15);
    } else {
        if      (pSPIx==SPI1) RCC->APB2ENR &= ~(1<<12);
        else if (pSPIx==SPI2) RCC->APB1ENR &= ~(1<<14);
        else if (pSPIx==SPI3) RCC->APB1ENR &= ~(1<<15);
    }
}

void SPI_Init(SPI_Handle_t *pH){
    uint32_t temp = 0;
    SPI_PeriClockControl(pH->pSPIx, ENABLE);

    temp |= pH->SPIConfig.SPI_DeviceMode << SPI_CR1_MSTR;
    temp |= pH->SPIConfig.SPI_SclkSpeed  << SPI_CR1_BR;
    temp |= pH->SPIConfig.SPI_CPOL       << 1;
    temp |= pH->SPIConfig.SPI_CPHA       << 0;
    temp |= pH->SPIConfig.SPI_DFF        << SPI_CR1_DFF;
    temp |= pH->SPIConfig.SPI_SSM        << SPI_CR1_SSM;

    pH->pSPIx->CR1 = temp;

    if (pH->SPIConfig.SPI_SSM == SPI_SSM_ENABLE)
        SPI_SSIConfig(pH->pSPIx, ENABLE);
}

void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len){
    while (Len > 0){
        while (SPI_GetFlagStatus(pSPIx, SPI_TXE_FLAG) == FLAG_RESET);
        if (pSPIx->CR1 & (1U << SPI_CR1_DFF)){
            pSPIx->DR = *((uint16_t*)pTxBuffer);
            pTxBuffer += 2;
            Len       -= 2;
        } else {
            pSPIx->DR = *pTxBuffer;
            pTxBuffer++;
            Len--;
        }
    }
    while (SPI_GetFlagStatus(pSPIx, SPI_BUSY_FLAG) == FLAG_SET);
}

void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len){
    while (Len > 0){
        while (SPI_GetFlagStatus(pSPIx, SPI_RXNE_FLAG) == FLAG_RESET);
        if (pSPIx->CR1 & (1U << SPI_CR1_DFF)){
            *((uint16_t*)pRxBuffer) = (uint16_t)pSPIx->DR;
            pRxBuffer += 2;
            Len       -= 2;
        } else {
            *pRxBuffer = (uint8_t)pSPIx->DR;
            pRxBuffer++;
            Len--;
        }
    }
}

void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi){
    if (EnOrDi) pSPIx->CR1 |=  (1<<SPI_CR1_SPE);
    else        pSPIx->CR1 &= ~(1<<SPI_CR1_SPE);
}

void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi){
    if (EnOrDi) pSPIx->CR1 |=  (1<<SPI_CR1_SSI);
    else        pSPIx->CR1 &= ~(1<<SPI_CR1_SSI);
}

void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi){
    if (EnOrDi) pSPIx->CR2 |=  (1U << SPI_CR2_SSOE);
    else        pSPIx->CR2 &= ~(1U << SPI_CR2_SSOE);
}

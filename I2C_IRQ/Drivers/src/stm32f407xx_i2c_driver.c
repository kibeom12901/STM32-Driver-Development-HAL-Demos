#include "stm32f407xx_i2c_driver.h"

/* ======= Private helpers ======= */
static void I2C_GenerateStart(I2C_RegDef_t *p) { p->CR1 |= (1U<<8); }
static void I2C_GenerateStop (I2C_RegDef_t *p) { p->CR1 |= (1U<<9); }
static void I2C_ExecuteAddrPhaseWrite(I2C_RegDef_t *p, uint8_t addr) { p->DR = (uint8_t)((addr<<1)&0xFE); }
static void I2C_ExecuteAddrPhaseRead (I2C_RegDef_t *p, uint8_t addr) { p->DR = (uint8_t)((addr<<1)|0x01); }

static void I2C_ClearADDRFlag(I2C_Handle_t *h) { (void)h->pI2Cx->SR1; (void)h->pI2Cx->SR2; }

static void I2C_CloseReceiveData(I2C_Handle_t *h) {
    h->pI2Cx->CR2 &= ~(1U<<10); /* ITBUFEN */
    h->pI2Cx->CR2 &= ~(1U<<9);  /* ITEVTEN */
    h->pI2Cx->CR2 &= ~(1U<<8);  /* ITERREN */
    if (h->I2C_Config.I2C_ACKControl == I2C_ACK_ENABLE) I2C_ManageAcking(h->pI2Cx, ENABLE);
    h->TxRxState = I2C_READY; h->pRxBuffer = 0; h->RxLen = h->RxSize = 0;
}
static void I2C_CloseSendData(I2C_Handle_t *h) {
    h->pI2Cx->CR2 &= ~(1U<<10); h->pI2Cx->CR2 &= ~(1U<<9); h->pI2Cx->CR2 &= ~(1U<<8);
    h->TxRxState = I2C_READY; h->pTxBuffer = 0; h->TxLen = 0;
}

/* If APB1 != 16 MHz, change this to your real PCLK1 value */
static uint32_t I2C_GetPCLK1Hz(void) { return 16000000UL; }

/* ======= Public APIs ======= */
void I2C_PeriClockControl(I2C_RegDef_t *p, uint8_t En) {
    if (En) {
        if (p==I2C1) RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
        else if (p==I2C2) RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
        else if (p==I2C3) RCC->APB1ENR |= RCC_APB1ENR_I2C3EN;
    } else {
        if (p==I2C1) RCC->APB1ENR &= ~RCC_APB1ENR_I2C1EN;
        else if (p==I2C2) RCC->APB1ENR &= ~RCC_APB1ENR_I2C2EN;
        else if (p==I2C3) RCC->APB1ENR &= ~RCC_APB1ENR_I2C3EN;
    }
}
void I2C_PeripheralControl(I2C_RegDef_t *p, uint8_t En) { if (En) p->CR1 |= 1U; else p->CR1 &= ~1U; }
void I2C_ManageAcking(I2C_RegDef_t *p, uint8_t En) { if (En) p->CR1 |= (1U<<10); else p->CR1 &= ~(1U<<10); }

void I2C_Init(I2C_Handle_t *h)
{
    uint32_t pclk1 = I2C_GetPCLK1Hz();
    I2C_PeriClockControl(h->pI2Cx, ENABLE);
    I2C_PeripheralControl(h->pI2Cx, DISABLE);

    /* CR2: PCLK1 in MHz */
    h->pI2Cx->CR2 = (pclk1/1000000U) & 0x3FU;

    /* OAR1: own address */
    h->pI2Cx->OAR1 = (1U<<14) | ((uint32_t)h->I2C_Config.I2C_DeviceAddress << 1U);

    /* CCR/TRISE */
    uint16_t ccr=0;
    if (h->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM) {
        ccr = (uint16_t)(pclk1/(2U*h->I2C_Config.I2C_SCLSpeed)); if (ccr<4) ccr=4;
        h->pI2Cx->CCR = ccr;
        h->pI2Cx->TRISE = (pclk1/1000000U) + 1U; /* 1000ns */
    } else {
        h->pI2Cx->CCR = (1U<<15) | ((h->I2C_Config.I2C_FMDutyCycle==I2C_FM_DUTY_16_9)?(1U<<14):0U);
        if (h->I2C_Config.I2C_FMDutyCycle==I2C_FM_DUTY_16_9) ccr=(uint16_t)(pclk1/(25U*h->I2C_Config.I2C_SCLSpeed));
        else ccr=(uint16_t)(pclk1/(3U*h->I2C_Config.I2C_SCLSpeed));
        if (!ccr) ccr=1U;
        h->pI2Cx->CCR |= (ccr & 0x0FFFU);
        h->pI2Cx->TRISE = ((pclk1/1000000U)*300U)/1000U + 1U; /* 300ns */
    }

    if (h->I2C_Config.I2C_ACKControl==I2C_ACK_ENABLE) I2C_ManageAcking(h->pI2Cx, ENABLE);
    else I2C_ManageAcking(h->pI2Cx, DISABLE);

    I2C_PeripheralControl(h->pI2Cx, ENABLE);
    h->TxRxState = I2C_READY;
}

void I2C_DeInit(I2C_RegDef_t *p) { (void)p; /* minimal */ }

void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint8_t En)
{
    if (En) {
        if (IRQNumber<32) NVIC->ISER[0] = (1U<<IRQNumber);
        else NVIC->ISER[1] = (1U<<(IRQNumber%32U));
    } else {
        if (IRQNumber<32) NVIC->ICER[0] = (1U<<IRQNumber);
        else NVIC->ICER[1] = (1U<<(IRQNumber%32U));
    }
}

void I2C_IRQPriorityConfig(uint8_t IRQNumber, uint32_t prio)
{
    uint8_t iprx = IRQNumber/4U, section = IRQNumber%4U, shift = (8U*section)+(8U-4U);
    NVIC->IPR[iprx] &= ~(0xFU<<shift);
    NVIC->IPR[iprx] |=  ((prio & 0x0FU) << shift);
}

uint8_t I2C_MasterSendDataIT(I2C_Handle_t *h, uint8_t *tx, uint32_t len, uint8_t addr, uint8_t sr)
{
    if (h->TxRxState != I2C_READY || len==0) return h->TxRxState;
    h->pTxBuffer=tx; h->TxLen=len; h->TxRxState=I2C_BUSY_IN_TX; h->DevAddr=addr; h->Sr=sr;
    I2C_GenerateStart(h->pI2Cx);
    h->pI2Cx->CR2 |= (1U<<9) | (1U<<10) | (1U<<8);
    return I2C_READY;
}

uint8_t I2C_MasterReceiveDataIT(I2C_Handle_t *h, uint8_t *rx, uint32_t len, uint8_t addr, uint8_t sr)
{
    if (h->TxRxState != I2C_READY || len==0) return h->TxRxState;
    h->pRxBuffer=rx; h->RxLen=len; h->RxSize=len; h->TxRxState=I2C_BUSY_IN_RX; h->DevAddr=addr; h->Sr=sr;
    I2C_GenerateStart(h->pI2Cx);
    h->pI2Cx->CR2 |= (1U<<9) | (1U<<10) | (1U<<8);
    return I2C_READY;
}

void I2C_EV_IRQHandling(I2C_Handle_t *h)
{
    uint32_t sr1 = h->pI2Cx->SR1;

    /* SB: start bit sent */
    if (sr1 & I2C_FLAG_SB) {
        if (h->TxRxState==I2C_BUSY_IN_TX) I2C_ExecuteAddrPhaseWrite(h->pI2Cx, h->DevAddr);
        else if (h->TxRxState==I2C_BUSY_IN_RX) I2C_ExecuteAddrPhaseRead(h->pI2Cx, h->DevAddr);
    }

    /* ADDR: address matched/completed */
    if (sr1 & I2C_FLAG_ADDR) {
        if (h->TxRxState==I2C_BUSY_IN_RX) {
            if (h->RxSize==1U) I2C_ManageAcking(h->pI2Cx, DISABLE);
            else if (h->RxSize==2U) I2C_ManageAcking(h->pI2Cx, DISABLE);
        }
        I2C_ClearADDRFlag(h);
    }

    /* TXE: write next byte */
    if ((sr1 & I2C_FLAG_TXE) && (h->TxRxState==I2C_BUSY_IN_TX)) {
        if (h->TxLen>0U) { h->pI2Cx->DR = *(h->pTxBuffer++); h->TxLen--; }
    }

    /* BTF: all bytes shifted and DR empty */
    if (sr1 & I2C_FLAG_BTF) {
        if (h->TxRxState==I2C_BUSY_IN_TX && h->TxLen==0U && (h->pI2Cx->SR1 & I2C_FLAG_TXE)) {
            if (h->Sr==RESET) I2C_GenerateStop(h->pI2Cx);
            I2C_CloseSendData(h);
            I2C_ApplicationEventCallback(h, I2C_EV_TX_CMPLT);
        }
    }

    /* RXNE: read incoming */
    if ((sr1 & I2C_FLAG_RXNE) && (h->TxRxState==I2C_BUSY_IN_RX)) {
        if (h->RxSize==1U) {
            if (h->Sr==RESET) I2C_GenerateStop(h->pI2Cx);
            *(h->pRxBuffer) = h->pI2Cx->DR; h->RxLen--; h->RxSize--;
            I2C_CloseReceiveData(h);
            I2C_ApplicationEventCallback(h, I2C_EV_RX_CMPLT);
        } else {
            if (h->RxLen==2U) { I2C_ManageAcking(h->pI2Cx, DISABLE); if (h->Sr==RESET) I2C_GenerateStop(h->pI2Cx); }
            *(h->pRxBuffer++) = h->pI2Cx->DR; h->RxLen--;
            if (h->RxLen==0U) { I2C_CloseReceiveData(h); I2C_ApplicationEventCallback(h, I2C_EV_RX_CMPLT); }
        }
    }

    /* STOPF: slave mode only (included for completeness) */
    if (sr1 & I2C_FLAG_STOPF) { (void)h->pI2Cx->SR1; h->pI2Cx->CR1 |= 0; I2C_ApplicationEventCallback(h, I2C_EV_STOP); }
}

void I2C_ER_IRQHandling(I2C_Handle_t *h)
{
    uint32_t sr1 = h->pI2Cx->SR1;
    if (sr1 & I2C_ERR_BERR)    { h->pI2Cx->SR1 &= ~I2C_ERR_BERR;    I2C_ApplicationEventCallback(h, I2C_ERROR_BERR); }
    if (sr1 & I2C_ERR_ARLO)    { h->pI2Cx->SR1 &= ~I2C_ERR_ARLO;    I2C_ApplicationEventCallback(h, I2C_ERROR_ARLO); }
    if (sr1 & I2C_ERR_AF)      { h->pI2Cx->SR1 &= ~I2C_ERR_AF;      I2C_ApplicationEventCallback(h, I2C_ERROR_AF); }
    if (sr1 & I2C_ERR_OVR)     { h->pI2Cx->SR1 &= ~I2C_ERR_OVR;     I2C_ApplicationEventCallback(h, I2C_ERROR_OVR); }
    if (sr1 & I2C_ERR_TIMEOUT) { h->pI2Cx->SR1 &= ~I2C_ERR_TIMEOUT; I2C_ApplicationEventCallback(h, I2C_ERROR_TIMEOUT); }
}

uint8_t I2C_GetFlagStatus(I2C_RegDef_t *p, uint32_t flag) { return (p->SR1 & flag) ? SET : RESET; }

void __attribute__((weak)) I2C_ApplicationEventCallback(I2C_Handle_t *p, uint8_t e) { (void)p; (void)e; }

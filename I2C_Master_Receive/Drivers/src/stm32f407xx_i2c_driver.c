#include "../inc/stm32f407xx_i2c_driver.h"

/* Adjust this if your PCLK1 differs */
#ifndef I2C_PCLK1_HZ
#define I2C_PCLK1_HZ 16000000U
#endif

static uint32_t RCC_GetPCLK1Value(void){ return I2C_PCLK1_HZ; }

/* === Clock enable/disable === */
void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnOrDi)
{
    if (EnOrDi) {
        if (pI2Cx == I2C1)      RCC->APB1ENR |= (1U << 21);
        else if (pI2Cx == I2C2) RCC->APB1ENR |= (1U << 22);
        else if (pI2Cx == I2C3) RCC->APB1ENR |= (1U << 23);
    } else {
        if (pI2Cx == I2C1)      RCC->APB1ENR &= ~(1U << 21);
        else if (pI2Cx == I2C2) RCC->APB1ENR &= ~(1U << 22);
        else if (pI2Cx == I2C3) RCC->APB1ENR &= ~(1U << 23);
    }
}

static void I2C_ExecuteAddressPhaseWrite(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr){ pI2Cx->DR = (SlaveAddr<<1) & 0xFE; }
static void I2C_ExecuteAddressPhaseRead (I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr){ pI2Cx->DR = (SlaveAddr<<1) | 0x01; }

static void I2C_ClearADDRFlag(I2C_Handle_t *pI2CHandle)
{
    volatile uint32_t dummy;
    dummy = pI2CHandle->pI2Cx->SR1; (void)dummy;
    dummy = pI2CHandle->pI2Cx->SR2; (void)dummy;
}

static uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint32_t FlagName)
{
    if (FlagName & (I2C_FLAG_MSL)) return ( (pI2Cx->SR2 & FlagName) ? 1U : 0U );
    else                           return ( (pI2Cx->SR1 & FlagName) ? 1U : 0U );
}

/* === Init / DeInit === */
void I2C_Init(I2C_Handle_t *h)
{
    I2C_PeriClockControl(h->pI2Cx, 1);

    /* Disable peripheral */
    h->pI2Cx->CR1 &= ~(1U << I2C_CR1_PE);

    uint32_t pclk1 = RCC_GetPCLK1Value();
    uint32_t fMHz  = pclk1 / 1000000U;

    /* CR2[5:0] = PCLK1 in MHz */
    h->pI2Cx->CR2  = (h->pI2Cx->CR2 & ~0x3FU) | (fMHz & 0x3FU);

    /* Own address (7-bit) */
    h->pI2Cx->OAR1 = (1U<<14) | ((h->I2CConfig.I2C_DeviceAddress & 0x7F) << 1);

    /* CCR / TRISE */
    if (h->I2CConfig.I2C_SCLSpeed <= I2C_SCL_SPEED_SM) {
        uint16_t ccr = (uint16_t)(pclk1 / (2U * h->I2CConfig.I2C_SCLSpeed));
        if (ccr < 4) ccr = 4; /* simple clamp */
        h->pI2Cx->CCR   = ccr;
        h->pI2Cx->TRISE = fMHz + 1U;
    } else {
        uint16_t ccr;
        h->pI2Cx->CCR = (1U<<15) | ((h->I2CConfig.I2C_FMDutyCycle==I2C_FM_DUTY_16_9) ? (1U<<14) : 0U);
        ccr = (h->I2CConfig.I2C_FMDutyCycle==I2C_FM_DUTY_16_9)
              ? (uint16_t)(pclk1 / (25U * h->I2CConfig.I2C_SCLSpeed))
              : (uint16_t)(pclk1 / (3U  * h->I2CConfig.I2C_SCLSpeed));
        if (ccr == 0) ccr = 1;
        h->pI2Cx->CCR |= (ccr & 0x0FFF);
        h->pI2Cx->TRISE = (uint32_t)((fMHz * 300U) / 1000U) + 1U;
    }

    /* ACK on/off */
    if (h->I2CConfig.I2C_ACKControl == I2C_ACK_ENABLE) h->pI2Cx->CR1 |=  (1U<<I2C_CR1_ACK);
    else                                               h->pI2Cx->CR1 &= ~(1U<<I2C_CR1_ACK);

    /* Enable peripheral */
    h->pI2Cx->CR1 |= (1U << I2C_CR1_PE);
}

void I2C_DeInit(I2C_RegDef_t *p)
{
    p->CR1 |=  (1U << I2C_CR1_SWRST);
    p->CR1 &= ~(1U << I2C_CR1_SWRST);
}

/* === Controls === */
void I2C_PeripheralControl(I2C_RegDef_t *p, uint8_t EnOrDi){ if (EnOrDi) p->CR1|=(1U<<I2C_CR1_PE); else p->CR1&=~(1U<<I2C_CR1_PE); }
void I2C_ManageAcking    (I2C_RegDef_t *p, uint8_t EnOrDi){ if (EnOrDi) p->CR1|=(1U<<I2C_CR1_ACK); else p->CR1&=~(1U<<I2C_CR1_ACK); }
void I2C_GenerateStartCondition(I2C_RegDef_t *p){ p->CR1 |= (1U<<I2C_CR1_START); }
void I2C_GenerateStopCondition (I2C_RegDef_t *p){ p->CR1 |= (1U<<I2C_CR1_STOP);  }

/* === Blocking master TX/RX === */
void I2C_MasterSendData(I2C_Handle_t *h, uint8_t *tx, uint32_t len, uint8_t addr, uint8_t sr)
{
    /* START + Address (W) */
    I2C_GenerateStartCondition(h->pI2Cx);
    while(!I2C_GetFlagStatus(h->pI2Cx, I2C_FLAG_SB));
    I2C_ExecuteAddressPhaseWrite(h->pI2Cx, addr);
    while(!I2C_GetFlagStatus(h->pI2Cx, I2C_FLAG_ADDR));
    I2C_ClearADDRFlag(h);

    /* Data */
    while(len--){
        while(!I2C_GetFlagStatus(h->pI2Cx, I2C_FLAG_TXE));
        h->pI2Cx->DR = *tx++;
    }
    while(!I2C_GetFlagStatus(h->pI2Cx, I2C_FLAG_TXE));
    while(!I2C_GetFlagStatus(h->pI2Cx, I2C_FLAG_BTF));

    if (sr == I2C_DISABLE_SR) I2C_GenerateStopCondition(h->pI2Cx);
}

void I2C_MasterReceiveData(I2C_Handle_t *h, uint8_t *rx, uint32_t len, uint8_t addr, uint8_t sr)
{
    /* START + Address (R) */
    I2C_GenerateStartCondition(h->pI2Cx);
    while(!I2C_GetFlagStatus(h->pI2Cx, I2C_FLAG_SB));
    I2C_ExecuteAddressPhaseRead(h->pI2Cx, addr);

    if (len == 1) {
        I2C_ManageAcking(h->pI2Cx, I2C_ACK_DISABLE);
        while(!I2C_GetFlagStatus(h->pI2Cx, I2C_FLAG_ADDR));
        I2C_ClearADDRFlag(h);
        if (sr == I2C_DISABLE_SR) I2C_GenerateStopCondition(h->pI2Cx);
        while(!I2C_GetFlagStatus(h->pI2Cx, I2C_FLAG_RXNE));
        *rx = h->pI2Cx->DR;
    } else {
        while(!I2C_GetFlagStatus(h->pI2Cx, I2C_FLAG_ADDR));
        I2C_ClearADDRFlag(h);
        for (uint32_t i=len; i>0; i--) {
            if (i == 2) {
                I2C_ManageAcking(h->pI2Cx, I2C_ACK_DISABLE);
                if (sr == I2C_DISABLE_SR) I2C_GenerateStopCondition(h->pI2Cx);
            }
            while(!I2C_GetFlagStatus(h->pI2Cx, I2C_FLAG_RXNE));
            *rx++ = h->pI2Cx->DR;
        }
    }

    if (h->I2CConfig.I2C_ACKControl == I2C_ACK_ENABLE)
        I2C_ManageAcking(h->pI2Cx, I2C_ACK_ENABLE);
}

/* === Simple slave helpers === */
void    I2C_SlaveSendData   (I2C_RegDef_t *p, uint8_t d){ p->DR = d; }
uint8_t I2C_SlaveReceiveData(I2C_RegDef_t *p){ return (uint8_t)p->DR; }

/* === IRQ stubs (unused here) === */
void I2C_EV_IRQHandling(I2C_Handle_t *h){ (void)h; }
void I2C_ER_IRQHandling(I2C_Handle_t *h){ (void)h; }

/* Weak app callback */
__attribute__((weak)) void I2C_ApplicationEventCallback(I2C_Handle_t *h, uint8_t ev){ (void)h; (void)ev; }


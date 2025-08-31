#include "stm32f407xx_i2c_driver.h"

/* Assume PCLK1 = 16 MHz */
static uint32_t RCC_GetPCLK1Value(void){ return 16000000U; }

static void I2C_ExecuteAddressPhaseWrite(I2C_RegDef_t *p, uint8_t addr){
    p->DR = (addr<<1) & ~1;
}

static void I2C_ClearADDRFlag(I2C_Handle_t *h){
    volatile uint32_t d;
    d = h->pI2Cx->SR1; (void)d;
    d = h->pI2Cx->SR2; (void)d;
}

static uint8_t I2C_GetFlagStatus(I2C_RegDef_t *p, uint32_t Flag){
    if(Flag & 0xFFFF0000) return (p->SR2 & Flag) ? 1 : 0;
    else return (p->SR1 & Flag) ? 1 : 0;
}

void I2C_PeriClockControl(I2C_RegDef_t *p, uint8_t En){
    if(p==I2C1) (En)? (RCC->APB1ENR|=(1<<21)) : (RCC->APB1ENR&=~(1<<21));
}

void I2C_Init(I2C_Handle_t *h){
    I2C_PeriClockControl(h->pI2Cx,1);
    h->pI2Cx->CR1 &= ~(1<<I2C_CR1_PE);

    uint32_t pclk1 = RCC_GetPCLK1Value();
    uint32_t fMHz  = pclk1/1000000U;
    h->pI2Cx->CR2  = (fMHz & 0x3F);

    h->pI2Cx->OAR1 = (1<<14) | ((h->I2CConfig.I2C_DeviceAddress & 0x7F)<<1);

    uint16_t ccr = (uint16_t)(pclk1/(2*h->I2CConfig.I2C_SCLSpeed));
    h->pI2Cx->CCR = ccr;
    h->pI2Cx->TRISE = fMHz+1;

    if(h->I2CConfig.I2C_ACKControl) h->pI2Cx->CR1|=(1<<I2C_CR1_ACK);

    h->pI2Cx->CR1|=(1<<I2C_CR1_PE);
}

void I2C_GenerateStartCondition(I2C_RegDef_t *p){ p->CR1 |= (1<<I2C_CR1_START); }
void I2C_GenerateStopCondition(I2C_RegDef_t *p){ p->CR1 |= (1<<I2C_CR1_STOP); }

void I2C_MasterSendData(I2C_Handle_t *h,uint8_t *tx,uint32_t len,uint8_t addr,uint8_t sr){
    I2C_GenerateStartCondition(h->pI2Cx);
    while(!I2C_GetFlagStatus(h->pI2Cx,I2C_FLAG_SB));
    I2C_ExecuteAddressPhaseWrite(h->pI2Cx,addr);
    while(!I2C_GetFlagStatus(h->pI2Cx,I2C_FLAG_ADDR));
    I2C_ClearADDRFlag(h);

    while(len--){
        while(!I2C_GetFlagStatus(h->pI2Cx,I2C_FLAG_TXE));
        h->pI2Cx->DR=*tx++;
    }
    while(!I2C_GetFlagStatus(h->pI2Cx,I2C_FLAG_TXE));
    while(!I2C_GetFlagStatus(h->pI2Cx,I2C_FLAG_BTF));
    if(sr==0) I2C_GenerateStopCondition(h->pI2Cx);
}

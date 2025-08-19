#include "stm32f407xx_i2c_driver.h"

static uint32_t RCC_GetPCLK1Value(void){ return 16000000U; } // adjust if you have a clock util

void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnOrDi){
  if (EnOrDi){
    if (pI2Cx==I2C1) RCC->APB1ENR |= (1U<<21);
    else if (pI2Cx==I2C2) RCC->APB1ENR |= (1U<<22);
    else if (pI2Cx==I2C3) RCC->APB1ENR |= (1U<<23);
  }else{
    if (pI2Cx==I2C1) RCC->APB1ENR &= ~(1U<<21);
    else if (pI2Cx==I2C2) RCC->APB1ENR &= ~(1U<<22);
    else if (pI2Cx==I2C3) RCC->APB1ENR &= ~(1U<<23);
  }
}

void I2C_Init(I2C_Handle_t *h){
  I2C_PeriClockControl(h->pI2Cx, ENABLE);
  h->pI2Cx->CR1 &= ~(1U<<I2C_CR1_PE);

  uint32_t pclk1=RCC_GetPCLK1Value(), fMHz=pclk1/1000000U;
  h->pI2Cx->CR2 = (h->pI2Cx->CR2 & ~0x3FU) | (fMHz & 0x3FU);
  h->pI2Cx->OAR1= (1U<<14) | ((h->I2CConfig.I2C_DeviceAddress & 0x7F)<<1);

  uint16_t ccr=0;
  if (h->I2CConfig.I2C_SCLSpeed<=I2C_SCL_SPEED_SM){
    ccr=(uint16_t)(pclk1/(2U*h->I2CConfig.I2C_SCLSpeed));
    h->pI2Cx->CCR=ccr;
    h->pI2Cx->TRISE=fMHz+1U;
  }else{
    h->pI2Cx->CCR=(1U<<15)|((h->I2CConfig.I2C_FMDutyCycle==I2C_FM_DUTY_16_9)?(1U<<14):0U);
    ccr=(h->I2CConfig.I2C_FMDutyCycle==I2C_FM_DUTY_16_9)?(uint16_t)(pclk1/(25U*h->I2CConfig.I2C_SCLSpeed))
                                                        :(uint16_t)(pclk1/(3U*h->I2CConfig.I2C_SCLSpeed));
    h->pI2Cx->CCR|=(ccr&0x0FFF);
    h->pI2Cx->TRISE=(uint32_t)((fMHz*300U)/1000U)+1U;
  }

  if (h->I2CConfig.I2C_ACKControl) h->pI2Cx->CR1 |= (1U<<I2C_CR1_ACK);
  else                             h->pI2Cx->CR1 &= ~(1U<<I2C_CR1_ACK);

  h->pI2Cx->CR1 |= (1U<<I2C_CR1_PE);
}

void I2C_GenerateStartCondition(I2C_RegDef_t *p){ p->CR1 |= (1U<<I2C_CR1_START); }
void I2C_GenerateStopCondition (I2C_RegDef_t *p){ p->CR1 |= (1U<<I2C_CR1_STOP);  }
void I2C_ManageAcking         (I2C_RegDef_t *p, uint8_t e){
  if (e) p->CR1|=(1U<<I2C_CR1_ACK); else p->CR1&=~(1U<<I2C_CR1_ACK);
}

static void I2C_ExecuteAddressPhaseWrite(I2C_RegDef_t *p, uint8_t addr){ p->DR=(addr<<1)&0xFE; }
static inline uint8_t I2C_GetFlagSR1(I2C_RegDef_t *p, uint32_t f){ return (p->SR1 & f)?1U:0U; }
static inline void I2C_ClearADDRFlag(I2C_RegDef_t *p){ volatile uint32_t d=p->SR1; (void)d; d=p->SR2; (void)d; }

void I2C_MasterSendData(I2C_Handle_t *h, uint8_t *tx, uint32_t len, uint8_t addr, uint8_t sr){
  I2C_GenerateStartCondition(h->pI2Cx);
  while(!I2C_GetFlagSR1(h->pI2Cx, (1U<<I2C_SR1_SB)));
  I2C_ExecuteAddressPhaseWrite(h->pI2Cx, addr);
  while(!I2C_GetFlagSR1(h->pI2Cx, (1U<<I2C_SR1_ADDR)));
  I2C_ClearADDRFlag(h->pI2Cx);

  while(len--){
    while(!I2C_GetFlagSR1(h->pI2Cx, (1U<<I2C_SR1_TXE)));
    h->pI2Cx->DR=*tx++;
  }
  while(!I2C_GetFlagSR1(h->pI2Cx, (1U<<I2C_SR1_TXE)));
  while(!I2C_GetFlagSR1(h->pI2Cx, (1U<<I2C_SR1_BTF)));
  if (sr==I2C_DISABLE_SR) I2C_GenerateStopCondition(h->pI2Cx);
}

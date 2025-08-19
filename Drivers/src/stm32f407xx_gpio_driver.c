#include "stm32f407xx_gpio_driver.h"

void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnOrDi){
  if (pGPIOx==GPIOA) { EnOrDi ? (RCC->AHB1ENR|=(1<<0)) : (RCC->AHB1ENR&=~(1<<0)); }
  else if (pGPIOx==GPIOB){ EnOrDi ? (RCC->AHB1ENR|=(1<<1)) : (RCC->AHB1ENR&=~(1<<1)); }
}

void GPIO_Init(GPIO_Handle_t *h){
  uint8_t pin=h->GPIO_PinConfig.GPIO_PinNumber;
  uint8_t mode=h->GPIO_PinConfig.GPIO_PinMode;
  GPIO_PeriClockControl(h->pGPIOx, ENABLE);

  h->pGPIOx->MODER   = (h->pGPIOx->MODER & ~(0x3U<<(2*pin))) | ((mode & 0x3U)<<(2*pin));
  h->pGPIOx->OTYPER  = (h->pGPIOx->OTYPER & ~(1U<<pin)) | ((h->GPIO_PinConfig.GPIO_PinOPType & 1U)<<pin);
  h->pGPIOx->OSPEEDR = (h->pGPIOx->OSPEEDR & ~(0x3U<<(2*pin))) | ((h->GPIO_PinConfig.GPIO_PinSpeed & 0x3U)<<(2*pin));
  h->pGPIOx->PUPDR   = (h->pGPIOx->PUPDR & ~(0x3U<<(2*pin))) | ((h->GPIO_PinConfig.GPIO_PinPuPdControl & 0x3U)<<(2*pin));

  if (mode==GPIO_MODE_ALTFN){
    uint8_t idx=pin/8, pos=(pin%8)*4;
    h->pGPIOx->AFR[idx]=(h->pGPIOx->AFR[idx] & ~(0xFU<<pos)) | ((h->GPIO_PinConfig.GPIO_PinAltFunMode & 0xF)<<pos);
  }
}

uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber){
  return (uint8_t)((pGPIOx->IDR>>PinNumber)&1U);
}

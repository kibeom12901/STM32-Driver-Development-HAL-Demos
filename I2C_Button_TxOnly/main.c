#include <string.h>
#include "../Drivers/inc/stm32f407xx_gpio_driver.h"
#include "../Drivers/inc/stm32f407xx_i2c_driver.h"

static void delay(volatile uint32_t t){ while(t--) __asm__("nop"); }

/* I2C1 pins: PB6=SCL, PB9=SDA (AF4, OD, PU) */
static void I2C1_GPIO_Init(void){
  GPIO_Handle_t p;
  GPIO_PeriClockControl(GPIOB, ENABLE);

  p.pGPIOx = GPIOB;
  p.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_ALTFN;
  p.GPIO_PinConfig.GPIO_PinOPType      = GPIO_OP_TYPE_OD;
  p.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_SPEED_FAST;
  p.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
  p.GPIO_PinConfig.GPIO_PinAltFunMode  = 4;

  p.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6; GPIO_Init(&p);   // SCL
  p.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_9; GPIO_Init(&p);   // SDA
}

static void Button_PA0_Init(void){
  GPIO_Handle_t btn;
  GPIO_PeriClockControl(GPIOA, ENABLE);
  btn.pGPIOx = GPIOA;
  btn.GPIO_PinConfig.GPIO_PinNumber      = GPIO_PIN_NO_0;
  btn.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_IN;
  btn.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_SPEED_FAST;
  btn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD; // change to PU/PD if needed
  GPIO_Init(&btn);
}

int main(void){
  I2C1_GPIO_Init();
  Button_PA0_Init();

  I2C_Handle_t hi2c1 = {0};
  hi2c1.pI2Cx = I2C1;
  hi2c1.I2CConfig.I2C_SCLSpeed      = I2C_SCL_SPEED_SM; // 100k
  hi2c1.I2CConfig.I2C_DeviceAddress = 0x61;             // own 7-bit
  hi2c1.I2CConfig.I2C_ACKControl    = I2C_ACK_ENABLE;
  hi2c1.I2CConfig.I2C_FMDutyCycle   = I2C_FM_DUTY_2;

  I2C_PeriClockControl(I2C1, ENABLE);
  I2C_Init(&hi2c1);

  const uint8_t SLAVE_ADDR = 0x3C;        // your Arduino / device address
  const char msg[] = "Hi from STM32!";

  while(1){
    if (GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0)){
      I2C_MasterSendData(&hi2c1, (uint8_t*)msg, (uint32_t)strlen(msg), SLAVE_ADDR, I2C_DISABLE_SR);

      /* simple hold-off so it doesn't spam while held */
      while (GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0));
      delay(200000);
    }
  }
}

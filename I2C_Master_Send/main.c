#include <string.h>
#include "Drivers/inc/stm32f407xx_gpio_driver.h"
#include "Drivers/inc/stm32f407xx_i2c_driver.h"

static void delay(volatile uint32_t t){ while(t--) __asm__("nop"); }

/* I2C1: PB6=SCL, PB9=SDA */
static void I2C1_GPIO_Init(void){
    GPIO_Handle_t g;
    GPIO_PeriClockControl(GPIOB,ENABLE);

    g.pGPIOx=GPIOB;
    g.GPIO_PinConfig.GPIO_PinMode=GPIO_MODE_ALTFN;
    g.GPIO_PinConfig.GPIO_PinOPType=GPIO_OP_TYPE_OD;
    g.GPIO_PinConfig.GPIO_PinSpeed=GPIO_SPEED_FAST;
    g.GPIO_PinConfig.GPIO_PinPuPdControl=GPIO_PIN_PU;
    g.GPIO_PinConfig.GPIO_PinAltFunMode=4;

    g.GPIO_PinConfig.GPIO_PinNumber=GPIO_PIN_NO_6; GPIO_Init(&g); // SCL
    g.GPIO_PinConfig.GPIO_PinNumber=GPIO_PIN_NO_9; GPIO_Init(&g); // SDA
}

/* PA0 button */
static void Button_Init(void){
    GPIO_Handle_t btn;
    GPIO_PeriClockControl(GPIOA,ENABLE);
    btn.pGPIOx=GPIOA;
    btn.GPIO_PinConfig.GPIO_PinNumber=GPIO_PIN_NO_0;
    btn.GPIO_PinConfig.GPIO_PinMode=GPIO_MODE_IN;
    btn.GPIO_PinConfig.GPIO_PinPuPdControl=GPIO_NO_PUPD;
    GPIO_Init(&btn);
}

int main(void){
    I2C1_GPIO_Init();
    Button_Init();

    I2C_Handle_t hi2c1;
    memset(&hi2c1,0,sizeof(hi2c1));
    hi2c1.pI2Cx=I2C1;
    hi2c1.I2CConfig.I2C_SCLSpeed=I2C_SCL_SPEED_SM;
    hi2c1.I2CConfig.I2C_DeviceAddress=0x61;
    hi2c1.I2CConfig.I2C_ACKControl=I2C_ACK_ENABLE;
    hi2c1.I2CConfig.I2C_FMDutyCycle=I2C_FM_DUTY_2;

    I2C_Init(&hi2c1);

    const uint8_t SLAVE_ADDR=0x3C;       // target slave address
    const char msg[]="Hello I2C Slave!";

    while(1){
        if(GPIO_ReadFromInputPin(GPIOA,GPIO_PIN_NO_0)){
            I2C_MasterSendData(&hi2c1,(uint8_t*)msg,strlen(msg),SLAVE_ADDR,0);
            delay(200000);
            while(GPIO_ReadFromInputPin(GPIOA,GPIO_PIN_NO_0));
        }
    }
}

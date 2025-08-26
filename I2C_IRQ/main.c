#include "stm32f407xx.h"
#include "stm32f407xx_gpio_driver.h"
#include "stm32f407xx_i2c_driver.h"
#include <stdio.h>

/* PB6=SCL, PB9=SDA */
#define I2C_SCL_PIN 6
#define I2C_SDA_PIN 9
#define SLAVE_ADDR  0x68   /* change to your target */

static I2C_Handle_t I2C1Handle;
static uint8_t rxBuf[6];

static void I2C1_GPIO_Init(void)
{
    GPIO_Handle_t g; g.pGPIOx = GPIOB;
    g.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    g.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
    g.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
    g.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;
    g.GPIO_PinConfig.GPIO_PinAltFunMode = 4; /* AF4 */

    g.GPIO_PinConfig.GPIO_PinNumber = I2C_SCL_PIN; GPIO_Init(&g);
    g.GPIO_PinConfig.GPIO_PinNumber = I2C_SDA_PIN; GPIO_Init(&g);
}

static void I2C1_Init_100kHz(void)
{
    I2C1Handle.pI2Cx = I2C1;
    I2C1Handle.I2C_Config.I2C_SCLSpeed = I2C_SCL_SPEED_SM;
    I2C1Handle.I2C_Config.I2C_DeviceAddress = 0x61; /* own addr */
    I2C1Handle.I2C_Config.I2C_ACKControl = I2C_ACK_ENABLE;
    I2C1Handle.I2C_Config.I2C_FMDutyCycle = I2C_FM_DUTY_2;
    I2C_Init(&I2C1Handle);
}

int main(void)
{
    I2C1_GPIO_Init();
    I2C1_Init_100kHz();

    /* NVIC */
    I2C_IRQPriorityConfig(IRQ_NO_I2C1_ER, 0);
    I2C_IRQPriorityConfig(IRQ_NO_I2C1_EV, 1);
    I2C_IRQInterruptConfig(IRQ_NO_I2C1_ER, ENABLE);
    I2C_IRQInterruptConfig(IRQ_NO_I2C1_EV, ENABLE);

    /* Start an IRQ-based read of 6 bytes */
    I2C_MasterReceiveDataIT(&I2C1Handle, rxBuf, sizeof(rxBuf), SLAVE_ADDR, RESET);

    while (1) { /* idle */ }
}

/* Vector forwarding */
void I2C1_EV_IRQHandler(void) { I2C_EV_IRQHandling(&I2C1Handle); }
void I2C1_ER_IRQHandler(void) { I2C_ER_IRQHandling(&I2C1Handle); }

/* App callback */
void I2C_ApplicationEventCallback(I2C_Handle_t *h, uint8_t evt)
{
    if (evt == I2C_EV_RX_CMPLT) {
        printf("RX: ");
        for (uint32_t i=0;i<h->RxSize;i++) printf("%02X ", rxBuf[i]);
        printf("\n");
    } else if (evt == I2C_ERROR_AF)      { printf("I2C: NACK\n"); }
      else if (evt == I2C_ERROR_BERR)    { printf("I2C: Bus error\n"); }
      else if (evt == I2C_ERROR_ARLO)    { printf("I2C: Arbitration lost\n"); }
      else if (evt == I2C_ERROR_OVR)     { printf("I2C: Overrun\n"); }
      else if (evt == I2C_ERROR_TIMEOUT) { printf("I2C: Timeout\n"); }
}

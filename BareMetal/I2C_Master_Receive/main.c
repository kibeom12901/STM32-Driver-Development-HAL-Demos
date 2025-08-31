#include <string.h>
#include <stdint.h>
#include "./Drivers/inc/stm32f407xx_gpio_driver.h"
#include "./Drivers/inc/stm32f407xx_i2c_driver.h"

/* Optional semihosted printf; comment out if unused */
extern void initialise_monitor_handles(void);
#include <stdio.h>

static void delay(volatile uint32_t t){ while(t--) __asm__("nop"); }

/* PB6=SCL, PB9=SDA : AF4, OD, internal pull-ups (per exercise) */
static void I2C1_GPIO_Init(void)
{
    GPIO_Handle_t p;

    GPIO_PeriClockControl(GPIOB, ENABLE);

    p.pGPIOx = GPIOB;
    p.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_ALTFN;
    p.GPIO_PinConfig.GPIO_PinOPType      = GPIO_OP_TYPE_OD;
    p.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_SPEED_FAST;
    p.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;   /* internal PU */
    p.GPIO_PinConfig.GPIO_PinAltFunMode  = 4;             /* AF4 = I2C */

    p.GPIO_PinConfig.GPIO_PinNumber      = GPIO_PIN_NO_6; /* SCL */
    GPIO_Init(&p);
    p.GPIO_PinConfig.GPIO_PinNumber      = GPIO_PIN_NO_9; /* SDA */
    GPIO_Init(&p);
}

/* PA0 button */
static void Button_PA0_Init(void)
{
    GPIO_Handle_t btn;

    GPIO_PeriClockControl(GPIOA, ENABLE);

    btn.pGPIOx = GPIOA;
    btn.GPIO_PinConfig.GPIO_PinNumber      = GPIO_PIN_NO_0;
    btn.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_IN;
    btn.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_SPEED_FAST;
    btn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD; /* change to PU/PD if needed */
    GPIO_Init(&btn);
}

int main(void)
{
    /* comment out if not using semihosted printf */
    initialise_monitor_handles();
    printf("\n[I2C] Master Receive (0x51/0x52) @100kHz on PB6/PB9 (PU). Press PA0.\n");

    I2C1_GPIO_Init();
    Button_PA0_Init();

    I2C_Handle_t hi2c1;
    memset(&hi2c1, 0, sizeof(hi2c1));
    hi2c1.pI2Cx = I2C1;
    hi2c1.I2CConfig.I2C_SCLSpeed      = I2C_SCL_SPEED_SM;  /* 100 kHz */
    hi2c1.I2CConfig.I2C_DeviceAddress = 0x61;              /* own addr (unused as master) */
    hi2c1.I2CConfig.I2C_ACKControl    = I2C_ACK_ENABLE;
    hi2c1.I2CConfig.I2C_FMDutyCycle   = I2C_FM_DUTY_2;

    I2C_PeriClockControl(I2C1, ENABLE);
    I2C_Init(&hi2c1);

    const uint8_t SLAVE_ADDR = 0x3C;  /* match your Arduino */

    while (1) {
        if (GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0)) {

            /* Step 1: send command 0x51 (request length) */
            uint8_t cmd = 0x51;
            I2C_MasterSendData(&hi2c1, &cmd, 1, SLAVE_ADDR, I2C_DISABLE_SR);

            /* Step 2: read 1-byte length */
            uint8_t len = 0;
            I2C_MasterReceiveData(&hi2c1, &len, 1, SLAVE_ADDR, I2C_DISABLE_SR);

            if (len == 0 || len > 64) {
                printf("[I2C] invalid len=%u\n", len);
            } else {
                /* Step 3: send command 0x52 (request data) */
                cmd = 0x52;
                I2C_MasterSendData(&hi2c1, &cmd, 1, SLAVE_ADDR, I2C_DISABLE_SR);

                /* Step 4: read payload */
                uint8_t buf[65] = {0};
                I2C_MasterReceiveData(&hi2c1, buf, len, SLAVE_ADDR, I2C_DISABLE_SR);
                buf[len] = '\0';
                printf("[I2C] len=%u data=\"%s\"\n", len, buf);
            }

            /* simple debounce & avoid repeat while held */
            while (GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0));
            delay(200000);
        }
    }
}

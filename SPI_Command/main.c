#include <stdio.h>
#include <string.h>
#include "stm32f4xx_custom.h"
#include "gpio_driver.h"
#include "spi_driver.h"

// semihosted printf
extern void initialise_monitor_handles(void);

/* ==== COMMAND & PIN DEFINITIONS ==== */
#define CMD_LED_CTRL      0x50
#define CMD_SENSOR_READ   0x51
#define CMD_LED_READ      0x52

#define LED_OFF           0
#define LED_ON            1

#define LED_PIN           9
#define ANALOG_PIN0       0

#define ACK_BYTE          0xF5
#define DUMMY_BYTE        0xFF

static void delay(void){
    for (volatile uint32_t i = 0; i < 500000; i++);
}

static void GPIO_ButtonInit(void){
    GPIO_Handle_t btn = {0};
    btn.pGPIOx                             = GPIOA;
    btn.GPIO_PinConfig.GPIO_PinNumber      = GPIO_PIN_NO_0;
    btn.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_IN;
    btn.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_SPEED_FAST;
    btn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
    GPIO_PeriClockControl(GPIOA, ENABLE);
    GPIO_Init(&btn);
}

static void SPI2_GPIOInits(void){
    GPIO_Handle_t sp = {0};
    sp.pGPIOx                             = GPIOB;
    sp.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_ALTFN;
    sp.GPIO_PinConfig.GPIO_PinAltFunMode  = 5;
    sp.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_SPEED_FAST;
    sp.GPIO_PinConfig.GPIO_PinOPType      = GPIO_OP_TYPE_PP;
    sp.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

    sp.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12; // NSS
    GPIO_Init(&sp);
    sp.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13; // SCLK
    GPIO_Init(&sp);
    sp.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15; // MOSI
    GPIO_Init(&sp);
}

static void SPI2_Inits(void){
    SPI_Handle_t hspi = {0};
    hspi.pSPIx                    = SPI2;
    hspi.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
    hspi.SPIConfig.SPI_BusConfig  = SPI_BUSCONFIG_FD;
    hspi.SPIConfig.SPI_SclkSpeed  = SPI_SCLK_SPEED_DIV8;
    hspi.SPIConfig.SPI_DFF        = SPI_DFF_8BIT;
    hspi.SPIConfig.SPI_CPOL       = SPI_CPOL_LOW;
    hspi.SPIConfig.SPI_CPHA       = SPI_CPHA_LOW;
    hspi.SPIConfig.SPI_SSM        = SPI_SSM_DISABLE;
    SPI_Init(&hspi);
}

static uint8_t SPI_VerifyResponse(uint8_t ack){
    return (ack == ACK_BYTE);
}

int main(void){
    uint8_t cmd, ack, dummy_read, led_state, sensor_lsb, sensor_msb;
    uint8_t dummy = DUMMY_BYTE;
    uint16_t sensor_val;

    initialise_monitor_handles();
    printf("Starting SPI master with semihosted debug...\n");

    GPIO_ButtonInit();
    SPI2_GPIOInits();
    SPI2_Inits();
    SPI_SSOEConfig(SPI2, ENABLE);

    while (1){
        // STEP 1: LED_CTRL
        while (!GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0));
        delay();
        SPI_PeripheralControl(SPI2, ENABLE);

        cmd = CMD_LED_CTRL;
        SPI_SendData(SPI2, &cmd, 1);
        SPI_ReceiveData(SPI2, &dummy_read, 1);
        SPI_SendData(SPI2, &dummy, 1);
        SPI_ReceiveData(SPI2, &ack, 1);
        printf("STEP1: CMD_LED_CTRL, ACK=0x%02X\n", ack);

        if (SPI_VerifyResponse(ack)){
            uint8_t args[2] = { LED_PIN, LED_ON };
            SPI_SendData(SPI2, args, 2);
            SPI_ReceiveData(SPI2, &dummy_read, 1);
            SPI_ReceiveData(SPI2, &dummy_read, 1);
            printf("STEP1: Payload sent (pin=%d,state=%d)\n", LED_PIN, LED_ON);
        }

        while (SPI_GetFlagStatus(SPI2, SPI_BUSY_FLAG));
        SPI_PeripheralControl(SPI2, DISABLE);

        // STEP 2: SENSOR_READ
        while (!GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0));
        delay();
        SPI_PeripheralControl(SPI2, ENABLE);

        cmd = CMD_SENSOR_READ;
        SPI_SendData(SPI2, &cmd, 1);
        SPI_ReceiveData(SPI2, &dummy_read, 1);
        SPI_SendData(SPI2, &dummy, 1);
        SPI_ReceiveData(SPI2, &ack, 1);
        printf("STEP2: CMD_SENSOR_READ, ACK=0x%02X\n", ack);

        if (SPI_VerifyResponse(ack)){
            uint8_t pin = ANALOG_PIN0;
            SPI_SendData(SPI2, &pin, 1);
            SPI_ReceiveData(SPI2, &dummy_read, 1);

            SPI_SendData(SPI2, &dummy, 1);
            SPI_ReceiveData(SPI2, &sensor_lsb, 1);

            SPI_SendData(SPI2, &dummy, 1);
            SPI_ReceiveData(SPI2, &sensor_msb, 1);

            sensor_val = (sensor_msb << 8) | sensor_lsb;
            printf("STEP2: ADC = %u\n", sensor_val);
        }

        while (SPI_GetFlagStatus(SPI2, SPI_BUSY_FLAG));
        SPI_PeripheralControl(SPI2, DISABLE);

        // STEP 3: LED_READ
        while (!GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0));
        delay();
        SPI_PeripheralControl(SPI2, ENABLE);

        cmd = CMD_LED_READ;
        SPI_SendData(SPI2, &cmd, 1);
        SPI_ReceiveData(SPI2, &dummy_read, 1);
        SPI_SendData(SPI2, &dummy, 1);
        SPI_ReceiveData(SPI2, &ack, 1);
        printf("STEP3: CMD_LED_READ, ACK=0x%02X\n", ack);

        if (SPI_VerifyResponse(ack)){
            SPI_SendData(SPI2, &LED_PIN, 1);
            SPI_ReceiveData(SPI2, &dummy_read, 1);

            SPI_SendData(SPI2, &dummy, 1);
            SPI_ReceiveData(SPI2, &led_state, 1);
            printf("STEP3: LED state = %s\n", led_state==LED_ON?"ON":"OFF");
        }

        while (SPI_GetFlagStatus(SPI2, SPI_BUSY_FLAG));
        SPI_PeripheralControl(SPI2, DISABLE);
    }
}

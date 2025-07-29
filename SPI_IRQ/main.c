#include <stdio.h>
#include <string.h>
#include "stm32f4xx_custom.h"
#include "gpio_driver.h"
#include "spi_driver.h"

/* global SPI handle for ISR */
SPI_Handle_t hspi2;

/* semihosted printf */
extern void initialise_monitor_handles(void);

/* commands & pins */
#define CMD_LED_CTRL      0x50
#define CMD_SENSOR_READ   0x51
#define CMD_LED_READ      0x52

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
    hspi2.pSPIx                    = SPI2;
    hspi2.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
    hspi2.SPIConfig.SPI_BusConfig  = SPI_BUSCONFIG_FD;
    hspi2.SPIConfig.SPI_SclkSpeed  = SPI_SCLK_SPEED_DIV8;
    hspi2.SPIConfig.SPI_DFF        = SPI_DFF_8BIT;
    hspi2.SPIConfig.SPI_CPOL       = SPI_CPOL_LOW;
    hspi2.SPIConfig.SPI_CPHA       = SPI_CPHA_LOW;
    hspi2.SPIConfig.SPI_SSM        = SPI_SSM_DISABLE;
    SPI_Init(&hspi2);
}

static uint8_t SPI_VerifyResponse(uint8_t ack){
    return (ack == ACK_BYTE);
}

/* override callback to get notified */
void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle, uint8_t AppEv) {
    if (AppEv == SPI_EVENT_TX_CMPLT) {
        printf(">> SPI TX complete\r\n");
    } else if (AppEv == SPI_EVENT_RX_CMPLT) {
        printf(">> SPI RX complete\r\n");
    } else if (AppEv == SPI_EVENT_OVR_ERR) {
        printf(">> SPI Overrun error\r\n");
    }
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

    /* NVIC setup */
    NVIC_EnableIRQ(SPI2_IRQn);
    NVIC_SetPriority(SPI2_IRQn, 1);

    while (1){
        // … (your three blocking steps unchanged) …
    }
}

/* SPI2 IRQ handler — uses our IRQ logic + callback */
void SPI2_IRQHandler(void) {
    SPI_IRQHandling(&hspi2);
}

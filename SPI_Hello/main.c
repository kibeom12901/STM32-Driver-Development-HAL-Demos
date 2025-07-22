#include <string.h>
#include "gpio_driver.h"
#include "spi_driver.h"

/* ~200 ms delay (crude debounce) */
static void delay(void){
    for(volatile uint32_t i=0;i<500000;i++);
}

/* PA0 button input */
static void GPIO_ButtonInit(void){
    GPIO_Handle_t btn = {0};
    btn.pGPIOx                             = GPIOA;
    btn.GPIO_PinConfig.GPIO_PinNumber      = 0;
    btn.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_IN;
    btn.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_SPEED_FAST;
    btn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

    GPIO_PeriClockControl(GPIOA, ENABLE);
    GPIO_Init(&btn);
}

/* PB12=NSS, PB13=SCLK, PB15=MOSI */
static void SPI2_GPIOInits(void){
    GPIO_Handle_t sp = {0};
    sp.pGPIOx                             = GPIOB;
    sp.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_ALTFN;
    sp.GPIO_PinConfig.GPIO_PinAltFunMode  = 5;
    sp.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_SPEED_FAST;
    sp.GPIO_PinConfig.GPIO_PinOPType      = GPIO_OP_TYPE_PP;
    sp.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

    sp.GPIO_PinConfig.GPIO_PinNumber = 12; // NSS
    GPIO_Init(&sp);
    sp.GPIO_PinConfig.GPIO_PinNumber = 13; // SCLK
    GPIO_Init(&sp);
    sp.GPIO_PinConfig.GPIO_PinNumber = 15; // MOSI
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
    hspi.SPIConfig.SPI_SSM        = SPI_SSM_DISABLE;   // HW NSS
    SPI_Init(&hspi);
}

int main(void)
{
    char user_data[] = "Hello world";

    GPIO_ButtonInit();
    SPI2_GPIOInits();
    SPI2_Inits();

    SPI_SSOEConfig(SPI2, ENABLE); // enable HW NSS output

    while (1)
    {
        while (!GPIO_ReadFromInputPin(GPIOA, 0)); // wait press
        delay();                                  // debounce

        SPI_PeripheralControl(SPI2, ENABLE);      // NSS low

        uint8_t dataLen = strlen(user_data);      // length byte
        SPI_SendData(SPI2, &dataLen, 1);
        SPI_SendData(SPI2, (uint8_t*)user_data, dataLen); // payload

        while (SPI_GetFlagStatus(SPI2, SPI_BUSY_FLAG));   // wait idle
        SPI_PeripheralControl(SPI2, DISABLE);     // NSS high
    }
    return 0;
}

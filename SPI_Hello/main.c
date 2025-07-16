#include <string.h>
#include "gpio_driver.h"
#include "spi_driver.h"

/* small delay */
static void delay(void){
    for(volatile uint32_t i=0;i<300000;i++);
}

/* PB12-PB15 -> SPI2 AF5 */
void SPI2_GPIOInits(void){
    GPIO_Handle_t h = {0};
    h.pGPIOx = GPIOB;
    h.GPIO_PinConfig.GPIO_PinMode      = GPIO_MODE_ALTFN;
    h.GPIO_PinConfig.GPIO_PinAltFunMode= 5;
    h.GPIO_PinConfig.GPIO_PinOPType    = GPIO_OP_TYPE_PP;
    h.GPIO_PinConfig.GPIO_PinPuPdControl=GPIO_NO_PUPD;
    h.GPIO_PinConfig.GPIO_PinSpeed     = GPIO_SPEED_FAST;

    for(uint8_t pin=12; pin<=15; pin++){
        h.GPIO_PinConfig.GPIO_PinNumber = pin;
        GPIO_Init(&h);
    }
}

/* SPI2 as master, full-duplex, fPCLK/8, 8-bit, CPOL=0, CPHA=0, SSM=1 */
void SPI2_Inits(void){
    SPI_Handle_t hspi = {0};
    hspi.pSPIx = SPI2;
    hspi.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
    hspi.SPIConfig.SPI_BusConfig  = SPI_BUSCONFIG_FD;
    hspi.SPIConfig.SPI_SclkSpeed  = SPI_SCLK_SPEED_DIV8;
    hspi.SPIConfig.SPI_DFF        = SPI_DFF_8BIT;
    hspi.SPIConfig.SPI_CPOL       = SPI_CPOL_LOW;
    hspi.SPIConfig.SPI_CPHA       = SPI_CPHA_LOW;
    hspi.SPIConfig.SPI_SSM        = SPI_SSM_ENABLE;
    SPI_Init(&hspi);
}

int main(void){
    char msg[] = "HELLO";
    SPI2_GPIOInits();
    SPI2_Inits();
    SPI_SSIConfig(SPI2, ENABLE);
    SPI_PeripheralControl(SPI2, ENABLE);

    while(1){
        SPI_SendData(SPI2, (uint8_t*)msg, strlen(msg));
        delay();
    }
    return 0;
}

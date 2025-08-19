# stm32_playground

A set of small STM32 projects for testing basic embedded features.

## Projects

- **Toggle_light** – simple example that blinks an LED using a custom GPIO driver.
- **ManualGPIO_Toggle** – toggles an LED when a button is pressed using a lightweight driver implementation.
- **ExternalButton_IT** – toggles PD12 through an interrupt triggered by the PA0 user button with a small debounce delay.
- **SPI_Hello** – Master sends a length byte + “Hello world” over SPI2 (PB12/13/15). Arduino (slave) reads the framed packet and prints it via Serial. Shows hardware NSS (SSOE), blocking TX, and basic framing.
- **SPI_Command** – advanced SPI master example with semihosted debug output and an Arduino-based slave using a command/ack protocol.
- **SPI_IRQ** – demonstrates SPI transfers using interrupts with an application callback for completion events.
- **I2C_Button_TxOnly** – presses PA0 to send "Hi from STM32!" over I2C1 (PB6/PB9).

This repository is my sandbox for experimenting with peripherals such as ADC, I2C and general GPIO control as I learn embedded programming.

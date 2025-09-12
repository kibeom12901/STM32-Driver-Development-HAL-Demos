# stm32_playground

A set of small STM32 projects for testing basic embedded features.

---

## Projects

Projects are organized into two categories:  

- **BareMetal/** – examples using minimal drivers and direct register access  
- **HAL_Practice/** – experiments built on STM32 HAL

---

### BareMetal

Examples that use minimal drivers and direct register access:

- [**Toggle_light**](BareMetal/Toggle_light/) – blinks an LED using a custom GPIO driver.  
- [**ManualGPIO_Toggle**](BareMetal/ManualGPIO_Toggle/) – toggles an LED when a button is pressed using a lightweight driver.  
- [**ExternalButton_IT**](BareMetal/ExternalButton_IT/) – toggles PD12 through an interrupt triggered by the PA0 user button with a small debounce delay.  
- [**SPI_Hello**](BareMetal/SPI_Hello/) – Master sends a length byte + “Hello world” over SPI2 (PB12/13/15). Arduino (slave) reads the framed packet and prints it via Serial. Shows hardware NSS (SSOE), blocking TX, and basic framing.  
- [**SPI_Command**](BareMetal/SPI_Command/) – advanced SPI master example with semihosted debug output and an Arduino-based slave using a command/ack protocol.  
- [**SPI_IRQ**](BareMetal/SPI_IRQ/) – demonstrates SPI transfers using interrupts with an application callback for completion events.  
- [**I2C_Master_Send**](BareMetal/I2C_Master_Send/) – sends a greeting over I2C1 to a slave device whenever the user button is pressed.  
- [**I2C_Master_Receive**](BareMetal/I2C_Master_Receive/) – Master reads a byte stream from Arduino (slave) over I2C1. Shows basic blocking RX with ACK handling.  
- [**I2C_IRQ**](BareMetal/I2C_IRQ/) – handles I2C communication using interrupt-driven transfers.  

---

### HAL_Practice

Examples using STM32 HAL:

- [**LCD_Stopwatch**](HAL_Practice/Stopwatch_LCD+TIM/) – 16×2 LCD demo showing a stopwatch (MM:SS.CS). Uses TIM7 @ 10 ms and updates the LCD from the main loop.  
- [**PWM_Demo**](HAL_Practice/PWM/) – quick notes and experiments with PWM. Shows how PSC (Prescaler), ARR (Auto-Reload), and CCR (Compare) control frequency and duty cycle. Useful for LED dimming, motor control, and buzzer tones.
- [**ADC_WaterSensor**](HAL_Practice/ADC_WaterSensor/) – interfaces a simple water sensor (analog output) with STM32. The ADC value is read via DMA on PC3 (ADC1_IN13), displayed on a 16×2 LCD, and mapped to a PWM output (TIM5_CH4 on PA3) to control LED brightness. Higher water levels → higher ADC values → brighter LED.

(More HAL-based projects will be added here.)

---

### PWM Quick Notes

- **PSC (Prescaler):** divides timer clock. Lower PSC = higher frequency.  
- **ARR (Auto-Reload Register):** max counter value → sets period with PSC.  
- **CCR (Compare Register):** ON-time within each cycle → sets duty cycle.  

Think of it this way:
- **ARR** → period length (when counter resets to 0).  
- **CCR (Pulse)** → number of ticks the output stays HIGH.  

Formulas:  
![fPWM](https://latex.codecogs.com/svg.latex?f_{PWM}=\frac{f_{CLK}}{(PSC+1)(ARR+1)})  
![Duty](https://latex.codecogs.com/svg.latex?Duty\%=\frac{CCR}{ARR+1}\times100)

- Frequency = how fast cycles repeat.  
- Duty cycle = how much of each cycle is HIGH (average output power).  

**Uses:** LED dimming, motor control, buzzer tones, signal generation.

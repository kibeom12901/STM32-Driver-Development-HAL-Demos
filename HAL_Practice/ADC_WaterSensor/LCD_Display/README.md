# ADC_WaterSensor

This project demonstrates how to interface a **water level sensor** with an **STM32F407** using the STM32 HAL library.  
The sensor’s analog output is read through the **ADC**, displayed on a **16×2 LCD**, and used to control the brightness of an **external LED** via **PWM**.

---

## 🔧 Features
- **ADC (PC3 / ADC1_IN13)** reads the analog signal from the water sensor.  
- **DMA** is used to continuously update ADC values in memory without CPU overhead.  
- **PWM (TIM5_CH4 on PA3)** drives an LED, with brightness proportional to the water sensor value.  
- **16×2 LCD** displays both the raw ADC reading and the mapped duty cycle percentage.

---

## ⚡ Hardware Connections
- **Water Sensor**  
  - `+` → 3.3V  
  - `-` → GND  
  - `S` → PC3 (ADC input)  

- **LED (with ~220 Ω resistor)**  
  - `PA3 (TIM5_CH4, PWM output)` → Resistor → LED → GND  

- **LCD 16×2 (HD44780)**  
  - Connected in 4-bit mode via custom `clcd.c` driver  

---

## 📊 Behavior
- As the water sensor gets **wetter**, its output voltage increases.  
- The ADC value rises (approx. **400 → 2000** in testing).  
- The LED brightness increases with the ADC value.  
- The LCD shows:  
  - **Line 1**: ADC value  
  - **Line 2**: Duty cycle %  

---

## FINAL PRODUCT
<img width="947" height="578" alt="Screenshot 2025-09-12 at 9 59 27 AM" src="https://github.com/user-attachments/assets/34cac537-0500-4c5e-80c1-04c2afea995b" />


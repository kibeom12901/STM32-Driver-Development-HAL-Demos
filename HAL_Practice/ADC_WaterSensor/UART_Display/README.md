# STM32F4 – ADC2 Water Sensor Logger

This project demonstrates how to use the **STM32F407 Discovery** board to read a water sensor (or any analog voltage) on **PA1 (ADC2_IN1)** and display the readings over **USART2** to a PC terminal.  

---

## 📌 Features
- Uses **ADC2** to sample the analog water sensor
- Reads at **12-bit resolution** (0–4095)
- Computes the equivalent **voltage (0–3.3V)** using VREF = 3.3V
- Averages multiple samples for stability
- Sends results over **USART2** (PA2 → USB-UART dongle) at **115200 baud**
- Output format:

---

## 🖧 Hardware Setup

### 🔌 Pin Connections

| STM32F407 Pin | Function   | Connection                  |
|---------------|-----------|-----------------------------|
| **PA1**       | ADC2_IN1  | Water sensor analog output  |
| **3V3**       | VCC       | Water sensor VCC (if 3.3V)  |
| **GND**       | GND       | Water sensor ground         |
| **PA2**       | USART2_TX | To USB-UART RX → PC terminal |
| **GND**       | GND       | Common ground with USB-UART |

⚠️ **Important:**  
- If your sensor is **5V only**, use a resistor divider (e.g., 10k:20k) to bring max output into 3.3V range.  
- PC connection: use **ST-LINK VCP** or an **external USB-UART module**.

---

## ⚙️ STM32CubeMX Configuration

### ADC2 (Water Sensor Input)
- Instance: **ADC2**
- Channel: **IN1 (PA1)**
- Mode: **Single Conversion**
- Trigger: **Software Start**
- Resolution: **12 bits**
- Alignment: **Right**
- Continuous Conv.: **Disabled** (we trigger manually)
- DMA: **Disabled** (simple polling)

### USART2 (PC Terminal)
- Mode: **Asynchronous**
- Baud Rate: **115200**
- Word Length: **8 bits**
- Stop Bits: **1**
- Parity: **None**
- Flow Control: **None**
- Pins:  
  - **PA2 → TX**
  - (RX not required unless you want to receive from PC)

### Clock
- Default STM32F4 Discovery settings:  
  - HSE = 8 MHz → PLL → SYSCLK 168 MHz

---

## 📝 Example Code (main loop)

<img width="579" height="168" alt="Screenshot 2025-09-17 at 3 19 45 PM" src="https://github.com/user-attachments/assets/90c62aac-f234-45b1-835d-7d02b8233736" />

---

## 🖥️ USART Display

1. Connect a **USB–UART** module (or use **ST-LINK VCP**).
2. Open a serial terminal and configure:

- **Baud:** 115200  
- **Data bits:** 8  
- **Stop bits:** 1  
- **Parity:** None  
- **Flow Control:** None

## RESULT

## Outside the CUP
<img width="578" height="366" alt="Screenshot 2025-09-17 at 3 12 01 PM" src="https://github.com/user-attachments/assets/02275518-99c5-49b9-8644-2a1227dd35cc" />

## Inside the CUP
<img width="584" height="363" alt="Screenshot 2025-09-17 at 3 16 08 PM" src="https://github.com/user-attachments/assets/7773de47-15b0-4c9a-9f2d-c727c87d7494" />

### LCD_Stopwatch (HAL)

**What it is:**  
A simple **MM:SS.CS** (minutes:seconds:centiseconds) stopwatch on a 16×2 HD44780 LCD, updated via **TIM7 @ 10 ms**.  
LCD writes happen in the main loop (not in ISR) for reliability.

---

#### 📌 Wiring (4-bit mode, Port E)

| LCD Pin | Function        | STM32                                             |
| ------- | --------------- | ------------------------------------------------- |
| 1 VSS   | GND             | GND                                               |
| 2 VDD   | VCC             | +5V (or 3.3V if module supports)                  |
| 3 V0    | Contrast        | 10k pot wiper (ends: VDD & GND)                   |
| 4 RS    | Register Select | **PE0**                                           |
| 5 RW    | Read/Write      | **GND** (recommended) *(or PE2 if you enable it)* |
| 6 E     | Enable          | **PE1**                                           |
| 11 D4   | Data 4          | **PE4**                                           |
| 12 D5   | Data 5          | **PE5**                                           |
| 13 D6   | Data 6          | **PE6**                                           |
| 14 D7   | Data 7          | **PE7**                                           |
| 15 BLA  | Backlight +     | +5V (via 100–220Ω)                                |
| 16 BLK  | Backlight –     | GND                                               |

---

#### ⏱ Timer setup (TIM7 @ 10 ms)

* APB1 timer clock = 84 MHz  
* `Prescaler = 8399` → 84 MHz / (8399+1) = **10 kHz**  
* `Period = 99` → 10 kHz / (99+1) = **100 Hz** ⇒ **10 ms** per update event  

  <img width="506" height="243" alt="Screenshot 2025-09-09 at 9 37 53 AM" src="https://github.com/user-attachments/assets/9f593b8f-9c15-4571-9b93-70c0cbd26059" />

#### FINAL PRODUCT

<img width="695" height="520" alt="Screenshot 2025-09-09 at 9 39 26 AM" src="https://github.com/user-attachments/assets/a8e098a7-7b1d-4af0-9565-355b91df7bd6" />

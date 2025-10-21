# Stage 06 — VS1003 + FatFS Refinements

Enhancements to the SD/FatFS MP3 player:

- Added retry logic for SD mount  
- Implemented single, double, and long press gestures on the PA0 user button  
- Added shuffle mode with in-place playlist reshuffle  
- Improved playback stability during track transitions  
- Verified stable SD communication and FatFS mounting behavior under long sessions  

---

## 🧩 ADC Test (Preparation for Volume Control)

<img width="536" height="377" alt="Screenshot 2025-10-21 at 2 16 48 PM" src="https://github.com/user-attachments/assets/15e1fcf4-92ff-46ca-8822-97f920ee55a1" />

Before integrating analog volume control, the ADC input was validated using a potentiometer connected to **PA1 (ADC1_IN1)**.

### ⚙️ Configuration
| Component | Setting |
|------------|----------|
| **Pin** | PA1 (ADC1_IN1) |
| **Mode** | Analog mode (No pull-up/pull-down) |
| **ADC Mode** | Single conversion, software-triggered |
| **Sampling Time** | 144 cycles |
| **Polling Method** | HAL_ADC_Start / PollForConversion |
| **Output** | Live UART print (USART2 @ 115200 baud) |

---

### 🧪 Test Output
After flashing the firmware, turning the potentiometer produced live UART output:

<img width="182" height="168" alt="Screenshot 2025-10-21 at 2 17 57 PM" src="https://github.com/user-attachments/assets/7dea0294-ed0e-4bda-b2cf-117876d952d4" />


## PWM Applications

### 1. Servo Motor (e.g. SG90)
- **Operating Voltage:** 3.0–7.2 V  
- **PWM Frequency:** 50 Hz (20 ms period)  
- **Control Method:** Pulse width determines angle  
  - 0.2 ms → full CW  
  - 1.5 ms → center  
  - 3.0 ms → full CCW  
- **Wiring:**  
  - Brown = GND  
  - Red = +5 V  
  - Orange = PWM input  

➡️ Position is controlled by **pulse width (CCR)** inside a fixed 20 ms cycle.

---

### 2. Passive Buzzer
- **Operating Voltage:** 3.3–5 V (matches PWM output level)  
- **PWM Duty Cycle:** Fixed at 50%  
- **Control Method:** Frequency determines tone  
  - 400 Hz → lower pitch  
  - 500 Hz → higher pitch  
- **Note:** Buzzer is polarized (+, –)  

➡️ Sound pitch is controlled by **PWM frequency**, not pulse width.

---

### Quick Comparison

| Device        | Controlled By      | Fixed Parameter | Example Use            |
|---------------|--------------------|-----------------|------------------------|
| Servo (SG90)  | Pulse width (CCR)  | Frequency = 50 Hz | Position control       |
| Buzzer        | Frequency (Hz)     | Duty = 50%      | Tone / pitch generation |

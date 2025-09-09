## PWM Applications

This project demonstrates how PWM (Pulse Width Modulation) can be applied to control different devices: **servo motor**, **passive buzzer**, and **DC motor**.

---

### 1. Servo Motor (e.g., SG90)

- **Operating Voltage:** 3.0–7.2 V  
- **PWM Frequency:** 50 Hz (20 ms period)  
- **Control Method:** Position (angle) is controlled by **pulse width**  
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

- **Operating Voltage:** 3.3–5 V  
- **PWM Duty Cycle:** Fixed at 50%  
- **Control Method:** **Frequency** determines tone  
  - 400 Hz → lower pitch  
  - 500 Hz → higher pitch  
- **Note:** Buzzer is polarized (+, –)  

➡️ Pitch is controlled by **PWM frequency**, not duty cycle.

---

### 3. DC Motor (via BA6208 or H-Bridge)

- **Operating Voltage:** 4.5–15 V (depends on motor + driver)  
- **PWM Frequency:** Typically ~100 Hz (datasheet suggests no strict limit)  
- **Control Method:**  
  - **Duty Cycle (0–100%)** → Proportional to speed  
  - **Input Signals (AIN/BIN)** → Determines direction (forward/reverse)  
- **Wiring (BA6208 example):**  
  - `AIN` / `BIN` = PWM inputs (set according to truth table)  
  - `Vcc` = Motor supply  
  - `GND` = Ground  
  - `Aout` / `Bout` = Motor terminals  

➡️ Speed is controlled by **duty cycle**.  
➡️ Direction is controlled by **input combination**.

---

### Quick Comparison

| Device        | Controlled By      | Fixed Parameter   | Example Use             |
|---------------|--------------------|-------------------|-------------------------|
| Servo (SG90)  | Pulse width (CCR)  | Frequency = 50 Hz | Angle / position        |
| Buzzer        | Frequency (Hz)     | Duty = 50%        | Tone / pitch generation |
| DC Motor      | Duty cycle (%)     | Frequency ≈ 100 Hz| Speed + direction       |

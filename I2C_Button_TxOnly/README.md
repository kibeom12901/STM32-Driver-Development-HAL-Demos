# STM32F4 I2C Master TX Only (PA0 triggers send)

- STM32F407: I2C1 on PB6 (SCL), PB9 (SDA), AF4, Open-Drain, Pull-Up.
- Press **PA0** → send `"Hi from STM32!"` to I2C slave `0x3C`.
- Minimal drivers included (GPIO + I2C). RX intentionally omitted from `main.c`.

## Wiring
- PB6 ↔ SCL, PB9 ↔ SDA, GND common.
- Pull-ups: 4.7 kΩ to 3.3 V on SCL & SDA.

## Build
```bash
cd I2C_Button_TxOnly
make clean && make
# flash:
# st-flash write build/I2C_Button_TxOnly.bin 0x8000000
```

Next step

Create a second example (e.g., I2C_Button_TxRx) that adds I2C_MasterReceiveData() after TX for echo tests.

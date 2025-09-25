# 01 – Connection Test (VS10xx SPI Bring-up)

This stage verifies **SPI + UART communication** with the VS1003/VS1053 audio codec by reading back its SCI registers after hardware reset.

## 🔌 Wiring (CubeMX pinout)

| VS10xx Pin | STM32F407 Pin | Notes                    |
|------------|---------------|--------------------------|
| **XCS**    | PA4           | SCI chip select (idle H) |
| **XDCS**   | PE3           | SDI chip select (unused) |
| **RST**    | PE2           | Reset (active LOW)       |
| **DREQ**   | PB0           | Data request (input)     |
| **SPI1**   | PA5/PA6/PA7   | SCK / MISO / MOSI, Mode 0|
| **UART2**  | PA2 (TX)      | 115200 baud for logs     |

## ▶️ Results

During bring-up, the SCI registers were read successfully:

![VS1003 SCI register read](../../docs/screenshots/vs1003_sci_read.png)


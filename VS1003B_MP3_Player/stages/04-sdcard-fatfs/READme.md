# Stage 04 — microSD (SPI2) + FatFS

Mount and read files from a microSD card using **FatFS** over **SPI2** on STM32F407.  
This stage builds on _03-sdcard-connection-test_ (CMD0/ACMD41 passed) and adds the file system layer.

---

## 🎯 Goals
- Keep **VS1003B** on **SPI1** (unchanged).
- Use **SPI2** for the microSD card.
- Enable **FatFS (User-defined / SPI)** and verify:
  - `f_mount()` OK
  - `f_opendir("/")` + `f_readdir()` list entries
  - Optional: open/read a small file


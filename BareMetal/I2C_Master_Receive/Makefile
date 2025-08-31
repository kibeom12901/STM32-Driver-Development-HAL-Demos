# -------- Toolchain --------
CC       := arm-none-eabi-gcc
OBJCOPY  := arm-none-eabi-objcopy

# -------- Project --------
TARGET   := I2C_Master_Receive
BUILD    := build

# -------- MCU / Flags (adjust to your board) --------
MCU      := -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=softfp
DEFS     := -DSTM32F407xx -DI2C_PCLK1_HZ=16000000U
CSTD     := -std=gnu11
OPT      := -O0 -g

# If you use semihosted printf, uncomment these:
# SPECS    := -specs=rdimon.specs
# LIBS     := -lrdimon

# ---- Include paths ----
INCLUDES := -I./Drivers/inc

# ---- Sources ----
SRCS := \
  main.c \
  ./Drivers/src/stm32f407xx_gpio_driver.c \
  ./Drivers/src/stm32f407xx_i2c_driver.c

# ---- Linker script (point this to your .ld) ----
LDSCRIPT := ./STM32F407VGTx_FLASH.ld

# ---- CFLAGS / LDFLAGS ----
CFLAGS  := $(MCU) $(CSTD) $(OPT) $(DEFS) $(INCLUDES) -Wall -ffreestanding -ffunction-sections -fdata-sections
LDFLAGS := $(MCU) -T$(LDSCRIPT) -Wl,--gc-sections -Wl,-Map=$(BUILD)/$(TARGET).map $(SPECS) $(LIBS)

# ---- Build rules ----
OBJ := $(SRCS:%=$(BUILD)/%.o)

all: $(BUILD)/$(TARGET).elf $(BUILD)/$(TARGET).bin

$(BUILD):
	@mkdir -p $(BUILD)
	@mkdir -p $(BUILD)/Drivers/src

$(BUILD)/%.c.o: %.c | $(BUILD)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/$(TARGET).elf: $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $@

$(BUILD)/$(TARGET).bin: $(BUILD)/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@

clean:
	rm -rf $(BUILD)

.PHONY: all clean

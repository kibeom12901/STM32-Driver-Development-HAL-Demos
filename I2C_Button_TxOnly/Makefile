CC:=arm-none-eabi-gcc
OBJCOPY:=arm-none-eabi-objcopy

TARGET:=I2C_Button_TxOnly
BUILD:=build
MCU:=-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=softfp
CSTD:=-std=gnu11
OPT:=-O0
DEFS:=-DSTM32F407xx
INCLUDES:=-I../Drivers/inc

SRCS:= \
  main.c \
  ../Drivers/src/stm32f407xx_gpio_driver.c \
  ../Drivers/src/stm32f407xx_i2c_driver.c

LDSCRIPT:=../STM32F407VGTx_FLASH.ld
CFLAGS:=$(MCU) $(CSTD) $(OPT) $(DEFS) $(INCLUDES) -Wall -ffreestanding -ffunction-sections -fdata-sections
LDFLAGS:=$(MCU) -T$(LDSCRIPT) -Wl,--gc-sections -Wl,-Map=$(BUILD)/$(TARGET).map

OBJ:=$(SRCS:%=$(BUILD)/%.o)

all: $(BUILD)/$(TARGET).elf $(BUILD)/$(TARGET).bin

$(BUILD):
	@mkdir -p $(BUILD)
	@mkdir -p $(BUILD)/../Drivers/src

$(BUILD)/%.c.o: %.c | $(BUILD)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/$(TARGET).elf: $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $@

$(BUILD)/$(TARGET).bin: $(BUILD)/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@

clean:
	rm -rf $(BUILD)

flash: all
	# st-flash write $(BUILD)/$(TARGET).bin 0x8000000

.PHONY: all clean flash

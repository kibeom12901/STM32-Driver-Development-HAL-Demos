CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy

TARGET=I2C_Master_Send
BUILD=build

MCU=-mcpu=cortex-m4 -mthumb
CFLAGS=$(MCU) -O0 -g -Wall -ffunction-sections -fdata-sections -I./Drivers/inc -DSTM32F407xx

SRCS=main.c      Drivers/src/stm32f407xx_gpio_driver.c      Drivers/src/stm32f407xx_i2c_driver.c

LDSCRIPT=STM32F407VGTx_FLASH.ld

OBJ=$(SRCS:%.c=$(BUILD)/%.o)

all: $(BUILD)/$(TARGET).elf

$(BUILD)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/$(TARGET).elf: $(OBJ)
	$(CC) $(CFLAGS) -T$(LDSCRIPT) $(OBJ) -o $@
	$(OBJCOPY) -O binary $@ $(BUILD)/$(TARGET).bin

clean:
	rm -rf $(BUILD)

.PHONY: all clean

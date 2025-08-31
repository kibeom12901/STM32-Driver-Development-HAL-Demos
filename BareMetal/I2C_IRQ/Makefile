.RECIPEPREFIX := >
CC        = arm-none-eabi-gcc
OBJCOPY   = arm-none-eabi-objcopy
SIZE      = arm-none-eabi-size

TARGET    = I2C_IRQ
BUILD     = build

MCU       = -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=softfp
CDEFS     = -DSTM32F407xx
CINCS     = -IDrivers/inc
CFLAGS    = $(MCU) -O0 -g3 -Wall -ffunction-sections -fdata-sections $(CDEFS) $(CINCS)
LDFLAGS   = $(MCU) -Wl,--gc-sections -TSTM32F407VGTx_FLASH.ld

SRCS = main.c stm32f407xx_gpio_driver.c stm32f407xx_i2c_driver.c
VPATH = . Drivers/src

OBJS = $(patsubst %.c,$(BUILD)/%.o,$(SRCS))

all: $(BUILD)/$(TARGET).elf $(BUILD)/$(TARGET).hex

$(BUILD)/%.o: %.c
> @mkdir -p $(dir $@)
> $(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/$(TARGET).elf: $(OBJS)
> $(CC) $(OBJS) $(LDFLAGS) -o $@
> $(SIZE) $@

$(BUILD)/$(TARGET).hex: $(BUILD)/$(TARGET).elf
> $(OBJCOPY) -O ihex $< $@

clean:
> rm -rf $(BUILD)

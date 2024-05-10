TARGET=slcam

ifndef BUILD_DIR
	BUILD_DIR=$(CURDIR)
endif

PARTNUMBER=stm32f103c8t6
OPENCM3_FAMILY=STM32F1

CC=arm-none-eabi-gcc
CPP=arm-none-eabi-g++
AS=arm-none-eabi-as
LD=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
OBJDUMP=arm-none-eabi-objdump
READELF=arm-none-eabi-readelf
NM=arm-none-eabi-nm
SIZE=arm-none-eabi-size
GDB=arm-none-eabi-gdb

FLASH=st-flash

INC=.
CFLAGS=--static -nostartfiles -g3 -Os -fno-common -ffunction-sections -fdata-sections -I. -mcpu=cortex-m3 -mthumb -msoft-float -Iconfig/ -Ios/include/ -Ios/portable/GCC/ARM_CM3/ -D$(OPENCM3_FAMILY)
LDFLAGS=--specs=nano.specs -Wl,--gc-sections,-Map,$(TARGET).map,-T$(PARTNUMBER).ld

.PHONY: all
all: libopencm3_stm32f1.a libs_rule drivers_rule os_rule system_rule tasks_rule crt.o main.o
	$(LD) $(LDFLAGS) $(BUILD_DIR)/*.o -o $(BUILD_DIR)/$(TARGET).elf -L$(BUILD_DIR)/ -lopencm3_stm32f1 -lm
	$(OBJCOPY) -O binary $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).bin
	$(SIZE) $(BUILD_DIR)/$(TARGET).elf

.PHONY: flash
flash:
	$(FLASH) --reset write $(BUILD_DIR)/$(TARGET).bin 0x08000000

.PHONY: clean
clean:
	$(MAKE) clean -C hal
	rm $(BUILD_DIR)/*.o $(BUILD_DIR)/*.a $(BUILD_DIR)/*.elf $(BUILD_DIR)/*.bin $(BUILD_DIR)/*.map

main.o: main.c
	$(CC) -c $(CFLAGS) $< -o $(BUILD_DIR)/$@

crt.o: crt.s
	$(AS) -o $@ $<

libopencm3_stm32f1.a: hal/Makefile
	$(MAKE) BUILD_DIR=$(BUILD_DIR) TARGETS='stm32/f1' -C hal
	mv hal/lib/libopencm3_stm32f1.a $(BUILD_DIR)/

devices: devices/Makefile
	$(MAKE) BUILD_DIR=$(BUILD_DIR) -C devices

drivers_rule: drivers/Makefile
	$(MAKE) BUILD_DIR=$(BUILD_DIR) OPENCM3_FAMILY=$(OPENCM3_FAMILY) -C drivers

libs_rule: libs/Makefile
	$(MAKE) BUILD_DIR=$(BUILD_DIR) -C libs

os_rule: os/Makefile
	$(MAKE) BUILD_DIR=$(BUILD_DIR) -C os

system_rule: system/Makefile
	$(MAKE) BUILD_DIR=$(BUILD_DIR) OPENCM3_FAMILY=$(OPENCM3_FAMILY) -C system

tasks_rule: tasks/Makefile
	$(MAKE) BUILD_DIR=$(BUILD_DIR) OPENCM3_FAMILY=$(OPENCM3_FAMILY) -C tasks

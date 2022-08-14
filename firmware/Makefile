TARGET=slcam

ifndef BUILD_DIR
	BUILD_DIR=$(CURDIR)
endif

PARTNUMBER=stm32f103c8t6

CC=arm-none-eabi-gcc
AS=arm-none-eabi-as
LD=arm-none-eabi-ld
OBJCOPY=arm-none-eabi-objcopy
OBJDUMP=arm-none-eabi-objdump
READELF=arm-none-eabi-readelf
NM=arm-none-eabi-nm
SIZE=arm-none-eabi-size
GDB=arm-none-eabi-gdb

FLASH=st-flash

INC=.
CFLAGS=-mcpu=cortex-m3 -mthumb -O0

.PHONY: all
all: main.o devices drivers hal libs os system tasks
	$(LD) -T$(PARTNUMBER).ld $(BUILD_DIR)/*.o -o $(BUILD_DIR)/$(TARGET).elf
	$(OBJCOPY) -O binary $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).bin

.PHONY: flash
flash:
	$(FLASH)

.PHONY: clean
clean:
	rm $(BUILD_DIR)/*.o $(BUILD_DIR)/*.elf $(BUILD_DIR)/*.bin

main.o: main.c
	$(CC) -c $(CFLAGS) $< -o $(BUILD_DIR)/$@

devices:
	$(MAKE) BUILD_DIR=$(BUILD_DIR) -C devices

drivers:
	$(MAKE) BUILD_DIR=$(BUILD_DIR) -C drivers

hal:
	$(MAKE) BUILD_DIR=$(BUILD_DIR) -C hal

libs:
	$(MAKE) BUILD_DIR=$(BUILD_DIR) -C libs

os:
	$(MAKE) BUILD_DIR=$(BUILD_DIR) -C os

system:
	$(MAKE) BUILD_DIR=$(BUILD_DIR) -C system

tasks:
	$(MAKE) BUILD_DIR=$(BUILD_DIR) -C tasks

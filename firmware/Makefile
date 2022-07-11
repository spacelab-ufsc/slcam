TARGET=slcam

ifndef BUILD_DIR
	BUILD_DIR=$(CURDIR)
endif

CC=arm-nome-eabi-gcc
AS=arm-nome-eabi-as
LD=arm-nome-eabi-ld
OBJCOPY=arm-nome-eabi-objcopy
OBJDUMP=arm-nome-eabi-objdump
READELF=arm-nome-eabi-readelf
NM=arm-nome-eabi-nm
GDB=arm-nome-eabi-gdb

FLASH=st-flash

INC=.
CFLAGS=

.PHONY: all
all: main.o devices drivers hal libs os system tasks
	$(CC) $(CFLAGS) $(BUILD_DIR)/*.o -o $(BUILD_DIR)/$(TARGET)

.PHONY: flash
flash:
	$(FLASH)

.PHONY: clean
clean:
	rm $(BUILD_DIR)/*.o $(BUILD_DIR)/$(TARGET)

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

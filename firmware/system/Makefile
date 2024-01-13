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

CFLAGS=-c -mcpu=cortex-m3 -mthumb -O0 -I../config/ -I../os/include/ -I../os/portable/GCC/ARM_CM3/

.PHONY: all
all: hooks_sys.o

hooks_sys.o: hooks.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

.PHONY: clean
clean:
	rm $(BUILD_DIR)/*.o

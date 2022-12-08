TARGET=hal
STM32F1=1

ifndef BUILD_DIR
	BUILD_DIR=$(CURDIR)
endif

PARTNUMBER=stm32f103c8t6

CC=arm-none-eabi-gcc
CPP=arm-none-eabi-g++
AS=arm-none-eabi-as
LD=arm-none-eabi-ld
OBJCOPY=arm-none-eabi-objcopy
OBJDUMP=arm-none-eabi-objdump
READELF=arm-none-eabi-readelf
NM=arm-none-eabi-nm
SIZE=arm-none-eabi-size
GDB=arm-none-eabi-gdb

INCDIRS=-I.

CFLAGS=-mcpu=cortex-m3 -mthumb -O0

.PHONY: all
all: $(TARGET)
		
$(TARGET):
		$(CC) $(CFLAGS) -Icm3/ -Istm32/ -Istm32/common/ -Istm32/f1/ stm32/*.c stm32/f1/*.c stm32/common/*.c -o $(BUILD_DIR)/$(TARGET).o


.PHONY: clean
clean:
		rm $(BUILD_DIR)/*.o

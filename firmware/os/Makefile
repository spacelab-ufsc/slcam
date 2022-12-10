ifndef BUILD_DIR
	BUILD_DIR=$(CURDIR)
endif

CC=arm-none-eabi-gcc
AS=arm-none-eabi-as
LD=arm-none-eabi-ld
OBJCOPY=arm-none-eabi-objcopy
OBJDUMP=arm-none-eabi-objdump
READELF=arm-none-eabi-readelf
NM=arm-none-eabi-nm
SIZE=arm-none-eabi-size
GDB=arm-none-eabi-gdb

CFLAGS=--static -nostartfiles -g3 -Os -fno-common -ffunction-sections -fdata-sections -I../config/ -Iinclude/ -Iportable/GCC/ARM_CM3/ -mcpu=cortex-m3 -mthumb -msoft-float -c

.PHONY: all
all: croutine.o event_groups.o list.o queue.o stream_buffer.o tasks.o timers.o heap_1.o

.PHONY: clean
clean:
	rm $(BUILD_DIR)/*.o

croutine.o: croutine.c
	$(CC) $(CFLAGS) $< -o $@

event_groups.o: event_groups.c
	$(CC) $(CFLAGS) $< -o $@

list.o: list.c
	$(CC) $(CFLAGS) $< -o $@

queue.o: queue.c
	$(CC) $(CFLAGS) $< -o $@

stream_buffer.o: stream_buffer.c
	$(CC) $(CFLAGS) $< -o $@

tasks.o: tasks.c
	$(CC) $(CFLAGS) $< -o $@

timers.o: timers.c
	$(CC) $(CFLAGS) $< -o $@

heap_1.o: portable/MemMang/heap_1.c
	$(CC) $(CFLAGS) $< -o $@

port.o: portable/GCC/ARM_CM3/port.c
	$(CC) $(CFLAGS) $< -o $@

TARGET_I2C	=	i2c_unit_test
TARGET_SPI	=	spi_unit_test


ifndef BUILD_DIR
	BUILD_DIR=$(CURDIR)
endif

CC=gcc
INC=../../
CFLAGS = -fpic -std=c99 -Wall -pedantic \
         -Wshadow \
         -Wpointer-arith \
         -Wcast-qual \
         -Wstrict-prototypes \
         -Wmissing-prototypes \
         -DSTM32F1 \
         -I$(INC) \
         -I../../tests/freertos_sim \
         -I../../hal/include

I2C_LDFLAGS = \
    -Wl,--wrap=rcc_periph_clock_enable \
    -Wl,--wrap=i2c_peripheral_disable \
    -Wl,--wrap=gpio_set_mode \
    -Wl,--wrap=i2c_set_clock_frequency \
    -Wl,--wrap=i2c_set_speed \
    -Wl,--wrap=i2c_set_own_7bit_slave_address \
    -Wl,--wrap=i2c_enable_ack \
    -Wl,--wrap=i2c_peripheral_enable \
	-Wl,--wrap=i2c_transfer7

<<<<<<< Updated upstream
# SPI_LDFLAGS = \
    -Wl,--wrap=WRAP \
=======
FIRMWARE_DIR    := ../../
SYSTEM_DIR			:= ../../system
CONFIG_DIR      := ../../config
FREERTOS_DIR    := ../../os/include/
SPI_DRIVER_DIR 	:= ../../drivers/spi
GPIO_DRIVER_DIR 	:= ../../drivers/gpio
OPENCM3_DIR     := ../../hal
>>>>>>> Stashed changes

.PHONY: all
.PHONY: all
all: i2c_test spi_test

.PHONY: i2c_test
i2c_test: $(BUILD_DIR)/i2c.o \
          $(BUILD_DIR)/i2c_test.o \
          $(BUILD_DIR)/hal_gpio_wrap.o \
          $(BUILD_DIR)/hal_i2c_wrap.o \
          $(BUILD_DIR)/hal_rcc_wrap.o
	$(CC) $(I2C_LDFLAGS) $^ -lcmocka -o $(BUILD_DIR)/$(TARGET_I2C)

spi_test: $(BUILD_DIR)/spi_test.o 
	$(CC) $^ -lcmocka -o $(BUILD_DIR)/$(TARGET_SPI)

#Drivers
$(BUILD_DIR)/i2c.o: ../../drivers/i2c/i2c.c
	$(CC) $(CFLAGS) -c $< -o $@

#Tests
$(BUILD_DIR)/i2c_test.o: i2c_test.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/spi_test.o: spi_test.c
	$(CC) $(CFLAGS) -c $< -o $@


# Mockups
$(BUILD_DIR)/hal_gpio_wrap.o: ../mockups/hal/hal_gpio_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/hal_i2c_wrap.o: ../mockups/hal/hal_i2c_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

<<<<<<< Updated upstream
$(BUILD_DIR)/hal_rcc_wrap.o: ../mockups/hal/hal_rcc_wrap.c
	$(CC) $(FLAGS) -c $< -o $@
=======
#------------------------------------------------------------------------------
# Compiler flags
#------------------------------------------------------------------------------

OPT   := -Os
DEBUG := -ggdb3
CSTD  := -std=c99

TGT_CFLAGS += $(OPT)
TGT_CFLAGS += $(CSTD)
TGT_CFLAGS += $(DEBUG)
TGT_CFLAGS += $(ARCH_FLAGS)

TGT_CFLAGS += -Wall
TGT_CFLAGS += -Wextra
TGT_CFLAGS += -Wshadow
TGT_CFLAGS += -Wimplicit-function-declaration
TGT_CFLAGS += -Wredundant-decls
TGT_CFLAGS += -Wmissing-prototypes
TGT_CFLAGS += -Wstrict-prototypes

TGT_CFLAGS += -fno-common
TGT_CFLAGS += -ffunction-sections
TGT_CFLAGS += -fdata-sections

#------------------------------------------------------------------------------
# Preprocessor
#------------------------------------------------------------------------------

TGT_CPPFLAGS += -MD
TGT_CPPFLAGS += $(DEFS)

TGT_CPPFLAGS += -I$(OPENCM3_DIR)/include
TGT_CPPFLAGS += -I$(SPI_DRIVER_DIR)
TGT_CPPFLAGS += -I$(GPIO_DRIVER_DIR)
TGT_CPPFLAGS += -I$(FIRMWARE_DIR)
TGT_CPPFLAGS += -I$(FREERTOS_DIR)
TGT_CPPFLAGS += -I$(FREERTOS_DIR)/../portable/GCC/ARM_CM3
TGT_CPPFLAGS += -I$(CONFIG_DIR)
TGT_CPPFLAGS += -I$(SYSTEM_DIR)

#------------------------------------------------------------------------------
# Linker flags
#------------------------------------------------------------------------------

TGT_LDFLAGS += --static
TGT_LDFLAGS += -nostartfiles
TGT_LDFLAGS += -T$(LDSCRIPT)
TGT_LDFLAGS += $(ARCH_FLAGS)
TGT_LDFLAGS += $(DEBUG)

TGT_LDFLAGS += -Wl,-Map=$(BINARY).map
TGT_LDFLAGS += -Wl,--cref
TGT_LDFLAGS += -Wl,--gc-sections

#------------------------------------------------------------------------------
# Libraries
#------------------------------------------------------------------------------

LDFLAGS += -L$(OPENCM3_DIR)/lib

LDLIBS += -l$(LIBNAME)

LDLIBS += -Wl,--start-group
LDLIBS += -lc
LDLIBS += -lgcc
LDLIBS += -lnosys
LDLIBS += -Wl,--end-group

#------------------------------------------------------------------------------
# Source files
#------------------------------------------------------------------------------

OBJS += $(BINARY).o
OBJS += $(SPI_DRIVER_DIR)/spi.o
OBJS += $(SPI_DRIVER_DIR)/spi_stm32f103.o

OBJS += $(GPIO_DRIVER_DIR)/gpio.o
OBJS += $(GPIO_DRIVER_DIR)/gpio_stm32f103.o

#------------------------------------------------------------------------------
# Targets
#------------------------------------------------------------------------------

all: elf bin

elf: $(BINARY).elf

bin: $(BINARY).bin

hex: $(BINARY).hex

list: $(BINARY).list

#------------------------------------------------------------------------------
# libopencm3
#------------------------------------------------------------------------------

$(OPENCM3_DIR)/lib/lib$(LIBNAME).a:
	$(Q)$(MAKE) -C $(OPENCM3_DIR)

#------------------------------------------------------------------------------
# Linking
#------------------------------------------------------------------------------

$(BINARY).elf: $(OBJS) $(LDSCRIPT) $(OPENCM3_DIR)/lib/lib$(LIBNAME).a Makefile
	$(Q)$(LD) $(TGT_LDFLAGS) $(LDFLAGS) \
		$(OBJS) \
		$(LDLIBS) \
		-o $@

#------------------------------------------------------------------------------
# Compilation
#------------------------------------------------------------------------------

%.o: %.c
	$(Q)$(CC) \
		$(TGT_CFLAGS) \
		$(TGT_CPPFLAGS) \
		$(CFLAGS) \
		$(CPPFLAGS) \
		-c $< \
		-o $@

#------------------------------------------------------------------------------
# Binary
#------------------------------------------------------------------------------

%.bin: %.elf
	$(Q)$(OBJCOPY) -Obinary $< $@

#------------------------------------------------------------------------------
# Intel HEX
#------------------------------------------------------------------------------

%.hex: %.elf
	$(Q)$(OBJCOPY) -Oihex $< $@

#------------------------------------------------------------------------------
# Disassembly
#------------------------------------------------------------------------------

%.list: %.elf
	$(Q)$(OBJDUMP) -S $< > $@

#------------------------------------------------------------------------------
# Flash
#------------------------------------------------------------------------------

flash: $(BINARY).bin
	st-flash write $(BINARY).bin 0x08000000

#------------------------------------------------------------------------------
# Clean
#------------------------------------------------------------------------------
>>>>>>> Stashed changes

.PHONY: clean
clean:
	rm -f $(BUILD_DIR)/$(TARGET_I2C) \
	      $(BUILD_DIR)/$(TARGET_SPI) \
	      $(BUILD_DIR)/*.o
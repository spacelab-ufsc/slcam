TARGET_I2C	=	i2c_unit_test


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

LDFLAGS = \
    -Wl,--wrap=rcc_periph_clock_enable \
    -Wl,--wrap=i2c_peripheral_disable \
    -Wl,--wrap=gpio_set_mode \
    -Wl,--wrap=i2c_set_clock_frequency \
    -Wl,--wrap=i2c_set_speed \
    -Wl,--wrap=i2c_set_own_7bit_slave_address \
    -Wl,--wrap=i2c_enable_ack \
    -Wl,--wrap=i2c_peripheral_enable \
	-Wl,--wrap=i2c_transfer7

.PHONY: all
.PHONY: all
all: i2c_test

.PHONY: i2c_test
i2c_test: $(BUILD_DIR)/i2c.o \
          $(BUILD_DIR)/i2c_test.o \
          $(BUILD_DIR)/hal_gpio_wrap.o \
          $(BUILD_DIR)/hal_i2c_wrap.o \
          $(BUILD_DIR)/hal_rcc_wrap.o
	$(CC) $(LDFLAGS) $^ -lcmocka -o $(BUILD_DIR)/$(TARGET_I2C)

#Drivers
$(BUILD_DIR)/i2c.o: ../../drivers/i2c/i2c.c
	$(CC) $(CFLAGS) -c $< -o $@

#Tests
$(BUILD_DIR)/i2c_test.o: i2c_test.c
	$(CC) $(CFLAGS) -c $< -o $@


# Mockups
$(BUILD_DIR)/hal_gpio_wrap.o: ../mockups/hal/hal_gpio_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/hal_i2c_wrap.o: ../mockups/hal/hal_i2c_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/hal_rcc_wrap.o: ../mockups/hal/hal_rcc_wrap.c
	$(CC) $(FLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm $(BUILD_DIR)/$(TARGET_I2C) $(BUILD_DIR)/*.o
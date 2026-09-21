/*
 * i2c_test.c
 * 
 * Copyright The OBDH 2.0 Contributors.
 * 
 * This file is part of OBDH 2.0.
 * 
 * OBDH 2.0 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * OBDH 2.0 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with OBDH 2.0. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Unit test of the I2C protocol.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Pedro Ferrari Barbosa <pedro.ferraribarbosa2007@gmail.com> 
 *
 * \version 0.9.13
 * 
 * \date 2021/09/01
 * 
 * \defgroup i2c_unit_test I2C
 * \ingroup tests
 * \{
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <float.h>
#include <cmocka.h>

#include <stdlib.h>
#include <math.h>

#include <drivers/i2c/i2c.h>

#define SLAVE_ADDR (0x50)

i2c_config_t config = {0};

static void i2c_init_test(void** state){
    (void)state;

    assert_return_code(i2c_init(I2C_PORT_0, config), -1);
}

static void i2c_write_test(void** state){
    (void)state;

    uint8_t data[7] = {13, 21, 44, 202, 55, 224, 3};
    uint16_t len = 7;

    assert_return_code(i2c_write(I2C_PORT_0, SLAVE_ADDR, data, len), -1);
}

static void i2c_read_test(void** state){
    (void)state;

    uint8_t data[7] = {0};
    uint8_t not_expected[7] = {0};

    uint16_t len = 7;

    assert_return_code(i2c_read(I2C_PORT_0, SLAVE_ADDR, data, len), -1);
    assert_memory_not_equal(data, not_expected, len);
}

int main(void)
{
    config.clock_freq_mhz = 42;
    config.speed_hz = i2c_speed_fm_400k;

    const struct CMUnitTest i2c_tests[] = {
        cmocka_unit_test(i2c_init_test),
        cmocka_unit_test(i2c_write_test),
        cmocka_unit_test(i2c_read_test),
    };

    return cmocka_run_group_tests(i2c_tests, NULL, NULL);
}
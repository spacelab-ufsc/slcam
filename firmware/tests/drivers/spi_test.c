/*
 * i2c_test.c
 * 
 * Copyright The SLCam Contributors.
 * 
 * This file is part of SLCam.
 * 
 * SLCam is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * SLCam is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with SLCam. If not, see <http://www.gnu.org/licenses/>.
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

static void spi_init_test(void** state){
    return -1;
}

static void spi_select_slave_test(void** state){
    return -1;
}

static void spi_configure_test(void** state){
    return -1;
}

static void spi_write_test(void** state){
    return -1;
}

static void spi_write_only_test(void** state){
    return -1;
}

static void spi_read_test(void** state){
    return -1;
}

static void spi_read_only_test(void** state){
    return -1;
}

static void spi_transfer_test(void** state){
    return -1;
}

int main(void)
{
    const struct CMUnitTest spi_tests[] = {
        cmocka_unit_test(spi_init_test),
        cmocka_unit_test(spi_select_slave_test),
        cmocka_unit_test(spi_configure_test),
        cmocka_unit_test(spi_write_test),
        cmocka_unit_test(spi_write_only_test),
        cmocka_unit_test(spi_read_test),
        cmocka_unit_test(spi_read_only_test),
        cmocka_unit_test(spi_transfer_test)     
    };

    return cmocka_run_group_tests(spi_tests, NULL, NULL);
}
/** \} End of i2c_unit_test group */


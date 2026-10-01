/*
 * hal_i2c_wrap.c
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
 * \brief HAL I2C wrap implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.2.2
 * 
 * \date 2023/07/28
 * 
 * \addtogroup can_wrap
 * \{
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <setjmp.h>
#include <float.h>
#include <cmocka.h>

#include "hal_i2c_wrap.h"

unsigned int generate_random(unsigned int l, unsigned int r);

void __wrap_i2c_peripheral_disable(uint32_t i2c){
    return;
}   

void __wrap_i2c_set_clock_frequency(uint32_t i2c, uint8_t freq){
    return;
}

void __wrap_i2c_set_speed(uint32_t i2c, enum i2c_speeds speed, uint32_t clock_megahz){
    return;
}

void __wrap_i2c_set_own_7bit_slave_address(uint32_t i2c, uint8_t slave){
    return;
}

void __wrap_i2c_enable_ack(uint32_t i2c){
    return;
}

void __wrap_i2c_peripheral_enable(uint32_t i2c){
    return;
}

void __wrap_i2c_transfer7(uint32_t i2c, uint8_t addr, const uint8_t *w, size_t wn, uint8_t *r, size_t rn){
    if(wn > 0){
        assert_non_null(w);
    }

    if(rn > 0){
        assert_non_null(r);

        // generate fake data
        for(int i = 0; i < rn; i++){
            r[i] = generate_random(0, 250);
        }
    }
    
}

unsigned int generate_random(unsigned int l, unsigned int r)
{
    return (rand() % (r - l + 1)) + l;
}


/** \} End of hal_i2c_wrap group */
/*
 * hal_i2c_wrap.h
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
 * \author Pedro Ferrari Barbosa <pedro.ferraribarbosa2007@gmail.com>
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.2.2
 * 
 * \date 2026/08/01
 * 
 * \defgroup can_wrap CAN Wrap
 * \ingroup tests
 * \{
 */

#ifndef HAL_I2C_WRAP_H_
#define HAL_I2C_WRAP_H_

#include <stdint.h>

enum i2c_speeds {
    i2c_speed_sm_100k = 0,
    i2c_speed_fm_400k,
    i2c_speed_fmp_1m,
    i2c_speed_unknown
};

void __wrap_i2c_peripheral_disable(uint32_t i2c);
void __wrap_i2c_set_clock_frequency(uint32_t i2c, uint8_t freq);
void __wrap_i2c_set_speed(uint32_t i2c, enum i2c_speeds speed, uint32_t clock_megahz);
void __wrap_i2c_set_own_7bit_slave_address(uint32_t i2c, uint8_t slave);
void __wrap__i2c_enable_ack(uint32_t i2c);
void __wrap_i2c_peripheral_enable(uint32_t i2c);

void i2c_transfer7(uint32_t i2c, uint8_t addr, const uint8_t *w, size_t wn, uint8_t *r, size_t rn);

#endif /* HAL_I2C_WRAP_H_ */

/** \} End of hal_i2c_wrap group */
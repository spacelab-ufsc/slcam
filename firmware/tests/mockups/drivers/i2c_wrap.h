/*
 * i2c_wrap.h
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
 * \brief CAN driver wrap definition.
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

#ifndef I2C_WRAP_H_
#define I2C_WRAP_H_

#include <stdint.h>

#include <drivers/i2c/i2c.h>

int __wrap_i2c_init(can_config_t config);

int __wrap_i2c_write(i2c_port_t port, i2c_slave_adr_t adr, uint8_t *data, uint16_t len);

int __wrap_i2c_read(i2c_port_t port, i2c_slave_adr_t adr, uint8_t *data, uint16_t len);


#endif /* I2C_WRAP_H_ */

/** \} End of can_wrap group */
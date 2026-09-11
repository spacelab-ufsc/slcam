/*
 * hal_gpio_wrap.h
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
 * \brief GPIO wrap implementation.
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

#ifndef GPIO_WRAP_H_
#define GPIO_WRAP_H_

#include <stdint.h>

void __wrap_gpio_set_mode(uint32_t gpioport, uint8_t mode, uint8_t cnf, uint16_t gpios);

#endif /* GPIO_WRAP_H_ */

/** \} End of can_wrap group */
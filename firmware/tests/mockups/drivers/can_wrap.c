/*
 * can_wrap.c
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
 * \brief CAN driver wrap implementation.
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
#include <stdbool.h>
#include <setjmp.h>
#include <float.h>
#include <cmocka.h>

#include "can_wrap.h"

int __wrap_can_init(can_config_t config)
{
    return -1;
}

int __wrap_can_write(can_config_t config, uint8_t *data, uint16_t len)
{
    return -1;
}

int __wrap_can_read(can_config_t config, uint8_t *data, uint16_t len)
{
    return -1;
}

/** \} End of can_wrap group */
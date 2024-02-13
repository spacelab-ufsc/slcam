/*
 * spi.c
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
 * along with SLCam. If not, see <http:/\/www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief SPI driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.2.10
 * 
 * \date 2024/02/13
 * 
 * \addtogroup spi
 * \{
 */


#include "spi.h"

int spi_select_slave(spi_port_t port, spi_cs_t cs, bool active)
{
    return -1;
}

int spi_init(spi_port_t port, spi_config_t config)
{
    return -1;
}

static void spi_write_byte(uint16_t base_address, uint8_t byte)
{
}

static uint8_t spi_read_byte(uint16_t base_address)
{
    return UINT8_MAX;
}

static uint8_t spi_transfer_byte(uint16_t base_address, uint8_t wb)
{
    spi_write_byte(base_address, wb);

    return spi_read_byte(base_address);
}

int spi_write(spi_port_t port, spi_cs_t cs, uint8_t *data, uint16_t len)
{
    return -1;
}

int spi_read(spi_port_t port, spi_cs_t cs, uint8_t *data, uint16_t len)
{
    return -1;
}

int spi_transfer(spi_port_t port, spi_cs_t cs, uint8_t *wd, uint8_t *rd, uint16_t len)
{
    return -1;
}

/** \} End of spi group */

/*
 * can.h
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
 * \brief CAN driver definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.2.2
 * 
 * \date 2023/07/28
 * 
 * \defgroup can CAN
 * \ingroup drivers
 * \{
 */

#ifndef CAN_H_
#define CAN_H_

#include <stdint.h>

#define CAN_MODULE_NAME             "CAN"

/**
 * \brief CAN ports.
 */
typedef enum
{
    CAN_PORT_1=0,           /**< CAN port 1. */
} can_port_t;

/**
 * \brief Configuration structure.
 */
typedef struct
{
    can_port_t port;        /**< Port. */
    can_speet_t speed;      /**< Speed in bits per second. */
    bool loopback;          /**< Loopback mode (TX=On, RX=Off). */
    bool silent;            /**< Silent mode (TX=Off, RX=On). */
} can_config_t;

/**
 * \brief Initializes a given CAN port.
 *
 * \param[in] config are the configuration parameters of the given UART port.
 *
 * \return The status/error code.
 */
int can_init(can_config_t config);

/**
 * \brief Writes data to a given CAN port.
 *
 * \param[in] config are the configuration parameters of the given CAN port.
 *
 * \param[in] data is the array of bytes to be written.
 *
 * \param[in] len is the number of bytes to be written.
 *
 * \return The status/error code.
 */
int can_write(can_config_t config, uint16_t *data, uint16_t len);

/**
 * \brief Reads data from a given CAN port.
 *
 * \param[in] config are the configuration parameters of the given CAN port.
 *
 * \param[in,out] data is a pointer to store the read data.
 *
 * \param[in] len is the number of bytes to read.
 *
 * \return The status/error code.
 */
int can_read(can_config_t config, uint16_t *data, uint16_t len);

#endif /* CAN_H_ */

/** \} End of can group */

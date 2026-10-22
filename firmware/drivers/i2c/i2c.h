/*
 * i2c.h
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
 * \brief I2C driver definition.
 * 
 * \author Pedro Ferrari Barbosa <pedro.ferraribarbosa2007@gmail.com>
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.41
 * 
 * \date 2026/08/27
 * 
 * \defgroup i2c I2C
 * \ingroup drivers
 * \{
 */

#ifndef I2C_H_
#define I2C_H_

#include <errno.h>
#include <stdint.h>
#include <libopencm3/cm3/memorymap.h>
#include <libopencm3/stm32/i2c.h>
#include <config/errno.h>

#include <utils/mutex/mutex.h>

#define I2C_MODULE_NAME "I2C"

#define I2C_SLAVE_OWN_7BIT_ADDR (0x00)

typedef enum i2c_port {
	I2C_PORT_0 = 0, /**< I2C port 0. */
	I2C_PORT_1, /**< I2C port 1. */
} i2c_port;

typedef struct i2c_config {
	uint32_t speed_hz; /**< Transfer rate in bps (choose between 100k, 400k and 1m)*/
	uint32_t clock_freq_mhz; /**< clock frequency in MHz*/
} i2c_config;

/**
 * \brief I2C slave 7-bit address.
 */
typedef uint8_t i2c_slave_adr_t;

/**
 * \brief I2C device struct (forward declared)
 */
struct i2c_device;

/**
 * \brief I2C bus controller struct (forward declared)
 */
struct i2c_controller;

/**
 *\brief i2c driver api
 */
struct i2c_driver_api {
	error_t (*init)(struct i2c_controller *controller, const struct i2c_config *config,
			const enum i2c_port port);
	error_t (*configure)(struct i2c_controller *controller, struct i2c_config *config);
	error_t (*write)(struct i2c_device *dev, i2c_slave_adr_t adr, uint8_t *buf, size_t len);
	error_t (*read)(struct i2c_device *dev, i2c_slave_adr_t adr, uint8_t *buf, size_t len);
};

/**
 * \brief i2c controller struct
 */
struct i2c_controller {
	struct i2c_driver_api api;
	struct i2c_config config;
	struct mutex lock;
	uint8_t port;
	uint8_t initialized;
};

/**
 * \brief i2c device struct;
 */
struct i2c_device {
	struct i2c_controller *controller;
	i2c_slave_adr_t slave_addr;
};

/**
 * \brief Initializes an I2C controller, configures it and populate its struct.
 *
 * \param[out] controller is a pointer to the I2C controller handle inside a 
 * i2c_device. This needs to be a double pointer since, after initialization, 
 * this handle will be pointing to the hardware specific controller instance, 
 * avoiding, therefore, multiple instances of the same physical controller.
 * If the function returned an error this should be in an invalid state. 
 *
 * \param[in] port is the specific hardware port number of the controller.
 *
 * \param[in] config is the configuration to apply to the controller.
 *
 * \return The status/error code.
 */
error_t i2c_init_controller(struct i2c_controller **controller, const enum i2c_port port,
			    const struct i2c_config *config);

/**
 * \brief Configures an I2C controller. It assumes that the controller is 
 * already initialized.
 *
 * \param[out] controller is the I2C controller struct to configured.
 *
 * \param[in] config is the configuration to apply to the controller.
 *
 * \return The status/error code.
 */
error_t i2c_configure_controller(struct i2c_controller *controller, struct i2c_config *config);

/**
 * \brief Initializes a I2C device.
 *
 * \param[out] dev is the I2C device struct to initialized.
 *
 * \param[in] port is the SPI port controller that the device is
 * connected to. It tries to initialize
 *
 * \param[in] config is the configuration to apply to the controller. 
 * If the controller is already initialized the config is not applied, 
 * for that spi_configure_controller() should be used.
 *
 * \param[in] cs_pin is the SPI chip select gpio pin to be initialized.
 *
 * \param[in] cs_active_level is the SPI chip select active level.
 *
 * \param[in] cs_active_level is the SPI chip select active level.
 *
 * \return The status/error code.
 */
error_t i2c_init_device(struct i2c_device *dev, enum i2c_port port,
			const struct i2c_config *config);

/**
 * \brief Write to a I2C device.
 *
 * \param[in] dev is the I2C device struct to write to.
 *
 * \param[in] buf is the data to be written.
 *
 * \param[in] len is the length of the buffer in bytes.
 *
 * \return The status/error code.
 */
error_t i2c_device_write(struct i2c_device *dev, i2c_slave_adr_t addr, uint8_t *buf, size_t len);

/**
 * \brief Read from a I2C device.
 *
 * \param[in] dev is the I2C device struct to read from.
 *
 * \param[out] buf is a buffer where read data is stored.
 *
 * \param[in] len is the number of bytes to be read.
 *
 * \return The number of bytes read, if errors occured the function returns 
 * negative numbers.
 */
error_t i2c_device_read(struct i2c_device *dev, i2c_slave_adr_t addr, uint8_t *buf, size_t len);

/**
 * \brief Get hardware specific i2c driver functions struct. THIS FUNCTION 
 * SHOULD BE IMPLEMENTED FOR EACH CPU, otherwise the driver does not work.
 *
 * \return The struct containing the hardware specific functions. 
 */
extern struct i2c_driver_api *i2c_hw_get_driver(void);

/**
 * \brief Get hardware specific controller instances, this is done to provide 
 * a `singleton` like controllers to avoid concurrency issues. THIS FUNCTION
 * SHOULD BE IMPLEMENTED FOR EACH CPU, otherwise the driver does not work. It 
 * could be implemented simply by: 
 *
 * static struct spi_controller *controller_list[] = {
 *      [I2C_PORT_0] = &spi_controller_0,
 *      [I2C_PORT_1] = &spi_controller_1,
 *      ...
 *      [I2C_PORT_5] = &spi_controller_5,
 * }
 *
 * struct i2c_controller *i2c_hw_get_controller_handle(enum i2c_port port) {
 *      return controller_list[port];
 * }
 *
 * \param[in] port is the hardware port to identify the controller.
 *
 * \return The struct containing the matching controller handle.
 */
extern struct i2c_controller *i2c_hw_get_controller_handle(enum i2c_port port);

#endif // I2C_H_

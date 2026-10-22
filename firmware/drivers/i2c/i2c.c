/*
 * i2c.c
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
 * \brief I2C driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Pedro Ferrari Barbosa <pedro.ferraribarbosa2007@gmail.com>
 * 
 * \version 0.2.10
 * 
 * \date 2024/02/13
 * 
 * \addtogroup i2c
 * \{
 */

#include "errno.h"
#include <errno.h>
#include <stdint.h>
#include <utils/mutex/mutex.h>
#include <drivers/gpio/gpio.h>

#include "i2c.h"

error_t i2c_init_controller(struct i2c_controller **controller, const enum i2c_port port,
			    const struct i2c_config *config)
{
	struct i2c_driver_api *api;
	struct i2c_controller *ctrl;
	int err;

	if (!controller)
		return -ERRNO_MISC_INVALID_ARG;

	ctrl = i2c_hw_get_controller_handle(port);

	if (ctrl == NULL)
		return -ERRNO_DRIVER_NO_PORT;

	*controller = ctrl;

	if (ctrl->initialized == 1)
		return ERRNO_SUCCESS;

	/* This check is in here to enable config to be NULL, if the controller 
     * is already be initialized. */
	if (!config)
		return -ERRNO_MISC_INVALID_ARG;

	api = i2c_hw_get_driver();

	if (!api)
		return -ERRNO_MISC_UNSUPPORTED_OP;

	// err = mutex_init(&ctrl->lock);

	if (err < 0)
		return err;

	err = api->init(ctrl, config, port);

	if (err < 0)
		return err;

	(*controller)->port = port;
	(*controller)->api = *api;
	(*controller)->config = *config;
	(*controller)->initialized = 1U;

	return 0;
}

error_t i2c_configure_controller(struct i2c_controller *controller, struct i2c_config *config)
{
	error_t err = ERRNO_SUCCESS;

	if (!config || !controller)
		return -ERRNO_MISC_INVALID_ARG;

	if (controller->initialized != 1U)
		return -ERRNO_DRIVER_UNINITIALIZED;

	err = controller->api.configure(controller, config);

	if (err < 0)
		return err;

	controller->config = *config;

	return 0;
}

error_t i2c_init_device(struct i2c_device *dev, enum i2c_port port, const struct i2c_config *config)
{
	error_t err = ERRNO_SUCCESS;

	if (!dev)
		return -ERRNO_MISC_INVALID_ARG;

	err = i2c_init_controller(&dev->controller, port, config);

	if (err < 0)
		return err;

	return 0;
}

error_t i2c_device_write(struct i2c_device *dev, i2c_slave_adr_t addr, uint8_t *buf, size_t len)
{
	struct i2c_controller *controller = dev->controller;
	error_t err = ERRNO_SUCCESS;

	if (!dev || !dev->controller)
		return -ERRNO_MISC_INVALID_ARG;

	err = mutex_lock(&controller->lock);

	if (err < 0)
		return err;

	err = controller->api.write(dev, addr, buf, len);

	mutex_unlock(&controller->lock);

	return err;
}

error_t i2c_device_read(struct i2c_device *dev, i2c_slave_adr_t addr, uint8_t *buf, size_t len)
{
	struct i2c_controller *controller = dev->controller;
	error_t err = ERRNO_SUCCESS;

	if (!dev || !dev->controller)
		return -ERRNO_MISC_INVALID_ARG;

	err = mutex_lock(&controller->lock);

	if (err < 0)
		return err;

	err = controller->api.read(dev, addr, buf, len);

	mutex_unlock(&controller->lock);

	return err;
}

/** \} End of spi group */

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

#include <stdint.h>
#include <config/errno.h>
#include <utils/mutex/mutex.h>
#include <drivers/gpio/gpio.h>

#include "spi.h"

int spi_init_controller(struct spi_controller **controller, enum spi_port port,
			const struct spi_config *config)
{
	struct spi_driver_api *api;
	struct spi_controller *ctrl;
	int err = ERRNO_SUCCESS;

	if (!controller)
		return -ERRNO_MISC_INVALID_ARG;

	ctrl = spi_hw_get_controller_handle(port);

	if (ctrl == NULL)
		return -ERRNO_DRIVER_NO_PORT;

	*controller = ctrl;

	if (ctrl->initialized == 1)
		return 0;

	/* This check is in here to enable config to be NULL, if the controller 
     * is already be initialized. */
	if (!config)
		return -ERRNO_MISC_INVALID_ARG;

	api = spi_hw_get_driver();

	if (!api)
		return -ERRNO_MISC_UNSUPPORTED_OP;

	err = mutex_init(&ctrl->lock);

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

int spi_configure_controller(struct spi_controller *controller, struct spi_config *config)
{
	int err = ERRNO_SUCCESS;

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

int spi_init_device(struct spi_device *dev, enum spi_port port, const struct spi_config *config,
		    const uint8_t cs_pin, const uint8_t cs_active_level)
{
	int err = ERRNO_SUCCESS;

	if (!dev)
		return -ERRNO_MISC_INVALID_ARG;

	err = spi_init_controller(&dev->controller, port, config);

	if (err < 0)
		return err;

	err = gpio_init_pin(cs_pin, GPIO_DRV_MODE_OUTPUT_PUSH_PULL);

	if (err < 0)
		return err;

	gpio_set_state(cs_pin, !cs_active_level);

	dev->cs = cs_pin;
	dev->cs_active_level = cs_active_level;

	return 0;
}

int spi_select_slave(struct spi_device *dev, bool state)
{
	struct spi_controller *controller = dev->controller;
	int err = ERRNO_SUCCESS;

	if (!dev || !dev->controller)
		return -ERRNO_MISC_INVALID_ARG;

	err = mutex_lock(&controller->lock);

	if (err < 0)
		return err;

	err = controller->api.select_slave(dev, state);

	mutex_unlock(&controller->lock);

	return err;
}

int spi_device_write(struct spi_device *dev, uint8_t *buf, size_t len)
{
	struct spi_controller *controller = dev->controller;
	int err = ERRNO_SUCCESS;

	if (!dev || !dev->controller)
		return -ERRNO_MISC_INVALID_ARG;

	err = mutex_lock(&controller->lock);

	if (err < 0)
		return err;

	err = controller->api.write(dev, buf, len);

	mutex_unlock(&controller->lock);

	return err;
}

int spi_device_write_only(struct spi_device *dev, uint8_t *buf, size_t len)
{
	struct spi_controller *controller = dev->controller;
	int err = ERRNO_SUCCESS;

	if (!dev || !dev->controller)
		return -ERRNO_MISC_INVALID_ARG;

	err = mutex_lock(&controller->lock);

	if (err < 0)
		return err;

	err = controller->api.write_only(dev, buf, len);

	mutex_unlock(&controller->lock);

	return err;
}

int spi_device_read(struct spi_device *dev, uint8_t *buf, size_t len)
{
	struct spi_controller *controller = dev->controller;
	int err = ERRNO_SUCCESS;

	if (!dev || !dev->controller)
		return -ERRNO_MISC_INVALID_ARG;

	err = mutex_lock(&controller->lock);

	if (err < 0)
		return err;

	err = controller->api.read(dev, buf, len);

	mutex_unlock(&controller->lock);

	return err;
}

int spi_device_read_only(struct spi_device *dev, uint8_t *buf, size_t len)
{
	struct spi_controller *controller = dev->controller;
	int err = ERRNO_SUCCESS;

	if (!dev || !dev->controller)
		return -ERRNO_MISC_INVALID_ARG;

	err = mutex_lock(&controller->lock);

	if (err < 0)
		return err;

	err = controller->api.read_only(dev, buf, len);

	mutex_unlock(&controller->lock);

	return err;
}

int spi_device_transfer(struct spi_device *dev, uint8_t *tx_buf, size_t tx_len, uint8_t *rx_buf,
			size_t rx_len)
{
	struct spi_controller *controller = dev->controller;
	int err = ERRNO_SUCCESS;

	if (!dev || !dev->controller)
		return -ERRNO_MISC_INVALID_ARG;

	err = mutex_lock(&controller->lock);

	if (err < 0)
		return err;

	err = controller->api.transfer(dev, tx_buf, tx_len, rx_buf, rx_len);

	mutex_unlock(&controller->lock);

	return err;
}

/** \} End of spi group */

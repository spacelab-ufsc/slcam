/*
 * spi_stm32f103.c
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
 * \brief STM32F103 SPI driver implementation.
 *
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 *
 * \version 0.2.10
 *
 * \date 2024/01/12
 *
 * \addtogroup spi
 * \{
 */
#include <stdint.h>
#include <config/errno.h>
#include <hal/include/libopencm3/stm32/spi.h>
#include <hal/include/libopencm3/stm32/gpio.h>
#include <hal/include/libopencm3/stm32/rcc.h>

#include "spi.h"
#include "system.h"

static inline uint32_t port_to_base_address(const enum spi_port port)
{
	uint32_t addr = UINT32_MAX;

	switch (port) {
	case SPI_PORT_0:
		addr = SPI1_BASE;
		break;
	case SPI_PORT_1:
		addr = SPI2_BASE;
		break;
	case SPI_PORT_2:
		addr = SPI3_BASE;
		break;
	default:
		break;
	}

	return addr;
}

uint32_t spi_get_prescaler(uint32_t speed_hz)
{
    uint32_t pclk = SYSTEM_CLOCK_FREQ_HZ;

    if (speed_hz >= pclk / 2)
        return SPI_CR1_BAUDRATE_FPCLK_DIV_2;

    if (speed_hz >= pclk / 4)
        return SPI_CR1_BAUDRATE_FPCLK_DIV_4;

    if (speed_hz >= pclk / 8)
        return SPI_CR1_BAUDRATE_FPCLK_DIV_8;

    if (speed_hz >= pclk / 16)
        return SPI_CR1_BAUDRATE_FPCLK_DIV_16;

    if (speed_hz >= pclk / 32)
        return SPI_CR1_BAUDRATE_FPCLK_DIV_32;

    if (speed_hz >= pclk / 64)
        return SPI_CR1_BAUDRATE_FPCLK_DIV_64;

    if (speed_hz >= pclk / 128)
        return SPI_CR1_BAUDRATE_FPCLK_DIV_128;

    return SPI_CR1_BAUDRATE_FPCLK_DIV_256;
}

static int spi_stm32_init(struct spi_controller *controller,
			  const struct spi_config *config, const enum spi_port port)
{
	int err = 0;
	uint32_t port_addr = port_to_base_address(port);

	if (port_addr == UINT32_MAX){
		return -ERRNO_DRIVER_NO_PORT;
	}

	uint32_t c_pol = UINT32_MAX;

	switch(config->mode){
		case SPI_MODE_0: case SPI_MODE_1:		c_pol = SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE; break;
		case SPI_MODE_2: case SPI_MODE_3:		c_pol = SPI_CR1_CPOL_CLK_TO_1_WHEN_IDLE; break;
		default:{
			#if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            	sys_log_print_event_from_module(SYS_LOG_ERROR, SPI_MODULE_NAME, "Invalid SPI mode!");
            	sys_log_new_line();
        	#endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            err = -1;   /* Invalid SPI mode */
            return err;
		}
	}

	uint32_t c_phase = UINT32_MAX;

	switch(config->mode){
		case SPI_MODE_0: case SPI_MODE_2:		c_phase = SPI_CR1_CPHA_CLK_TRANSITION_1; break;
		case SPI_MODE_1: case SPI_MODE_3:		c_phase = SPI_CR1_CPHA_CLK_TRANSITION_2; break;
		default:{
			#if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            	sys_log_print_event_from_module(SYS_LOG_ERROR, SPI_MODULE_NAME, "Invalid SPI mode!");
            	sys_log_new_line();
        	#endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            err = -1;   /* Invalid SPI mode */
            return err;
		}
	}

	switch(port_addr){
		case(SPI1_BASE):{
			rcc_periph_clock_enable(RCC_SPI1);

			gpio_set_mode(GPIOA, 
				GPIO_MODE_OUTPUT_50_MHZ, 
				GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, 
				GPIO4 | GPIO5 | GPIO6 | GPIO7);
		}break;

		case(SPI2_BASE):{
			rcc_periph_clock_enable(RCC_SPI2);

			gpio_set_mode(GPIOA, 
				GPIO_MODE_OUTPUT_50_MHZ, 
				GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, 
				GPIO4 | GPIO5 | GPIO6 | GPIO7);
		}break;

		case(SPI3_BASE):{
			rcc_periph_clock_enable(RCC_SPI3);

			gpio_set_mode(GPIOA, 
				GPIO_MODE_OUTPUT_50_MHZ, 
				GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, 
				GPIO4 | GPIO5 | GPIO6 | GPIO7);
		}break;

		defalt: {
			#if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            	sys_log_print_event_from_module(SYS_LOG_ERROR, SPI_MODULE_NAME, "Invalid SPI port!");
            	sys_log_new_line();
        	#endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            err = -1;   /* Invalid SPI port */
            return err;
		}
	}

	spi_reset(port_addr);
	spi_set_dff_8bit(port_addr);

	spi_init_master(port_addr, 
		SPI_CR1_BAUDRATE_FPCLK_DIV_16, 
		c_pol, 
		c_phase,
		SPI_CR1_DFF_8BIT, 
		SPI_CR1_MSBFIRST);
			
	/* software Chip Select management */
    spi_enable_software_slave_management(port_addr);
    spi_set_nss_high(port_addr);

    spi_enable(port_addr);

	return ERRNO_SUCCESS;
}

static int spi_stm32_configure(struct spi_controller *controller,
			       struct spi_config *config)
{
	if(config == NULL || controller == NULL) {
		return -ERRNO_MISC_INVALID_ARG;
	}

	uint32_t port_addr = port_to_base_address(controller->port);
	uint32_t baudrate_prescaler = spi_get_prescaler(config->speed_hz);
	
	controller->config = *config;

	spi_set_standard_mode(port_addr, config->mode);
	spi_set_baudrate_prescaler(port_addr, baudrate_prescaler);

	return ERRNO_SUCCESS;
}

static int spi_stm32_write(struct spi_device *dev, uint8_t *buf, size_t len){
	if(dev == NULL || buf == NULL){
		return ERRNO_MISC_INVALID_ARG;
	}

	uint32_t port_addr = port_to_base_address(dev->controller->port);

	/* change CS state */
	spi_set_nss_low(port_addr);

	spi_stm32_write_only(dev, buf, len);

	/* change CS state */
	spi_set_nss_high(port_addr);
	return ERRNO_SUCCESS;
}

static int spi_stm32_read(struct spi_device *dev, uint8_t *buf, size_t len){
	if(dev == NULL || buf == NULL){
		return ERRNO_MISC_INVALID_ARG;
	}

	uint32_t port_addr = port_to_base_address(dev->controller->port);

	/* change CS state */
	spi_set_nss_low(port_addr);

	spi_stm32_read_only(dev, buf, len);

	/* change CS state */
	spi_set_nss_high(port_addr);
	return ERRNO_SUCCESS;
}

static int spi_stm32_transfer(struct spi_device *dev, uint8_t *tx_buf, size_t tx_len, uint8_t *rx_buf, size_t rx_len){
	if(dev == NULL || tx_buf == NULL || rx_buf == NULL){
		return -1;
	}

	int errno = 0;
	uint32_t bytes_read = 0;

	if(rx_len > 0){
		bytes_read = spi_stm32_read(dev, rx_buf, rx_len);
	}

	if(tx_len > 0){
		errno = spi_stm32_write(dev, tx_buf, tx_len);
	}

	if(errno != 0) return errno;
	else return bytes_read;
}

static int spi_stm32_select_slave(struct spi_device *dev, bool slave_state){
	if(dev == NULL){
		return ERRNO_MISC_INVALID_ARG;
	}

	uint32_t port_addr = port_to_base_address(dev->controller->port);

	dev->cs_active_level = slave_state;
	spi_set_nss_low(port_addr);

	return ERRNO_SUCCESS;
}

static int spi_stm32_write_only(struct spi_device *dev, uint8_t *buf, size_t len){
	if(dev == NULL || buf == NULL){
		return ERRNO_MISC_INVALID_ARG;
	}

	uint32_t port_addr = port_to_base_address(dev->controller->port);

	for(uint16_t i = 0; i < len; i++){
		spi_send(port_addr, buf[i]);
	}

	return ERRNO_SUCCESS;
}

static int spi_stm32_read_only(struct spi_device *dev, uint8_t *buf, size_t len){
	if(dev == NULL || buf == NULL){
		return ERRNO_MISC_INVALID_ARG;
	}

	uint32_t port_addr = port_to_base_address(dev->controller->port);
	uint32_t bytes_read = 0;

	for(uint16_t i = 0; i < len; i++){
		buf[i] = spi_read(port_addr);
	}

	return bytes_read;
}

static struct spi_driver_api stm32_spi_api = {
	.init = spi_stm32_init,
	.select_slave = spi_stm32_select_slave,
	.configure = spi_stm32_configure,
	.write = spi_stm32_write,
	.write_only = spi_stm32_write_only,
	.read = spi_stm32_read,
	.read_only = spi_stm32_read_only,
	.transfer = spi_stm32_transfer,
};

static struct spi_controller stm32_controller_list[] = {
	[SPI_PORT_0] = { 0 },
	[SPI_PORT_1] = { 0 },
	[SPI_PORT_2] = { 0 },
};

struct spi_controller *spi_hw_get_controller_handle(enum spi_port port)
{
	if (port > SPI_PORT_2)
		return NULL;

	return &stm32_controller_list[port];
}

struct spi_driver_api *spi_hw_get_driver(void)
{
	return &stm32_spi_api;
}

/** \} End of spi group */

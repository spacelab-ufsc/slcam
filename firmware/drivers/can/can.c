/*
 * can.c
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
 * \brief CAN driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.2.2
 * 
 * \date 2023/07/28
 * 
 * \addtogroup can
 * \{
 */

#include <hal/include/libopencm3/stm32/rcc.h>
#include <hal/include/libopencm3/stm32/gpio.h>
#include <hal/include/libopencm3/stm32/can.h>

#include <system/sys_log/sys_log.h>

#include <config/errno.h>

#include "can.h"

int can_init(can_config_t config)
{
    int err = ERRNO_SUCCESS;

    /* Enable clock to the CAN peripheral */
    rcc_periph_clock_enable(RCC_CAN1);

    /* Reset the can peripheral */
    can_reset(CAN1);

    if (can_init(CAN1, false, true, true, false, false, false,
                 CAN_BTR_SJW_1TQ, CAN_BTR_TS1_11TQ, CAN_BTR_TS2_4TQ, 6,
                 config.loopback, config.silent) < 0)
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, CAN_MODULE_NAME, "Error initializing the CAN port!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        err = ERRNO_DRIVER_FAILED;
    }
    else
    {
        /* Enable CAN interrupts for FIFO message pending (FMPIE) */
        can_enable_irq(CONTROLLER_CAN, CAN_IER_FMPIE0 | CAN_IER_FMPIE1);
        nvic_enable_irq(NVIC_CEC_CAN_IRQ);

        /* Route the CAN signal to our selected GPIOs */
        gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO11 | GPIO12);
        gpio_set_af(GPIOA, GPIO_AF4, GPIO11 | GPIO12);
    }

    return err;
}

int can_write(can_config_t config, uint16_t *data, uint16_t len)
{
    return can_transmit(CAN1, uint32_t id, bool ext, bool rtr, len, data);
}

int can_read(can_config_t config, uint16_t *data, uint16_t *len)
{
    can_receive(CAN1,
                fifo,   /* FIFO id */
                true,   /* Automatically release FIFO after rx */
                &frame.id, &frame.extended_id, &frame.rtr, &frame.filter_id,
                len, data, &frame.ts);

    return ERRNO_SUCCESS;
}

/** \} End of can group */

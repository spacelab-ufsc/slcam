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

#include <hal/include/libopencm3/cm3/nvic.h>
#include <hal/include/libopencm3/stm32/can.h>
#include <hal/include/libopencm3/stm32/gpio.h>
#include <hal/include/libopencm3/stm32/rcc.h>

#include <config/errno.h>

#include "can.h"

int can_init_drv(can_config_t config)
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
        err = ERRNO_DRIVER_FAILED;
    }
    else
    {
        /* Create a filter mask that passes all critical broadcast & command CAN messages */
        can_filter_id_mask_16bit_init(0,        /* Filter number */
                                      0, 0,     /* First filter */
                                      0, 0,     /* Second filter */
                                      0,        /* FIFO 0 */
                                      true);    /* Enable */

        /* Enable CAN interrupts for FIFO message pending (FMPIE) */
        can_enable_irq(CAN1, CAN_IER_FMPIE0);
        nvic_enable_irq(NVIC_SV_CALL_IRQ);

        /* Route the CAN signal to our selected GPIOs */
        gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, GPIO11);
        gpio_set(GPIOA, GPIO11);

        gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO12);
        gpio_set(GPIOA, GPIO12);
    }

    return err;
}

int can_write(can_config_t config, can_id_t dst, uint16_t *data, uint16_t len)
{
    return can_transmit(CAN1, dst, false, false, len, data);
}

int can_available(can_config_t config)
{
    return -1;
}

int can_read(can_config_t config, uint16_t *data, uint16_t *len)
{
    can_receive(CAN1,
                0,      /* FIFO id */
                true,   /* Automatically release FIFO after rx */
                &frame.id, &frame.extended_id, &frame.rtr, &frame.filter_id,
                len, data, &frame.ts);

    return ERRNO_SUCCESS;
}

void cec_can_isr(void)
{
    /* Message pending on FIFO 0? */
    if (CAN_RF0R(CONTROLLER_CAN) & CAN_RF0R_FMP0_MASK)
    {
        receive(0);
    }

    /* Message pending on FIFO 1? */
    if (CAN_RF1R(CONTROLLER_CAN) & CAN_RF1R_FMP1_MASK)
    {
        receive(1);
    }
}

/** \} End of can group */

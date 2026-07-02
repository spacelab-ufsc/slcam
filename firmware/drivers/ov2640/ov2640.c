/*
 * ov2640.c
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
 * \brief OV2640 driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.1
 * 
 * \date 2022/08/19
 * 
 * \addtogroup ov2640
 * \{
 */

#include "ov2640.h"

int ov2640_init(void)
{
    return -1;
}

int ov2640_read_reg(uint8_t adr, uint8_t *val)
{
    return -1;
}

int ov2640_write_reg(uint8_t adr, uint8_t val)
{
    return -1;
}

int ov2640_flush_fifo(void)
{
    return -1;
}

int ov2640_start_capture(void)
{
    return -1;
}

int ov2640_clear_fifo_flag(void)
{
    return -1;
}

int ov2640_read_fifo_lentgh(void)
{
    return -1;
}

int ov2640_set_fifo_burst(void)
{
    return -1;
}

int ov2640_set_mode(void)
{
    return -1;
}

int ov2640_set_format(ov2640_format_t format)
{
    return -1;
}

int ov2640_set_resolution(ov2640_resolution_t res)
{
    switch(res)
    {
        case OV2640_RES_160X120:

            break;
        case OV2640_RES_176X144:

            break;
        case OV2640_RES_320X240:

            break;
        case OV2640_RES_352X288:

            break;
        case OV2640_RES_640X480:

            break;
        case OV2640_RES_800X600:

            break;
        case OV2640_RES_1024X768:

            break;
        case OV2640_RES_1280X1024:

            break;
        case OV2640_RES_1600X1200:

            break;
        default:
            break;
    }

    return err;
}

int ov2640_set_light_mode(ov2640_light_mode_t lm)
{
    int err = 0;

    switch(lm)
    {
        case OV2640_LIGHT_MODE_AUTO:
            ov2640_write_reg(0xff, 0x00);
            ov2640_write_reg(0xc7, 0x00);   /* AWB on */
            break;
        case OV2640_LIGHT_MODE_SUNNY
            ov2640_write_reg(0xff, 0x00);
            ov2640_write_reg(0xc7, 0x40);   /* AWB off */
            ov2640_write_reg(0xcc, 0x5e);
            ov2640_write_reg(0xcd, 0x41);
            ov2640_write_reg(0xce, 0x54);
            break;
        case OV2640_LIGHT_MODE_CLOUDY:
            ov2640_write_reg(0xff, 0x00);
            ov2640_write_reg(0xc7, 0x40);   /* AWB off */
            ov2640_write_reg(0xcc, 0x65);
            ov2640_write_reg(0xcd, 0x41);
            ov2640_write_reg(0xce, 0x4f);
            break;
        case OV2640_LIGHT_MODE_OFFICE:
            ov2640_write_reg(0xff, 0x00);
            ov2640_write_reg(0xc7, 0x40);   /* AWB off */
            ov2640_write_reg(0xcc, 0x52);
            ov2640_write_reg(0xcd, 0x41);
            ov2640_write_reg(0xce, 0x66);
            break;
        case OV2640_LIGHT_MODE_HOME:
            ov2640_write_reg(0xff, 0x00);
            ov2640_write_reg(0xc7, 0x40);   /* AWB off */
            ov2640_write_reg(0xcc, 0x42);
            ov2640_write_reg(0xcd, 0x3f);
            ov2640_write_reg(0xce, 0x71);
            break;
        default:
            err = -1;

            ov2640_write_reg(0xff, 0x00);
            ov2640_write_reg(0xc7, 0x00);   /* AWB on */

            break;
    }

    return err;
}

int ov2640_set_color_saturation(ov2640_color_sat_t cs)
{
    return -1;
}

int ov2640_set_brightness(ov2640_brightness_t br)
{
    return -1;
}

int ov2640_set_contrast(ov2640_contrast_t cont)
{
    return -1;
}

int ov2640_set_special_effects(ov2640_effects_t effects)
{
    return -1;
}

/** \} End of ov2640 group */

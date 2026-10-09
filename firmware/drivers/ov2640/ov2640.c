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
 * along with SLCam. If not, see <www.gnu.org/licenses/>.
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
#include "ov2640_regs.h"
#include "ov2640_controls.h"

#define ARDUCHIP_TEST       0x00U
#define ARDUCHIP_MODE       0x02U
#define ARDUCHIP_FIFO       0x04U
#define ARDUCHIP_TRIG       0x41U
#define FIFO_SIZE1          0x42U
#define FIFO_SIZE2          0x43U
#define FIFO_SIZE3          0x44U
#define FIFO_CLEAR          0x01U
#define FIFO_START          0x02U
#define CAPTURE_DONE        0x08U
#define BURST_FIFO_READ     0x3cU

static ov2640_bus_t bus = { NULL, NULL, NULL, NULL, NULL, NULL };
static bool configured = false;
/* A failed CS release blocks new operations until recovery succeeds. */
static bool controller_selected = false;
static bool burst_active = false;

int32_t ov2640_configure(const ov2640_bus_t *transport)
{
    int32_t status = OV2640_ERROR_ARGUMENT;

    if (controller_selected)
    {
        status = OV2640_ERROR_BUSY;
    }
    else if (transport != NULL)
    {
        if ((transport->sensor_read != NULL) && (transport->sensor_write != NULL) &&
            (transport->delay_ms != NULL) &&
            ((transport->spi_select == NULL) == (transport->spi_transfer == NULL)))
        {
            bus = *transport;
            configured = true;
            status = 0;
        }
    }
    else
    {
        /* Invalid transport; keep the previous configuration. */
    }

    return status;
}

int32_t ov2640_read_reg(uint8_t adr, uint8_t *val)
{
    int32_t status;

    if (val == NULL)
    {
        status = OV2640_ERROR_ARGUMENT;
    }
    else if (!configured)
    {
        status = OV2640_ERROR_TRANSPORT;
    }
    else if (controller_selected)
    {
        status = OV2640_ERROR_BUSY;
    }
    else
    {
        status = bus.sensor_read(bus.ctx, adr, val);
    }

    return status;
}

int32_t ov2640_write_reg(uint8_t adr, uint8_t val)
{
    int32_t status;

    if (!configured)
    {
        status = OV2640_ERROR_TRANSPORT;
    }
    else if (controller_selected)
    {
        status = OV2640_ERROR_BUSY;
    }
    else
    {
        status = bus.sensor_write(bus.ctx, adr, val);
    }

    return status;
}

int32_t ov2640_write_regs(const ov2640_reg_t *reglist, size_t count)
{
    int32_t status = 0;
    size_t index = 0U;
    bool terminated = false;

    if ((reglist == NULL) || (count == 0U))
    {
        status = OV2640_ERROR_ARGUMENT;
    }
    else
    {
        while ((index < count) && (status == 0) && (!terminated))
        {
            if ((reglist[index].adr == 0xffU) && (reglist[index].val == 0xffU))
            {
                terminated = true;
            }
            else if ((reglist[index].adr > 0xffU) || (reglist[index].val > 0xffU))
            {
                status = OV2640_ERROR_ARGUMENT;
            }
            else
            {
                status = ov2640_write_reg((uint8_t)reglist[index].adr,
                                          (uint8_t)reglist[index].val);
            }

            index++;
        }
        if ((status == 0) && (!terminated))
        {
            status = OV2640_ERROR_ARGUMENT;
        }
    }

    return status;
}

static int32_t release_controller(void)
{
    int32_t status = bus.spi_select(bus.ctx, false);
    controller_selected = (status != 0);
    burst_active = false;

    return status;
}

static int32_t controller_transfer(const uint8_t *tx, uint8_t *rx, size_t len)
{
    int32_t status;
    int32_t release_status;

    if ((!configured) || (bus.spi_transfer == NULL))
    {
        status = OV2640_ERROR_TRANSPORT;
    }
    else if (controller_selected)
    {
        status = OV2640_ERROR_BUSY;
    }
    else
    {
        status = bus.spi_select(bus.ctx, true);

        if (status == 0)
        {
            status = bus.spi_transfer(bus.ctx, tx, rx, len);
        }

        release_status = release_controller();

        if (status == 0)
        {
            status = release_status;
        }
    }

    return status;
}

int32_t ov2640_read_controller_reg(uint8_t adr, uint8_t *val)
{
    int32_t status;
    const uint8_t tx[2] = { adr, 0U };
    uint8_t rx[2] = { 0U, 0U };

    if ((val == NULL) || (adr > 0x7fU))
    {
        status = OV2640_ERROR_ARGUMENT;
    }
    else
    {
        status = controller_transfer(tx, rx, sizeof(tx));

        if (status == 0)
        {
            *val = rx[1];
        }
    }

    return status;
}

int32_t ov2640_write_controller_reg(uint8_t adr, uint8_t val)
{
    int32_t status;
    const uint8_t tx[2] = { (uint8_t)(adr | 0x80U), val };

    if (adr > 0x7fU)
    {
        status = OV2640_ERROR_ARGUMENT;
    }
    else
    {
        status = controller_transfer(tx, NULL, sizeof(tx));
    }

    return status;
}

int32_t ov2640_set_format(ov2640_format_t format)
{
    int32_t status = OV2640_ERROR_ARGUMENT;

    if ((format == OV2640_FORMAT_JPEG) || (format == OV2640_FORMAT_RGB565))
    {
        status = ov2640_write_reg(0xffU, 0x01U);
    }
    if (status == 0)
    {
        status = ov2640_write_reg(0x12U, 0x80U);
    }
    if (status == 0)
    {
        bus.delay_ms(bus.ctx, 100U);
        if (format == OV2640_FORMAT_RGB565)
        {
            status = ov2640_write_regs(ov2640_register_tables.OV2640_QVGA, sizeof(ov2640_register_tables.OV2640_QVGA) / sizeof(ov2640_register_tables.OV2640_QVGA[0]));
        }
        else
        {
            if (status == 0)
            {
                status = ov2640_write_regs(ov2640_register_tables.OV2640_JPEG_INIT, sizeof(ov2640_register_tables.OV2640_JPEG_INIT) / sizeof(ov2640_register_tables.OV2640_JPEG_INIT[0]));
            }

            if (status == 0)
            {
                status = ov2640_write_regs(ov2640_register_tables.OV2640_YUV422, sizeof(ov2640_register_tables.OV2640_YUV422) / sizeof(ov2640_register_tables.OV2640_YUV422[0]));
            }

            if (status == 0)
            {
                status = ov2640_write_regs(ov2640_register_tables.OV2640_JPEG, sizeof(ov2640_register_tables.OV2640_JPEG) / sizeof(ov2640_register_tables.OV2640_JPEG[0]));
            }

            if (status == 0)
            {
                status = ov2640_write_reg(0xffU, 0x01U);
            }

            if (status == 0)
            {
                status = ov2640_write_reg(0x15U, 0x00U);
            }

            if (status == 0)
            {
                status = ov2640_set_resolution(OV2640_RES_320X240);
            }
        }
    }

    return status;
}

int32_t ov2640_init(void)
{
    int32_t status = 0;
    uint8_t high = 0U;
    uint8_t low = 0U;
    uint8_t test = 0U;

    if (!configured)
    {
        status = OV2640_ERROR_TRANSPORT;
    }
    else if (controller_selected)
    {
        status = OV2640_ERROR_BUSY;
    }
    else if (bus.spi_transfer != NULL)
    {
        status = ov2640_write_controller_reg(ARDUCHIP_TEST, 0x55U);

        if (status == 0)
        {
            status = ov2640_read_controller_reg(ARDUCHIP_TEST, &test);
        }

        if ((status == 0) && (test != 0x55U))
        {
            status = OV2640_ERROR_ID;
        }

        if (status == 0)
        {
            status = ov2640_set_mode();
        }

        if (status == 0)
        {
            status = ov2640_flush_fifo();
        }
    }
    else
    {
        /* Bare sensor: no ArduChip to initialize. */
    }

    if (status == 0)
    {
        status = ov2640_write_reg(0xffU, 0x01U);
    }

    if (status == 0)
    {
        status = ov2640_read_reg(OV2640_CHIPID_HIGH, &high);
    }

    if (status == 0)
    {
        status = ov2640_read_reg(OV2640_CHIPID_LOW, &low);
    }

    if ((status == 0) && ((high != 0x26U) || ((low != 0x41U) && (low != 0x42U))))
    {
        status = OV2640_ERROR_ID;
    }

    if (status == 0)
    {
        status = ov2640_set_format(OV2640_FORMAT_JPEG);
    }

    return status;
}

int32_t ov2640_flush_fifo(void)
{
    return ov2640_write_controller_reg(ARDUCHIP_FIFO, FIFO_CLEAR);
}

int32_t ov2640_start_capture(void)
{
    return ov2640_write_controller_reg(ARDUCHIP_FIFO, FIFO_START);
}

int32_t ov2640_clear_fifo_flag(void)
{
    return ov2640_flush_fifo();
}

int32_t ov2640_read_fifo_lentgh(void)
{
    return ov2640_read_fifo_length();
}

int32_t ov2640_set_mode(void)
{
    return ov2640_set_controller_mode(OV2640_MODE_MCU2LCD);
}

int32_t ov2640_read_fifo_length(void)
{
    uint8_t low = 0U;
    uint8_t middle = 0U;
    uint8_t high = 0U;
    uint32_t length;
    int32_t status = ov2640_read_controller_reg(FIFO_SIZE1, &low);

    if (status == 0)
    {
        status = ov2640_read_controller_reg(FIFO_SIZE2, &middle);
    }

    if (status == 0)
    {
        status = ov2640_read_controller_reg(FIFO_SIZE3, &high);
    }

    if (status == 0)
    {
        length = (((uint32_t)high & 0x7fU) << 16U) | ((uint32_t)middle << 8U) | (uint32_t)low;
        /* At most 0x7fffff, representable by int32_t. */
        status = (int32_t)length;
    }

    return status;
}

int32_t ov2640_capture_done(void)
{
    uint8_t flags = 0U;
    int32_t status = ov2640_read_controller_reg(ARDUCHIP_TRIG, &flags);

    if (status == 0)
    {
        if ((flags & CAPTURE_DONE) != 0U)
        {
            status = 1;
        }
    }

    return status;
}

int32_t ov2640_set_fifo_burst(void)
{
    const uint8_t command[1] = { BURST_FIFO_READ };
    int32_t status;

    if ((!configured) || (bus.spi_transfer == NULL))
    {
        status = OV2640_ERROR_TRANSPORT;
    }
    else if (controller_selected)
    {
        status = OV2640_ERROR_BUSY;
    }
    else
    {
        status = bus.spi_select(bus.ctx, true);

        if (status == 0)
        {
            status = bus.spi_transfer(bus.ctx, command, NULL, sizeof(command));
        }

        if (status == 0)
        {
            controller_selected = true;
            burst_active = true;
        }

        else
        {
            /* Preserve the transfer error; release_controller records CS state. */
            (void)release_controller();
        }
    }

    return status;
}

int32_t ov2640_end_fifo_burst(void)
{
    int32_t status = 0;

    if (controller_selected)
    {
        status = release_controller();
    }

    return status;
}

int32_t ov2640_read_fifo(uint8_t *buf, size_t len)
{
    const uint8_t dummy[32] = { 0U };
    size_t offset = 0U;
    size_t chunk;
    int32_t status = 0;

    if ((buf == NULL) || (len == 0U))
    {
        status = OV2640_ERROR_ARGUMENT;
    }
    else if (!burst_active)
    {
        status = OV2640_ERROR_BUSY;
    }
    else
    {
        while ((offset < len) && (status == 0))
        {
            chunk = len - offset;

            if (chunk > sizeof(dummy))
            {
                chunk = sizeof(dummy);
            }

            status = bus.spi_transfer(bus.ctx, dummy, &buf[offset], chunk);

            if (status == 0)
            {
                offset += chunk;
            }
            else
            {
                /* Preserve transfer error; a failed release keeps the bus busy. */
                (void)ov2640_end_fifo_burst();
            }
        }
    }

    return status;
}

int32_t ov2640_set_controller_mode(ov2640_mode_t mode)
{
    int32_t status;

    switch (mode)
    {
        case OV2640_MODE_MCU2LCD:   status = ov2640_write_controller_reg(ARDUCHIP_MODE, 0U);    break;
        case OV2640_MODE_CAM2LCD:   status = ov2640_write_controller_reg(ARDUCHIP_MODE, 1U);    break;
        case OV2640_MODE_LCD2MCU:   status = ov2640_write_controller_reg(ARDUCHIP_MODE, 2U);    break;
        default:                    status = OV2640_ERROR_ARGUMENT;                             break;
    }

    return status;
}

int32_t ov2640_set_resolution(ov2640_resolution_t res)
{
    int32_t status;

    switch (res)
    {
        case OV2640_RES_160X120:
            status = ov2640_write_regs(ov2640_register_tables.OV2640_160x120_JPEG, sizeof(ov2640_register_tables.OV2640_160x120_JPEG) / sizeof(ov2640_register_tables.OV2640_160x120_JPEG[0]));
            break;
        case OV2640_RES_176X144:
            status = ov2640_write_regs(ov2640_register_tables.OV2640_176x144_JPEG, sizeof(ov2640_register_tables.OV2640_176x144_JPEG) / sizeof(ov2640_register_tables.OV2640_176x144_JPEG[0]));
            break;
        case OV2640_RES_320X240:
            status = ov2640_write_regs(ov2640_register_tables.OV2640_320x240_JPEG, sizeof(ov2640_register_tables.OV2640_320x240_JPEG) / sizeof(ov2640_register_tables.OV2640_320x240_JPEG[0]));
            break;
        case OV2640_RES_352X288:
            status = ov2640_write_regs(ov2640_register_tables.OV2640_352x288_JPEG, sizeof(ov2640_register_tables.OV2640_352x288_JPEG) / sizeof(ov2640_register_tables.OV2640_352x288_JPEG[0]));
            break;
        case OV2640_RES_640X480:
            status = ov2640_write_regs(ov2640_register_tables.OV2640_640x480_JPEG, sizeof(ov2640_register_tables.OV2640_640x480_JPEG) / sizeof(ov2640_register_tables.OV2640_640x480_JPEG[0]));
            break;
        case OV2640_RES_800X600:
            status = ov2640_write_regs(ov2640_register_tables.OV2640_800x600_JPEG, sizeof(ov2640_register_tables.OV2640_800x600_JPEG) / sizeof(ov2640_register_tables.OV2640_800x600_JPEG[0]));
            break;
        case OV2640_RES_1024X768:
            status = ov2640_write_regs(ov2640_register_tables.OV2640_1024x768_JPEG, sizeof(ov2640_register_tables.OV2640_1024x768_JPEG) / sizeof(ov2640_register_tables.OV2640_1024x768_JPEG[0]));
            break;
        case OV2640_RES_1280X1024:
            status = ov2640_write_regs(ov2640_register_tables.OV2640_1280x1024_JPEG, sizeof(ov2640_register_tables.OV2640_1280x1024_JPEG) / sizeof(ov2640_register_tables.OV2640_1280x1024_JPEG[0]));
            break;
        case OV2640_RES_1600X1200:
            status = ov2640_write_regs(ov2640_register_tables.OV2640_1600x1200_JPEG, sizeof(ov2640_register_tables.OV2640_1600x1200_JPEG) / sizeof(ov2640_register_tables.OV2640_1600x1200_JPEG[0]));
            break;
        default:
            status = OV2640_ERROR_ARGUMENT;
            break;
    }

    return status;
}

int32_t ov2640_set_light_mode(ov2640_light_mode_t lm)
{
    int32_t status;
    switch (lm)
    {
        case OV2640_LIGHT_MODE_AUTO:
            status = ov2640_write_regs(ov2640_control_tables.light_0, sizeof(ov2640_control_tables.light_0) / sizeof(ov2640_control_tables.light_0[0]));
            break;
        case OV2640_LIGHT_MODE_SUNNY:
            status = ov2640_write_regs(ov2640_control_tables.light_1, sizeof(ov2640_control_tables.light_1) / sizeof(ov2640_control_tables.light_1[0]));
            break;
        case OV2640_LIGHT_MODE_CLOUDY:
            status = ov2640_write_regs(ov2640_control_tables.light_2, sizeof(ov2640_control_tables.light_2) / sizeof(ov2640_control_tables.light_2[0]));
            break;
        case OV2640_LIGHT_MODE_OFFICE:
            status = ov2640_write_regs(ov2640_control_tables.light_3, sizeof(ov2640_control_tables.light_3) / sizeof(ov2640_control_tables.light_3[0]));
            break;
        case OV2640_LIGHT_MODE_HOME:
            status = ov2640_write_regs(ov2640_control_tables.light_4, sizeof(ov2640_control_tables.light_4) / sizeof(ov2640_control_tables.light_4[0]));
            break;
        default:
            status = OV2640_ERROR_ARGUMENT;
            break;
    }
    return status;
}

int32_t ov2640_set_color_saturation(ov2640_color_sat_t cs)
{
    int32_t status;

    switch (cs)
    {
        case OV2640_COLOR_SAT_MINUS_2:
            status = ov2640_write_regs(ov2640_control_tables.saturation_0, sizeof(ov2640_control_tables.saturation_0) / sizeof(ov2640_control_tables.saturation_0[0]));
            break;
        case OV2640_COLOR_SAT_MINUS_1:
            status = ov2640_write_regs(ov2640_control_tables.saturation_1, sizeof(ov2640_control_tables.saturation_1) / sizeof(ov2640_control_tables.saturation_1[0]));
            break;
        case OV2640_COLOR_SAT_DEFAULT:
            status = ov2640_write_regs(ov2640_control_tables.saturation_2, sizeof(ov2640_control_tables.saturation_2) / sizeof(ov2640_control_tables.saturation_2[0]));
            break;
        case OV2640_COLOR_SAT_PLUS_1:
            status = ov2640_write_regs(ov2640_control_tables.saturation_3, sizeof(ov2640_control_tables.saturation_3) / sizeof(ov2640_control_tables.saturation_3[0]));
            break;
        case OV2640_COLOR_SAT_PLUS_2:
            status = ov2640_write_regs(ov2640_control_tables.saturation_4, sizeof(ov2640_control_tables.saturation_4) / sizeof(ov2640_control_tables.saturation_4[0]));
            break;
        default:
            status = OV2640_ERROR_ARGUMENT;
            break;
    }

    return status;
}

int32_t ov2640_set_brightness(ov2640_brightness_t br)
{
    int32_t status;

    switch (br)
    {
        case OV2640_BRIGHTNESS_MINUS_2:
            status = ov2640_write_regs(ov2640_control_tables.brightness_0, sizeof(ov2640_control_tables.brightness_0) / sizeof(ov2640_control_tables.brightness_0[0]));
            break;
        case OV2640_BRIGHTNESS_MINUS_1:
            status = ov2640_write_regs(ov2640_control_tables.brightness_1, sizeof(ov2640_control_tables.brightness_1) / sizeof(ov2640_control_tables.brightness_1[0]));
            break;
        case OV2640_BRIGHTNESS_DEFAULT:
            status = ov2640_write_regs(ov2640_control_tables.brightness_2, sizeof(ov2640_control_tables.brightness_2) / sizeof(ov2640_control_tables.brightness_2[0]));
            break;
        case OV2640_BRIGHTNESS_PLUS_1:
            status = ov2640_write_regs(ov2640_control_tables.brightness_3, sizeof(ov2640_control_tables.brightness_3) / sizeof(ov2640_control_tables.brightness_3[0]));
            break;
        case OV2640_BRIGHTNESS_PLUS_2:
            status = ov2640_write_regs(ov2640_control_tables.brightness_4, sizeof(ov2640_control_tables.brightness_4) / sizeof(ov2640_control_tables.brightness_4[0]));
            break;
        default:
            status = OV2640_ERROR_ARGUMENT;
            break;
    }

    return status;
}

int32_t ov2640_set_contrast(ov2640_contrast_t cont)
{
    int32_t status;

    switch (cont)
    {
        case OV2640_CONTRAST_MINUS_2:
            status = ov2640_write_regs(ov2640_control_tables.contrast_0, sizeof(ov2640_control_tables.contrast_0) / sizeof(ov2640_control_tables.contrast_0[0]));
            break;
        case OV2640_CONTRAST_MINUS_1:
            status = ov2640_write_regs(ov2640_control_tables.contrast_1, sizeof(ov2640_control_tables.contrast_1) / sizeof(ov2640_control_tables.contrast_1[0]));
            break;
        case OV2640_CONTRAST_DEFAULT:
            status = ov2640_write_regs(ov2640_control_tables.contrast_2, sizeof(ov2640_control_tables.contrast_2) / sizeof(ov2640_control_tables.contrast_2[0]));
            break;
        case OV2640_CONTRAST_PLUS_1:
            status = ov2640_write_regs(ov2640_control_tables.contrast_3, sizeof(ov2640_control_tables.contrast_3) / sizeof(ov2640_control_tables.contrast_3[0]));
            break;
        case OV2640_CONTRAST_PLUS_2:
            status = ov2640_write_regs(ov2640_control_tables.contrast_4, sizeof(ov2640_control_tables.contrast_4) / sizeof(ov2640_control_tables.contrast_4[0]));
            break;
        default:
            status = OV2640_ERROR_ARGUMENT;
            break;
    }

    return status;
}

int32_t ov2640_set_special_effects(ov2640_effects_t effects)
{
    int32_t status;

    switch (effects)
    {
        case OV2640_EFFECT_NORMAL:
            status = ov2640_write_regs(ov2640_control_tables.effects_0, sizeof(ov2640_control_tables.effects_0) / sizeof(ov2640_control_tables.effects_0[0]));
            break;
        case OV2640_EFFECT_ANTIQUE:
            status = ov2640_write_regs(ov2640_control_tables.effects_1, sizeof(ov2640_control_tables.effects_1) / sizeof(ov2640_control_tables.effects_1[0]));
            break;
        case OV2640_EFFECT_BLUISH:
            status = ov2640_write_regs(ov2640_control_tables.effects_2, sizeof(ov2640_control_tables.effects_2) / sizeof(ov2640_control_tables.effects_2[0]));
            break;
        case OV2640_EFFECT_GREENISH:
            status = ov2640_write_regs(ov2640_control_tables.effects_3, sizeof(ov2640_control_tables.effects_3) / sizeof(ov2640_control_tables.effects_3[0]));
            break;
        case OV2640_EFFECT_REDDISH:
            status = ov2640_write_regs(ov2640_control_tables.effects_4, sizeof(ov2640_control_tables.effects_4) / sizeof(ov2640_control_tables.effects_4[0]));
            break;
        case OV2640_EFFECT_BW:
            status = ov2640_write_regs(ov2640_control_tables.effects_5, sizeof(ov2640_control_tables.effects_5) / sizeof(ov2640_control_tables.effects_5[0]));
            break;
        case OV2640_EFFECT_NEGATIVE:
            status = ov2640_write_regs(ov2640_control_tables.effects_6, sizeof(ov2640_control_tables.effects_6) / sizeof(ov2640_control_tables.effects_6[0]));
            break;
        case OV2640_EFFECT_BW_NEGATIVE:
            status = ov2640_write_regs(ov2640_control_tables.effects_7, sizeof(ov2640_control_tables.effects_7) / sizeof(ov2640_control_tables.effects_7[0]));
            break;
        default:
            status = OV2640_ERROR_ARGUMENT;
            break;
    }

    return status;
}

/** \} End of ov2640 group */

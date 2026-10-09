/*
 * ov2640.h
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
 * \brief OV2640 driver definition.
 *
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 *
 * \version 0.1.1
 *
 * \date 2022/08/19
 *
 * \defgroup ov2640 OV2640
 * \ingroup drivers
 * \{
 */

#ifndef OV2640_H_
#define OV2640_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define OV2640_SCCB_ADDRESS 0x30U
#define OV2640_ERROR_ARGUMENT (-1)
#define OV2640_ERROR_TRANSPORT (-2)
#define OV2640_ERROR_BUSY (-3)
#define OV2640_ERROR_ID (-4)

/**
 * \brief Output formats (initialization defaults to JPEG at 320x240).
 */
typedef enum
{
    OV2640_FORMAT_JPEG,
    OV2640_FORMAT_RGB565
} ov2640_format_t;

/**
 * \brief Color saturation level.
 */
typedef enum
{
    OV2640_COLOR_SAT_MINUS_2 = -2,
    OV2640_COLOR_SAT_MINUS_1,
    OV2640_COLOR_SAT_DEFAULT,
    OV2640_COLOR_SAT_PLUS_1,
    OV2640_COLOR_SAT_PLUS_2
} ov2640_color_sat_t;

/**
 * \brief Brightness level.
 */
typedef enum
{
    OV2640_BRIGHTNESS_MINUS_2 = -2,
    OV2640_BRIGHTNESS_MINUS_1,
    OV2640_BRIGHTNESS_DEFAULT,
    OV2640_BRIGHTNESS_PLUS_1,
    OV2640_BRIGHTNESS_PLUS_2
} ov2640_brightness_t;

/**
 * \brief Contrast level.
 */
typedef enum
{
    OV2640_CONTRAST_MINUS_2 = -2,
    OV2640_CONTRAST_MINUS_1,
    OV2640_CONTRAST_DEFAULT,
    OV2640_CONTRAST_PLUS_1,
    OV2640_CONTRAST_PLUS_2
} ov2640_contrast_t;

/**
 * \brief Effects.
 */
typedef enum {
    OV2640_EFFECT_NORMAL,
    OV2640_EFFECT_ANTIQUE,
    OV2640_EFFECT_BLUISH,
    OV2640_EFFECT_GREENISH,
    OV2640_EFFECT_REDDISH,
    OV2640_EFFECT_BW,
    OV2640_EFFECT_NEGATIVE,
    OV2640_EFFECT_BW_NEGATIVE
} ov2640_effects_t;

/**
 * \brief .
 */
typedef enum {
    OV2640_MODE_MCU2LCD = 0,
    OV2640_MODE_CAM2LCD = 1,
    OV2640_MODE_LCD2MCU = 2
} ov2640_mode_t;

/**
 * Board transport. Callbacks return zero on success or a negative error.
 * Sensor callbacks perform an SCCB register transaction at 7-bit address 0x30.
 * spi_transfer clocks len bytes full duplex, without changing CS; rx may be NULL.
 * spi_select(true) asserts CS, false releases it. SPI mode 0, MSB first.
 * delay_ms must wait at least the specified time. ctx is owned by the caller.
 * Status callbacks use int32_t; positive results are outside the contract.
 * Callbacks must not reenter the driver and must honor buffer sizes.
 * SPI callbacks may both be NULL for a bare sensor without an ArduChip FIFO.
 * The driver is a singleton; callers must serialize all operations, including
 * the entire interval between set_fifo_burst() and end_fifo_burst().
 */
typedef struct {
    void *ctx;
    int32_t (*sensor_read)(void *ctx, uint8_t reg, uint8_t *value);
    int32_t (*sensor_write)(void *ctx, uint8_t reg, uint8_t value);
    int32_t (*spi_select)(void *ctx, bool active);
    int32_t (*spi_transfer)(void *ctx, const uint8_t *tx, uint8_t *rx, size_t len);
    void (*delay_ms)(void *ctx, uint32_t ms);
} ov2640_bus_t;

/**
 * \brief Resolution options.
 */
typedef enum
{
    OV2640_RES_160X120=0,   /**< 160x120 px. */
    OV2640_RES_176X144,     /**< 176x144 px. */
    OV2640_RES_320X240,     /**< 320x240 px. */
    OV2640_RES_352X288,     /**< 352x288 px. */
    OV2640_RES_640X480,     /**< 640x480 px. */
    OV2640_RES_800X600,     /**< 800x600 px. */
    OV2640_RES_1024X768,    /**< 1024x768 px. */
    OV2640_RES_1280X1024,   /**< 1280x1024 px. */
    OV2640_RES_1600X1200    /**< 1600x1200 px. */
} ov2640_resolution_t;

/**
 * \brief Light mode options.
 */
typedef enum
{
    OV2640_LIGHT_MODE_AUTO=0,   /**< Auto. */
    OV2640_LIGHT_MODE_SUNNY,    /**< Sunny. */
    OV2640_LIGHT_MODE_CLOUDY,   /**< Cloudy. */
    OV2640_LIGHT_MODE_OFFICE,   /**< Office. */
    OV2640_LIGHT_MODE_HOME      /**< Home. */
} ov2640_light_mode_t;

/**
 * \brief Register value.
 */
typedef struct ov2640_reg
{
    uint16_t adr;           /**< Address. */
    uint16_t val;           /**< Value. */
} ov2640_reg_t;

/**
 * \brief OV2640 initialization routine.
 *
 * \return The status/error code.
 */
int32_t ov2640_init(void);

/**
 * \brief Reads the value of a register.
 *
 * \param[in] adr is the register address to read.
 *
 * \param[in,out] val is a pointer to store the read value.
 *
 * \return The status/error code.
 */
int32_t ov2640_read_reg(uint8_t adr, uint8_t *val);

/**
 * \brief Writes a value to a given register.
 *
 * \param[in] adr is the register address to write.
 *
 * \param[in] val is the register value to write.
 *
 * \return The status/error code.
 */
int32_t ov2640_write_reg(uint8_t adr, uint8_t val);

/**
 * \brief Writes a register list, stopping at its sentinel or first error.
 *
 * \param[in] reglist Register pairs terminated by {0xff, 0xff}.
 * \param[in] count Number of accessible entries, including the sentinel.
 *
 * \return The status/error code.
 */
int32_t ov2640_write_regs(const ov2640_reg_t *reglist, size_t count);

/**
 * \brief Clears the ArduChip FIFO and capture flag.
 *
 * \return The status/error code.
 */
int32_t ov2640_flush_fifo(void);

/**
 * \brief Starts capture into the ArduChip FIFO.
 *
 * \return The status/error code.
 */
int32_t ov2640_start_capture(void);

/**
 * \brief Clears the FIFO and capture flag (alias of flush_fifo).
 *
 * \return The status/error code.
 */
int32_t ov2640_clear_fifo_flag(void);

/**
 * \brief Returns FIFO size in bytes; legacy spelling of read_fifo_length.
 *
 * \return The status/error code.
 */
int32_t ov2640_read_fifo_lentgh(void);

/**
 * \brief Asserts CS and sends the burst command; finish with end_fifo_burst.
 *
 * \return The status/error code.
 */
int32_t ov2640_set_fifo_burst(void);

/**
 * \brief Selects the default ArduChip MCU2LCD mode.
 *
 * \return The status/error code.
 */
int32_t ov2640_set_mode(void);

/**
 * \brief Resets and configures JPEG or RGB565 at 320x240.
 *
 * \param[in] format Output format.
 *
 * \return The status/error code.
 */
int32_t ov2640_set_format(ov2640_format_t format);

/**
 * \brief Sets the resolution of incoming frames.
 *
 * \param[in] res is the resolution of frames. It can be:
 * \parblock
 *     -\b OV2640_RES_160X120
 *     -\b OV2640_RES_176X144
 *     -\b OV2640_RES_320X240
 *     -\b OV2640_RES_352X288
 *     -\b OV2640_RES_640X480
 *     -\b OV2640_RES_800X600
 *     -\b OV2640_RES_1024X768
 *     -\b OV2640_RES_1280X1024
 *     -\b OV2640_RES_1600X1200
 *     .
 * \endparblock
 *
 * \return The status/error code.
 */
int32_t ov2640_set_resolution(ov2640_resolution_t res);

/**
 * \brief Sets the light mode of the sensor.
 *
 * \param[in] lm is the light mode. It can be:
 * \parblock
 *     -\b OV2640_LIGHT_MODE_AUTO
 *     -\b OV2640_LIGHT_MODE_SUNNY
 *     -\b OV2640_LIGHT_MODE_CLOUDY
 *     -\b OV2640_LIGHT_MODE_OFFICE
 *     -\b OV2640_LIGHT_MODE_HOME
 *     .
 * \endparblock
 *
 * \return The status/error code.
 */
int32_t ov2640_set_light_mode(ov2640_light_mode_t lm);

/**
 * \brief Sets saturation from -2 to +2.
 *
 * \param[in] cs is the saturation level. It can be:
 * \parblock
 *      -\b OV2640_COLOR_SAT_MINUS_2
 *      -\b OV2640_COLOR_SAT_MINUS_1
 *      -\b OV2640_COLOR_SAT_DEFAULT
 *      -\b OV2640_COLOR_SAT_PLUS_1
 *      -\b OV2640_COLOR_SAT_PLUS_2
 *      .
 * \endparblock
 *
 * \return The status/error code.
 */
int32_t ov2640_set_color_saturation(ov2640_color_sat_t cs);

/**
 * \brief Sets brightness from -2 to +2.
 *
 * \param[in] br is the brightness level. It can be:
 * \parblock
 *      -\b OV2640_BRIGHTNESS_MINUS_2
 *      -\b OV2640_BRIGHTNESS_MINUS_1
 *      -\b OV2640_BRIGHTNESS_DEFAULT
 *      -\b OV2640_BRIGHTNESS_PLUS_1
 *      -\b OV2640_BRIGHTNESS_PLUS_2
 *      .
 * \endparblock
 *
 * \return The status/error code.
 */
int32_t ov2640_set_brightness(ov2640_brightness_t br);

/**
 * \brief Sets contrast from -2 to +2.
 *
 * \param[in] cont is the contrast level. It can be:
 * \parblock
 *      -\b OV2640_CONTRAST_MINUS_2
 *      -\b OV2640_CONTRAST_MINUS_1
 *      -\b OV2640_CONTRAST_DEFAULT
 *      -\b OV2640_CONTRAST_PLUS_1
 *      -\b OV2640_CONTRAST_PLUS_2
 *      .
 * \endparblock
 *
 * \return The status/error code.
 */
int32_t ov2640_set_contrast(ov2640_contrast_t cont);

/**
 * \brief Selects an image effect.
 *
 * \param[in] effects is the image effect. It can be:
 * \parblock
 *      -\b OV2640_EFFECT_NORMAL
 *      -\b OV2640_EFFECT_ANTIQUE
 *      -\b OV2640_EFFECT_BLUISH
 *      -\b OV2640_EFFECT_GREENISH
 *      -\b OV2640_EFFECT_REDDISH
 *      -\b OV2640_EFFECT_BW
 *      -\b OV2640_EFFECT_NEGATIVE
 *      -\b OV2640_EFFECT_BW_NEGATIVE
 *      .
 * \endparblock
 *
 * \return The status/error code.
 */
int32_t ov2640_set_special_effects(ov2640_effects_t effects);

/**
 * \brief Copy transport callbacks before init. Buses must already be initialized.
 *
 * \return The status/error code.
 */
int32_t ov2640_configure(const ov2640_bus_t *transport);

/**
 * \brief ArduChip SPI registers, distinct from sensor SCCB registers.
 *
 * \return The status/error code.
 */
int32_t ov2640_read_controller_reg(uint8_t adr, uint8_t *val);

/**
 * \brief .
 *
 * \return The status/error code.
 */
int32_t ov2640_write_controller_reg(uint8_t adr, uint8_t val);

/**
 * \brief Return FIFO size in bytes (23 bits), or a negative error.
 *
 * \return The status/error code.
 */
int32_t ov2640_read_fifo_length(void);

/**
 * \brief Read bytes within an active burst; CS remains asserted across chunks.
 *
 * \return The status/error code.
 */
int32_t ov2640_read_fifo(uint8_t *buf, size_t len);

/**
 * \brief Release CS and finish a burst; also safe when no burst is active.
 *
 * \return The status/error code.
 */
int32_t ov2640_end_fifo_burst(void);

/**
 * \brief Select an ArduChip operating mode.
 *
 * \return The status/error code.
 */
int32_t ov2640_set_controller_mode(ov2640_mode_t mode);

/**
 * \brief Return 1 when capture is complete, 0 while pending, or a negative error.
 *
 * \return The status/error code.
 */
int32_t ov2640_capture_done(void);

#endif /* OV2640_H_ */

/** \} End of ov2640 group */

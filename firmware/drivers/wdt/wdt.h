/*
 * uart.h
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
 * \brief Internal Watchdog driver definition.
 * 
 * \author Pedro Ferrari Barbosa <pedro.ferraribarbosa2007@gmail.com>
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.2.1
 * 
 * \date 2026/08/21
 * 
 * \defgroup wdt WDT
 * \ingroup drivers
 * \{
 */

#ifndef WDT_H_
#define WDT_H_

#include <hal/include/libopencm3/stm32/rcc.h>
#include <hal/include/libopencm3/stm32/iwdg.h>

#include <stdint.h>

#define WDT_MODULE_NAME             "WDT"

/**
 * \brief Watchdog period in miliseconds.
 */
#define DEFAULT_WTD_PERIOD_MS 500

/**
 * \brief Watchdog configuration.
 */
typedef struct
{
    uint64_t clk_period_ms; 
} wdt_config_t;

void wdt_init(wdt_config_t config);
void wdt_reset(void);

#endif /* WDT_H_ */

/** \} End of wdt group */
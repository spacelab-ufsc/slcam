/*
 * wdt.c
 * 
 * Copyright (C) 2021, SpaceLab.
 * 
 * This file is part of OBDH 2.0.
 * 
 * OBDH 2.0 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * OBDH 2.0 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with OBDH 2.0. If not, see <http:/\/www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Internal watchdog timer driver implementation.
 * 
 * \author Pedro Ferrari Barbosa <pedro.ferraribarbosa2007@gmail.com>
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.8.8
 * 
 * \date 2026/08/21
 * 
 * \addtogroup wdt
 * \{
 */

#include <config/config.h>
#include <system/sys_log/sys_log.h>

#include "wdt.h"

void wdt_init(wdt_config_t config)
{
    iwdg_set_period_ms(config.clk_period_ms);
    iwdg_start();

#if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
    sys_log_print_event_from_module(SYS_LOG_INFO, WDT_MODULE_NAME, "Internal Watchdog Timer Initialized!");
    sys_log_new_line();
#endif /* CONFIG_DRIVERS_DEBUG_ENABLED */          
}

void wdt_reset(void)
{
    iwdg_reset();
}

/** \} End of wdt group */
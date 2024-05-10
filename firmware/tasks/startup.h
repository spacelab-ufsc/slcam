/*
 * startup.h
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
 * \brief Startup task definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.2.7
 * 
 * \date 2024/01/17
 * 
 * \defgroup startup Startup
 * \ingroup tasks
 * \{
 */

#ifndef STARTUP_H_
#define STARTUP_H_

#include <FreeRTOS.h>
#include <task.h>
#include <event_groups.h>

#define TASK_STARTUP_NAME                   "Startup"
#define TASK_STARTUP_STACK_SIZE             500
#define TASK_STARTUP_PRIORITY               5

/* Status done bit position */
#define TASK_STARTUP_DONE                   (1 << 0)

/**
 * \brief Startup task handle.
 */
extern xTaskHandle xTaskStartupHandle;

/**
 * \brief Startup status event group.
 */
extern EventGroupHandle_t task_startup_status;

/**
 * \brief System startup task.
 *
 * This task runs in a single shot (non-periodic). After it's conclusion, it is deleted.
 *
 * \param[in] pvParameters is a value that will passed as the task's parameter.
 *
 * \return None.
 */
void vTaskStartup(void *pvParameters);

#endif /* STARTUP_H_ */

/** \} End of startup group */
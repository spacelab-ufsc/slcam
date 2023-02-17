/*
 * errno.h
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
 * \brief Errors definition.
 * 
 * \author Miguel Boing <miguelboing13@gmail.com>
 * 
 * \version 0.1.3
 * 
 * \date 2023/02/16
 * 
 * \defgroup errno Error Codes
 * \{
 */

#ifndef ERRNO_H
#define ERRNO_H

/* No error ocurred */
#define ERRNO_SUCCESS                        0x00U       /** No errors found */

/* Driver Level errors */
#define ERRNO_DRIVER_NO_PORT                 0x11U       /**  No port found */
#define ERRNO_DRIVER_NO_PARAMETER            0x12U       /**  Invalid parameter value */
#define ERRNO_DRIVER_FAILED                  0x13U       /** Failed to execute HAL function */             

/* Device Level errors */
#define ERRNO_DEVICE_FAILED_CONFIG           0x21U       /** Failed to configure driver */
#define ERRNO_DEVICE_FAILED_COM              0x22U       /** Failed to communicate */

#endif /*ERRNO_H */

/** \} End of errno group */

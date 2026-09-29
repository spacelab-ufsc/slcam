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
 * \brief Error definitions.
 * 
 * \author Miguel Boing <miguelboing13@gmail.com>
 * \author Pedro Ferrari Barbosa <pedro.ferraribarbosa2007@gmail.com>
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

#include <utils/macros/macros.h>

/*
 * \brief error listing using X Macros pattern
 * \to add an error, just add another entry
*/
#define ERROR_LIST \
  /*no error ocurred*/ \
  X(ERRNO_SUCCESS) \
  /*driver level errors*/ \
  X(ERROR_DRIVER_NO_PORT) \
  X(ERROR_DRIVER_NO_PARAMETER) \
  X(ERROR_DRIVER_FAILED) \
  X(ERROR_DRIVER_UNINITIALIZED) \
  X(ERROR_DRIVER_NO_HW_IMPL) \
  /*device level errors*/ \
  X(ERROR_DEVICE_FAILED_CONFIG) \
  X(ERROR_DEVICE_FAILED_COM) \
  /*miscellaneous errors*/ \
  X(ERROR_MISC_INVALID_ARG) \
  X(ERROR_MISC_FAILED_ALLOC) \
  X(ERRNO_MISC_UNSUPPORTED_OP) \
  X(ERRNO_MISC_UNKNOWN) \
  X(ERRNO_MISC_TIMEOUT) \

/*
 * \brief retrieve error code as string
 */
#define ERROR_AS_STRING(error) (#error)

/*
 * \brief error enum type
 */
typedef enum error_t{
#define X(error) error, 
  ERROR_LIST
#undef X /*avoid global namespace pollution*/
}error_t;

#endif /*ERRNO_H */

/** \} End of errno group */

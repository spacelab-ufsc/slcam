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
 * \brief error enum type
 */
typedef enum error_t{
#define ERROR(errno) errno, 
  #include "error.inc"
#undef ERROR /*avoid global namespace pollution*/

  ERRNO_LIST_LENGTH
}error_t;

/**
 * \brief converts errno value to its string representation.
 *
 * \param error is the error that will be converted.
 *
 * \return the string representation of the parameter.
 */
static const char *errno_to_string(error_t error)
{
    switch (error) {
#define ERROR(errno) case errno: return #errno;
  #include "error.inc"
#undef ERROR
      case ERRNO_LIST_LENGTH: return "ERROR_LIST_LENGTH";
    }

    return "ERROR_FAILED_STRING_CONVERSION";
}

#endif /*ERRNO_H */
/** \} End of errno group */

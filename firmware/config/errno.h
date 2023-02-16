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

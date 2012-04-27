/***************************************************************************//**
 * @file      : cox_types.h
 * @brief     : Contains the COX typedefs for C standard types.
 * @version   : V1.0
 * @date      : 26. May. 2010
 * @author    : CooCox
*******************************************************************************/
#ifndef __COX_TYPES_H
#define __COX_TYPES_H      
#include "stdint.h"

/** Status type definition */
typedef enum {
  COX_SUCCESS = 0,
  COX_ERROR   = 1,  
} COX_Status;

#define COX_NULL             ((void *)0)

#define COX_PIN_Dev          uint16_t

/** Define a PIO device using port number and pin number */
#define COX_PIN(port_num, pin_num)  ((((uint16_t)(port_num) & 0xFF) << 8) | ((pin_num) & 0xFF))
#define COX_PORT_NUM(pin)    ((pin >> 8) & 0xFF)
#define COX_PIN_NUM(pin)     ((pin >> 0) & 0xFF)

/** Invalid PIN */
#define COX_PIN_NC           COX_PIN(0xFF, 0xFF)

#endif

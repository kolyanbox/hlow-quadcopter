/**************************************************************************//**
 * @file      : cox_serial.h
 * @brief     : COX Serial Peripheral Interface
 * @version   : V1.0
 * @date      : 7 Jul. 2010
 * @author    : CooCox
******************************************************************************/
#ifndef __COX_SERIAL_H
#define __COX_SERIAL_H

#include "cox_types.h"

/** Serial Configuration Index Definition */
typedef enum {
  COX_SERIAL_CFG_RATE      = 0,  /*!< Baud rate */
  COX_SERIAL_CFG_BITS      = 1,  /*!< Number of data bits */
  COX_SERIAL_CFG_STOP_BITS = 2,  /*!< Number of stop bits */
  COX_SERIAL_CFG_PARITY    = 3,  /*!< Parity selection */
} COX_SERIAL_CFG_INDEX_Def;

/** Serial Stop Bits Definition */
typedef enum {
  COX_SERIAL_STOPBIT_1   = 0,    /*!< 1 Stop Bits Select   */
  COX_SERIAL_STOPBIT_1_5 = 1,    /*!< 1.5 Stop Bits Select */
  COX_SERIAL_STOPBIT_2   = 2,    /*!< 2 Stop Bits Select   */
  COX_SERIAL_STOPBIT_0_5 = 3,
} SERIAL_STOP_BITS_Def;

/** Serial Data Bits Definition */
typedef enum {
  COX_SERIAL_BIT_5  = 5,         /*!< 5 bit data mode */
  COX_SERIAL_BIT_6  = 6,         /*!< 6 bit data mode */
  COX_SERIAL_BIT_7  = 7,         /*!< 7 bit data mode */
  COX_SERIAL_BIT_8  = 8,         /*!< 8 bit data mode */
  COX_SERIAL_BIT_9  = 9,         /*!< 9 bit data mode */
} SERIAL_DATA_BITS_Def;

/** Serial Parity Mode Definition */
typedef enum {
  COX_SERIAL_PARITY_NONE  = 0,   /*!< No parity   */
  COX_SERIAL_PARITY_ODD   = 1,   /*!< Odd parity  */
  COX_SERIAL_PARITY_EVEN  = 2,   /*!< Even parity */
  COX_SERIAL_PARITY_SP_1  = 3,   /*!< Forced "1" stick parity */
  COX_SERIAL_PARITY_SP_0  = 4,   /*!< Forced "0" stick parity */
} SERIAL_PARITY_MODE_Def;


/** COX Serial Peripheral Interface Definition  
*****************************************************************************/
typedef struct {
  COX_Status (*Init)     (uint32_t baudrate);      
  uint32_t   (*Write)    (const void* wbuf, uint32_t wlen);
  uint32_t   (*Read)     (void *rbuf, uint32_t rlen);
  COX_Status (*Cfg)      (uint8_t index, uint32_t arg, uint32_t *pre_arg);  
} COX_SERIAL_PI_Def;

typedef const COX_SERIAL_PI_Def COX_SERIAL_PI;

#endif

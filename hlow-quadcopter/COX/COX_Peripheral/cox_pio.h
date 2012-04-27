/**************************************************************************//**
 * @file      : cox_pio.h
 * @brief     : COX PIO Peripheral Interface
 * @version   : V1.0
 * @date      : 26. May. 2010
 * @author    : CooCox
******************************************************************************/
#ifndef __COX_PIO_H
#define __COX_PIO_H
#include "cox_types.h"

/** PIO Configuration Index Definition */
typedef enum {  
  COX_PIO_CFG_MODE = 0,        /*!< Configure PIO mode : 
                                    (Pull-up, Pull-down, Tri-state) */
} COX_PIO_CFG_INDEX_Def;


/** PIO Mode Definition */
typedef enum {  
  COX_PIO_MODE_PULLUP   = 0,   /*!< Internal pull-up resistor   */
  COX_PIO_MODE_PULLDOWN = 1,   /*!< Internal pull-down resistor */
  COX_PIO_MODE_TRISTATE = 2, 	 /*!< Tri-state                   */
} COX_PIO_MODE_Def;

/** PIO Direction Definition */
typedef enum {
  COX_PIO_INPUT   = 0,         /*!< Input Direction  */
  COX_PIO_OUTPUT  = 1,         /*!< Output Direction */
} COX_PIO_DIR_Def;

/** PIO State Definition */
typedef enum {              
  COX_PIO_LOW  = 0,            /*!< Low level or Digital 0  */
  COX_PIO_HIGH = 1,            /*!< High level or Digital 1 */
} COX_PIO_STATE_Def;


/** PIO Device Definition */
#define COX_PIO_Dev          uint16_t      

/** Get Port number and Pin number via PIO */
#define COX_PIO_PORT(pio)    ((pio >> 8) & 0xFF)
#define COX_PIO_PIN(pio)     ((pio >> 0) & 0xFF)  



/** COX PIO Peripheral Interface Definition 
*******************************************************************************/
typedef struct {
  COX_Status (*Init) (COX_PIO_Dev pio);
  COX_Status (*Dir)  (COX_PIO_Dev pio, uint8_t dir);
  uint8_t    (*Out)  (COX_PIO_Dev pio, uint8_t level);
  uint8_t    (*Read) (COX_PIO_Dev pio);
  COX_Status (*Cfg)  (COX_PIO_Dev pio, uint8_t index, uint32_t arg, uint32_t *pre_arg);
} COX_PIO_PI_Def;

typedef const COX_PIO_PI_Def COX_PIO_PI;

#endif

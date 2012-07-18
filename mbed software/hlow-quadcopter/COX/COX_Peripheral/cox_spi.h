/**************************************************************************//**
 * @file      : cox_spi.h
 * @brief     : COX SPI Peripheral Interface
 * @version   : V1.0
 * @date      : 25. May. 2010
 * @author    : CooCox
******************************************************************************/
#ifndef __COX_SPI_H
#define __COX_SPI_H

#include "cox_types.h"

/** SPI Configuration Index Definition */
typedef enum {
  COX_SPI_CFG_MODE  = 0,  /*!< SPI mode */
  COX_SPI_CFG_RATE  = 1,  /*!< SPI clock rate */
  COX_SPI_CFG_BITS  = 2,  /*!< The number of bits per transfer */
  COX_SPI_CFG_FSB   = 3,  /*!< Data transfer order */
} COX_SPI_CFG_INDEX_Def;

/** SPI Mode Definition */
typedef enum {
  COX_SPI_MODE0  = 0,     /*!< SPI Mode 0 */  
  COX_SPI_MODE1  = 1,     /*!< SPI Mode 1 */  
  COX_SPI_MODE2  = 2,     /*!< SPI Mode 2 */  
  COX_SPI_MODE3  = 3,     /*!< SPI Mode 3 */  
} COX_SPI_MODE_Def;  

/* SPI Transfer Order Definition */
typedef enum {
  COX_MSPI_FSB_MSB = 0,   /*!< MSB first sent */
  COX_MSPI_FSB_LSB = 1,   /*!< LSB first sent */
} COX_SPI_ORDER_Def;

/** CoX SPI Peripheral Interface 
********************************************************************************/
typedef struct {
  COX_Status (*Init)      (uint8_t mode,  uint32_t rate);      
  uint32_t   (*Write)     (const void *wbuf, uint32_t wlen);
  uint32_t   (*Read)      (void *rbuf,    uint32_t rlen);
  uint32_t   (*ReadWrite) (uint32_t wdat);  
  COX_Status (*Cfg)       (uint8_t index, uint32_t arg, uint32_t *pre_arg);  
} COX_SPI_PI_Def;

typedef const COX_SPI_PI_Def COX_SPI_PI;

#endif

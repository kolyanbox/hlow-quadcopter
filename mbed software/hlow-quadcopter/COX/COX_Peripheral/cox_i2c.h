/******************************************************************************
 * @file      : cox_i2c.h
 * @brief     : COX I2C Master Peripheral Interface
 * @version   : V1.0
 * @date      : 28 Jun. 2010
 * @author    : CooCox
******************************************************************************/
#ifndef __COX_I2C_H
#define __COX_I2C_H

#include "cox_types.h"

/** I2C Configuration Index Definition */
typedef enum {
  COX_I2C_CFG_RATE = 0,     /*!< I2C clock rate */
} COX_I2C_CFG_INDEX_Def;


/** I2C transfer setup data structure definitions */
typedef struct {
  uint16_t slave;       /*!< Slave address */  
  const void * wbuf;    /*!< Pointer to transmit data */
  uint16_t wlen;        /*!< Length of transmit data  */
  uint16_t wcnt;        /*!< The actual length of the data transmited */
  void *   rbuf;        /*!< Pointer to receive data  */ 
  uint16_t rlen;        /*!< Length of receive data   */
  uint16_t rcnt;        /*!< The actual length of the data received */
} I2C_Transfer_Cfg;


/** I2C Master Peripheral Interface Definition
*****************************************************************************/
typedef struct {
  COX_Status (*Init)       (uint32_t rate);      
  COX_Status (*Transfer)   (I2C_Transfer_Cfg *cfg);
  COX_Status (*CheckSlave) (uint8_t slave_adr);  
  COX_Status (*BusReset)   (void); 
  COX_Status (*Cfg)        (uint8_t index, uint32_t arg, uint32_t *pre_arg);  
} COX_I2C_PI_Def;

typedef const COX_I2C_PI_Def COX_I2C_PI;

#endif

/******************************************************************************
 * @file      : cox_i2c_slave.h
 * @brief     : COX I2C Slave Peripheral Interface
 * @version   : V1.0
 * @date      : 28. July. 2010
 * @author    : CooCox
******************************************************************************/
#ifndef __COX_I2C_SLAVE_H
#define __COX_I2C_SLAVE_H

#include "cox_types.h"

/** I2C power and clock control */
#define COX_SI2C_CFG_ADDRESS       0

/** Leading byte number */
#define COX_SI2C_CFG_LEADING       1

/** Write operation call back function */
#define COX_SI2C_CFG_CB_WRITE      2

/** Read operation call back function */
#define COX_SI2C_CFG_CB_READ       3

#define COX_SI2C_LEAD_LEN_MAX      4

typedef  uint8_t (*cbWriteFun)(uint8_t index, uint8_t write_data);
typedef  uint8_t (*cbReadFun)(uint8_t index, uint8_t *read_data);

typedef struct {    
  uint32_t write_cnt;
  uint32_t read_cnt;
  uint8_t (*cbWrite) (uint8_t index, uint8_t write_data);
  uint8_t (*cbRead)  (uint8_t index, uint8_t *read_data);
} SI2C_Setup_Cfg;

/** COX I2C Slave Peripheral Interface Definition 
*****************************************************************************/
typedef struct {
  COX_Status (*Init)     (uint8_t adr);
  void       (*SetAdr)   (uint8_t adr);  
  void       (*SetWrite) (cbWriteFun write_func);
  void       (*SetRead)  (cbReadFun read_func);
  COX_Status (*Cfg)      (uint8_t index, uint32_t arg, uint32_t *pre_arg);
} COX_SI2C_PI_Def;

typedef const COX_SI2C_PI_Def COX_SI2C_PI;

#endif

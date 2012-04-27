/**************************************************************************//**
 * @file      : lpc_i2c.h
 * @brief     : LPC17xx I2C Master Peripheral Interface
 * @version   : V1.0
 * @date      : 30. Jun. 2010
 * @author    : CooCox
******************************************************************************/
#ifndef __LPC_I2C_H
#define __LPC_I2C_H

#include "cox_types.h"
#include "cox_i2c.h"

/**
 * Pin assignment                                                           \n  
 *   +-----------+---------+--------+                                       \n
 *   | Interface |  SDA    |  SCL   |                                       \n
 *   +-----------+---------+--------+                                       \n
 *   |    I2C0   |  P0.27  |  P0.28 |                                       \n
 *   +-----------+---------+--------+                                       \n
 *   |    I2C1   |  P0.0   |  P0.1  |                                       \n
 *   +-----------+---------+--------+                                       \n
 *   |    I2C2   |  P0.10  |  P0.11 |                                       \n
 *   +-----------+---------+--------+                                       \n
 *                                                                          \n
 * Configuration Support:                                                   \n
 *   COX_MI2C_CFG_RATE   :  Set I2C clock rate                              \n
******************************************************************************/

#define LPC_DEV_I2C0  { COX_PIN(0, 27), COX_PIN(0, 28) }
#define LPC_DEV_I2C1  { COX_PIN(0,  0), COX_PIN(0,  1) } 
#define LPC_DEV_I2C2  { COX_PIN(0, 10), COX_PIN(0, 11) }


/** Define LPC17xx CoX I2C Master Peripheral Interface based on I2C0
*******************************************************************************/
extern COX_I2C_PI pi_i2c0;
extern COX_I2C_PI pi_i2c1;
extern COX_I2C_PI pi_i2c2;

#endif

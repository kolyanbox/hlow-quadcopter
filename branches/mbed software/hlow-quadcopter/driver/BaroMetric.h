#ifndef BAROMETRIC_H_
#define BAROMETRIC_H_

//#include "lpc17xx_i2c.h"
#include <Interfaces/Actuators/Actuators.h>
//#include <driver/I2C.h>

//#define I2CDEV_S_ADDR	(0xEF)
//#define I2CDEV_M LPC_I2C2

void BaroMetricInit(void);
void Barometric_get(unsigned char* rxBuffer);

#endif

#ifndef I2C_H
#define I2C_H

#include "lpc17xx_i2c.h"
#include <Interfaces/Actuators/Actuators.h>
#include <General/util.h>

void I2C_Initialize(void);

void I2C_Initialize();
void I2C2_IRQHandler(void);

#endif

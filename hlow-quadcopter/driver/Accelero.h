#ifndef Accelero_H_
#define Accelero_H_

#include "lpc17xx_i2c.h"
#include <Interfaces/Actuators/Actuators.h>
#include <driver/I2C.h>

int Accelero_init();
void Accelero_get(unsigned char* rxBuffer);
uint16_t getAccelero(int ValueNumber);

#endif

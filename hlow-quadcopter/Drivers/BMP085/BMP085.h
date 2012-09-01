#ifndef BMP085_H
#define BMP085_H

#include <General/util.h>
#include "lpc17xx_i2c.h"

Bool initializeBMP085();
long getTemperature(void);
long getPressure(void);

#endif

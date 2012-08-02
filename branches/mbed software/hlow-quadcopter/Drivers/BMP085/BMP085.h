#ifndef BMP085_H
#define BMP085_H

#include <General/util.h>
#include "lpc17xx_i2c.h"

Bool initializeBMP085();
short getDataBMP085(uint8_t transmitMessage);
Bool writeDataBmp085(uint8_t transmitData, uint8_t addres);

long getUpBMP085(uint8_t transmitMessage, short oss);
long getUtBMP085(uint8_t transmitMessage);

long getTemperature(void);
long getPressure(void);

#endif

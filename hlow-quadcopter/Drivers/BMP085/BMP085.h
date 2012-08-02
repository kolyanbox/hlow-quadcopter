#ifndef BMP085_H
#define BMP085_H

#include <General/util.h>
#include "lpc17xx_i2c.h"
#include <Interfaces/Actuators/Actuators.h>

Bool initializeBMP085();
Bool getDataBMP085(uint8_t transmitMessage);
Bool writeDataBmp085(uint8_t transmitData, uint8_t addres);

long getUpBMP085(uint8_t transmitMessage, short oss);
long getUtBMP085(uint8_t transmitMessage);

long getTemperature(void);
long getPressure(void);

void dataBMP085Callback (void);
void UPBMP085Callback (void);
void UTBMP085Callback (void);

short get_value_bmp085();

#endif

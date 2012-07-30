#ifndef WIIMOTIONPLUS_H
#define WIIMOTIONPLUS_H

#include <General/util.h>
#include <Drivers/I2C/I2C.h>

float gyroscope_get_value(int valueNumber);
Bool initializeWiiMotionPlus();
void gyroscope_get();

#endif

#include <lpc_types.h>
#include <driver/UltrasonicSensor.h>
#include <driver/Accelero.h>
#include <driver/ADC.h>
#include <CoOS.h>

#ifndef SENSORS_H_
#define SENSORS_H_



enum SENSOR{
	accelero = 0,
	distanceToGround = 1
};

/*Get a specific value from a sensor. Returns true if succeeds and false if it does'nt succeed*/
Bool getValue(enum SENSOR Sensor, int ValueNumber, void *Value);

/*Get a specific value from the accelero sensor. Returns true if succeeds and false if it doesn't succeed
 *it isn't recommended to use this method. rather use getValue*/
Bool getAcceleroValue(int ValueNumber, uint16_t *Value);

Bool getDistanceToGround(int ValueNumber, float *Value);

/*Initialize sensors*/
Bool SensorInit();
#endif

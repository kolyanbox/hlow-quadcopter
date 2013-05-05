#ifndef SENSORS_H_
#define SENSORS_H_

/*common includes*/
#include <lpc_types.h>

enum Axle {
	X = 2,
	Y = 1,
	Z = 0
};

enum SensorType{
	SensorUart = 0,
	SensorDistanceToGround = 1,
	SensorAccelero = 2,
	SensorPressure = 3,
	SensorRotation = 4,
	SensorGPS = 5
};

extern volatile unsigned char lastReceivedChar;

/*This method returns TRUE if all sensors are correct initialized and FALSE if one sensor fails to initialize*/
Bool sensorInitialization(enum SensorType sensorType);
int getCurrentAngle(enum Axle axle);
int getCurrentBatteryStatus();
int getCurrentHeightInCm();
float getRotationAroundAxle(enum Axle axle);
long getCurrentTemperature();
long getCurrentPressure();
float calculateCurrentPressureAtSeaLevel(float currentAltitude);
/**
 * If pressureAtSea is set to -1 the pressure calculated by the method
 * calculateCurrentPressureAtSeaLevel will be used
 */
float getCurrentAltitude(float pressureatSea);
//Only use this method in startup sequence
unsigned char getLastCharacterFromUart();

#endif

#ifndef SENSORS_H_
#define SENSORS_H_

/*common includes*/
#include <lpc_types.h>

enum Axle {
	X = 0,
	Y = 1,
	Z = 2
};

enum Command {
	commandOsTime = 0,
	commandDistanceToGround = 1,
	CommandRotationX = 2,
	CommandRotationY = 3,
	CommandRotationZ = 4,
	CommandNoCommand = 5,
	CommandHelp = 6,
	CommandAllTaskStatus = 7,
	CommandStopTaskWithId = 8
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
int getCurrentHeightInCm();
int getRotation(void);
long getCurrentTemperature();
long getCurrentPressure();
float calculateCurrentPressureAtSeaLevel(float currentAltitude);
float getCurrentAltitude();
enum Command getCommand();
//Only use this method in startup sequence
unsigned char getLastCharacterFromUart();

#endif

#ifndef SENSORS_H_
#define SENSORS_H_

/*common includes*/
#include <lpc_types.h>
#include "lpc17xx_timer.h"
#include "lpc_exti.h"

#include <Drivers/GPIO/GPIO.h>
#include <Drivers/ADC/ADC.h>
#include <Drivers/Ultrasonic/Ultrasonic.h>
#include <Interfaces/Actuators/Actuators.h>
#include <Drivers/I2C/I2C.h>
#include <Drivers/BMP085/BMP085.h>
#include <Drivers/WiiMotionPlus/WiiMotionPlus.h>
#include <CoOS.h>

enum Axle {
	X = 0,
	Y = 1,
	Z = 2
};

/*This method returns TRUE if all sensors are correct initialized and FALSE if one sensor fails to initialize*/
Bool sensorInitialization(void);
int getCurrentAngle(enum Axle axle);
int getCurrentHeightInCm();
int getRotation(void);
long getCurrentTemperature();
long getCurrentPressure();
float getCurrentPressureAtSeaLevel();
float getCurrentAltitude();

#endif
